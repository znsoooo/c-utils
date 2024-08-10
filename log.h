#ifndef __LOG_H__
#define __LOG_H__


/* including headers */

#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


/* alias of types */

#define abs fabs
#define max fmax
#define min fmin

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;


/* user define options */

#define LINE_NUMBER 0
#define PRINT_TO_SCREEN 1
#define PRINT_TO_STRING 0
#define PRINT_TO_FILE   0


/* print line number */

#if LINE_NUMBER
    #define LN printf("(ln: %3d) ", __LINE__)
#else
    #define LN 0
#endif


/* log functions */

#define puts(s) LN + printf("%s\n", s)
#define log_str(x) LN + printf(#x" = \"%s\"\n", x)
#define log_num(x) LN + printf(#x" = %.15g\n", (double)x)
#define log_hex(x) LN + printf(#x" = %#x\n", (int)x)
#define log_arr(x) LN + printf(#x" = ") + print_arr(x, sizeof(x) / sizeof(x[0]))
#define log_mat(x) LN + printf(#x" = ") + print_mat((int*)x, sizeof(x) / sizeof(x[0]), sizeof(x[0]) / sizeof(x[0][0]))
#define log_bytes(x) LN + printf(#x" = ") + print_bytes(x, sizeof(x))

int print_arr(int* nums, int length);
int print_mat(int* nums, int height, int width);


/* redirect 'printf' */

#define printf log_print

extern char log_buff[];
int log_print(char* format, ...);


/* quick functions */

#define new(type) (type*)calloc(1, sizeof(type))
#define range(x, start, stop) for (int x = start; x < stop; x++)


#endif  /* __LOG_H__ */
