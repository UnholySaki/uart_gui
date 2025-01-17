/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include <stdio.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f103xb.h"

#include <stdint.h>

#define NUM_BYTE_MAX 64
#define UPLOADING_TIMEOUT (2000000) //
#define NUM_VEC (4)                 // Check number of reset vectors when check validation
#define START_WRITE 1
#define READ_CRC 2
#define WRITE_DATA 3
#define TXDATA_SIZE 4

#define READ_BYTECOUNT 1
#define RT_DATA 0
#define RT_EOF 1
#define RT_MAIN_ADDR 4

#define RQ_ERASE 0xF3          //
#define RQ_UPLOADING 0xF2      //
#define RQ_CHECK_BLANKING 0xF4 //
#define RQ_CHECK_CRC 0xF5      //
#define RQ_SYSTEM_RESET 0xF6   //
#define RQ_GET_BLD_VER 0xF7
#define RQ_ENTER_BLD 0xF8
#define RQ_EXIT_BLD 0xF9

#define RES_EXIT_BLD_SUCCESS 'c'
#define RES_EXIT_BLD_FAIL 'd'
#define RES_ENTER_BLD_SUCCESS 'a'
#define RES_ENTER_BLD_FAIL 'b'
#define RES_INVALID_COMMAND '9' //
#define RES_ERASE_COMPLETE '8'  // 8 - Erase memory success
#define RES_CRC_FAIL '7'        // 7 - Check crc fail
#define RES_CRC_SUCCESS '6'     // 6 - Upload image success
#define RES_UPLOAD_SUCCESS '5'  // 5 - Upload line success
#define RES_UPLOAD_FAIL '4'     // 4 - Upload line fail - Ask for line again
#define RES_UPLOAD_INPROG '3'   // 3 - Uploading in progress - Ask to wait
#define RES_APP_FLASH_CLEAN '2'
#define RES_APP_FLASH_DIRTY '1'

#define IMAGE_TAG 0xD1B45E82
#define RQ_KEY 0x84AE9B23
#define RES_KEY 0x18E2B8AC
#define BLANK_KEY 0xFFFFFFFF
#define BLD_ENTER_NUM_KEY (125)
#define BLD_VERSION_MAJOR '0'
#define BLD_VERSION_MINOR '1'
#define BLD_VERSION_PATCH 'A'
#define BLD_VERSION                                               \
    (BLD_VERSION_MAJOR | ('.' << 8) | (BLD_VERSION_MINOR << 16) | \
     (BLD_VERSION_PATCH << 24))

/*============================VARIABLES====================================*/
typedef struct
{
    uint32_t rq_key;
    uint32_t res_key;
} bld_enter_rq_res_t;

typedef struct
{
    uint32_t image_tag;
    uint32_t image_start;
    uint32_t image_size;
    uint32_t image_crc;
    uint32_t jump_addr;
    uint32_t reserve;
    bld_enter_rq_res_t bld_enter_key[BLD_ENTER_NUM_KEY];
} image_info_t;
typedef enum
{
    CMD_NOP,
    CMD_ERASE,
    CMD_BLANKING,
    CMD_UPLOADING,
    CMD_CHECK_CRC,
    CMD_SYSTEM_RESET,
    CMD_GET_BLD_VER,
    CMD_ENTER_BLD,
    CMD_EXIT_BLD,

    CMD_NUM
} bld_cmd_e;

typedef enum
{
    IRQ_CMD_NOP,
    IRQ_CMD_ERASE,
    IRQ_CMD_UPLOADING,
    IRQ_CMD_CHECK_BLANKING,
    IRQ_CMD_CHECK_CRC,
    IRQ_CMD_SYSTEM_RESET,
    IRQ_CMD_GET_BLD_VER,
    IRQ_CMD_ENTER_BLD,
    IRQ_CMD_EXIT_BLD,
} interrupt_rq_e;

void Error_Handler(void);

#endif /* __MAIN_H */
