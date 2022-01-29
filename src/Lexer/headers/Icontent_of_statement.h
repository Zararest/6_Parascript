#pragma once
#include "Inode.h"
#include "Ioperand.h"
#include "Ioperator.h"

class Icontent_of_statement: public Inode{

protected: 

    Icontent_of_statement* next_expr = nullptr;

    void print_next(FILE* out_file) const;

public: 

    Icontent_of_statement() = default;
    Icontent_of_statement(const Icontent_of_statement&) = delete;
    virtual ~Icontent_of_statement();
    
    Icontent_of_statement& operator =(const Icontent_of_statement&) = delete;

    void add_content_of_statement(Icontent_of_statement* new_content);
    
    virtual void print_graphviz(FILE* out_file) const = 0;
};



class Call final: public Icontent_of_statement{

    char* statement_name = nullptr;
    Var** arr_of_vars = nullptr;
    int num_of_vars = 0;

public:

    Call() = default;
    Call(const Call&) = delete;
    ~Call();

    Call& operator =(const Call&) = delete;

    void add_name(char* new_name);
    void add_new_var(Var* new_var);

    void print_graphviz(FILE* out_file) const override;
};



class Return_statement final: public Icontent_of_statement{

    Call* ret_stat = nullptr;
    Var* dest_var = nullptr;

public:

    Return_statement() = default;
    Return_statement(const Return_statement&) = delete;
    ~Return_statement();

    Return_statement& operator =(const Return_statement&) = delete;

    void add_call(Call* new_call);
    void add_var(Var* new_var);

    void print_graphviz(FILE* out_file) const override;
};



class Statements_return final: public Icontent_of_statement{

    Var* ret_var = nullptr;

public:

    Statements_return() = default;
    Statements_return(const Statements_return&) = delete;
    ~Statements_return();

    void add_var(Var* new_var);

    void print_graphviz(FILE* out_file) const override;
};



class If final: public Icontent_of_statement{

    Ioperator* condition = nullptr; //тут может быть и нестандартный оператор
    Call** arr_of_calls = nullptr;
    int num_of_calls = 0;

public:

    If() = default;
    If(const If&) = delete;
    ~If();

    If& operator =(const If&) = delete;

    void add_condition(Ioperator* new_condition);
    void add_new_call(Call* new_call);

    void print_graphviz(FILE* out_file) const override;
};



class Cycle final: public Icontent_of_statement{

    Ioperator* condition = nullptr;
    Call* loop_body = nullptr;

public:

    Cycle() = default;
    Cycle(const Cycle&) = delete;
    ~Cycle();

    Cycle& operator =(const Cycle&) = delete;

    void add_condition(Ioperator* new_condition);
    void add_loop_body(Call* new_loop_body);

    void print_graphviz(FILE* out_file) const override;
};


//потом
class Custom_operator final: public Icontent_of_statement{

    char* op_name = nullptr;
    Var** arr_of_vars = nullptr;

public:

    ~Custom_operator(){}
};
//потом
    

class Assign final: public Icontent_of_statement{

    Var* l_value = nullptr;
    Ioperator* r_value = nullptr;

public:

    Assign() = default;
    Assign(const Assign&) = delete;
    ~Assign();

    Assign& operator =(const Assign&) = delete;

    void add_l_value(Var* new_l_value);
    void add_r_value(Ioperator* new_r_value);
    
    void print_graphviz(FILE* out_file) const override;
};