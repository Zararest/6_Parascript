#include <iostream>
#include "src/Compiler/headers/Compiler.h"
#include "src/Lexer/headers/Lexer.h"


int main(int, char**){

    std::list<lexem> tokens;
    std::list<lexem>::iterator iter = tokens.begin();
    Parcer parcer;
    FILE* input = fopen("../bin/input.txt", "r");

    parcer.parse_file(input, tokens);    

    Lexer new_tree(tokens);

    Compiler new_compile(new_tree.get_syntax_tree());
}   

