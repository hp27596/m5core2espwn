#pragma once

// const char CHOICE_NAME[][1024] = {
//     "Exit menu",
//     "Wifi hacking",
//     "Start game",
//     "Shutdown"};

// struct menuOption {
//     char name[1024];
//     char description[1024];
//     void control();
// };

// const menuOption exit = {"exit", "Exit menu"};
// const menuOption hack = {"hack", "Wifi hacking"};
// const menuOption shutdown = {"shutdown", "Shutdown EsPwn32"};

// menuOption MENU_OPTION[3] = {
//     {"exit", "Exit menu"},
//     {"hack", "Wifi hacking"},
//     {"shutdown", "Shutdown EsPwn32", &power_off}
// };

// class MenuOption {
//     public:
//         char name[1024];
//         char desc[1024];

//         void (*run_command)();
//         void (*btn_control)();

//         // void run_command() { ;
//         // void button_control();
// };

struct MenuOption {
    char name[1024];
    char desc[1024];
    void (*run)();
    void (*control)();
};

void launch_menu(void);

void power_off(void);

void clear_screen(void);

void display_info(bool refr = false);

void button_ctrl(void);
