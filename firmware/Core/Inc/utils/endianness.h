/*
 * endianness.h
 *
 *  Created on: Jun 16, 2024
 *      Author: Angelo
 */

#ifndef INC_UTILS_ENDIANNESS_H_
#define INC_UTILS_ENDIANNESS_H_


uint32_t reverseEndianness(uint32_t value)
{
    return __builtin_bswap32(value);
}


#endif /* INC_UTILS_ENDIANNESS_H_ */
