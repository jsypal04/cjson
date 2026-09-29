#pragma once

#include <string>

#define LOG_LOCATION() std::cout << "Running " << __FILE__ ":" << __LINE__ << '\n';

enum class Token {
    End,
    Null,
    String,
    Number,
    Bool,

    LBrace,
    RBrace,
    LBracket,
    RBracket,
    Colon,
    Comma
};

class Lexer {
    public:
        Token token;
        std::string lexeme;

        Lexer(std::string json);

        void lex();

    private:
        std::string source;
        int next_unread_char;
        char next_char;
        bool end_of_src;

        void        get_next_char();
        std::string clear_spaces(std::string json);
};
