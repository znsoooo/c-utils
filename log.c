#include "log.h"

int print_arr(char *name, int *nums, int len)
{
    if (name && strlen(name)) {
        printf("%s = ", name);
    }

    printf("[", name);
    for (int i = 0; i < len; i++) {
        printf(", %d" + !i * 2, nums[i]);
    }
    printf("]\n");

    return 0;
}

int print_mat(char *name, int *nums, int h, int w)
{
    if (name && strlen(name)) {
        printf("%s = ", name);
    }

    printf("[\n", name);
    for (int r = 0; r < h; r++) {
        printf("  [");
        for (int c = 0; c < w; c++) {
            printf(", %d" + !c * 2, nums[r * w + c]);
        }
        printf("],\n");
    }
    printf("]\n");

    return 0;
}

int print_bytes(char *name, u8 *data, int len)
{
    if (name && strlen(name)) {
        printf("%s = ", name);
    }

    printf("\"", name);
    for (int i = 0; i < len; i++) {
        printf("\\x%02x", data[i]);
    }
    printf("\"\n");

    return 0;
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
