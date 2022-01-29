#include <iostream>
//#include "src/Lexer/headers/Lexer.h"
//#include "src/Parcer/headers/Parcer.h"

class Fir_op;
class Sec_op;

class Ireq{

public:

    virtual void call_req(Fir_op* fir_op_req) = 0;
    virtual void call_req(Sec_op* sec_op_req) = 0;
};

class Req: public Ireq{

public:

    void call_req(Fir_op* fir_op_req){ printf("first\n"); };
    void call_req(Sec_op* sec_op_req){ printf("second\n"); };

};

class Inode{

public:

    virtual void get_req(Ireq* new_req) = 0;
};


class Ioperand: public Inode{

public:

    virtual void get_req(Ireq* new_req) = 0;
};


class Sec_op: public Ioperand{

public:

    void get_req(Ireq* new_req){

        new_req->call_req(this);
    }
};

class Fir_op final: public Ioperand{

    Inode* next = nullptr;

public:

    Fir_op(){

        next = new Sec_op;
    }

    void call_sec_req(Ireq* new_req){

        next->get_req(new_req);
    }

    void get_req(Ireq* new_req){

        new_req->call_req(this);
    }
};



int main(int, char**){

    Fir_op* fir = new Fir_op;
    Inode* inode = fir;

    Req* new_req = new Req;

    fir->call_sec_req(new_req);

    /*std::list<lexem> tokens;
    std::list<lexem>::iterator iter = tokens.begin();
    Parcer parcer;
    FILE* input = fopen("../bin/input.txt", "r");

    parcer.parse_file(input, tokens);    

    Lexer new_tree(tokens);
    new_tree.get_syntax_tree();
    new_tree.dump_graphviz();

    for (iter = tokens.begin(); iter != tokens.end(); iter++){

        //printf("|%s|->", iter->lexem);
    }*/
}   
