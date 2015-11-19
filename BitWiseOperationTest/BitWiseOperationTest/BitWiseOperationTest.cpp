// BitWiseOperationTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdint.h>

#include <stdio.h>

#define SWAP(a, b)   {a ^= b; b ^= a; a ^= b;}

#define BX_(x)       ((x) - (((x)>>1) & 0x77777777) \
                          - (((x)>>2) & 0x33333333) \
                          - (((x)>>3) & 0x11111111))

#define BITCOUNT(x)  (((BX_(x)+(BX_(x)>>4)) & 0x0F0F0F0F) % 255)


int _tmain(int argc, _TCHAR* argv[])
{
  int32_t a = 0;
  int32_t b = 0;

  printf("Please input integer numbers you want to SWAP\
         (use \", \" to divide different number):");
  scanf_s("%d, %d", &a, &b);
  printf("The number you input is: a = %d, b = %d\n", a, b);

  SWAP(a, b);

  printf("After SWAP, a = %d, b = %d\n", a, b);

  int32_t c = 0;
  printf("Now test the number of BITS in a integer!\n");
  printf("Please input the number you want to test in the form of binary:");
  scanf_s("%d", &c);
  printf("c = %d\n", c);
  printf("The number of bits is %d\n", BITCOUNT(c));

	return 0;
}

