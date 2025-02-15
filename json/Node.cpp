#include "Node.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "Parser.h"

using namespace jsonParser::json;

Node::Node() : type(JSON_NULL) {}

Node::Node(Type type_) : type(type_) {
    if (type == JSON_NULL) {

    } else if (type == JSON_BOOL) {
        value = false;
    } else if (type == JSON_INT) {
        value = 0;
    } else if (type == JSON_DOUBLE) {
        value = 0.0;
    } else if (type == JSON_ARRAY) {
        value = JsonArray();
    } else if (type == JSON_OBJECT) {
        value = JsonObject();
    } else {
        throw std::logic_error("Type Error: the 'type' must be one of the following options: ['JSON_NULL', 'JSON_BOOL', 'JSON_INT', 'JSON_DOUBLE', 'JSON_ARRAY', 'JSON_OBJECT']");
    }
}

Node::Node(JsonBool value_) : type(JSON_BOOL), value(value_) {}

Node::Node(JsonInt value_) : type(JSON_INT), value(value_) {}

Node::Node(JsonDouble value_) : type(JSON_DOUBLE), value(value_) {}

Node::Node(const JsonString &value_) : type(JSON_STRING), value(value_) {}

Node::Node(const char *value_) : Node(JsonString(value_)) {}

Node::operator bool() {
    return std::get<JsonBool>(value);
}

Node::operator int() {
    return std::get<JsonInt>(value);
}

Node::operator double() {
    return std::get<JsonDouble>(value);
}

Node::operator std::string() {
    return std::get<JsonString>(value);
}

void Node::pushBack(const Node &node) {
    std::get<JsonArray>(value).push_back(node);
}

Node &Node::operator[](int idx) {
    return std::get<JsonArray>(value)[idx];
}

Node Node::operator[](int idx) const {
    return std::get<JsonArray>(value)[idx];
}

Node &Node::operator[](const std::string &key) {
    return std::get<JsonObject>(value)[key];
}

Node Node::operator[](const std::string &key) const {
    auto tmp = std::get<JsonObject>(value);
    return tmp[key];
}

Node &Node::operator[](const char *key) {
    return (*this)[std::string(key)];
}

Node Node::operator[](const char *key) const {
    return (*this)[std::string(key)];
}

size_t Node::size() const {
    if (type == JSON_ARRAY) {
        return std::get<JsonArray>(value).size();
    }
    if (type == JSON_OBJECT) {
        return std::get<JsonObject>(value).size();
    }
    throw std::logic_error("Type Error: Only 'JSON_ARRAY' and 'JSON_OBJECT' can call 'size()'");
}

Type Node::getType() const {
    return type;
}

std::string Node::str() const {
    if (type == JSON_NULL) {
        return "null";
    } else if (type == JSON_BOOL) {
        return std::get<JsonBool>(value) ? "true" : "false";
    } else if (type == JSON_INT) {
        return std::to_string(std::get<JsonInt>(value));
    } else if (type == JSON_DOUBLE) {
        return std::to_string(std::get<JsonDouble>(value));
    } else if (type == JSON_STRING) {
        return '"' + std::get<JsonString>(value) + '"';
    } else if (type == JSON_ARRAY) {
        std::string ret;
        const auto &array = std::get<JsonArray>(value);
        ret += '[';
        for (size_t i = 0; i < size(); i++) {
            ret += array[i].str();
            if (i + 1 != size()) {
                ret += ',';
            }
        }
        ret += ']';
        return ret;
    } else {
        std::string ret;
        const auto &object = std::get<JsonObject>(value);
        ret += '{';
        for (auto it = object.begin(); it != object.end(); it++) {
            if (it != object.begin()) {
                ret += ',';
            }
            ret += '"' + it->first + '"';
            ret += ':';
            ret += it->second.str();
        }
        ret += '}';
        return ret;
    }
}

void Node::parse(const std::string &s) {
    Parser parser;
    parser.load(s);
    *this = parser.parse();
}

void Node::parse(const std::ifstream &fin) {
    std::stringstream ss;
    ss << fin.rdbuf();
    parse(ss.str());
}

std::ostream &jsonParser::json::operator<<(std::ostream &os, const Node &node) {
    os << node.str();
    return os;
}
