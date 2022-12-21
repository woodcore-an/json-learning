#include "woodjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */

// 如果宏里有多过一个语句（statement），就需要用 do {/*...*/} while(0) 包裹成单个语句
// 如果错误是由于程序员错误编码所造成的（例如传入不合法的参数），那么应用断言
// 如果错误是程序员无法避免，而是由运行时的环境所造成的，就要处理运行时错误（例如开启文件失败）
#define EXPECT(c, ch)    do { assert(*c->json == (ch)); c->json++; } while(0)

// 为了减少解析函数之间传递多个参数，我们把这些数据都放进一个 wood_context 结构体：
typedef struct
{
    const char* json;
}wood_context;

// 解析空白且剔除空白（通过移动指针p），由于这个函数是不会出现错误的，返回类型为 void。其他的解析函数会返回错误码，传递至顶层。
// ws = *(%x20 / %x09 / %x0A / %x0D)
//       空格符 / 制表符 / 换行符 / 回车符
static void wood_parse_whitespace(wood_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

// // 解析 null
// /* null = "null" */
// static int wood_parse_null(wood_context* c, wood_value* v) {
//     EXPECT(c, 'n');  // 利用宏（宏里面利用断言）判断第一个字符是否为 'n'，并跳到下一字符
//     if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
//         return WOOD_PARSE_INVALID_VALUE;
//     c->json += 3;
//     v->type = WOOD_NULL;
//     return WOOD_PARSE_OK;
// }

// // 解析 true
// /* null = "true" */
// static int wood_parse_true(wood_context* c, wood_value* v) {
//     EXPECT(c, 't');
//     if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
//         return WOOD_PARSE_INVALID_VALUE;
//     c->json += 3;
//     v->type = WOOD_TRUE;
//     return WOOD_PARSE_OK;
// }

// // 解析 false
// /* null = "false" */
// static int wood_parse_false(wood_context* c, wood_value* v) {
//     EXPECT(c, 'f');
//     if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
//         return WOOD_PARSE_INVALID_VALUE;
//     c->json += 4;
//     v->type = WOOD_FALSE;
//     return WOOD_PARSE_OK;
// }

// 重构合并以上的解析 null、true、false的代码为wood_parse_literal()
static int wood_parse_literal(wood_context* c, wood_value* v, const char * literal, wood_type type) {
    size_t i;  // 在 C 语言中，数组长度、索引值最好使用 size_t 类型，而不是 int 或 unsigned 。
    EXPECT(c, literal[0]);
    // 由于 true/false/null 的字符数量不一样，因此可以使用 for 循环来作比较，直至'\0'
    for (i = 0; literal[i + 1]; i++) {
        if (c->json[i] != literal[i + 1]) {
            return WOOD_PARSE_INVALID_VALUE;
        }
    }
    c->json += i;
    v->type = type;
    return WOOD_PARSE_OK;
}

// 解析值，根据剔除空白之后的第一个字符来决定怎么解析
static int wood_parse_value(wood_context* c, wood_value* v) {
    switch (*c->json) {
        // case 'n':  return wood_parse_null(c, v);
        // case 't':  return wood_parse_true(c, v);
        // case 'f':  return wood_parse_false(c, v);
        case 'n':  return wood_parse_literal(c, v, "null", WOOD_NULL);
        case 't':  return wood_parse_literal(c, v, "true", WOOD_TRUE);
        case 'f':  return wood_parse_literal(c, v, "false", WOOD_FALSE);
        case '\0': return WOOD_PARSE_EXPECT_VALUE;  // 一个 JSON 只含有空白
        default:   return WOOD_PARSE_INVALID_VALUE;
    }
}

/* 提示：这里应该是 JSON-text = ws value ws */
/* 以下实现没处理最后的 ws 和 WOOD_PARSE_ROOT_NOT_SINGULAR，所以关于 WOOD_PARSE_ROOT_NOT_SINGULAR 这个返回值的测试用例会过不了 */
/* 现在加上最后的 ws（即，解析第三部分的空白），加上之后测试用例则可以全部通过，可以和tutorial01进行对比 */
int wood_parse(wood_value* v, const char* json) {
    wood_context c;
    int ret;  // 声明了一个 int 型变量
    assert(v != NULL);
    c.json = json;
    v->type = WOOD_NULL;
    wood_parse_whitespace(&c);  // 解析空白
    if ((ret = wood_parse_value(&c, v)) == WOOD_PARSE_OK) {  // 解析值
        wood_parse_whitespace(&c);  // 如果可以解析出完整的有效的值，则再解析值后面是否是空白
        if (*c.json != '\0') {  // 如果解析完空白之后，后面的字符不是空字符'\0'的话，则说明后面还有其他值
            v->type = WOOD_NULL;  // 最后的空白之后还有其他值，需要将类型重置为WOOD_NULL
            ret = WOOD_PARSE_ROOT_NOT_SINGULAR;  // 返回值设置为 WOOD_PARSE_ROOT_NOT_SINGULAR
        }
    }
    return ret;
}

wood_type wood_get_type(const wood_value* v) {
    assert(v != NULL);
    return v->type;
}