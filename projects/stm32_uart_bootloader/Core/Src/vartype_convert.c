#include "vartype_convert.h"

/**
 * @brief Convert interger type to String type
 * @param num ref number to copy into string
 * @param str string/array that will be modified
 */
void int_to_str(int num, uint8_t *str)
{
  uint8_t i = 0;
  uint8_t is_negative = 0;

  // Handle negative number
  if (num < 0)
  {
    is_negative = 1;
    num = -num;
  }

  // Process each digit
  do
  {
    str[i++] = (num % 10) + '0'; // Convert digit to character
    num /= 10;
  } while (num != 0);

  // Add '-' for negative numbers
  if (is_negative)
  {
    str[i++] = '-';
  }

  // Null-terminate the string
  str[i] = '\0';

  // Reverse the string
  uint8_t start = 0;
  uint8_t end = i - 1;
  while (start < end)
  {
    uint8_t temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    start++;
    end--;
  }
}