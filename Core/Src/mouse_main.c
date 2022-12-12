//
// Created by Ursin Cantieni on 27.11.22.
//

#include "mouse_main.h"

void mouse_send(struct mouseHID_t* mouseHID) {
    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) mouseHID, sizeof(struct mouseHID_t));
}

void mouse_zero(struct mouseHID_t* mouseHID) {
    mouseHID->button = 0;
    mouseHID->x = 0;
    mouseHID->y = 0;
    mouseHID->wheel = 0;
}

/* Reset mouse report */
void mouse_click(struct mouseHID_t* mouseHID) {
    mouseHID->x = 0;
    mouseHID->y = 0;
    mouseHID->button = 1;
    mouse_send(mouseHID);

    HAL_Delay(50);

    mouseHID->button = 0;
    mouse_send(mouseHID);
}