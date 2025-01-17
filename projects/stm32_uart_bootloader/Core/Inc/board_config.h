/**
  ******************************************************************************
  * @file    board_config.h
  * @author  VCD
  * @brief   Header file to compute CRC using software
  * @date    Dec 20 2023
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 Austin Circuit Design.
  * All rights reserved.
  ******************************************************************************
  */
#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H
#include "config.h"
#define BOARD_MAX_SUPPORT_PIN_IN_PORT  16
#define BOARD_MAX_SUPPORT_EXT_NUM      16
#define TEST_WITH_NUCLEO               0

#define PROJEC_BLD
#define APP_WITH_BLD

#ifdef PROJEC_BLD
// Definitions for bootloader
#define FORCE_BLD_MODE_GPIO_PORT                GPIOA
#define FORCE_BLD_MODE_GPIO_PIN                 GPIO_PIN_3
#define FORCE_BLD_MODE_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define FORCE_BLD_MODE_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOA_CLK_DISABLE()
#endif
// Definitions app
#ifdef APP_WITH_BLD
/*Static IP ADDRESS*/
#define IP_ADDR0   ((uint8_t)192U)
#define IP_ADDR1   ((uint8_t)168U)
#define IP_ADDR2   ((uint8_t)1U)
#define IP_ADDR3   ((uint8_t)51U)

/*NETMASK*/
#define NETMASK_ADDR0   ((uint8_t)255U)
#define NETMASK_ADDR1   ((uint8_t)255U)
#define NETMASK_ADDR2   ((uint8_t)255U)
#define NETMASK_ADDR3   ((uint8_t)0U)

/*Gateway Address*/
#define GW_ADDR0   ((uint8_t)192U)
#define GW_ADDR1   ((uint8_t)168U)
#define GW_ADDR2   ((uint8_t)1U)
#define GW_ADDR3   ((uint8_t)1U)

#define DMA1_CLK_ENABLE                   __HAL_RCC_DMA1_CLK_ENABLE
/*SPI Instance 0*/
#define SPII0_MODULE                       SPI1
#define SPII0_TX_DMA_STREAM                DMA1_Stream1
#define SPII0_RX_DMA_STREAM                DMA1_Stream0
#define SPII0_TX_DMA_REQUEST               DMA_REQUEST_SPI1_TX
#define SPII0_RX_DMA_REQUEST               DMA_REQUEST_SPI1_RX
#define SPII0_DMA_TX_IRQn                  DMA1_Stream1_IRQn
#define SPII0_DMA_RX_IRQn                  DMA1_Stream0_IRQn
#define SPII0_DMA_TX_IRQHandler            DMA1_Stream1_IRQHandler
#define SPII0_DMA_RX_IRQHandler            DMA1_Stream0_IRQHandler
#define SPII0_IRQn                         SPI1_IRQn
#define SPII0_IRQHandler                   SPI1_IRQHandler
#define SPII0_FORCE_RESET                  __HAL_RCC_SPI1_FORCE_RESET
#define SPII0_RELEASE_RESET                __HAL_RCC_SPI1_RELEASE_RESET

#define SPII0_CLK_ENABLE                   __HAL_RCC_SPI1_CLK_ENABLE
#define SPII0_CLK_DISABLE                  __HAL_RCC_SPI1_CLK_DISABLE

#define SPII0_SCK_GPIO_PORT                GPIOA
#define SPII0_SCK_GPIO_PIN                 GPIO_PIN_5
#define SPII0_SCK_FUNC                     GPIO_AF5_SPI1
#define SPII0_SCK_GPIO_CLK_EN              __HAL_RCC_GPIOA_CLK_ENABLE

#define SPII0_MISO_GPIO_PORT               GPIOA
#define SPII0_MISO_GPIO_PIN                GPIO_PIN_6
#define SPII0_MISO_FUNC                    GPIO_AF5_SPI1
#define SPII0_MISO_GPIO_CLK_EN             __HAL_RCC_GPIOA_CLK_ENABLE

#define SPII0_MOSI_GPIO_PORT               GPIOD //GPIOD
#define SPII0_MOSI_GPIO_PIN                GPIO_PIN_7 //GPIO_PIN_7
#define SPII0_MOSI_FUNC                    GPIO_AF5_SPI1
#define SPII0_MOSI_GPIO_CLK_EN             __HAL_RCC_GPIOD_CLK_ENABLE //__HAL_RCC_GPIOD_CLK_ENABLE


#define SPII0_NSS_GPIO_PORT                GPIOG //GPIOG
#define SPII0_NSS_GPIO_PIN                 GPIO_PIN_10 //GPIO_PIN_10
#define SPII0_NSS_FUNC                     GPIO_AF5_SPI1
#define SPII0_NSS_GPIO_CLK_EN              __HAL_RCC_GPIOG_CLK_ENABLE //__HAL_RCC_GPIOG_CLK_ENABLE

/*SPI Instance 1*/
#define SPII1_MODULE                       SPI2
#define SPII1_TX_DMA_STREAM                DMA1_Stream3
#define SPII1_RX_DMA_STREAM                DMA1_Stream2
#define SPII1_TX_DMA_REQUEST               DMA_REQUEST_SPI2_TX
#define SPII1_RX_DMA_REQUEST               DMA_REQUEST_SPI2_RX
#define SPII1_DMA_TX_IRQn                  DMA1_Stream3_IRQn
#define SPII1_DMA_RX_IRQn                  DMA1_Stream2_IRQn
#define SPII1_DMA_TX_IRQHandler            DMA1_Stream3_IRQHandler
#define SPII1_DMA_RX_IRQHandler            DMA1_Stream2_IRQHandler
#define SPII1_IRQn                         SPI2_IRQn
#define SPII1_IRQHandler                   SPI2_IRQHandler
#define SPII1_FORCE_RESET                  __HAL_RCC_SPI2_FORCE_RESET
#define SPII1_RELEASE_RESET                __HAL_RCC_SPI2_RELEASE_RESET

#define SPII1_CLK_ENABLE                   __HAL_RCC_SPI2_CLK_ENABLE
#define SPII1_CLK_DISABLE                  __HAL_RCC_SPI2_CLK_DISABLE

#define SPII1_SCK_GPIO_PORT                GPIOB
#define SPII1_SCK_GPIO_PIN                 GPIO_PIN_13
#define SPII1_SCK_FUNC                     GPIO_AF5_SPI2
#define SPII1_SCK_GPIO_CLK_EN              __HAL_RCC_GPIOB_CLK_ENABLE

#define SPII1_MISO_GPIO_PORT               GPIOB
#define SPII1_MISO_GPIO_PIN                GPIO_PIN_14
#define SPII1_MISO_FUNC                    GPIO_AF5_SPI2
#define SPII1_MISO_GPIO_CLK_EN             __HAL_RCC_GPIOB_CLK_ENABLE

#define SPII1_MOSI_GPIO_PORT               GPIOB
#define SPII1_MOSI_GPIO_PIN                GPIO_PIN_15
#define SPII1_MOSI_FUNC                    GPIO_AF5_SPI2
#define SPII1_MOSI_GPIO_CLK_EN             __HAL_RCC_GPIOB_CLK_ENABLE

#define SPII1_NSS_GPIO_PORT                GPIOB
#define SPII1_NSS_GPIO_PIN                 GPIO_PIN_12
#define SPII1_NSS_FUNC                     GPIO_AF5_SPI2
#define SPII1_NSS_GPIO_CLK_EN              __HAL_RCC_GPIOB_CLK_ENABLE

/*SPI Instance 2*/
#define SPII2_MODULE                       SPI4
#define SPII2_TX_DMA_STREAM                DMA1_Stream5
#define SPII2_RX_DMA_STREAM                DMA1_Stream4
#define SPII2_TX_DMA_REQUEST               DMA_REQUEST_SPI4_TX
#define SPII2_RX_DMA_REQUEST               DMA_REQUEST_SPI4_RX
#define SPII2_DMA_TX_IRQn                  DMA1_Stream5_IRQn
#define SPII2_DMA_RX_IRQn                  DMA1_Stream4_IRQn
#define SPII2_DMA_TX_IRQHandler            DMA1_Stream5_IRQHandler
#define SPII2_DMA_RX_IRQHandler            DMA1_Stream4_IRQHandler
#define SPII2_IRQn                         SPI4_IRQn
#define SPII2_IRQHandler                   SPI4_IRQHandler
#define SPII2_FORCE_RESET                  __HAL_RCC_SPI4_FORCE_RESET
#define SPII2_RELEASE_RESET                __HAL_RCC_SPI4_RELEASE_RESET
#define SPII2_CLK_ENABLE                   __HAL_RCC_SPI4_CLK_ENABLE
#define SPII2_CLK_DISABLE                  __HAL_RCC_SPI4_CLK_DISABLE

#define SPII2_SCK_GPIO_PORT                GPIOE
#define SPII2_SCK_GPIO_PIN                 GPIO_PIN_2
#define SPII2_SCK_FUNC                     GPIO_AF5_SPI4
#define SPII2_SCK_GPIO_CLK_EN              __HAL_RCC_GPIOE_CLK_ENABLE

#define SPII2_MISO_GPIO_PORT               GPIOE
#define SPII2_MISO_GPIO_PIN                GPIO_PIN_5
#define SPII2_MISO_FUNC                    GPIO_AF5_SPI4
#define SPII2_MISO_GPIO_CLK_EN             __HAL_RCC_GPIOE_CLK_ENABLE

#define SPII2_MOSI_GPIO_PORT               GPIOE
#define SPII2_MOSI_GPIO_PIN                GPIO_PIN_6
#define SPII2_MOSI_FUNC                    GPIO_AF5_SPI4
#define SPII2_MOSI_GPIO_CLK_EN             __HAL_RCC_GPIOE_CLK_ENABLE

#define SPII2_NSS_GPIO_PORT                GPIOE
#define SPII2_NSS_GPIO_PIN                 GPIO_PIN_4
#define SPII2_NSS_FUNC                     GPIO_AF5_SPI4
#define SPII2_NSS_GPIO_CLK_EN              __HAL_RCC_GPIOE_CLK_ENABLE
/*SPI Instance 3*/
#define SPII3_MODULE                       SPI5
#define SPII3_TX_DMA_STREAM                DMA1_Stream7
#define SPII3_RX_DMA_STREAM                DMA1_Stream6
#define SPII3_TX_DMA_REQUEST               DMA_REQUEST_SPI5_TX
#define SPII3_RX_DMA_REQUEST               DMA_REQUEST_SPI5_RX
#define SPII3_DMA_TX_IRQn                  DMA1_Stream7_IRQn
#define SPII3_DMA_RX_IRQn                  DMA1_Stream6_IRQn
#define SPII3_DMA_TX_IRQHandler            DMA1_Stream7_IRQHandler
#define SPII3_DMA_RX_IRQHandler            DMA1_Stream6_IRQHandler
#define SPII3_IRQn                         SPI5_IRQn
#define SPII3_IRQHandler                   SPI5_IRQHandler
#define SPII3_FORCE_RESET                  __HAL_RCC_SPI5_FORCE_RESET
#define SPII3_RELEASE_RESET                __HAL_RCC_SPI5_RELEASE_RESET
#define SPII3_CLK_ENABLE                   __HAL_RCC_SPI5_CLK_ENABLE
#define SPII3_CLK_DISABLE                  __HAL_RCC_SPI5_CLK_DISABLE

#define SPII3_SCK_GPIO_PORT                GPIOK
#define SPII3_SCK_GPIO_PIN                 GPIO_PIN_0
#define SPII3_SCK_FUNC                     GPIO_AF5_SPI5
#define SPII3_SCK_GPIO_CLK_EN              __HAL_RCC_GPIOK_CLK_ENABLE

#define SPII3_MISO_GPIO_PORT               GPIOJ
#define SPII3_MISO_GPIO_PIN                GPIO_PIN_11
#define SPII3_MISO_FUNC                    GPIO_AF5_SPI5
#define SPII3_MISO_GPIO_CLK_EN             __HAL_RCC_GPIOJ_CLK_ENABLE

#define SPII3_MOSI_GPIO_PORT               GPIOF
#define SPII3_MOSI_GPIO_PIN                GPIO_PIN_11
#define SPII3_MOSI_FUNC                    GPIO_AF5_SPI5
#define SPII3_MOSI_GPIO_CLK_EN             __HAL_RCC_GPIOF_CLK_ENABLE

#define SPII3_NSS_GPIO_PORT                GPIOK
#define SPII3_NSS_GPIO_PIN                 GPIO_PIN_1
#define SPII3_NSS_FUNC                     GPIO_AF5_SPI5
#define SPII3_NSS_GPIO_CLK_EN              __HAL_RCC_GPIOK_CLK_ENABLE

/*DAQ AMC131M02*/
#define AMC_DATA_READY_GPIO_PORT          HAL_GPIO_PORTC
#define AMC_DATA_READY_GPIO_PIN           HAL_GPIO_PIN13

#if TEST_WITH_NUCLEO
#define AMC_DATA_NRESET_GPIO_PORT         HAL_GPIO_PORTE
#define AMC_DATA_NRESET_GPIO_PIN          HAL_GPIO_PIN2
#else
#define AMC_DATA_NRESET_GPIO_PORT         HAL_GPIO_PORTF
#define AMC_DATA_NRESET_GPIO_PIN          HAL_GPIO_PIN12
#endif
#define AMC_DATA_CLKIN_EN_GPIO_PORT       HAL_GPIO_PORTG
#define AMC_DATA_CLKIN_EN_GPIO_PIN        HAL_GPIO_PIN14

#define AMC_DATA_I2C_ADDR0_GPIO_PORT      HAL_GPIO_PORTB
#define AMC_DATA_I2C_ADDR0_GPIO_PIN       HAL_GPIO_PIN5

#define AMC_DATA_I2C_ADDR1_GPIO_PORT      HAL_GPIO_PORTE
#define AMC_DATA_I2C_ADDR1_GPIO_PIN       HAL_GPIO_PIN7

#define AMC_DATA_I2C_ADDR2_GPIO_PORT      HAL_GPIO_PORTF
#define AMC_DATA_I2C_ADDR2_GPIO_PIN       HAL_GPIO_PIN2

/* I2C configuration ==========================*/
#define I2C_USE_DMA                       0
#if I2C_USE_DMA
#define DMA2_CLK_ENABLE                   __HAL_RCC_DMA2_CLK_ENABLE
#endif
#define I2CI0_MODULE                      I2C1
#define I2CI0_CLK_ENABLE                  __HAL_RCC_I2C1_CLK_ENABLE
#define I2CI0_FORCE_RESET()               __HAL_RCC_I2C1_FORCE_RESET()
#define I2CI0_RELEASE_RESET()             __HAL_RCC_I2C1_RELEASE_RESET()

#define I2CI0_EV_IRQn                     I2C1_EV_IRQn
#define I2CI0_ER_IRQn                     I2C1_ER_IRQn
#define I2CI0_EV_IRQHandler               I2C1_EV_IRQHandler
#define I2CI0_ER_IRQHandler               I2C1_ER_IRQHandler
#if I2C_USE_DMA
#define I2CI0_DMA_INSTANCE_TX             DMA2_Stream1
#define I2CI0_DMA_INSTANCE_RX             DMA2_Stream0
#define I2CI0_DMA_REQUEST_TX              DMA_REQUEST_I2C1_TX
#define I2CI0_DMA_REQUEST_RX              DMA_REQUEST_I2C1_RX

#define I2CI0_DMA_TX_IRQn                 DMA2_Stream1_IRQn
#define I2CI0_DMA_RX_IRQn                 DMA2_Stream0_IRQn
#define I2CI0_DMA_TX_IRQHandler           DMA2_Stream1_IRQHandler
#define I2CI0_DMA_RX_IRQHandler           DMA2_Stream0_IRQHandler
#endif
#define I2CI0_SCL_GPIO_PORT               HAL_GPIO_PORTB
#define I2CI0_SCL_GPIO_PIN                HAL_GPIO_PIN6
#define I2CI0_SCL_GPIO_FUNC               HAL_GPIO_ALT_FUNC4

#define I2CI0_SDA_GPIO_PORT               HAL_GPIO_PORTB
#define I2CI0_SDA_GPIO_PIN                HAL_GPIO_PIN7
#define I2CI0_SDA_GPIO_FUNC               HAL_GPIO_ALT_FUNC4

#define I2CI1_MODULE                      I2C2

#define I2CI1_CLK_ENABLE                  __HAL_RCC_I2C2_CLK_ENABLE
#define I2CI1_FORCE_RESET()               __HAL_RCC_I2C2_FORCE_RESET()
#define I2CI1_RELEASE_RESET()             __HAL_RCC_I2C2_RELEASE_RESET()

#define I2CI1_EV_IRQn                     I2C2_EV_IRQn
#define I2CI1_ER_IRQn                     I2C2_ER_IRQn
#define I2CI1_EV_IRQHandler               I2C2_EV_IRQHandler
#define I2CI1_ER_IRQHandler               I2C2_ER_IRQHandler
#if I2C_USE_DMA
#define I2CI1_DMA_INSTANCE_TX             DMA2_Stream3
#define I2CI1_DMA_INSTANCE_RX             DMA2_Stream2
#define I2CI1_DMA_REQUEST_TX              DMA_REQUEST_I2C2_TX
#define I2CI1_DMA_REQUEST_RX              DMA_REQUEST_I2C2_RX

#define I2CI1_DMA_TX_IRQn                 DMA2_Stream3_IRQn
#define I2CI1_DMA_RX_IRQn                 DMA2_Stream2_IRQn
#define I2CI1_DMA_TX_IRQHandler           DMA2_Stream3_IRQHandler
#define I2CI1_DMA_RX_IRQHandler           DMA2_Stream2_IRQHandler
#endif
#define I2CI1_SCL_GPIO_PORT               HAL_GPIO_PORTB
#define I2CI1_SCL_GPIO_PIN                HAL_GPIO_PIN10
#define I2CI1_SCL_GPIO_FUNC               HAL_GPIO_ALT_FUNC4

#define I2CI1_SDA_GPIO_PORT               HAL_GPIO_PORTB
#define I2CI1_SDA_GPIO_PIN                HAL_GPIO_PIN11
#define I2CI1_SDA_GPIO_FUNC               HAL_GPIO_ALT_FUNC4

// HAL UART
#define DMA2_CLK_ENABLE                   __HAL_RCC_DMA2_CLK_ENABLE

// HAL UART instance 0
#define UARTI0_MODULE                     UART5
#define UARTI0_CLK_ENABLE                 __HAL_RCC_UART5_CLK_ENABLE
#define UARTI0_FORCE_RESET()              __HAL_RCC_UART5_FORCE_RESET()
#define UARTI0_RELEASE_RESET()            __HAL_RCC_UART5_RELEASE_RESET()

#define UARTI0_IRQn                       UART5_IRQn
#define UARTI0_IRQHandler                 UART5_IRQHandler
#define UARTI0_DMA_TX                     DMA2_Stream1
#define UARTI0_DMA_RX                     DMA2_Stream0
#define UARTI0_DMA_REQUEST_TX             DMA_REQUEST_UART5_TX
#define UARTI0_DMA_REQUEST_RX             DMA_REQUEST_UART5_RX
#define UARTI0_DMA_TX_IRQn                DMA2_Stream1_IRQn
#define UARTI0_DMA_RX_IRQn                DMA2_Stream0_IRQn
#define UARTI0_DMA_TX_IRQHandler          DMA2_Stream1_IRQHandler
#define UARTI0_DMA_RX_IRQHandler          DMA2_Stream0_IRQHandler

#define UARTI0_RTS_PIN                    HAL_GPIO_PIN8
#define UARTI0_RTS_PORT                   HAL_GPIO_PORTC
#define UARTI0_RTS_GPIO_FUNC              HAL_GPIO_ALT_FUNC8

#define UARTI0_CTS_PIN                    HAL_GPIO_PIN9
#define UARTI0_CTS_PORT                   HAL_GPIO_PORTC
#define UARTI0_CTS_GPIO_FUNC              HAL_GPIO_ALT_FUNC8

#define UARTI0_TX_PIN                     HAL_GPIO_PIN12
#define UARTI0_TX_PORT                    HAL_GPIO_PORTC
#define UARTI0_TX_GPIO_FUNC               HAL_GPIO_ALT_FUNC8

#define UARTI0_RX_PIN                     HAL_GPIO_PIN2
#define UARTI0_RX_PORT                    HAL_GPIO_PORTD
#define UARTI0_RX_GPIO_FUNC               HAL_GPIO_ALT_FUNC8

// HAL UART instance 1
#define UARTI1_MODULE                     UART8
#define UARTI1_CLK_ENABLE                 __HAL_RCC_UART8_CLK_ENABLE
#define UARTI1_FORCE_RESET()              __HAL_RCC_UART8_FORCE_RESET()
#define UARTI1_RELEASE_RESET()            __HAL_RCC_UART8_RELEASE_RESET()

#define UARTI1_IRQn                       UART8_IRQn
#define UARTI1_IRQHandler                 UART8_IRQHandler
#define UARTI1_DMA_TX                     DMA2_Stream3
#define UARTI1_DMA_RX                     DMA2_Stream2
#define UARTI1_DMA_REQUEST_TX             DMA_REQUEST_UART8_TX
#define UARTI1_DMA_REQUEST_RX             DMA_REQUEST_UART8_RX
#define UARTI1_DMA_TX_IRQn                DMA2_Stream3_IRQn
#define UARTI1_DMA_RX_IRQn                DMA2_Stream2_IRQn
#define UARTI1_DMA_TX_IRQHandler          DMA2_Stream3_IRQHandler
#define UARTI1_DMA_RX_IRQHandler          DMA2_Stream2_IRQHandler

#define UARTI1_RTS_PIN                    HAL_GPIO_PIN15
#define UARTI1_RTS_PORT                   HAL_GPIO_PORTD
#define UARTI1_RTS_GPIO_FUNC              HAL_GPIO_ALT_FUNC8

#define UARTI1_CTS_PIN                    HAL_GPIO_PIN14
#define UARTI1_CTS_PORT                   HAL_GPIO_PORTD
#define UARTI1_CTS_GPIO_FUNC              HAL_GPIO_ALT_FUNC8

#define UARTI1_TX_PIN                     HAL_GPIO_PIN8
#define UARTI1_TX_PORT                    HAL_GPIO_PORTJ
#define UARTI1_TX_GPIO_FUNC               HAL_GPIO_ALT_FUNC8

#define UARTI1_RX_PIN                     HAL_GPIO_PIN9
#define UARTI1_RX_PORT                    HAL_GPIO_PORTJ
#define UARTI1_RX_GPIO_FUNC               HAL_GPIO_ALT_FUNC8

// HAL UART instance 2
#define UARTI2_MODULE                     USART2
#define UARTI2_CLK_ENABLE                 __HAL_RCC_USART2_CLK_ENABLE
#define UARTI2_FORCE_RESET()              __HAL_RCC_USART2_FORCE_RESET()
#define UARTI2_RELEASE_RESET()            __HAL_RCC_USART2_RELEASE_RESET()

#define UARTI2_IRQn                       USART2_IRQn
#define UARTI2_IRQHandler                 USART2_IRQHandler
#define UARTI2_DMA_TX                     DMA2_Stream5
#define UARTI2_DMA_RX                     DMA2_Stream4
#define UARTI2_DMA_REQUEST_TX             DMA_REQUEST_USART2_TX
#define UARTI2_DMA_REQUEST_RX             DMA_REQUEST_USART2_RX
#define UARTI2_DMA_TX_IRQn                DMA2_Stream5_IRQn
#define UARTI2_DMA_RX_IRQn                DMA2_Stream4_IRQn
#define UARTI2_DMA_TX_IRQHandler          DMA2_Stream5_IRQHandler
#define UARTI2_DMA_RX_IRQHandler          DMA2_Stream4_IRQHandler

#define UARTI2_RTS_PIN                    HAL_GPIO_PIN4
#define UARTI2_RTS_PORT                   HAL_GPIO_PORTD
#define UARTI2_RTS_GPIO_FUNC              HAL_GPIO_ALT_FUNC7

#define UARTI2_CTS_PIN                    HAL_GPIO_PIN3
#define UARTI2_CTS_PORT                   HAL_GPIO_PORTD
#define UARTI2_CTS_GPIO_FUNC              HAL_GPIO_ALT_FUNC7

#define UARTI2_TX_PIN                     HAL_GPIO_PIN5
#define UARTI2_TX_PORT                    HAL_GPIO_PORTD
#define UARTI2_TX_GPIO_FUNC               HAL_GPIO_ALT_FUNC7

#define UARTI2_RX_PIN                     HAL_GPIO_PIN6
#define UARTI2_RX_PORT                    HAL_GPIO_PORTD
#define UARTI2_RX_GPIO_FUNC               HAL_GPIO_ALT_FUNC7

// HAL UART instance 3
#define UARTI3_MODULE                     USART6
#define UARTI3_CLK_ENABLE                 __HAL_RCC_USART6_CLK_ENABLE
#define UARTI3_FORCE_RESET()              __HAL_RCC_USART6_FORCE_RESET()
#define UARTI3_RELEASE_RESET()            __HAL_RCC_USART6_RELEASE_RESET()

#define UARTI3_IRQn                       USART6_IRQn
#define UARTI3_IRQHandler                 USART6_IRQHandler
#define UARTI3_DMA_TX                     DMA2_Stream7
#define UARTI3_DMA_RX                     DMA2_Stream6
#define UARTI3_DMA_REQUEST_TX             DMA_REQUEST_USART6_TX
#define UARTI3_DMA_REQUEST_RX             DMA_REQUEST_USART6_RX
#define UARTI3_DMA_TX_IRQn                DMA2_Stream7_IRQn
#define UARTI3_DMA_RX_IRQn                DMA2_Stream6_IRQn
#define UARTI3_DMA_TX_IRQHandler          DMA2_Stream7_IRQHandler
#define UARTI3_DMA_RX_IRQHandler          DMA2_Stream6_IRQHandler

#define UARTI3_RTS_PIN                    HAL_GPIO_PIN8
#define UARTI3_RTS_PORT                   HAL_GPIO_PORTG
#define UARTI3_RTS_GPIO_FUNC              HAL_GPIO_ALT_FUNC7

#define UARTI3_CTS_PIN                    HAL_GPIO_PIN15
#define UARTI3_CTS_PORT                   HAL_GPIO_PORTG
#define UARTI3_CTS_GPIO_FUNC              HAL_GPIO_ALT_FUNC7

#define UARTI3_TX_PIN                     HAL_GPIO_PIN6
#define UARTI3_TX_PORT                    HAL_GPIO_PORTC
#define UARTI3_TX_GPIO_FUNC               HAL_GPIO_ALT_FUNC7

#define UARTI3_RX_PIN                     HAL_GPIO_PIN7
#define UARTI3_RX_PORT                    HAL_GPIO_PORTC
#define UARTI3_RX_GPIO_FUNC               HAL_GPIO_ALT_FUNC7

/**----------------------------------------------------*/
/* UPG board adaptor*/
#define UPG_MR_RST_GPIO_PORT              HAL_GPIO_PORTB
#define UPG_MR_RST_GPIO_PIN               HAL_GPIO_PIN2
#define UPG_UTM_SPI_FLASH_MODULE          HAL_SPI0
#define UPG_SPI_LS_OE_PIN                 HAL_GPIO_PIN11
#define UPG_SPI_LS_OE_PORT                HAL_GPIO_PORTB

#define DISCONNECT_VIN_TO_DAQ_PIN         HAL_GPIO_PIN6
#define DISCONNECT_VIN_TO_DAQ_PORT        HAL_GPIO_PORTE
/**-----------------------------------------------------*/
// Stepper motor for fixture control
#define AUX_IN1_PIN                       HAL_GPIO_PIN3
#define AUX_IN1_PORT                      HAL_GPIO_PORTD

#define AUX_IN2_PIN                       HAL_GPIO_PIN4
#define AUX_IN2_PORT                      HAL_GPIO_PORTD

#define AUX_IN3_PIN                       HAL_GPIO_PIN0
#define AUX_IN3_PORT                      HAL_GPIO_PORTA

#define AUX_IN4_PIN                       HAL_GPIO_PIN10
#define AUX_IN4_PORT                      HAL_GPIO_PORTB

#define AUX_IN5_PIN                       HAL_GPIO_PIN8
#define AUX_IN5_PORT                      HAL_GPIO_PORTA

#define AUX_IN6_PIN                       HAL_GPIO_PIN1
#define AUX_IN6_PORT                      HAL_GPIO_PORTD

#define AUX_IN7_PIN                       HAL_GPIO_PIN0
#define AUX_IN7_PORT                      HAL_GPIO_PORTD
// OUTPUT
#define AUX_OUT1_PIN                       HAL_GPIO_PIN12
#define AUX_OUT1_PORT                      HAL_GPIO_PORTC

#define AUX_OUT2_PIN                       HAL_GPIO_PIN2
#define AUX_OUT2_PORT                      HAL_GPIO_PORTD

#define AUX_OUT3_PIN                       HAL_GPIO_PIN9
#define AUX_OUT3_PORT                      HAL_GPIO_PORTC

#define AUX_OUT4_PIN                       HAL_GPIO_PIN8
#define AUX_OUT4_PORT                      HAL_GPIO_PORTC

#define AUX_OUT5_PIN                       HAL_GPIO_PIN5
#define AUX_OUT5_PORT                      HAL_GPIO_PORTD

#define AUX_OUT6_PIN                       HAL_GPIO_PIN6
#define AUX_OUT6_PORT                      HAL_GPIO_PORTD

// UTM module
#define PWR_SW_EN_PIN                      HAL_GPIO_PIN0
#define PWR_SW_EN_PORT                     HAL_GPIO_PORTE

#define PWR_SW_DIAG_EN_PIN                 HAL_GPIO_PIN3
#define PWR_SW_DIAG_EN_PORT                HAL_GPIO_PORTE


#if  TEST_WITH_NUCLEO
#define PWR_SW_SNS_PIN                     HAL_GPIO_PIN11
#define PWR_SW_SNS_PIN_ST                  GPIO_PIN_11
#define PWR_SW_SNS_ADC_MODULE              ADC1
#define PWR_SW_VREF                        3.3f
#else
#define PWR_SW_SNS_ADC_MODULE              ADC2
#define PWR_SW_SNS_PIN                     HAL_GPIO_PIN13
#define PWR_SW_SNS_PIN_ST                  GPIO_PIN_13
#define PWR_SW_VREF                        2.5f
#endif
#define PWR_SW_R_SNS                       324.0f
#define PWR_SW_SNS_RATIO                   1300.0f


#define PWR_SW_SNS_PORT                    HAL_GPIO_PORTF
#define PWR_SW_SNS_PORT_ST                 GPIOF

#define PWR_SW_FAULT_PIN                   HAL_GPIO_PIN1
#define PWR_SW_FAULT_PORT                  HAL_GPIO_PORTE

// High speed usb interface
#define USBH_VBUS_EN_PIN                   HAL_GPIO_PIN1
#define USBH_VBUS_EN_PORT                  HAL_GPIO_PORTG

#define USBH_DATA_EN_PIN                   HAL_GPIO_PIN2
#define USBH_DATA_EN_PORT                  HAL_GPIO_PORTG

#define USBH_VBUS_FAULT_PIN                HAL_GPIO_PIN0
#define USBH_VBUS_FAULT_PORT               HAL_GPIO_PORTG

// ISO OUT
#define ISO_OUT_EN1_PIN                    HAL_GPIO_PIN0
#define ISO_OUT_EN1_PORT                   HAL_GPIO_PORTC

#define ISO_OUT_EN2_PIN                    HAL_GPIO_PIN10
#define ISO_OUT_EN2_PORT                   HAL_GPIO_PORTE

#define ISO_OUT_EN3_PIN                    HAL_GPIO_PIN12
#define ISO_OUT_EN3_PORT                   HAL_GPIO_PORTE

#define ISO_OUT_EN4_PIN                    HAL_GPIO_PIN15
#define ISO_OUT_EN4_PORT                   HAL_GPIO_PORTE

#define ISO_OUT_EN5_PIN                    HAL_GPIO_PIN8
#define ISO_OUT_EN5_PORT                   HAL_GPIO_PORTD

#define ISO_OUT_EN6_PIN                    HAL_GPIO_PIN9
#define ISO_OUT_EN6_PORT                   HAL_GPIO_PORTD

#define ISO_OUT_EN7_PIN                    HAL_GPIO_PIN10
#define ISO_OUT_EN7_PORT                   HAL_GPIO_PORTD

#define ISO_OUT_EN8_PIN                    HAL_GPIO_PIN10
#define ISO_OUT_EN8_PORT                   HAL_GPIO_PORTJ

// ISO IN
#define ISO_IN1_PIN                        HAL_GPIO_PIN2
#define ISO_IN1_PORT                       HAL_GPIO_PORTK

#define ISO_IN2_PIN                        HAL_GPIO_PIN3
#define ISO_IN2_PORT                       HAL_GPIO_PORTG

#define ISO_IN3_PIN                        HAL_GPIO_PIN4
#define ISO_IN3_PORT                       HAL_GPIO_PORTG

#define ISO_IN4_PIN                        HAL_GPIO_PIN5
#define ISO_IN4_PORT                       HAL_GPIO_PORTG

#define ISO_IN5_PIN                        HAL_GPIO_PIN6
#define ISO_IN5_PORT                       HAL_GPIO_PORTG

#define ISO_IN6_PIN                        HAL_GPIO_PIN7
#define ISO_IN6_PORT                       HAL_GPIO_PORTG

#define ISO_IN7_PIN                        HAL_GPIO_PIN8
#define ISO_IN7_PORT                       HAL_GPIO_PORTE

#define ISO_IN8_PIN                        HAL_GPIO_PIN10
#define ISO_IN8_PORT                       HAL_GPIO_PORTA

#define PWR_SW_I2C_MODULE                  HAL_I2C0
#define PWR_SW_I2C_ADDR                    0b01011000

// SWD flasher
#define FLASHER_SWDCLK_PIN                 HAL_GPIO_PIN10
#define FLASHER_SWDCLK_PORT                HAL_GPIO_PORTC
#define FLASHER_SWDIO_PIN                  HAL_GPIO_PIN11
#define FLASHER_SWDIO_PORT                 HAL_GPIO_PORTC
#define FLASHER_RESET_PIN                  HAL_GPIO_PIN7
#define FLASHER_RESET_PORT                 HAL_GPIO_PORTB

#endif



#endif