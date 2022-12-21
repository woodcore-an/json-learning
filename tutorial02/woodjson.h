#ifndef WOODJSON_H__
#define WOODJSON_H__

// 由于 C 语言没有 C++ 的命名空间（namespace）功能，一般会使用项目的简写作为标识符的前缀。
// 通常枚举值用全大写（如WOOD_NULL），而类型及函数则用小写（如wood_type）
// JSON的7种数据类型
typedef enum { WOOD_NULL, WOOD_FALSE, WOOD_TRUE, WOOD_NUMBER, WOOD_STRING, WOOD_ARRAY, WOOD_OBJECT } wood_type;

// 声明 JSON 数据结构。
// JSON 是一个树形结构，我们最终需要实现一个树的数据结构，每个节点使用 wood_value 结构体表示，我们会称它为一个 JSON 值（JSON value）
// C 语言的结构体是以 struct X {} 形式声明的，定义变量时也要写成 struct X x;。为了使用方便，使用了typedef
typedef struct {
    wood_type type;
}wood_value;

// 返回值是以下枚举值，无错误会返回 WOOD_PARSE_OK
enum {
    WOOD_PARSE_OK = 0,            // 没有错误
    WOOD_PARSE_EXPECT_VALUE,      // 一个 JSON 只含有空白，例如""
    WOOD_PARSE_INVALID_VALUE,     // 若值不是那三种字面值，例如"nul"
    WOOD_PARSE_ROOT_NOT_SINGULAR  // 若一个值之后，在空白之后还有其他字符，例如"null x"
};


// 传入的 JSON 文本是一个 C 字符串（空结构字符串），由于不应该改动这个输入字符串，所以使用 const char* 类型
// 传入的根节点指针 v 是由使用方负责分配的，所以一般的用法是：
// wood_value v;
// const char json[] = ...;
// int ret = wood_parse(&v, json);
/**
 * @description: 用于解析json文本
 * @param {wood_value*} v：结构体指针，结构体内有一个枚举变量 wood_type，表示 JSON 的7种数据类型
 * @param {char*} json：json 文本，是一个 C 字符串
 * @return {int}
 */
int wood_parse(wood_value* v, const char* json);

// 需要一个访问结果的函数，就是获取其类型
/**
 * @description: 访问结果，即获取其类型
 * @param {wood_value*} v
 * @return {wood_type}：枚举变量，表示 JSON 的7种数据类型
 */
wood_type wood_get_type(const wood_value* v);


#endif /* WOODJSON_H__ */