/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "gpio.h"
#include "usart.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

#include "b_l475e_iot01a1_bus.h"
#include "mouse_main.h"
#include "tflite_interface.hpp"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

struct mouseHID_t myMouse = {0, 0, 0, 0};
LSM6DSL_Object_t MotionSensor;

uint32_t Button_Flag;
volatile uint32_t dataRdyIntReceived;

uint8_t Rx_data[10];
float prediction;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MEMS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU
     * Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the
     * Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_USB_DEVICE_Init();
    /* USER CODE BEGIN 2 */
    Button_Flag = RESET;
    dataRdyIntReceived = 0;
    MEMS_Init();
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    printf("\n\r\n\rFinished Initialization!\n\r");
    initTflite();
    int32_t counter = 0;
    int32_t speed = 1;

    // while(1) {}
    while (1) {
        if (dataRdyIntReceived != 0) {
            dataRdyIntReceived = 0;
            LSM6DSL_Axes_t acc_axes;
            LSM6DSL_ACC_GetAxes(&MotionSensor, &acc_axes);
            //printf("%5d,%5d,%5d\r\n", (int)acc_axes.x, (int)acc_axes.y,
             //      (int)acc_axes.z);

            addValues(acc_axes.x, acc_axes.y, acc_axes.z);
            runInference(&prediction);

            if (counter < 500) {
                ++counter;
            }
            //printf("Result: %f\r\n", prediction);

            if (prediction >= 0.75f && prediction <= 1.2f && counter && counter >= 100) {
                mouse_click(&myMouse);
                HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                printf("CLICK\r\n");
                counter = 0;
            }

            myMouse.x = - speed * acc_axes.y / 100;
            myMouse.y = - speed * acc_axes.x / 100;

            if (Button_Flag) {
                Button_Flag = 0;
                if (speed >= 3) {
                    speed = 1;
                } else {
                    speed += 0.5;
                }
            }
            mouse_send(&myMouse);
        }
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) !=
        HAL_OK) {
        Error_Handler();
    }

    /** Configure LSE Drive Capability
     */
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType =
        RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
        Error_Handler();
    }

    /** Enable MSI Auto calibration
     */
    HAL_RCCEx_EnableMSIPLLMode();
}

/* USER CODE BEGIN 4 */
void MEMS_Init(void) {
    LSM6DSL_IO_t io_ctx;
    uint8_t id;
    LSM6DSL_AxesRaw_t axes;

    /* Link I2C functions to the LSM6DSL driver */
    io_ctx.BusType = LSM6DSL_I2C_BUS;
    io_ctx.Address = LSM6DSL_I2C_ADD_L;
    io_ctx.Init = BSP_I2C2_Init;
    io_ctx.DeInit = BSP_I2C2_DeInit;
    io_ctx.ReadReg = BSP_I2C2_ReadReg;
    io_ctx.WriteReg = BSP_I2C2_WriteReg;
    io_ctx.GetTick = BSP_GetTick;
    LSM6DSL_RegisterBusIO(&MotionSensor, &io_ctx);

    /* Read the LSM6DSL WHO_AM_I register */
    LSM6DSL_ReadID(&MotionSensor, &id);
    if (id != LSM6DSL_ID) {
        Error_Handler();
    }

    /* Initialize the LSM6DSL sensor */
    LSM6DSL_Init(&MotionSensor);

    /* Configure the LSM6DSL accelerometer (ODR, scale and interrupt) */
    LSM6DSL_ACC_SetOutputDataRate(&MotionSensor, 48.0f); /* 26 Hz */
    LSM6DSL_ACC_SetFullScale(&MotionSensor, 4);       /* [-4000mg; +4000mg] */
    LSM6DSL_ACC_Set_INT1_DRDY(&MotionSensor, ENABLE); /* Enable DRDY */
    LSM6DSL_ACC_GetAxesRaw(&MotionSensor, &axes);     /* Clear DRDY */

    /* Start the LSM6DSL accelerometer */
    LSM6DSL_ACC_Enable(&MotionSensor);
}

/* EXTI callback */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_11) {
        dataRdyIntReceived = 1;
    } else {
        Button_Flag = SET;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    HAL_UART_Receive(&huart1, Rx_data, 10, 10);
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state
     */

    __disable_irq();
    while (1) {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        HAL_Delay(50);
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
