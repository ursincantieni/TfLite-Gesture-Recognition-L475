//
// Created by Ursin Cantieni on 27.11.22.
//

//
// Created by Ursin Cantieni on 27.11.22.
//

#include "mouse_main.h"
#include "main.h"

#include "mouse_sensor.h"

struct mouseHID_t myMouse = {0,0,0, 0};
uint8_t Button_Flag;
volatile uint32_t dataRdyIntReceived;

static void mouse_move(LSM6DSL_Axes_t* axes, struct mouseHID_t* mouseHID);

/* main loop */
void mouse_main() {
    Button_Flag = RESET;
    dataRdyIntReceived = 0;

    MEMS_Init();

    while (1) {
        if (dataRdyIntReceived != 0) {
            dataRdyIntReceived = 0;
            LSM6DSL_Axes_t acc_axes;
            LSM6DSL_ACC_GetAxes(&MotionSensor, &acc_axes);
            printf("% 5d, % 5d, % 5d\n", (int) acc_axes.x, (int) acc_axes.y, (int) acc_axes.z);

            mouse_move(&acc_axes, &myMouse);
        }
    }
}

static void mouse_move(LSM6DSL_Axes_t* axes, struct mouseHID_t* mouseHID) {
    /*int flag = 0;
    if ( (int) axes->x > 200 || (int) axes->x < -200 ) {
        mouseHID->y -=  axes->x / 10;
        flag = 1;
    }
    if ( (int) axes->y > 200  || (int) axes->y < -200 ) {
        mouseHID->x -= axes->y / 10;
        flag = 1;
    }
    if (flag == 1) {
        mouse_send(&myMouse);
    }*/

    UNUSED(axes);
    mouseHID->y = 10;
    mouseHID->x = -10;
    mouse_send(mouseHID);
}

/* Reset mouse report */
void mouse_zero(struct mouseHID_t* mouseHID) {
    mouseHID->button = 0;
    mouseHID->x = 0;
    mouseHID->y = 0;
    mouseHID->wheel = 0;
}

/* Send mouse report in mouseHID */
void mouse_send(struct mouseHID_t* mouseHID) {
    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) mouseHID, sizeof(struct mouseHID_t));
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

/* EXTI callback */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_11) {
        dataRdyIntReceived = 1;
    } else {
        Button_Flag = SET;
    }
}