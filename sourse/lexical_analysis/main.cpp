//
//  main.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 19.04.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "Lex.hpp"
#include "Lexer.hpp"
#include "Ident.hpp"
#include "Tabl_Ident.hpp"
#include "Exceptions.hpp"
#include "Poliz.hpp"
#include "Parser.hpp"
#include "Executer.hpp"

/*
 0 - no debug
 1 - with debug
*/
#define debug_TID   0
#define debug_POLIZ 0
#define debug_LEXER 0

//======================================================================================================================================================

int main(int argc, const char * argv[]) {
    
    const char * program_name = "program_struct.txt";
    
    Parser parser(program_name);
    try {
        parser.analyze();
        
        if (debug_POLIZ) {
            std::cout << std::endl << " <POLIZ>" << std::endl;
            parser.prog.print();
            std::cout << std::endl;
        }
        
        if (debug_TID) {
            std::cout << std::endl << " <TID>" << std::endl;
            parser.TID.print();
            std::cout << std::endl;
        }

        Executer executer(&(parser.TID), parser.struct_map);

        std::cout << " Program output:" << std::endl;
        executer.execute(parser.prog);
    }
    catch (LexExeption lex_exeption) {
        std::cerr << "unhandled error: " << lex_exeption.get_lex_type() << lex_exeption.get_message() << std::endl;
    }
    catch (Exeption exeption) {
        std::cerr << "unhandled error: " << exeption.get_message() << std::endl;
    }

    
    if (debug_LEXER) {
        std::cout << std::endl << " <LEXER>" << std::endl;
        Lexer debug_lexer(program_name);
        Lex debug_lex;
        do {
            debug_lex = debug_lexer.get_lex();
            std::cout << debug_lex;
        } while (debug_lex.get_type() != Lex::FIN);
        std::cout << std::endl;
    }
    
    return 0;
}
