#pragma once
#include "Lexer.h"

class Ioperator: public Inode{

    Inode* left = nullptr;
    Inode* right = nullptr;

public: 

    virtual void print_graphviz(FILE* outp_file);
    virtual Inode* copy_tree() const;
    virtual ~Ioperator(){};
};

class Custom_op_with_ret: public Ioperator{

    char* op_name = nullptr;
    Var** arr_of_vars = nullptr;

public:

    Custom_op_with_ret* copy_tree() const;
};

