#include <iostream>
#include "src/Lexer/headers/Lexer.h"
//#include "src/Parcer/headers/Parcer.h"

class Interf{

public:

    virtual void req() = 0;
};


class Isec: public Interf{

public:

    virtual void req() override{ printf("second\n"); }
};

class Der: public Isec{

public:

    //void req(){ printf("derived\n"); }
};

int main(int, char**) {
    
    Der tmp;
    tmp.req();

    Interf* base = &tmp;

    //std::list<lexem> tokens;
    //Lexer tmp(tokens);
    /*std::list<lexem>::iterator iter = tokens.begin();
    Parcer parcer;
    FILE* input = fopen("../bin/input.txt", "r");

    parcer.parse_file(input, tokens);    

    for (iter = tokens.begin(); iter != tokens.end(); iter++){

        printf("|%s|->", iter->lexem);
    }*/
}
