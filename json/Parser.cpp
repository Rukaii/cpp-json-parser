#include "Parser.h"

#include <cassert>
#include <stdexcept>
using namespace jsonParser::json;

Parser::Parser() : str(), idx() {}

void Parser::load(const std::string &str_) {
    str = str_;
    idx = 0;
}

Node Parser::parse() {
    skipWhiteChar();
    if (idx >= str.size()) {
        throw std::logic_error("Empty json file");
    }
    Node ret;
    if (str[idx] == 'n') {
        ret = getNull();
    } else if (str[idx] == 't' || str[idx] == 'f') {
        ret = getBool();
    } else if (str[idx] == '-' || isdigit(str[idx])) {
        ret = getNumber();
    } else if (str[idx] == '"') {
        ret = getString();
    } else if (str[idx] == '[') {
        ret = getArray();
    } else if (str[idx] == '{') {
        ret = getObject();
    }
    skipWhiteChar();
    if (idx != str.size()) {
        throw std::logic_error("Extra content at the end of the file");
    }
    return ret;
}

Node Parser::getNextNode() {
    skipWhiteChar();
    if (idx >= str.size()) {
        throw std::logic_error("Incomplete file content");
    }
    if (str[idx] == 'n') {
        return getNull();
    } else if (str[idx] == 't' || str[idx] == 'f') {
        return getBool();
    } else if (str[idx] == '-' || isdigit(str[idx])) {
        return getNumber();
    } else if (str[idx] == '"') {
        return getString();
    } else if (str[idx] == '[') {
        return getArray();
    } else if (str[idx] == '{') {
        return getObject();
    }
    throw std::logic_error("Found an unexpected character");
}

void Parser::skipWhiteChar() {
    while (idx < str.size()
        && (str[idx] == ' ' || str[idx] == '\n' || str[idx] == '\r' || str[idx] == '\t')) {
        idx++;
    }
}

Node Parser::getNull() {
    if (str.compare(idx, 4, "null") == 0) {
        idx += 4;
        return Node(JSON_NULL);
    }
    throw std::logic_error("Found an illegal value. Do you mean 'null'?");
}

Node Parser::getBool() {
    if (str[idx] == 't') {
        if (str.compare(idx, 4, "true") == 0) {
            idx += 4;
            return Node(true);
        }
        throw std::logic_error("Found an illegal value. Do you mean 'true'?");
    } else {
        if (str.compare(idx, 5, "false") == 0) {
            idx += 5;
            return Node(false);
        }
        throw std::logic_error("Found an illegal value. Do you mean 'false'?");
    }
}

Node Parser::getNumber() {
    int beg = idx;
    if (str[idx] == '-') {
        idx++;
    }
    while (idx < str.size() && isdigit(str[idx])) {
        idx++;
    }
    if (idx < str.size() && str[idx] == '.') {
        idx++;
        while (idx < str.size() && isdigit(str[idx])) {
            idx++;
        }
        return std::stod(str.c_str() + beg);
    } else {
        return std::stoi(str.c_str() + beg);
    }
}

Node Parser::getString() {
    std::string ret;
    idx++;
    while (true) {
        if (idx >= str.size()) {
            throw std::logic_error(R"(Expect a '"' at the end of the string)");
        }
        if (str[idx] == '"') {
            idx++;
            skipWhiteChar();
            break;
        }
        char c = str[idx++];
        ret += c;
        if (c == '\\') {
            if (idx >= str.size()) {
                throw std::logic_error(R"(Expect a '"' at the end of the string)");
            }
            c = str[idx++];
            if (c == 'b' || c == 't' || c == 'n' || c == 'f' || c == 'r' || c == '"' || c == '\\' || c == 'u') {
                ret += c;
            } else {
                throw std::logic_error(R"(Don't use '\' directly in a string, use '\\')");
            }
        }
    }
    return Node(ret);
}

Node Parser::getArray() {
    Node array(JSON_ARRAY);
    idx++;
    skipWhiteChar();
    if (idx >= str.size()) {
        throw std::logic_error("Expect a ']' at the end of the array");
    }
    if (str[idx] == ']') {
        idx++;
        skipWhiteChar();
        return array;
    }
    while (true) {
        array.pushBack(getNextNode());
        skipWhiteChar();
        if (idx >= str.size()) {
            throw std::logic_error("Expect a ']' at the end of the array");
        } else if (str[idx] == ']') {
            idx++;
            skipWhiteChar();
            break;
        } else if (str[idx] == ',') {
            idx++;
            skipWhiteChar();
        } else {
            throw std::logic_error("Expect a ',' to separate the elements");
        }
    }
    return array;
}

Node Parser::getObject() {
    Node object(JSON_OBJECT);
    idx++;
    skipWhiteChar();
    if (idx >= str.size()) {
        throw std::logic_error("Expect a '}' at the end of the object");
    }
    if (str[idx] == '}') {
        idx++;
        skipWhiteChar();
        return object;
    }
    while (true) {
        Node key = getNextNode();
        if (key.getType() != JSON_STRING) {
            throw std::logic_error("The key must be a string");
        }
        skipWhiteChar();
        if (idx >= str.size()) {
            throw std::logic_error("This key does not have a value");
        } else if (str[idx] == ':') {
            idx++;
            skipWhiteChar();
        } else {
            throw std::logic_error("Expect a ':' after the key");
        }
        Node value = getNextNode();
        object[std::string(key)] = value;

        skipWhiteChar();
        if (idx >= str.size()) {
            throw std::logic_error("Expect a '}' at the end of the object");
        } else if (str[idx] == '}') {
            idx++;
            skipWhiteChar();
            break;
        } else if (str[idx] == ',') {
            idx++;
            skipWhiteChar();
        } else {
            throw std::logic_error("Expect a ',' to separate the elements");
        }
    }
    return object;
}
