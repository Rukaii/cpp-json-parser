#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <variant>

namespace jsonParser {
namespace json {

class Node;

enum Type {
    JSON_NULL,
    JSON_BOOL,
    JSON_INT,
    JSON_DOUBLE,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
};

using JsonNull = std::monostate;
using JsonBool = bool;
using JsonInt = int;
using JsonDouble = double;
using JsonString = std::string;
using JsonArray = std::vector<Node>;
using JsonObject = std::unordered_map<std::string, Node>;

using Value = std::variant<JsonNull, JsonBool, JsonInt, JsonDouble, JsonString, JsonArray, JsonObject>;

class Node {
public:
    Node();
    Node(Type type_);
    Node(JsonBool value_);
    Node(JsonInt value_);
    Node(JsonDouble value_);
    Node(const JsonString &value_);
    Node(const char *value_);

    operator JsonBool();
    operator JsonInt();
    operator JsonDouble();
    operator JsonString();

    void pushBack(const Node &node);
    Node &operator[](int idx);
    Node operator[](int idx) const;

    Node &operator[](const std::string &key);
    Node operator[](const std::string &key) const;
    Node &operator[](const char *key);
    Node operator[](const char *key) const;

    size_t size() const;
    Type getType() const;

    std::string str() const;

    void parse(const std::string &s);
    void parse(const std::ifstream &fin);

private:
    Type type;
    Value value;
};

std::ostream &operator<<(std::ostream &os, const Node &node);

}
}
