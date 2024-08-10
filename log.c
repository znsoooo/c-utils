#include "log.h"

int sum(uint8_t* data, int length) {
    int sum = 0;
    for (int i = 0; i < length; ++i) {
        sum += data[i];
    }
    return sum;
}

uint16_t crc(uint8_t* data, int length)
{
    uint16_t crc = 0;
    while (length--) {
        crc ^= *(data++) << 8;
        for (int i = 0; i < 8; i++) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc = crc << 1;
        }
    }
    return crc;
}

int print_arr(int* nums, int length)
{
    printf("[");
    for (int i = 0; i < length; i++) {
        printf(", %d" + !i * 2, nums[i]);
    }
    printf("]\n");

    return 0;
}

int print_mat(int* nums, int height, int width)
{
    printf("[\n");
    for (int row = 0; row < height; row++) {
        printf("  [");
        for (int col = 0; col < width; col++) {
            printf(", %d" + !col * 2, nums[row * width + col]);
        }
        printf("],\n");
    }
    printf("]\n");

    return 0;
}

int print_bytes(uint8_t* data, int length)
{
    printf("\"");
    for (int i = 0; i < length; i++) {
        printf("\\x%02x", data[i]);
    }
    printf("\"\n");

    return 0;
}

void print_memory(uint8_t* data, int length, int width, int offset)
{
    for (int i = 0; i < length; i += width) {
        printf("%08X", i + offset);
        for (int j = 0; j < width && (i + j) < length; ++j) {
            printf(" %02X", data[i + j]);
        }
        printf("\n");
    }
}

char  log_buff[0x100000];  // 1MB
char *log_pbuff = log_buff;
int   log_reset = 1;

int log_print(char* format, ...)
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

void log_test()
{
    char text[] = "ABCDEFGH\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8";

    log_num(1.1 - 1); // 0.1
    log_num(sum(text, strlen(text))); // 1992
    log_hex(crc(text, strlen(text))); // 0x9854
    log_bytes(text);  // "\x41\x42...\xb7\xb8"

    print_memory(text, strlen(text), 8, 0x1000); // 00001000 41 42 43 ...

    int array[] = {3, 1, 4, 1, 5, 9};
    int matrix[2][3] = {3, 1, 4, 1, 5, 9};

    log_arr(array);
    log_mat(matrix);
}

// int main() { log_test(); }
