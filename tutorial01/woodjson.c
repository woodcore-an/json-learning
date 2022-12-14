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

// 解析 null
/* null = "null" */
static int wood_parse_null(wood_context* c, wood_value* v) {
    EXPECT(c, 'n');  // 利用宏（宏里面利用断言）判断第一个字符是否为 'n'，并跳到下一字符
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return WOOD_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = WOOD_NULL;
    return WOOD_PARSE_OK;
}

// 解析值，根据剔除空白之后的第一个字符来决定怎么解析
static int wood_parse_value(wood_context* c, wood_value* v) {
    switch (*c->json) {
        case 'n':  return wood_parse_null(c, v);
        case '\0': return WOOD_PARSE_EXPECT_VALUE;  // 一个 JSON 只含有空白
        default:   return WOOD_PARSE_INVALID_VALUE;
    }
}

/* 提示：这里应该是 JSON-text = ws value ws */
/* 以下实现没处理最后的 ws 和 WOOD_PARSE_ROOT_NOT_SINGULAR */ 
int wood_parse(wood_value* v, const char* json) {
    wood_context c;
    assert(v != NULL);
    c.json = json;
    v->type = WOOD_NULL;
    wood_parse_whitespace(&c);
    return wood_parse_value(&c, v);
}

wood_type wood_get_type(const wood_value* v) {
    assert(v != NULL);
    return v->type;
}