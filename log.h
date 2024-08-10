#ifndef __LOG_H__
#define __LOG_H__


/* user define options */

#define LINE_NUMBER 0
#define PRINT_TO_SCREEN 1
#define PRINT_TO_STRING 0
#define PRINT_TO_FILE   0


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


/* redirect 'printf' */

char  log_buff[0x100000];  // 1MB
char *log_pbuff = log_buff;
int   log_reset = 1;

static int log_print(char* format, ...)
{
    va_list p;
    va_start(p, format);

#if PRINT_TO_SCREEN
    vprintf(format, p);
#endif

#if PRINT_TO_STRING
    vsprintf(log_pbuff, format, p);
    log_pbuff = strchr(log_pbuff, 0);
#endif

#if PRINT_TO_FILE
    FILE *fp = fopen("log.txt", log_reset ? "w+" : "a+");
    log_reset = 0;
    vfprintf(fp, format, p);
    fclose(fp);
#endif

    va_end(p);
    return 0;
}

#define printf log_print


/* quick functions */

#define new(type) (type*)calloc(1, sizeof(type))
#define range(x, start, stop) for (int x = start; x < stop; x++)


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

#define log_arr(x) do { \
    LN + printf(#x" = ["); \
    for (int _i = 0; _i < sizeof(x) / sizeof(x[0]); _i++) { \
        printf(", %.15g" + !_i * 2, (double)x[_i]); \
    } \
    printf("]\n"); \
} while (0)

#define log_mat(x) do { \
    LN + printf(#x" = "); \
    printf("[\n"); \
    for (int _r = 0; _r < sizeof(x) / sizeof(x[0]); _r++) { \
        printf("  ["); \
        for (int _c = 0; _c < sizeof(x[0]) / sizeof(x[0][0]); _c++) { \
            printf(", %.15g" + !_c * 2, (double)x[_r][_c]); \
        } \
        printf("],\n"); \
    } \
    printf("]\n"); \
} while (0)

#define log_bytes(x) do { \
    LN + printf(#x" = \""); \
    for (int _i = 0; _i < sizeof(x); _i++) \
        printf("\\x%02x", ((uint8_t*)x)[_i]); \
    printf("\"\n"); \
} while (0)

#define expr       log_num
#define log_number log_num
#define log_string log_str
#define log_array  log_arr
#define log_matrix log_mat


/* util functions */

static int checksum(void* data, int length) {
    uint8_t* _data = data;
    int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += _data[i];
    }
    return sum;
}

static uint16_t crc(void* data, int length)
{
    uint8_t* _data = data;
    uint16_t crc = 0;
    while (length--) {
        crc ^= *(_data++) << 8;
        for (int i = 0; i < 8; i++) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc = crc << 1;
        }
    }
    return crc;
}

static void memoryview(void* data, int length, int width, int offset)
{
    uint8_t* _data = data;
    for (int i = 0; i < length; i += width) {
        printf("%08X", i + offset);
        for (int j = 0; j < width && (i + j) < length; j++) {
            printf(" %02X", _data[i + j]);
        }
        printf("\n");
    }
}


/* unit test */

static void log_test()
{
    char text[20] = "ABCDEFGH\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8";

    log_num(1.1 - 1); // 0.1
    log_num(checksum(text, strlen(text))); // 1992
    log_hex(crc(text, strlen(text))); // 0x9854
    log_bytes(text);  // "\x41\x42...\xb7\xb8"

    memoryview(text, strlen(text), 8, 0x1000); // 00001000 41 42 43 ...

    double array[] = {3, 1, 4, 1.59};
    int matrix[2][3] = {3, 1, 4, 1, 5, 9};

    log_arr(array);
    log_mat(matrix);
}


/* end of header */

#endif  /* __LOG_H__ */
