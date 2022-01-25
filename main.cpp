#include <iostream>
#include "src/Parcer/headers/Parcer.h"

int main(int, char**) {
    
    std::list<lexem> tokens;
    std::list<lexem>::iterator iter = tokens.begin();
    Parcer parcer;
    FILE* input = fopen("../bin/input.txt", "r");

    parcer.parse_file(input, tokens);    

    for (iter = tokens.begin(); iter != tokens.end(); iter++){

        printf("|%s|->", iter->lexem);
    }
}
