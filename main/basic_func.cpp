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

void clear_screen(void) {
    M5.Display.clearDisplay();
    M5.Display.setCursor(0,0);
    M5.Display.println();
    display_info(true);
}


short menu_choice = 0;
bool menu_mode = false;
bool app_mode = false;

void power_off(void) {
    M5.Power.powerOff();
}

MenuOption option[5] = {
    {"Power off", "Powering off device", &power_off},
    {"Wifi hacking", "Starting wifipwn", &wifi_mode, &wifi_control},


};

void launch_menu(void) {
    clear_screen();
    M5.Display.println("Opening menu");
    for (int i = 0; i < ArraySize(option); i++) {
        M5.Display.printf("%d . %s\n", i, option[i].name);
        vTaskDelay(100);
    }
    menu_choice = 0;
    M5.Display.printf("Press left and right to choose.\nChoosing Option: %d", menu_choice);
    menu_mode = true;
}

void normal_control(void) {
    if (M5.BtnA.wasPressed()) {
        clear_screen();
        M5.Display.println("Printing Info...");
    }

    if (M5.BtnB.wasPressed()) {
        M5.Display.println("Clearning screen...");
        vTaskDelay(300);
        clear_screen();
    }

    if (M5.BtnC.wasPressed()) {
        clear_screen();
        launch_menu();
    }
}

void menu_control(void) {
    if (M5.BtnA.wasPressed()) {
        M5.Display.setCursor(0, M5.Display.getCursorY());
        if (menu_choice <= 0) {
            menu_choice = ArraySize(option) - 1;
        } else {
            menu_choice--;
        }
        M5.Display.printf("Choosing Option: %d", menu_choice);
    }

    if (M5.BtnC.wasPressed()) {
        M5.Display.setCursor(0, M5.Display.getCursorY());
        if (menu_choice >= ArraySize(option) - 1) {
            menu_choice = 0;
        } else {
            menu_choice++;
        }
        M5.Display.printf("Choosing Option: %d", menu_choice);
    }

    if (M5.BtnB.wasPressed()) {
        M5.Display.printf("\n\nChosen %d\n%s\n", menu_choice, option[menu_choice].desc);
        vTaskDelay(500);
        option[menu_choice].run();

        app_mode = true;
        menu_mode = false;
    }

}

void button_ctrl(void) {
    if (M5.BtnPWR.wasHold()) {
        M5.Display.println("Powering off device");
        vTaskDelay(300);
        M5.Power.powerOff();
    }

    if (M5.BtnPWR.wasClicked()) {
        M5.Display.println("Restarting device");
        vTaskDelay(300);
        esp_restart();
    }

    // button state
    if (app_mode) {
        option[menu_choice].control();
    } else if (menu_mode) {
        menu_control();
    } else {
        normal_control();
    }
}

