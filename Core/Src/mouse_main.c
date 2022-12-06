//
// Created by Ursin Cantieni on 27.11.22.
//

#include "mouse_main.h"
#include "main.h"
#include "lsm6dsl.h"

void mouse_move(LSM6DSL_Axes_t* axes, struct mouseHID_t* mouseHID) {

    UNUSED(axes);
    mouseHID->y = 10;
    mouseHID->x = -10;
    mouse_send(mouseHID);
}

/* Send mouse report in mouseHID */
void mouse_send(struct mouseHID_t* mouseHID) {
    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) mouseHID, sizeof(struct mouseHID_t));
}

/* Reset mouse report */
void mouse_zero(struct mouseHID_t* mouseHID) {
    mouseHID->button = 0;
    mouseHID->x = 0;
    mouseHID->y = 0;
    mouseHID->wheel = 0;
}

/* left click */
void mouse_it(struct mouseHID_t* mouseHID) {
    mouseHID->button = 1;
    mouse_send(mouseHID);
    HAL_Delay(100);

    mouseHID->button = 0;
    mouse_send(mouseHID);
    HAL_Delay(100);
}