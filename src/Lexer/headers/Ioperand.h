#pragma once
#include "Inode.h"

class Ioperand: public Inode{

public: 

    virtual void print_graphviz() const = 0;
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

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");

        fprintf(out_file, "\"%p\" [label = \"%s\" fillcolor=yellow]\n", this, name);

        fclose(out_file);
    }
};

class Num final: public Ioperand{

    double value = 0;

public:

    Num(double new_value){

        value = new_value;
    }

    ~Num(){}

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");

        fprintf(out_file, "\"%p\" [label = \"%lf\" fillcolor=yellow]\n", this, value);

        fclose(out_file);
    }
};

class String final: public Ioperand{

    char* string = nullptr;
    int size = 0;

public:

    ~String(){

        delete[] string;
    }

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");

        fprintf(out_file, "\"%p\" [label = \"%s\" fillcolor=yellow]\n", this, string);

        fclose(out_file);
    }
};
