//
// Created by Ursin Cantieni on 05.11.22.
//

#include "project.h"

void foo() {
    while (1) {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        HAL_Delay(200);
    }
}