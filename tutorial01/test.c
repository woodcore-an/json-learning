#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "woodjson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;


// 反斜线代表该行未结束，会串接下一行。
// 如果宏里有多过一个语句（statement），就需要用 do {/*...*/} while(0) 包裹成单个语句
#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " format "actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    } while(0)

// 提供了一个 EXPECT_EQ_INT(expect, actual) 的宏，每次使用这个宏时，如果 expect != actual （预期值不等于实际值），便会输出错误信息。
#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

static void test_parse_null() {
    wood_value v;
    v.type = WOOD_FALSE;
    EXPECT_EQ_INT(WOOD_PARSE_OK, wood_parse(&v, "null"));
    EXPECT_EQ_INT(WOOD_NULL, wood_get_type(&v));
}

static void test_parse_expect_value() {
    wood_value v;
    
    v.type = WOOD_FALSE;
    EXPECT_EQ_INT(WOOD_PARSE_EXPECT_VALUE, wood_parse(&v, ""));
    EXPECT_EQ_INT(WOOD_NULL, wood_get_type(&v));

    EXPECT_EQ_INT(WOOD_PARSE_EXPECT_VALUE, wood_parse(&v, " "));
    EXPECT_EQ_INT(WOOD_NULL, wood_get_type(&v));
}

static void test_parse_invalid_value() {
    wood_value v;
    v.type = WOOD_FALSE;
    EXPECT_EQ_INT(WOOD_PARSE_INVALID_VALUE, wood_parse(&v, "nul"));
    EXPECT_EQ_INT(WOOD_NULL, wood_get_type(&v));

    v.type = WOOD_FALSE;
    EXPECT_EQ_INT(WOOD_PARSE_INVALID_VALUE, wood_parse(&v, "?"));
    EXPECT_EQ_INT(WOOD_NULL, wood_get_type(&v));  
}

static void test_parse_root_not_singular() {
    wood_value v;
    v.type = WOOD_FALSE;
    EXPECT_EQ_INT(WOOD_PARSE_ROOT_NOT_SINGULAR, wood_parse(&v, "null x"));
    EXPECT_EQ_INT(WOOD_NULL, wood_get_type(&v));
}

static void test_parse() {
    test_parse_null();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
}

int main() {
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}
