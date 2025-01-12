#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

typedef char* str_t;

#define str_protect(s) char s##_[2] = {0}; if (!s) s = s##_ // NULL convert to ""

bool str_lt(str_t string, str_t other);
bool str_le(str_t string, str_t other);
bool str_gt(str_t string, str_t other);
bool str_ge(str_t string, str_t other);
bool str_eq(str_t string, str_t other);
bool str_ne(str_t string, str_t other);
bool str_in(str_t string, str_t other);
bool str_endswith(str_t string, str_t other);
bool str_startswith(str_t string, str_t other);

int str_len(str_t string);
int str_find(str_t string, str_t sub);
int str_count(str_t string, str_t sub);

str_t str_init(int length);
str_t str_add(str_t string, str_t other);
str_t str_mul(str_t string, int count);
str_t str_copy(str_t string);
str_t str_lower(str_t string);
str_t str_upper(str_t string);
str_t str_slice(str_t string, int start, int end);
str_t str_split(str_t string, str_t sep, int index);
str_t str_strip(str_t string, str_t chars);
str_t str_replace(str_t string, str_t find, str_t repl);

// str_join(int count, str_t string, ...);
// str_findn(str_t string, str_t sub, int index);

bool str_lt(str_t string, str_t other) {
    str_protect(string);
    str_protect(other);
    return strcmp(string, other) < 0;
}

bool str_le(str_t string, str_t other) {
    str_protect(string);
    str_protect(other);
    return strcmp(string, other) <= 0;
}

bool str_gt(str_t string, str_t other) {
    return !str_le(string, other);
}

bool str_ge(str_t string, str_t other) {
    return !str_lt(string, other);
}

bool str_eq(str_t string, str_t other) {
    str_protect(string);
    str_protect(other);
    return strcmp(string, other) == 0;
}

bool str_ne(str_t string, str_t other) {
    return !str_eq(string, other);
}

bool str_in(str_t string, str_t other) {
    str_protect(string);
    str_protect(other);
    return strstr(other, string) != 0;
}

bool str_endswith(str_t string, str_t other) {
    str_protect(string);
    str_protect(other);
    int len_string = strlen(string);
    int len_other = strlen(other);
    if (len_string < len_other) {
        return 0;
    } else {
        return memcmp(string + len_string - len_other, other, len_other) == 0;
    }
}

bool str_startswith(str_t string, str_t other) {
    str_protect(string);
    str_protect(other);
    int len_string = strlen(string);
    int len_other = strlen(other);
    if (len_string < len_other) {
        return 0;
    } else {
        return memcmp(string, other, len_other) == 0;
    }
}

int str_len(str_t string) {
    str_protect(string);
    return strlen(string);
}

int str_find(str_t string, str_t sub) {
    str_protect(string);
    str_protect(sub);
    str_t found;
    if (found = strstr(string, sub)) {
        return found - string;
    } else {
        return -1;
    }
}

int str_count(str_t string, str_t sub) {
    str_protect(string);
    str_protect(sub);
    int len_sub = strlen(sub);
    str_t temp = string;
    int index = 0, count = 0;
    while (1) {
        if ((index = str_find(temp, sub)) == -1) {
            return count;
        } else {
            temp += index + len_sub;
            count++;
        }
    }
}

str_t str_init(int length) {
    return calloc(1, length + 1);
}

str_t str_add(str_t string, str_t other) {
    str_protect(string);
    str_protect(other);
    str_t ret = str_init(strlen(string) + strlen(other));
    return strcat(strcat(ret, string), other);
}

str_t str_mul(str_t string, int count) {
    str_protect(string);
    count = count > 0 ? count : 0;
    str_t ret = str_init(strlen(string) * count);
    for (int i = 0; i < count; i++) {
        strcat(ret, string);
    }
    return ret;
}

str_t str_copy(str_t string) {
    return str_mul(string, 1);
}

str_t str_lower(str_t string) {
    int len = strlen(string);
    char* result = str_init(len);
    for (int i = 0; i < len; i++) {
        result[i] = tolower(string[i]);
    }
    return result;
}

str_t str_upper(str_t string) {
    int len = strlen(string);
    char* result = str_init(len);
    for (int i = 0; i < len; i++) {
        result[i] = toupper(string[i]);
    }
    return result;
}

str_t str_slice(str_t string, int start, int end) {
    str_protect(string);
    if (!string) return 0;
    int len = strlen(string);
    if (start < 0) start += len;
    if (end < 0) end += len;
    if (start < 0) start = 0;
    if (end > len) end = len;
    if (end < start) end = start;
    str_t ret = str_init(end - start);
    memcpy(ret, string + start, end - start);
    return ret;
}

str_t str_split(str_t string, str_t sep, int index) {
    str_protect(string);
    str_protect(sep);
    int len_sep = strlen(sep);
    int count = str_count(string, sep) + 1;
    if (index < 0) index += count;
    if (index < 0 || index > count) return 0;
    int start = 0, end = 0;
    str_t temp = string;
    for (int i = 0; i < index; i++) {
        int idx = str_find(temp, sep);
        temp += idx + len_sep;
    }
    start = temp - string;
    int idx = str_find(temp, sep);
    if (idx == -1) {
        end = strlen(string);
    } else {
        end = start + idx;
    }
    return str_slice(string, start, end);
}

str_t str_strip(str_t string, str_t chars) {
    str_protect(string);
    str_protect(chars);
    int len = strlen(string);
    int start, end;
    for (start = 0; start < len; start++) {
        if (!strchr(chars, string[start])) {
            break;
        }
    }
    for (end = len - 1; end >= start; end--) {
        if (!strchr(chars, string[end])) {
            break;
        }
    }
    return str_slice(string, start, end + 1);
}

str_t str_replace(str_t string, str_t find, str_t repl) {
    str_protect(string);
    str_protect(find);
    str_protect(repl);

    // calc basic infos
    int count = str_count(string, find);
    int len_string = strlen(string);
    int len_find = strlen(find);
    int len_repl = strlen(repl);

    // init new string
    int len_result = len_string + count * (len_repl - len_find);
    str_t result = str_init(len_result);

    // fill new string
    str_t ptr_string = string, ptr_result = result;
    for (int i = 0; i < count; i++) {
        int len_match = str_find(ptr_string, find);

        // fill same part
        memcpy(ptr_result, ptr_string, len_match);
        ptr_string += len_match;
        ptr_result += len_match;

        // fill repl part
        memcpy(ptr_result, repl, len_repl);
        ptr_string += len_find;
        ptr_result += len_repl;
    }
    // fill last part
    int len_last = string + len_string - ptr_string;
    memcpy(ptr_result, ptr_string, len_last);

    return result;
}

double str_to_num(str_t string) {
    str_protect(string);
    return atof(string);
}

str_t str_from_num(double num) {
    str_t result = str_init(64);
    sprintf(result, "%.15g", num);
    return result;
}

int str_test() {

    int last_test = 0;
    #define assert_print(cond, fmt, expr, result) if (cond) printf("."); else printf(fmt + !last_test, expr, result); last_test = cond
    #define assert_int(expr, value) assert_print(expr == value,        "\n%s == %d\n",     #expr, expr)
    #define assert_str(expr, value) assert_print(!strcmp(expr, value), "\n%s == \"%s\"\n", #expr, expr)

    printf("---- STRING TEST START ----\n");

    assert_int(str_eq("Hello", "Hello"), 1);
    assert_int(str_eq("Hello", "World"), 0);

    assert_int(str_in("Hello", "World"), 0);
    assert_int(str_in("Hello", "HelloWorld"), 1);
    assert_int(str_in("HelloWorld", "Hello"), 0);

    assert_int(str_endswith("HelloWorld", "Hello"), 0);
    assert_int(str_endswith("HelloWorld", "World"), 1);
    assert_int(str_startswith("HelloWorld", "Hello"), 1);
    assert_int(str_startswith("HelloWorld", "World"), 0);

    assert_int(str_len("Hello, World!"), 13);

    assert_int(str_find("Hello, World!", "Hello"), 0);
    assert_int(str_find("Hello, World!", "World"), 7);
    assert_int(str_find("Hello, World!", "Universe"), -1);

    assert_int(str_count("Hello, World!", "Universe"), 0);
    assert_int(str_count("Hello, World!", "World"), 1);
    assert_int(str_count("Hello, World!", "o"), 2);
    assert_int(str_count("Hello, World!", "l"), 3);

    assert_str(str_add("Hello", "World"), "HelloWorld");
    assert_str(str_mul("Hello", 3), "HelloHelloHello");

    assert_str(str_lower("Hello"), "hello");
    assert_str(str_upper("Hello"), "HELLO");

    assert_str(str_slice("Hello, World!", 0, 5), "Hello");
    assert_str(str_slice("Hello, World!", -6, -1), "World");
    assert_str(str_slice("Hello, World!", 6, 6), "");
    assert_str(str_slice("Hello, World!", 6, 4), "");

    assert_str(str_split("Hello World and Universe", " ", 0), "Hello");
    assert_str(str_split("Hello World and Universe", " ", 1), "World");
    assert_str(str_split("Hello World and Universe", " ", -1), "Universe");
    assert_str(str_split("Hello World", " ", 1), "World");
    assert_str(str_split("HelloWorld", "o", 0), "Hell");
    assert_str(str_split("HelloWorld", "o", 1), "W");
    assert_str(str_split("HelloWorld", "o", -1), "rld");

    assert_str(str_strip("Hello", ""), "Hello");
    assert_str(str_strip("Hello", " "), "Hello");
    assert_str(str_strip("  Hello   ", " "), "Hello");
    assert_str(str_strip("  Hello!   ", " !"), "Hello");
    assert_str(str_strip("    ", " "), "");

    assert_str(str_replace("Hello, World!", "World", "Universe"), "Hello, Universe!");
    assert_str(str_replace("Hello, World!", "Universe", "World"), "Hello, World!");
    assert_str(str_replace("Hello, World!", "l", "*"), "He**o, Wor*d!");

    printf("\n----  STRING TEST END  ----\n");

    #undef assert_int
    #undef assert_str

    return 0;
}

int main() {
    return str_test();
}
