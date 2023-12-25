#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cJSON.h"
#include "civetweb.h"
#include "send_json.h"
#include "b64.c/b64.h"
#include "lorawan_send.h"

int request = 0;

static int
LorawanSendGET(struct mg_connection *conn)
{
	cJSON *obj = cJSON_CreateObject();

	if (!obj) {
		/* insufficient memory? */
		mg_send_http_error(conn, 500, "Server error");
		return 500;
	}

	cJSON_AddStringToObject(obj, "version", CIVETWEB_VERSION);
	cJSON_AddStringToObject(obj, "modem_function", "lorawan");
	cJSON_AddStringToObject(obj, "operation", "send");
	cJSON_AddNumberToObject(obj, "request", ++request);
	SendJSON(conn, obj);
	cJSON_Delete(obj);

	return 200;
}


static int
LorawanSendDELETE(struct mg_connection *conn)
{
	mg_send_http_error(conn,
	                   204,
	                   "%s",
	                   ""); /* Return "deleted" = "204 No Content" */

	return 204;
}


static int
LorawanSendPUT(struct mg_connection *conn)
{
	char buffer[1024];
	int dlen = mg_read(conn, buffer, sizeof(buffer) - 1);
	cJSON *obj, *port_elem, *payload_elem;
    char* payload;
	uint newvalue, port;

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

	port_elem = cJSON_GetObjectItemCaseSensitive(obj, "port");

	if (!cJSON_IsNumber(port_elem)) {
		cJSON_Delete(obj);
		mg_send_http_error(conn,
		                   400,
		                   "%s",
		                   "No \"request\" number in body data");
		return 400;
	}

	newvalue = (uint)port_elem->valuedouble;

	if ((double)newvalue != port_elem->valuedouble) {
		cJSON_Delete(obj);
		mg_send_http_error(conn,
		                   400,
		                   "%s",
		                   "Invalid \"request\" number in body data");
		return 400;
	}
    
    payload_elem = cJSON_GetObjectItemCaseSensitive(obj, "payload64");
    if (cJSON_IsString(payload_elem) && (payload_elem->valuestring != NULL))
    {
        printf("Checking Payload \"%s\"\n", payload_elem->valuestring);
    } else {
		cJSON_Delete(obj);
		mg_send_http_error(conn,
		                   400,
		                   "%s",
		                   "Invalid \"payload64\" in body data");
		return 400;
    }

    // Decode from base64
    payload = b64_decode(payload_elem->valuestring, strlen(payload_elem->valuestring));
    if (payload==NULL) {
		cJSON_Delete(obj);
		mg_send_http_error(conn,
		                   400,
		                   "%s",
		                   "Invalid \"payload64\" in body data");
		return 400;
    }

	port = newvalue;
    printf("%s",payload);

    //Generate JSON response
    cJSON *ret = cJSON_CreateObject();
    if (ret == NULL)
    {
        cJSON_Delete(ret);
        printf("Unable to generate response for response");
	    mg_send_http_error(conn,
		                   500,
		                   "%s",
		                   "Unable to generate response");   
        return 500;    
    }

    printf("here");
    payload_elem = cJSON_CreateString(payload);
    if (payload == NULL)
    {
        cJSON_Delete(ret);
        printf("Unable to generate response for payload");
	    mg_send_http_error(conn,
		                   500,
		                   "%s",
		                   "Unable to generate response for payload");   
        return 500;
    }

    cJSON_AddItemToObject(ret, "payload", payload_elem);

    if (cJSON_AddNumberToObject(ret, "port", port) == NULL) {
        cJSON_Delete(ret);
        printf("Unable to generate response for port");
	    mg_send_http_error(conn,
		                   500,
		                   "%s",
		                   "Unable to generate response for port");   
        return 500;           
    }

    mg_printf(conn,
            "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: "
            "close\r\n\r\n");
	mg_printf(conn, "%s", cJSON_Print(ret));
	cJSON_Delete(obj);
	return 201;
}

int
LorawanSendHandler(struct mg_connection *conn, void *cbdata)
{
	char path1[1024], path2[1024];
	const struct mg_request_info *ri = mg_get_request_info(conn);
	const char *url = ri->local_uri;
	size_t url_len = strlen(url);

	(void)cbdata; /* currently unused */

	/* According to method */
	if (0 == strcmp(ri->request_method, "GET")) {
		return LorawanSendGET(conn);
	}
	if ((0 == strcmp(ri->request_method, "PUT"))
	    || (0 == strcmp(ri->request_method, "POST"))
	    || (0 == strcmp(ri->request_method, "PATCH"))) {
		/* In this example, do the same for PUT, POST and PATCH */
		return LorawanSendPUT(conn);
	}
	if (0 == strcmp(ri->request_method, "DELETE")) {
		return LorawanSendDELETE(conn);
	}

	/* this is not a GET request */
	mg_send_http_error(
	    conn, 405, "Only GET, PUT, POST, DELETE and PATCH method supported");
	return 405;
}