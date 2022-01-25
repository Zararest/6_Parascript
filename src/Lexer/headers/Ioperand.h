#pragma once
#include "Lexer.h"

class Ioperand: public Inode{

public: 

    virtual void print_graphviz(FILE* outp_file);
    virtual Inode* copy_tree() const;
    virtual ~Ioperand(){};
};

class Var final: public Ioperand{ 

    bool inner = false;
    char* name = nullptr;

public:

    Var* copy_tree() const;

};

class Num final: public Ioperand{

    double value = 0;

public:

    Num* copy_tree() const;
};

class String final: public Ioperand{

    char* string = nullptr;
    int size = 0;

public:

    String* copy_tree() const;
};
