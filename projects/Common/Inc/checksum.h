
#ifndef __CHECKSUM_H
#define __CHECKSUM_H

#include <stdint.h>
#include <stdbool.h>

bool check_cs(uint8_t *data, uint8_t ref_cs, uint8_t len);

#endif /* __CHECKSUM_H */