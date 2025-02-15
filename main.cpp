#include <iostream>
#include <fstream>
#include <chrono>

#include <json/Node.h>
using namespace jsonParser::json;

constexpr int N = 100000;

int main() {
    std::ifstream fin("test/test.json");
    std::stringstream ss;
    ss << fin.rdbuf();
    fin.close();

    const std::string &data = ss.str();
    Node node(JSON_OBJECT);

    auto start = std::chrono::steady_clock::now();
    for (int i = 1; i <= N; i++) {
        node.parse(data);
    }
    auto end = std::chrono::steady_clock::now();

    double second = std::chrono::duration<double>(end - start).count();
    double millisecond = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Total cost: " << second << "s" << std::endl;
    std::cout << "Average cost: " << millisecond / N <<  "ms" << std::endl;

    return 0;
}