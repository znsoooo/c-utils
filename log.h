#ifndef __LOG_H__
#define __LOG_H__


/* including headers */

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


/* alias of types */

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;


/* user define options */

#define LINE_NUMBER 1
#define PRINT_TO_SCREEN 1
#define PRINT_TO_STRING 1
#define PRINT_TO_FILE   1


/* print line number */

#if LINE_NUMBER
    #define LN printf("(ln: %3d) ", __LINE__)
#else
    #define LN 0
#endif


/* log functions */

#define puts(s) LN + printf("%s\n", s)
#define log_str(x) LN + printf(#x" = \"%s\"\n", x)
#define log_num(x) LN + printf(#x" = %.10g\n", (double)x)
#define log_arr(x) LN + print_arr(#x, x, sizeof(x) / sizeof(x[0]))
#define log_mat(x) LN + print_mat(#x, (int*)x, sizeof(x) / sizeof(x[0]), sizeof(x[0]) / sizeof(x[0][0]))
#define log_bytes(x) LN + print_bytes(#x, x, sizeof(x))

int print_arr(char *name, int *nums, int len);
int print_mat(char *name, int *nums, int h, int w);
int print_bytes(char *name, u8 *data, int len);


/* redirect `printf` */

#define printf log_print

extern char log_buff[];
int log_print(char* format, ...);


/* quick functions */

#define new(type) (type*)calloc(1, sizeof(type))
#define range(x, start, stop) for (int x = start; x < stop; x++)


#endif  /* __LOG_H__ */
