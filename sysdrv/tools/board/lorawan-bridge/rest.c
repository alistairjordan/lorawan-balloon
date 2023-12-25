/*
 * Copyright (c) 2018 the CivetWeb developers
 * Revisited version: Copyright (c) 2022 the CivetWeb developers
 * MIT License
 */

/* Simple demo of a REST callback. */
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cJSON.h"
#include "civetweb.h"
#include "send_json.h"
#include "lorawan_send.h"

#define PORT "8089"
#define HOST_INFO "http://localhost:8089"

#define LORAWAN_URI "/lorawan"
#define LORAWAN_SEND_URI "/lorawan/send"
#define EXIT_URI "/exit"

int exitNow = 0;

static unsigned request = 0; /* demo data: request counter */

static int
LorawanGET(struct mg_connection *conn, const char *p1)
{
	cJSON *obj = cJSON_CreateObject();

	if (!obj) {
		/* insufficient memory? */
		mg_send_http_error(conn, 500, "Server error");
		return 500;
	}

	printf("GET lorawan/%s\n", p1);
	cJSON_AddStringToObject(obj, "version", CIVETWEB_VERSION);
	cJSON_AddStringToObject(obj, "modem_function", "lorawan");
	cJSON_AddStringToObject(obj, "operation", p1);
	cJSON_AddNumberToObject(obj, "request", ++request);
	SendJSON(conn, obj);
	cJSON_Delete(obj);

	return 200;
}


static int
LorawanDELETE(struct mg_connection *conn, const char *p1, const char *p2)
{
	printf("DELETE %s/%s\n", p1, p2);
	mg_send_http_error(conn,
	                   204,
	                   "%s",
	                   ""); /* Return "deleted" = "204 No Content" */

	return 204;
}


static int
LorawanPUT(struct mg_connection *conn, const char *p1, const char *p2)
{
	char buffer[1024];
	int dlen = mg_read(conn, buffer, sizeof(buffer) - 1);
	cJSON *obj, *elem;
	unsigned newvalue;

	printf("PUT %s/%s\n", p1, p2);
	if ((dlen < 1) || (dlen >= sizeof(buffer))) {
		mg_send_http_error(conn, 400, "%s", "No request body data");
		return 400;
	}
	buffer[dlen] = 0;

	obj = cJSON_Parse(buffer);
	if (obj == NULL) {
		mg_send_http_error(conn, 400, "%s", "Invalid request body data");
		return 400;
	}

	elem = cJSON_GetObjectItemCaseSensitive(obj, "request");

	if (!cJSON_IsNumber(elem)) {
		cJSON_Delete(obj);
		mg_send_http_error(conn,
		                   400,
		                   "%s",
		                   "No \"request\" number in body data");
		return 400;
	}

	newvalue = (unsigned)elem->valuedouble;

	if ((double)newvalue != elem->valuedouble) {
		cJSON_Delete(obj);
		mg_send_http_error(conn,
		                   400,
		                   "%s",
		                   "Invalid \"request\" number in body data");
		return 400;
	}

	request = newvalue;
	cJSON_Delete(obj);

	mg_send_http_error(conn, 201, "%s", ""); /* Return "201 Created" */

	return 201;
}

static int
LorawanHandler(struct mg_connection *conn, void *cbdata)
{
	char path1[1024], path2[1024];
	const struct mg_request_info *ri = mg_get_request_info(conn);
	const char *url = ri->local_uri;
	size_t url_len = strlen(url);

	struct mg_match_context mcx;
	mcx.case_sensitive = 0;
	ptrdiff_t ret = mg_match(LORAWAN_URI, url, &mcx);
	if ((ret != url_len) || (mcx.num_matches != 1)) {
		/* Note: Could have done this with a $ at the end of the match
		 * pattern as well. Then we would have to check for a return value
		 * of -1 only. Here we use this version as minimum modification
		 * of the existing code. */
		printf("Match ret: %i\n", (int)ret);
		mg_send_http_error(conn, 404, "Invalid path: %s\n", url);
		return 404;
	}
	memcpy(path1, mcx.match[0].str, mcx.match[0].len);
	path1[mcx.match[0].len] = 0;

	(void)cbdata; /* currently unused */

	/* According to method */
	if (0 == strcmp(ri->request_method, "GET")) {
		return LorawanGET(conn, path1);
	}
	if ((0 == strcmp(ri->request_method, "PUT"))
	    || (0 == strcmp(ri->request_method, "POST"))
	    || (0 == strcmp(ri->request_method, "PATCH"))) {
		/* In this example, do the same for PUT, POST and PATCH */
		return LorawanPUT(conn, path1, path2);
	}
	if (0 == strcmp(ri->request_method, "DELETE")) {
		return LorawanDELETE(conn, path1, path2);
	}

	/* this is not a GET request */
	mg_send_http_error(
	    conn, 405, "Only GET, PUT, POST, DELETE and PATCH method supported");
	return 405;
}


static int
ExitHandler(struct mg_connection *conn, void *cbdata)
{
	mg_printf(conn,
	          "HTTP/1.1 200 OK\r\nContent-Type: "
	          "text/plain\r\nConnection: close\r\n\r\n");
	mg_printf(conn, "Server will shut down.\n");
	mg_printf(conn, "Bye!\n");
	exitNow = 1;
	return 1;
}


static int
log_message(const struct mg_connection *conn, const char *message)
{
	puts(message);
	return 1;
}

int
main(int argc, char *argv[])
{
	const char *options[] = {"listening_ports",
	                         PORT,
	                         "request_timeout_ms",
	                         "10000",
	                         "error_log_file",
	                         "error.log",
	                         0};

	struct mg_callbacks callbacks;
	struct mg_context *ctx;
	int err = 0;

	/* Init libcivetweb. */
	mg_init_library(0);

	/* Callback will print error messages to console */
	memset(&callbacks, 0, sizeof(callbacks));
	callbacks.log_message = log_message;

	/* Start CivetWeb web server */
	ctx = mg_start(&callbacks, 0, options);

	/* Check return value: */
	if (ctx == NULL) {
		fprintf(stderr, "Cannot start CivetWeb - mg_start failed.\n");
		return EXIT_FAILURE;
	}

	mg_set_request_handler(ctx, LORAWAN_SEND_URI, LorawanSendHandler, 0);
	mg_set_request_handler(ctx, LORAWAN_URI, LorawanHandler, 0);
	mg_set_request_handler(ctx, EXIT_URI, ExitHandler, 0);

	/* Show some info */
	printf("Start example: %s%s\n", HOST_INFO, LORAWAN_URI);
	printf("Exit example:  %s%s\n", HOST_INFO, EXIT_URI);

	/* Wait until the server should be closed */
	while (!exitNow) {
		sleep(1);
	}

	/* Stop the server */
	mg_stop(ctx);

	printf("Server stopped.\n");
	printf("Bye!\n");

	return EXIT_SUCCESS;
}