#include <cctype>
#include <cstdio>
#include <stdexcept>

#include "lexer.h"

Lexer::Lexer(std::string json) {
    source = clear_spaces(json);
    next_unread_char = 1;
    next_char = json.at(0);
    end_of_src = false;
}

void Lexer::lex() {
    if (end_of_src) {
        lexeme = "\0";
        token = Token::End;
        return;
    }

    lexeme = std::string("");
    
    try {
        if (next_char == '"') {
            get_next_char();
            while (next_char != '"') {
                lexeme.push_back(next_char);
                get_next_char();
            }
            token = Token::String;
            get_next_char();
        } else if (std::isdigit(next_char) || next_char == '-') {
            lexeme.push_back(next_char);
            get_next_char();
            
            while (std::isdigit(next_char)) {
                lexeme.push_back(next_char);
                get_next_char();
            }

            if (next_char == '.') {
                lexeme.push_back(next_char);
                get_next_char();
                while (std::isdigit(next_char)) {
                    lexeme.push_back(next_char);
                    get_next_char();
                }
            }

            token = Token::Number;
        } else if (std::isalpha(next_char)) {
            lexeme.push_back(next_char);
            get_next_char();

            while (std::isalpha(next_char)) {
                lexeme.push_back(next_char);
                get_next_char();
            }

            if (lexeme == "true" || lexeme == "false") {
                token = Token::Bool;
            } else if (lexeme == "null") {
                token = Token::Null;
            } else {
                char buf[1024] = "\0";
                sprintf(buf, "Invalid identifier: %s", lexeme.c_str());
                throw std::runtime_error(buf);
            }
        } else if (next_char == '[') {
            lexeme.push_back('[');
            token = Token::LBracket;
            get_next_char();
        } else if (next_char == ']') {
            lexeme.push_back(']');
            token = Token::RBracket;
            get_next_char();
        } else if (next_char == '{') {
            lexeme.push_back('{');
            token = Token::LBrace;
            get_next_char();
        } else if (next_char == '}') {
            lexeme.push_back('}');
            token = Token::RBrace;
            get_next_char();
        } else if (next_char == ':') {
            lexeme.push_back(':');
            token = Token::Colon;
            get_next_char();
        } else if (next_char == ',') {
            lexeme.push_back(',');
            token = Token::Comma;
            get_next_char();
        } else {
            char buf[1024] = "\0";
            sprintf(buf, "Unexpected character: %c", next_char);
            throw std::runtime_error(buf);
        }
    } catch (std::out_of_range& e) {
        end_of_src = true;

        if (lexeme != "}" || token != Token::RBrace) {
            char buf[1024] = "\0";
            sprintf(buf, "Invalid final token: %s", lexeme.c_str());
            throw std::runtime_error(buf);
        }
    }
}

void Lexer::get_next_char() {
    next_char = source.at(next_unread_char);
    next_unread_char++;
}

std::string Lexer::clear_spaces(std::string json) {
    bool in_a_string = false;

    std::string cleared_source = std::string("");

    for (int i = 0; i < json.length(); i++) {
        char c = json.at(i);

        if (c == '"') in_a_string = !in_a_string;

        if (in_a_string) {
            cleared_source.push_back(c);
            continue;
        }

        if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
            continue;
        }

        cleared_source.push_back(c);
    }

    return cleared_source;
}
