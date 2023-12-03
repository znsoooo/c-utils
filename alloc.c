#define mmry_pool_len 512

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define expr(v) printf(#v" = %g\n", (double)v)
#define puts(v) printf(#v" = \"%s\"\n", v)
#define print_ptr(ptr) printf(#ptr" @ 0x%08x\n", ptr ? ptr - mmry_pool : 0)

char mmry_pool[mmry_pool_len] = {0};

typedef struct mmry_block {
    size_t size; // sum of block and data size
    size_t prev; // distance from prev to here
    size_t next; // distance from here to next
} mmry_block;

#define mmry_block_offset(ptr, offset) ((mmry_block*)(((char*)ptr)+(offset)))

void* mmry_malloc(size_t size)
{
    mmry_block *prev, *next, *block = (mmry_block*)mmry_pool;
    if (block->next == 0) {
        block->size = sizeof(mmry_block);
        block->next = sizeof(mmry_block);
        block = mmry_block_offset(block, sizeof(mmry_block));
        block->prev = sizeof(mmry_block);
    }

    size_t block_size = size + sizeof(mmry_block);

    while ((size_t)block + block_size < (size_t)mmry_pool + mmry_pool_len) {
        if (block->next == 0) {
            block->size = block->next = block_size;
            next = mmry_block_offset(block, block->next);
            next->prev = block_size;
            next->next = 0;
            return mmry_block_offset(block, sizeof(mmry_block));

        } else if (block_size <= block->next - block->size) {
            prev = block;
            next = mmry_block_offset(prev, prev->next);
            block = mmry_block_offset(prev, prev->size);
            block->size = block_size;
            block->next = next->prev = prev->next - prev->size;
            block->prev = prev->next = prev->size;
            return mmry_block_offset(block, sizeof(mmry_block));

        } else {
            block = mmry_block_offset(block, block->next);
        }
    }

    return 0;
}

void* mmry_calloc(size_t size)
{
    void* ptr = mmry_malloc(size);
    if (ptr != NULL) {
        memset(ptr, 0, size);
    }
    return ptr;
}

void* mmry_realloc(void* ptr, size_t size)
{
    if (ptr) {
        mmry_block* block = mmry_block_offset(ptr, -sizeof(mmry_block));
        if (size <= block->size) {
            block->size = size;
        } else {
            ptr = 0;
        }
    }
    return ptr;
}

void mmry_free(void* ptr)
{
    if (ptr) {
        mmry_block* block = mmry_block_offset(ptr, -sizeof(mmry_block));
        if (block->prev) {
            mmry_block* prev = mmry_block_offset(block, -block->prev);
            prev->next += block->next;
        }
        if (block->next) {
            mmry_block* next = mmry_block_offset(block, block->next);
            next->prev += block->prev;
        }
    }
}

void mmry_free_many(void* ptr, ...) {
    va_list args;
    va_start(args, ptr);
    while (ptr) {
        mmry_free(ptr);
        ptr = va_arg(args, void*);
    }
    va_end(args);
}

void mmry_clear()
{
    memset(mmry_pool, 0, mmry_pool_len);
}

void mmry_view()
{
    unsigned char* ptr = mmry_pool;
    for (size_t i = 0; i < mmry_pool_len; i++) {
        if (i % 16 == 0) {
            printf("0x%08x ", i);
        }
        if (ptr[i]) {
            printf("%02x ", ptr[i]);
        } else {
            printf("__ ");
        }
        if ((i + 1) % 16 == 0 || i == mmry_pool_len - 1) {
            printf("\n");
        }
    }
    printf("\n");
}

void mmry_check(char* var, void* ptr)
{
    if (ptr) {
        mmry_block* block = mmry_block_offset(ptr, -sizeof(mmry_block));
        printf("0x%08x %s = {size: %2d, prev: %2d, next: %2d, data: \"%s\"}\n", (size_t)ptr - (size_t)mmry_pool, var, block->size, block->prev, block->next, ptr);
    } else {
        printf("0x%08x %s is NULL\n", 0, var);
    }
}
#define mmry_check(v) mmry_check(#v, v)

void mmry_test()
{
    char *s1 = 0, *s2 = 0, *s3 = 0, *s4 = 0, *s5 = 0;

    #define test_check() {mmry_check(s1);mmry_check(s2);mmry_check(s3);mmry_check(s4);mmry_check(s5);}if(0)

    // first malloc
    s1 = mmry_malloc(30);
    strcpy(s1, "first string");
    test_check();
    printf("----------\n");

    // second malloc
    s2 = mmry_malloc(20);
    strcpy(s2, "second string");
    test_check();
    printf("----------\n");

    // free and malloc a small one
    mmry_free(s1);
    s3 = mmry_malloc(20);
    strcpy(s3, "third string");
    test_check();
    printf("----------\n");

    // free and malloc a big one
    mmry_free(s1);
    s4 = mmry_malloc(40);
    strcpy(s4, "forth string");
    test_check();
    printf("----------\n");

    // free last and malloc
    mmry_free(s4);
    s5 = mmry_malloc(50);
    strcpy(s5, "fifth string");
    test_check();
    printf("----------\n");

    // free many
    mmry_free_many(s1, s2, s3, s4, 0);
    test_check();
    printf("----------\n");

    // free all
    mmry_clear();
    test_check();

    #undef test_check
}

int main()
{
    mmry_test();
    return 0;
}
