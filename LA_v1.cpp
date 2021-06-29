//
//  main.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 19.04.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include <iostream>
#include <stdio.h>

#define buf_size 50

enum type_of_lex {
    LEX_NULL, LEX_FIN,
    LEX_NUM, LEX_IDENT,
    LEX_PROGRAM, LEX_AND, LEX_OR, LEX_NOT, LEX_IF, LEX_CASE, LEX_OF, LEX_END, LEX_DO, LEX_WHILE, LEX_FOR, LEX_UNTIL, LEX_CONTINUE, LEX_BREAK, LEX_TRUE, LEX_FALSE,
    LEX_INT, LEX_REAL, LEX_BOOLEAN, LEX_STRING,
    LEX_SEMICOLON, LEX_COMMA, LEX_OPEN_PAREN, LEX_CLOSE_PAREN, LEX_OPEN_BRACES, LEX_CLOSE_BRACES, LEX_PLUS, LEX_MINUS, LEX_MUL, LEX_DIV, LEX_BIGGER, LEX_LESS, LEX_EQUAL, LEX_BIGGER_EQUAL, LEX_LESS_EQUAL, LEX_NOT_EQUAL
};
const char * type_of_lex_readable[] = {
    "LEX_NULL", "LEX_FIN",
    "LEX_NUM", "LEX_IDENT",
    "LEX_PROGRAM", "LEX_AND", "LEX_OR", "LEX_NOT", "LEX_IF", "LEX_CASE", "LEX_OF", "LEX_END", "LEX_DO", "LEX_WHILE", "LEX_FOR", "LEX_UNTIL", "LEX_CONTINUE", "LEX_BREAK", "LEX_TRUE", "LEX_FALSE",
    "LEX_INT", "LEX_REAL", "LEX_BOOLEAN", "LEX_STRING",
    "LEX_SEMICOLON", "LEX_COMMA", "LEX_OPEN_PAREN", "LEX_CLOSE_PAREN", "LEX_OPEN_BRACES", "LEX_CLOSE_BRACES", "LEX_PLUS", "LEX_MINUS", "LEX_MUL", "LEX_DIV", "LEX_BIGGER", "LEX_LESS", "LEX_EQUAL", "LEX_BIGGER_EQUAL", "LEX_LESS_EQUAL", "LEX_NOT_EQUAL"
};

//---------------------------------| Lex |-------------------------------------
class Lex {
    type_of_lex lex_type;
    char lex_value[20];
    
public:
    
    Lex(type_of_lex type = LEX_NULL, const char * value = NULL) {
        lex_type = type;
        if (value)
            strcpy(lex_value, value);
        else lex_value[0] = '\0';
    }
    
    type_of_lex get_type() { return lex_type; }
    char * get_value() { return lex_value; }
    friend std::ostream & operator << (std::ostream & s, Lex lex) {
        s << "(" << type_of_lex_readable[lex.lex_type] << ", " << lex.lex_value << "), ";
        return s;
    }
};

//-------------------------------| Lexer |--------------------------------------
class Lexer {
    FILE * fp;
    char c;
    char buf[buf_size];
    int buf_top;
    enum state {H, LITERAL, NUM, DELIM};
    state CS;
    static const char * Reserved_Table[];
    static const char * Delim_Table[];
    
    void read_char() { c = fgetc(fp); }
    
    void clear() {
        buf_top = 0;
        for (int i = 0; i < buf_size; i++)
            buf[i] = '\0';
    }
    
    int look(const char * buf, const char ** list) {
        int i = 0;
        while(list[i]) {
            if (!strcmp(buf, list[i]))
                return i + 1; //  + 1 ???
            i++;
        }
        return 0;
    }
    
    void add_char() { buf[buf_top++] = c; }
    
public:
    
    Lexer(const char * program_file_path) {
        fp = fopen(program_file_path, "r");
        if (!fp)
            std::cout << "file not found!!!\n";
        CS = H;
        buf_top = 0;
        read_char();
    }
    
    Lex get_lex() {
        CS = H;
        int index;
        while(true) {
            switch (CS) {
                case H:
                    if ((c == ' ') || (c == '\n')) {
                        read_char();
                        break;
                    }
                    else
                    if (isalpha(c)) {
                        clear(); add_char(); read_char(); CS = LITERAL;
                        break;
                    }
                    else
                    if (isdigit(c)) {
                        clear(); add_char(); read_char(); CS = NUM;
                        break;
                    }
                    else
                        clear(); add_char(); read_char(); CS = DELIM;
                    break;
                    
                case LITERAL:
                    if (isalpha(c) || isdigit(c)) {
                        add_char(); read_char();
                    }
                    else
                    if ((index = look(buf, Reserved_Table))) return Lex(type_of_lex(3 + index)); // change when new Lex is added in type_of_lex!!!!!
                    else return Lex(LEX_IDENT, buf);
                    break;
                
                case NUM:
                    if (isdigit(c)) {
                        add_char(); read_char();
                    }
                    else return Lex(LEX_NUM, buf);
                    break;
                
                case DELIM:
                    if ((index = look(buf, Delim_Table))) return Lex(type_of_lex(23 + index)); // change when new Lex is added in type_of_lex!!!!!
                break;
                    
                default:
                    break;
            }
        }
    }
};
const char * Lexer::Reserved_Table[] = {"program", "and", "or", "not", "if", "case", "of", "end", "do", "while", "for", "until", "continue", "break", "true", "false", "int", "real", "boolean", "string", NULL}; // change when new Lex is added in type_of_lex!!!!!
const char * Lexer::Delim_Table[] = {";", ",", "(", ")", "{", "}", "+", "-", "*", "/", ">", "<", "=", ">=", "<=", "!=", NULL}; // change when new Lex is added in type_of_lex!!!!!



int main(int argc, const char * argv[]) {
    
    Lexer lexer("program.txt");
    for (int i = 0; i < 5; i++) {
        Lex lex = lexer.get_lex();
        std::cout << lex;
    }
    std::cout << "\n";
    
    return 0;
}
