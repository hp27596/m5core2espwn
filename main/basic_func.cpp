#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <M5Unified.h>

#include "basic_func.hpp"

#define INFO_TAG "M5 Info:"

void display_info(bool refr) {
    static int prev_battery = INT_MAX;
    int battery = M5.Power.getBatteryLevel();

    if (prev_battery != battery) {
        prev_battery = battery;
        refr = true;
    }

    if (refr) {
        M5.Display.startWrite();

        M5.Display.setCursor(245,0);
        M5.Display.print("Bat:");

        if (battery >= 0) {
            M5.Display.printf("%02d\n", battery);
        } else {
            M5.Display.print("NaN");
        }
        M5.Display.endWrite();
    }
}

void reset_scr(void) {
    M5.Display.clearDisplay();
    M5.Display.setCursor(1,0);
    display_info(true);
}

void button_ctrl(void) {
    if (M5.BtnA.wasPressed()) {
        M5.Display.println("Printing a very looooooooooooooooooooooooooong line");
    }

    if (M5.BtnB.wasPressed()) {
        M5.Display.println("Clearning screen...");
        vTaskDelay(300);
        reset_scr();
    }

    if (M5.BtnC.wasPressed()) {
        M5.Display.println("Printing Power Info...");
    }

    if (M5.BtnPWR.wasClicked()) {
        M5.Display.println("Powering off device");
        M5.Power.powerOff();
    }
}
