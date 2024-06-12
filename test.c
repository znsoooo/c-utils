#include "log.h"

void main()
{
    puts("start!");

    char s[10] = "Hello!";
    log_str(s);
    log_bytes(s);

    int n1 = 1024;
    int n2 = 299792458;
    double n3 = 3.14;
    log_num(n1);
    log_num(n2);
    log_num(n3);

    int arr[4] = {1, 0, 2, 4};
    log_arr(arr);

    int mat[3][4] = {{1, 0, 2}, {4, 0, 9, 6}};
    log_mat(mat);
}
