#include "../Inc/checksum.h"
#include "stdio.h"

/**
 * Checksum algorithm
 * @param cs reference checksum
 * @param len data length
 * @return true if varification complete, false otherwise
 */
bool check_cs(uint8_t *data, uint8_t ref_cs, uint8_t len) {
  // combine all data
  int total_cs = 0;
  for (uint8_t i = 0; i < len; i++) {
    total_cs += data[i];
  }

  uint8_t calc_cs = (256 - (total_cs & 0xFF)) & 0xFF;
  // compare with received checksums
  if (calc_cs == ref_cs)
    return true;
  else
    return false;
}