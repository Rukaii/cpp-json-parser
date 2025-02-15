#pragma once

#include <json/Node.h>

namespace jsonParser {
namespace json {

class Parser {
public:
    Parser();
    void load(const std::string &str_);
    Node parse();

private:
    Node getNextNode();
    void skipWhiteChar();
    Node getNull();
    Node getBool();
    Node getNumber();
    Node getString();
    Node getArray();
    Node getObject();

private:
    std::string str;
    size_t idx;
};

} // json
} // jsonParser

