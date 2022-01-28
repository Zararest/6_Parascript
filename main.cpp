#include <iostream>
#include "src/Lexer/headers/Lexer.h"
//#include "src/Parcer/headers/Parcer.h"

class Interf{

public:

    virtual void req() = 0;
    virtual ~Interf(){};
};

class Isec: public Interf{

public:

    virtual void req() override{ printf("second\n"); }
    ~Isec() override{};
};

class Der: public Isec{

public:

    //void req(){ printf("derived\n"); }
    ~Der() override{}
};

int main(int, char**){

    std::list<lexem> tokens;
    std::list<lexem>::iterator iter = tokens.begin();
    Parcer parcer;
    FILE* input = fopen("../bin/input.txt", "r");

    parcer.parse_file(input, tokens);    

    Lexer new_tree(tokens);
    new_tree.get_syntax_tree();
    new_tree.dump_graphviz();

    for (iter = tokens.begin(); iter != tokens.end(); iter++){

        //printf("|%s|->", iter->lexem);
    }
}   
