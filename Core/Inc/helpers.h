//
// Created by Ursin Cantieni on 05.11.22.
//

#ifndef L475_MOUSE_HELPERS_H
#define L475_MOUSE_HELPERS_H

#include "stm32l4xx_hal.h"
#include <stdio.h>

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int getc(FILE *f)
#endif /* __GNUC__ */


#endif //L475_MOUSE_HELPERS_H
