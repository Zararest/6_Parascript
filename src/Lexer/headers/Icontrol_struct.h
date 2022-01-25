#pragma once
#include "Lexer.h"

class Icontrol_struct: public Inode{
    
    Icontrol_struct* next_struct = nullptr;

public:

    virtual void print_graphviz(FILE* outp_file);
    virtual Inode* copy_tree() const;
    virtual ~Icontrol_struct(){};
};

class Overload_op final: public Icontrol_struct{

    char* op_name = nullptr;
    Initialization_prot* arr_of_initializations;
    Icontrol_struct* first_expr = nullptr;

public:

    Overload_op* copy_tree() const;
};

class Overload_set final: public Icontrol_struct{

    char* set_name = nullptr;
    Overload_op* first_overload = nullptr;

public:

    Overload_set* copy_tree() const;
};

class Use final: public Icontrol_struct{

    char** arr_of_overloads = nullptr;

public:

    Use* copy_tree() const;
};

class Statement final: public Icontrol_struct{

    char* statment_name = nullptr;
    Initialization_prot* arr_of_vars = nullptr;
    Icontent_of_statement* first_expr = nullptr;

public:

    Statement* copy_tree() const;
};

class Asm_code final: public Icontrol_struct{

    char* code = nullptr;

public:

    Asm_code* copy_tree() const;
};