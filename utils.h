/*
 * utils.h
 *
 *  Created on: 2021-11-30
 *      Author: @v-barros
 */
#ifndef UTILS_H_
#define UTILS_H_

// convert n to array of char in dest and return num of chars
// ex: n = 65535
// put "65535" on dest
// return 5
// n must be positive( return -1 and does nothing if n is negative)
int itostring(int n,char*dest);

// convert n to ascii equivalent
// ex: n = 1
// return '1'(0x30)
// return space (0x20) if n is not in [0-9]
char itochar(int n);

#endif // UTILS_H_