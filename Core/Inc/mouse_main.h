//
// Created by Ursin Cantieni on 27.11.22.
//

#ifndef MOUSE_MAIN_H
#define MOUSE_MAIN_H

#include <stdint.h>
#include "usbd_hid.h"
#include "lsm6dsl.h"

struct mouseHID_t {
    int8_t button;
    int8_t x;
    int8_t y;
    int8_t wheel;
};

extern LSM6DSL_Object_t MotionSensor;
extern USBD_HandleTypeDef hUsbDeviceFS;

void mouse_move(struct mouseHID_t* mouseHID);
void mouse_zero(struct mouseHID_t* mouseHID);

#endif //MOUSE_MAIN_H
