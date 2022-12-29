#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <M5Unified.h>

#include "wifi_ctrl.hpp"
#include "basic_func.hpp"

#define INFO_TAG "M5 Info:"
template<typename T, size_t N> size_t ArraySize(T(&)[N]){ return N; }


void display_info(bool refr) {
    static int prev_battery = INT_MAX;
    int battery = M5.Power.getBatteryLevel();

    if (prev_battery != battery) {
        prev_battery = battery;
        refr = true;
    }

    if (refr) {
        M5.Display.setTextDatum(TR_DATUM);
        char batstr[1024];
        snprintf(batstr, 1024, "Bat:%02d", battery);
        M5.Display.startWrite();
        M5.Display.drawString("          ", 320,0);
        M5.Display.drawString(batstr, 320, 0);
        M5.Display.endWrite();
    }
}

void reset_scr(void) {
    M5.Display.clearDisplay();
    M5.Display.setCursor(0,0);
    M5.Display.println();
    display_info(true);
}

short menu_choice = 1;
bool menu_mode = false;

void normal_control(void) {
    if (M5.BtnA.wasPressed()) {
        reset_scr();
        M5.Display.startWrite();

        for (int i = 0; i < 30; i++) {
            M5.Display.printf("Printing line on loc %d %d \n", M5.Display.getCursorX(), M5.Display.getCursorY());
            vTaskDelay(100);
        }
        M5.Display.endWrite();
    }

    if (M5.BtnB.wasPressed()) {
        M5.Display.println("Clearning screen...");
        vTaskDelay(300);
        reset_scr();
    }

    if (M5.BtnC.wasPressed()) {
        reset_scr();
        M5.Display.println("Opening menu");
        for (int i = 0; i < ArraySize(CHOICE_NAME); i++) {
            int j = i+1;
            M5.Display.printf("%d . %s\n", j, CHOICE_NAME[i]);
            vTaskDelay(100);
        }
        menu_choice = 1;
        M5.Display.printf("Choosing Option: %d", menu_choice);
        menu_mode = true;
    }
}

void menu_control(void) {
    if (M5.BtnC.wasPressed()) {
        int line = M5.Display.getCursorY();
        M5.Display.setCursor(0, line);
        if (menu_choice >= ArraySize(CHOICE_NAME)) {
            menu_choice = 1;
        } else {
            menu_choice++;
        }
        M5.Display.printf("Choosing Option: %d", menu_choice);
    }

    if (M5.BtnB.wasPressed()) {
        M5.Display.printf("\nChosen %d\n", menu_choice);

        switch (menu_choice) {
            case 1:
                M5.Display.println("Exiting menu");
                vTaskDelay(300);
                reset_scr();
                break;
            case 2:
                M5.Display.println("Start Wifi");
                vTaskDelay(300);
                reset_scr();
                start_wifi();
                break;
            case 3:
                M5.Display.println("Doing stuff");
                break;
            case 4:
                break;
        }
        menu_mode = false;
    }

    if (M5.BtnA.wasPressed()) {
        int line = M5.Display.getCursorY();
        M5.Display.setCursor(0, line);
        if (menu_choice <= 1) {
            menu_choice = ArraySize(CHOICE_NAME);
        } else {
            menu_choice--;
        }
        M5.Display.printf("Choosing Option: %d", menu_choice);
    }
}

void button_ctrl(void) {
    if (M5.BtnPWR.wasClicked()) {
        M5.Display.println("Powering off device");
        M5.Power.powerOff();
    }

    if (!menu_mode) {
        normal_control();
    } else {
        menu_control();
    }
}
