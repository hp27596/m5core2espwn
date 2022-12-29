#pragma once

const char CHOICE_NAME[][1024] = {
    "Exit menu",
    "Wifi management",
    "Capture 4 way handshake",
    "Start game"};

void reset_scr(void);

void display_info(bool refr = false);

void button_ctrl(void);
