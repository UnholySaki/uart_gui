/**
 ******************************************************************************
 * @file    hal_i2c.h
 * @author  VCD
 * @brief   header file for hal i2c master mode
 * @date    Jan 19 2024
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Austin Circuit Design.
 * All rights reserved.
 ******************************************************************************
 */
#ifndef __HAL_I2C_H
#define __HAL_I2C_H
#include <stdint.h>
#include <stdbool.h>
typedef enum
{
    HAL_I2C0,
    HAL_I2C1,
    HAL_I2C_NUM,
} hal_i2c_instance_e;

typedef enum
{
    HAL_I2C_SUCCESS,
    HAL_I2C_FAILED,
} hal_i2c_stt_e;

typedef enum
{
    HAL_I2C_100K,
    HAL_I2C_400K,
} hal_i2c_speed_e;

typedef struct
{
    uint8_t slave_addr;
    hal_i2c_speed_e speed;
} hal_i2c_cfg_t;

typedef void hal_i2c_hdl_t;

hal_i2c_stt_e hal_i2c_init(void);
hal_i2c_hdl_t *hal_i2c_open(hal_i2c_instance_e i2c, hal_i2c_cfg_t cfg);
hal_i2c_stt_e hal_i2c_set_slave_addr(hal_i2c_hdl_t *handle, uint8_t addr);
hal_i2c_stt_e hal_i2c_read(hal_i2c_hdl_t *handle, uint16_t subaddr, uint16_t len_addr, uint8_t *buf, uint16_t len);
hal_i2c_stt_e hal_i2c_write(hal_i2c_hdl_t *handle, uint16_t subaddr, uint16_t len_addr, uint8_t *buf, uint16_t len);
hal_i2c_stt_e hal_i2c_cancel(hal_i2c_hdl_t *handle);
hal_i2c_stt_e hal_i2c_close(hal_i2c_hdl_t **handle);
#endif