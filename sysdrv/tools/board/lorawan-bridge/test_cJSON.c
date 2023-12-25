#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "cJSON.h"
#include "b64.c/b64.h"

char data[] = "{\n\
    \"port\": 1234,\n\
    \"payload64\": \"SGVsbG8gV29ybGQK\"\n\
}";

int main() {
    const cJSON *port = NULL;
    const cJSON *payload = NULL;
    cJSON *json = cJSON_Parse(data);
    if (!json) {
        printf("Cannot Decode JSON");
    }
    //printf("%s",cJSON_Print(json));
    port = cJSON_GetObjectItemCaseSensitive(json, "port");
    if (cJSON_IsNumber(port))
    {
        printf("Checking port \"%d\"\n", port->valueint);
    } else {
        printf("Cannot Decode");
        return -1;
    }
    payload = cJSON_GetObjectItemCaseSensitive(json, "payload64");
    if (cJSON_IsString(payload) && (payload->valuestring != NULL))
    {
        printf("Checking Payload \"%s\"\n", payload->valuestring);
    } else {
        printf("Cannot Decode");
        return -1;
    }

    // Decode from base64
    char *dec = b64_decode(payload->valuestring, strlen(payload->valuestring));

    printf("%s\n", dec); // brian the monkey and bradley the kinkajou are friends

}