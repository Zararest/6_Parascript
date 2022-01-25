#pragma once
#include "Lexer.h"

class Icontent_of_statement: public Inode{

    Icontent_of_statement* next_expr = nullptr;

public: 

    virtual void print_graphviz(FILE* outp_file);
    virtual Inode* copy_tree() const;
    virtual ~Icontent_of_statement(){};
};

class Call final: public Icontent_of_statement{

    char* statement_name = nullptr;
    Var** arr_of_vars = nullptr;

public:

    Call* copy_tree() const;
};

class If final: public Icontent_of_statement{

    Ioperator* condition = nullptr; //тут может быть и нестандартный оператор
    Call** arr_off_calls = nullptr;

public:

    If* copy_tree() const;
};

class Cycle final: public Icontent_of_statement{

    Ioperator* condition = nullptr;
    Call* loop_body = nullptr;

public:

    Cycle* copy_tree() const;
};

class Custom_operator final: public Icontent_of_statement{

    char* op_name = nullptr;
    Var** arr_of_vars = nullptr;

public:

    Custom_operator* copy_tree() const;
};
    
class Assign final: public Icontent_of_statement{

    Var* l_value = nullptr;
    Ioperator* r_value = nullptr;

public:

    Assign* copy_tree() const;
};