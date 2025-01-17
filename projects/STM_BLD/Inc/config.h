/**
  ******************************************************************************
  * @file    config.h
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
#ifndef __CONFIG_H
#define __CONFIG_H
#include <stdint.h>
#define REQUEST_STAY_IN_BLD   "Bld_stay_req"
#define IMG_HDR(a, b, c, d)     ((a)|((b)<<8)|((c)<<16)|((d)<<24))
#define FW_FOLDER              "/system/firmware"

#define FW_VERSION_H     1
#define FW_VERSION_L     3

typedef enum{
  JUMP_TO_APP,
  STAY_IN_BLD,
}state_in_bld_e;

typedef struct {
  uint32_t tag;
  uint16_t version_h;
  uint16_t version_l;
  uint32_t offset;
  uint32_t load_addr;
  uint32_t length;
  uint32_t checksum;
  uint32_t headerchecksum;
}fw_image_info_t;
#endif