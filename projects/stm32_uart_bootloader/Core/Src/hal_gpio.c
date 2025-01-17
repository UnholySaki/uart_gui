/**
  ******************************************************************************
  * @file    hal_gpio.c
  * @author  VCD
  * @brief   header file for hal gpio
  * @date    Jan 17 2024
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 Austin Circuit Design.
  * All rights reserved.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "board_config.h"
#include "hal_gpio.h"
#include "main.h"

/* Private variables ---------------------------------------------------------*/
const GPIO_TypeDef *port_hal_gpio_port[] = {
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOE
};
const uint16_t port_hal_gpio_pin[BOARD_MAX_SUPPORT_PIN_IN_PORT] = {
    GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2,  GPIO_PIN_3,  GPIO_PIN_4,  GPIO_PIN_5,  GPIO_PIN_6,  GPIO_PIN_7,
    GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15
};
static hal_gpio_cb_t   interrupt_cb[BOARD_MAX_SUPPORT_EXT_NUM];
/* Private function prototypes ------------------------------------------------*/
static GPIO_TypeDef * port_hal_gpio_get_port(hal_gpio_t pin);
static uint16_t       port_hal_gpio_get_pin(hal_gpio_t pin);
/* Public function             ------------------------------------------------*/

bool hal_gpio_pin_valid(hal_gpio_t pin)
{
    bool valid = true;
    if ((pin.pin >= HAL_GPIO_PIN_NUM) || (pin.port >= HAL_GPIO_PORT_NUM))
    {
        valid = false;
    }
    return valid;
}

/**
  * @brief  gpio inititialize function
  * @param  pin: gpio to configure
  * @param  mode: mode of the gpio to configure
  * @note   This function is a wrapper base on the st sdk
  * @retval None
  */
void hal_gpio_pin_config(hal_gpio_t pin, hal_gpio_mode_t mode)
{
    GPIO_InitTypeDef  gpio_init_structure;
    uint32_t          pin_mode;
    uint32_t          pull_mode;
    GPIO_TypeDef      *GPIOx = port_hal_gpio_get_port(pin);
    // port clock enable
    switch(pin.port)
    {
        case HAL_GPIO_PORTA:
        {
            __HAL_RCC_GPIOA_CLK_ENABLE();
        }
        break;
        case HAL_GPIO_PORTB:
        {
            __HAL_RCC_GPIOB_CLK_ENABLE();
        }
        break;
        case HAL_GPIO_PORTC:
        {
            __HAL_RCC_GPIOC_CLK_ENABLE();
        }
        break;
        case HAL_GPIO_PORTD:
        {
            __HAL_RCC_GPIOD_CLK_ENABLE();
        }
        break;
        case HAL_GPIO_PORTE:
        {
            __HAL_RCC_GPIOE_CLK_ENABLE();
        }
        break;
        default:
        break;
    }
    // 
    pin_mode = 0;
    switch (mode.mode)
    {
        case HAL_GPIO_INPUT:
        {
            pin_mode = GPIO_MODE_INPUT;
        }
        break;
        case HAL_GPIO_OUTPUT_PP:
        {
            pin_mode = GPIO_MODE_OUTPUT_PP;
        }
        break;
        case HAL_GPIO_OUTPUT_OD:
        {
            pin_mode = GPIO_MODE_OUTPUT_OD;
        }
        break;
        case HAL_GPIO_AF_PP:
        {
            pin_mode = GPIO_MODE_AF_PP;
        }
        break;
        case HAL_GPIO_AF_OD:
        {
            pin_mode = GPIO_MODE_AF_OD;
        }
        break;
        case HAL_GPIO_ANALOG:
        {
            pin_mode = GPIO_MODE_ANALOG;
        }
        break;
        case HAL_GPIO_INT_RISING:
        {
            pin_mode = GPIO_MODE_IT_RISING;
        }
        break;
        case HAL_GPIO_INT_FALLING:
        {
            pin_mode = GPIO_MODE_IT_FALLING;
        }
        break;
        case HAL_GPIO_INT_RISING_FALLING:
        {
            pin_mode = GPIO_MODE_IT_RISING_FALLING;
        }
        break;
        default:
        break;
    }
    pull_mode = 0;
    switch(mode.pull)
    {
        case HAL_GPIO_NO_PULL:
        {
            pull_mode = GPIO_NOPULL;
        }
        break;
        case HAL_GPIO_PULL_UP:
        {
            pull_mode = GPIO_PULLUP;
        }
        break;
        case HAL_GPIO_PULL_DOWN:
        {
            pull_mode = GPIO_PULLDOWN;
        }
        break;
        default:
        break;
    }
    gpio_init_structure.Pin   = port_hal_gpio_get_pin(pin);
    gpio_init_structure.Mode  = pin_mode;
    gpio_init_structure.Pull  = pull_mode;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOx, &gpio_init_structure);
    // in the case of interrupt need to enable it
    if ((mode.mode == HAL_GPIO_INT_RISING)||
        (mode.mode == HAL_GPIO_INT_FALLING)||
        (mode.mode == HAL_GPIO_INT_RISING_FALLING))
    {
        interrupt_cb[pin.pin] = mode.int_cb;
        switch (pin.pin)
        {
            case HAL_GPIO_PIN0:
            {
                HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
                HAL_NVIC_EnableIRQ(EXTI0_IRQn);
            }
            break;
            case HAL_GPIO_PIN1:
            {
                HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 0);
                HAL_NVIC_EnableIRQ(EXTI1_IRQn);
            }
            break;
            case HAL_GPIO_PIN2:
            {
                HAL_NVIC_SetPriority(EXTI2_IRQn, 1, 0);
                HAL_NVIC_EnableIRQ(EXTI2_IRQn);
            }
            break;
            case HAL_GPIO_PIN3:
            {
                HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 0);
                HAL_NVIC_EnableIRQ(EXTI3_IRQn);
            }
            break;
            case HAL_GPIO_PIN4:
            {
                HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 0);
                HAL_NVIC_EnableIRQ(EXTI4_IRQn);
            }
            break;
            case HAL_GPIO_PIN5:
            case HAL_GPIO_PIN6:
            case HAL_GPIO_PIN7:
            case HAL_GPIO_PIN8:
            case HAL_GPIO_PIN9:
            {
                HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
                HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
            }
            break;
            case HAL_GPIO_PIN10:
            case HAL_GPIO_PIN11:
            case HAL_GPIO_PIN12:
            case HAL_GPIO_PIN13:
            case HAL_GPIO_PIN14:
            case HAL_GPIO_PIN15:
            {
                HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);
                HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
            }
            break;
            default:
            break;
        }
    }
}

/**
  * @brief  Enable pin interrupt for the line
  * @param  pin: gpio to configure
  * @note   None
  * @retval None
  */

void hal_gpio_enable_int(hal_gpio_t pin)
{
    uint32_t temp;
    uint32_t temp1;
    uint32_t position;
    EXTI_TypeDef *EXTI_CurrentCPU;
    EXTI_CurrentCPU = EXTI; /* EXTI for CM7 CPU */
    temp = EXTI_CurrentCPU->IMR;
    position = 1<< pin.pin;
    temp1  = temp;
    temp1 |= position;
    if (temp1 != temp)
    {
        EXTI_CurrentCPU->IMR = temp1;
    }
}
/**
  * @brief  Disable pin interrupt for the line
  * @param  pin: gpio to configure
  * @note   None
  * @retval None
  */
void hal_gpio_disable_int(hal_gpio_t pin)
{
    uint32_t temp;
    uint32_t temp1;
    uint32_t position;
    EXTI_TypeDef *EXTI_CurrentCPU;
    EXTI_CurrentCPU = EXTI; /* EXTI for CM7 CPU */

    temp = EXTI_CurrentCPU->IMR;
    position = 1<< pin.pin;
    temp1  = temp;
    temp1 &= (~position);
    if (temp1 != temp)
    {
        EXTI_CurrentCPU->IMR = temp1;
    }
}
/**
  * @brief  wrapper for gpio interrupt handle
  * @param  start: Start of the pin
  * @param  end: end of the pin
  * @note   None
  * @retval None
  */

void hal_gpio_irq_handle(uint32_t start, uint32_t end)
{
  uint32_t exint_line;
  int i;
  exint_line = __HAL_GPIO_EXTI_GET_IT(GPIO_PIN_All);
  for (i = start; i <= end; i++)
  {
    if(exint_line & (1 << i))
    {
      HAL_GPIO_EXTI_IRQHandler(port_hal_gpio_pin[i]);
    }
  }
}

/**
  * @brief  GPIO interrupt callback
  * @param  GPIO_Pin: GPIO pin
  * @note   None
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  int i;
  for (i = 0; i < BOARD_MAX_SUPPORT_PIN_IN_PORT; i++)
  {
    if (port_hal_gpio_pin[i] == GPIO_Pin)
    {
        if (interrupt_cb[i] != NULL)
        {
            interrupt_cb[i]();
        }
        break;
    }
  }
}

/**
  * @brief  Read pin status
  * @param  pin: GPIO pin
  * @note   None
  * @retval {HAL_GPIO_PIN_RESET, HAL_GPIO_PIN_SET}
  */
hal_gpio_val_e hal_gpio_read(hal_gpio_t pin)
{
    hal_gpio_val_e val;
    GPIO_TypeDef *GPIOx = port_hal_gpio_get_port(pin);
    uint16_t GPIO_Pin =  port_hal_gpio_get_pin(pin);
    val =  (hal_gpio_val_e) HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
    return val;
}
/**
  * @brief  write gpio pin
  * @param  pin: GPIO pin
  * @param  val :{HAL_GPIO_PIN_RESET, HAL_GPIO_PIN_SET}
  * @note   None
  * @retval None
  */
void hal_gpio_write(hal_gpio_t pin, hal_gpio_val_e val)
{
    GPIO_TypeDef *GPIOx = port_hal_gpio_get_port(pin);
    uint16_t GPIO_Pin =  port_hal_gpio_get_pin(pin);
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, val);
}
/* Private function ---------------------------------------------------------*/
/**
  * @brief  Get device port address from the enum
  * @param  pin: GPIO pin
  * @note   None
  * @retval GPIO port 
  */
static GPIO_TypeDef * port_hal_gpio_get_port(hal_gpio_t pin)
{
    return (GPIO_TypeDef *)port_hal_gpio_port[pin.port];
}
/**
  * @brief  Get gpio pin  from the enum
  * @param  pin: GPIO pin
  * @note   None
  * @retval GPIO pin 
  */
static uint16_t port_hal_gpio_get_pin(hal_gpio_t pin)
{
    return (uint16_t)port_hal_gpio_pin[pin.pin];
}
