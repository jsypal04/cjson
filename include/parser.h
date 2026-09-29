#pragma once

#include "json.h"
#include "lexer.h"
#include <exception>
#include <vector>

class Parser {
    public:
        Parser(std::string json);

        std::map<std::string, Value> parse();

    private:
        Lexer lexer;

        std::map<std::string, Value> parse_object();
        std::vector<Value> parse_array();
        Value parse_value();
};

class ParserException : std::exception {
    public:
    const char* what() const noexcept override {
        return "JSON parsing failed";
    }
};
