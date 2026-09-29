#include "parser.h"
#include "lexer.h"
#include <iostream>


Parser::Parser(std::string source) : lexer(source) {
    // nothing else to do here
}

std::map<std::string, Value> Parser::parse() {
    lexer.lex();
    return parse_object();
}

std::map<std::string, Value> Parser::parse_object() {
    auto object = std::map<std::string, Value>();

    if (lexer.token != Token::LBrace) {
        throw ParserException();
    }

    lexer.lex();

    while (lexer.token == Token::String) {
        std::string key = lexer.lexeme;
        lexer.lex();
        
        if (lexer.token != Token::Colon) {
            throw ParserException();
        }

        lexer.lex();

        Value value = parse_value();
        object.emplace(key, value);

        // TODO: Should maybe verify that every entry is concluded with a comma except the last one.
        if (lexer.token == Token::Comma) {
            lexer.lex();
        }
    }

    if (lexer.token != Token::RBrace) {
        std::cout << "Lexeme = " << static_cast<int>(lexer.token) << '\n';
        throw ParserException();
    }

    lexer.lex();

    return object;
}

std::vector<Value> Parser::parse_array() {
    auto array = std::vector<Value>();

    if (lexer.token != Token::LBracket) {
        throw ParserException();
    }

    lexer.lex();

    // TODO: Should maybe verify that every entry is concluded with a comma except the last one.
    while (lexer.token != Token::RBracket) {
        Value value = parse_value();
        array.push_back(value);

        if (lexer.token == Token::Comma) {
            lexer.lex();
        }
    }

    lexer.lex();

    return array;
}

Value Parser::parse_value() {
    switch (lexer.token) {
        case Token::String: {
            Value parsed_value(lexer.lexeme);
            lexer.lex();
            return parsed_value;
        }
        case Token::Number: {
            Value parsed_value(std::stof(lexer.lexeme));
            lexer.lex();
            return parsed_value;
        }
        case Token::Bool: {
            Value parsed_value(lexer.lexeme == "true");
            lexer.lex();
            return parsed_value;
        }
        case Token::Null: {
            Value parsed_value(nullptr);
            lexer.lex();
            return parsed_value;
        }
        case Token::LBrace: {
            Value parsed_value(parse_object());
            return parsed_value;
        }
        case Token::LBracket: {
            Value parsed_value(parse_array());
            return parsed_value;
        }
        default:
            throw ParserException();
    }
}

