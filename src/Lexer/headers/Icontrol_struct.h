#pragma once
#include "Lexer.h"

class Icontrol_struct: public Inode{
    
    Icontrol_struct* next_struct = nullptr;

public:

    Icontrol_struct* get_next(){ return next_struct; }
    void add_control_struct(Icontrol_struct* new_struct); //тоже пробрасывает через цепочку, но теперь через control_struct

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

    void add_operator(Overload_op* new_op); //должен прокидывать эту ссылку через цепочку операторов(first_overload)
    bool add_name(char* name);
    Overload_set* copy_tree() const;
};

class Use final: public Icontrol_struct{

    char** arr_of_overloads = nullptr;

public:

    Use* copy_tree() const;
};

class Statement final: public Icontrol_struct{

    char* statment_name = nullptr;
    Initialization_list init_list;
    Icontent_of_statement* first_expr = nullptr;

public:

    bool add_first_expr(Icontent_of_statement* new_first_expr);
    bool add_init_list(Initialization_list new_init_list);
    bool add_name(char* name);
    Statement* copy_tree() const;
};

class Asm_code final: public Icontrol_struct{

    char* code = nullptr;

public:

    Asm_code* copy_tree() const;
};