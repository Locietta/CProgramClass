#include "simpio.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    FILE *fp = fopen("../../layout.json", "r+");
    string str_json = ReadFile(fp);
    fclose(fp);
    cJSON *layout_data = cJSON_Parse(str_json);
    free(str_json);
    // double btn_prop =
    //     cJSON_GetObjectItem(cJSON_GetObjectItem(layout_data, "propotion")->child, "btn")
    //         ->valuedouble;
    // printf("%f", btn_prop);
    printf("%s\n",cJSON_Print(layout_data));
    
    char title[36];
    strcpy(title, cJSON_GetObjectItem(layout_data, "winTitle")->valuestring);
    printf("%s\n", title);
    int size = cJSON_GetArraySize(cJSON_GetObjectItem(layout_data, "button"));
    printf("%d", size);
    return 0;
}