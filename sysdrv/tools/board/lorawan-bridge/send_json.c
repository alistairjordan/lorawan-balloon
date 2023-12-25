#include <string.h>
#include "cJSON.h"
#include "civetweb.h"
#include "send_json.h"

int
SendJSON(struct mg_connection *conn, cJSON *json_obj)
{
	char *json_str = cJSON_PrintUnformatted(json_obj);
	size_t json_str_len = strlen(json_str);

	/* Send HTTP message header (+1 for \n) */
	mg_send_http_ok(conn, "application/json; charset=utf-8", json_str_len + 1);

	/* Send HTTP message content */
	mg_write(conn, json_str, json_str_len);

	/* Add a newline. This is not required, but the result is more
	 * human-readable in a debugger. */
	mg_write(conn, "\n", 1);

	/* Free string allocated by cJSON_Print* */
	cJSON_free(json_str);

	return (int)json_str_len;
}