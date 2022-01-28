#pragma once
#include "Inode.h"

class Ioperand: public Inode{

public: 

    virtual void print_graphviz(FILE* out_file) const = 0;
    virtual ~Ioperand(){};
};

class Var final: public Ioperand{ 

    bool inner = false;
    char* name = nullptr;

public:

    Var(char* new_name, bool is_inner){//тут без копирования имени

        name = new_name;
        inner = is_inner;
    } 

    ~Var(){

        delete name;
    }

    void print_graphviz(FILE* out_file) const override{

        fprintf(out_file, "\"%p\" [label = \"%s\" fillcolor=yellow]\n", this, name);
    }
};

class Num final: public Ioperand{

    double value = 0;

public:

    Num(double new_value){

        value = new_value;
    }

    ~Num(){}

    void print_graphviz(FILE* out_file) const override{

        fprintf(out_file, "\"%p\" [label = \"%lf\" fillcolor=yellow]\n", this, value);
    }
};

class String final: public Ioperand{

    char* string = nullptr;
    int size = 0;

public:

    ~String(){

        delete[] string;
    }

    void print_graphviz(FILE* out_file) const override{

        fprintf(out_file, "\"%p\" [label = \"%s\" fillcolor=yellow]\n", this, string);
    }
};
