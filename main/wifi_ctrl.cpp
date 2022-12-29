#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <M5Unified.h>

#include "esp_err.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_netif.h"
#include "esp_event.h"

extern "C" {
#include <wifi_controller.h>
}

#include "wifi_ctrl.hpp"

bool start_sta = false;

void start_wifi(void) {
    // wifi_config_t ap_config = {
    //     .sta = {
    //         .scan_method = WIFI_FAST_SCAN,
    //     },
    //     // .ap = {
    //     //     // .ssid_len = strlen((char *)ap_record->ssid),
    //     //     // .channel = ap_record->primary,
    //     //     // .authmode = ap_record->authmode,
    //     //     // .password = "dummypassword",
    //     //     // .max_connection = 1

    //     // },
    // };
    // esp_event_loop_create_default();
    // wifictl_mgmt_ap_start();
    // wifictl_ap_start(&ap_config);

    if (!start_sta) {
        esp_netif_init();
        esp_netif_create_default_wifi_sta();

        wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
        ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

        ESP_ERROR_CHECK(esp_wifi_start());
        start_sta = true;
    }

    wifictl_scan_nearby_aps();

    const wifictl_ap_records_t *ap_records;
    ap_records = wifictl_get_ap_records();

    char resp_chunk[40];

    for(unsigned i = 0; i < ap_records->count; i++){
        memcpy(resp_chunk, ap_records->records[i].ssid, 33);
        memcpy(&resp_chunk[33], ap_records->records[i].bssid, 6);
        memcpy(&resp_chunk[39], &ap_records->records[i].rssi, 1);
        M5.Display.printf("%s\n", resp_chunk);
    }

    M5.Display.printf("Found %d APs\n", ap_records->count);
}
