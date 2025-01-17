
#ifndef __HAL_GPIO_H
#define __HAL_GPIO_H
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    HAL_GPIO_PORTA,
    HAL_GPIO_PORTB,
    HAL_GPIO_PORTC,
    HAL_GPIO_PORTD,
    HAL_GPIO_PORTE,
    HAL_GPIO_PORT_NUM,
    HAL_GPIO_PORT_INVALID = 0xff,
}hal_gpio_port_e;

typedef enum {
    HAL_GPIO_PIN0,
    HAL_GPIO_PIN1,
    HAL_GPIO_PIN2,
    HAL_GPIO_PIN3,
    HAL_GPIO_PIN4,
    HAL_GPIO_PIN5,
    HAL_GPIO_PIN6,
    HAL_GPIO_PIN7,
    HAL_GPIO_PIN8,
    HAL_GPIO_PIN9,
    HAL_GPIO_PIN10,
    HAL_GPIO_PIN11,
    HAL_GPIO_PIN12,
    HAL_GPIO_PIN13,
    HAL_GPIO_PIN14,
    HAL_GPIO_PIN15,
    HAL_GPIO_PIN_NUM,
    HAL_GPIO_PIN_INVALID = 0xff,
}hal_gpio_pin_e;

typedef struct {
    hal_gpio_port_e port;
    hal_gpio_pin_e  pin;
}hal_gpio_t;

typedef enum{
    HAL_GPIO_PIN_RESET,
    HAL_GPIO_PIN_SET,
}hal_gpio_val_e;

typedef void (*hal_gpio_cb_t)(void);

typedef enum{
    HAL_GPIO_NO_PULL,
    HAL_GPIO_PULL_UP,
    HAL_GPIO_PULL_DOWN,
}hal_gpio_pull_e;

typedef enum{
    HAL_GPIO_INPUT,
    HAL_GPIO_OUTPUT_PP,
    HAL_GPIO_OUTPUT_OD,
    HAL_GPIO_AF_PP,
    HAL_GPIO_AF_OD,
    HAL_GPIO_ANALOG,
    HAL_GPIO_INT_RISING,
    HAL_GPIO_INT_FALLING,
    HAL_GPIO_INT_RISING_FALLING,
}hal_gpio_mode_e;

typedef enum{
    HAL_GPIO_ALT_FUNC0,
    HAL_GPIO_ALT_FUNC1,
    HAL_GPIO_ALT_FUNC2,
    HAL_GPIO_ALT_FUNC3,
    HAL_GPIO_ALT_FUNC4,
    HAL_GPIO_ALT_FUNC5,
    HAL_GPIO_ALT_FUNC6,
    HAL_GPIO_ALT_FUNC7,
    HAL_GPIO_ALT_FUNC8,
    HAL_GPIO_ALT_FUNC9,
    HAL_GPIO_ALT_FUNC10,
    HAL_GPIO_ALT_FUNC11,
    HAL_GPIO_ALT_FUNC12,
    HAL_GPIO_ALT_FUNC13,
    HAL_GPIO_ALT_FUNC14,
    HAL_GPIO_ALT_FUNC15,
}hal_gpio_alt_e;

typedef struct {
    hal_gpio_mode_e mode;
    hal_gpio_pull_e pull;
    hal_gpio_alt_e  func;
    hal_gpio_cb_t   int_cb;
}hal_gpio_mode_t;

bool           hal_gpio_pin_valid(hal_gpio_t pin);
void           hal_gpio_pin_config(hal_gpio_t pin, hal_gpio_mode_t mode);
void           hal_gpio_enable_int(hal_gpio_t pin);
void           hal_gpio_disable_int(hal_gpio_t pin);
hal_gpio_val_e hal_gpio_read(hal_gpio_t pin);
void           hal_gpio_write(hal_gpio_t pin, hal_gpio_val_e val);
#endif