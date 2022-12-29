#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <M5Unified.h>

#include "basic_func.hpp"
#include "esp_event.h"

extern "C" {
#include <wifi_controller.h>
}


#define INFO_TAG "M5 Info:"

void device_setup() {
    auto cfg = M5.config();
    cfg.internal_mic = false;
    cfg.internal_spk = false;
    cfg.internal_imu = false;
    M5.begin(cfg);



    vTaskDelay(1000);
    M5.Display.fillScreen(BLACK);
    M5.Display.setTextSize(1.5);
    M5.Display.setTextColor(GREEN, BLACK);
    M5.Display.setTextWrap(true);
    M5.Display.setTextScroll(true);
    M5.Display.println("EsPwn32 Initialized.");
    ESP_LOGI(INFO_TAG, "Boot successfully");

    M5.Display.printf("Base task running on thread:%d\n", xPortGetCoreID());
}

int thread;
void thread_task(void *pvParameter) {
    thread = xPortGetCoreID();
    vTaskDelete(NULL);
}

void main_loop(void *pvParameter) {
    // Setup
    device_setup();
    xTaskCreatePinnedToCore(&thread_task, "thread", 1024, NULL, 2, NULL, 0);

    vTaskDelay(1000);
    M5.Display.printf("Thread task running on thread:%d\n", thread);

    for (;;) {
        vTaskDelay(1);
        M5.update();
        // Loop
        button_ctrl();
        display_info();

    }
}


extern "C" void app_main() {
    esp_event_loop_create_default();
    // wifictl_mgmt_ap_start();

    xTaskCreate(&main_loop, "main loop", 16384, NULL, 1, NULL);
}
