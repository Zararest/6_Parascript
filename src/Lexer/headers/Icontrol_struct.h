#pragma once
#include "Inode.h"
#include "Icontent_of_statement.h"

class Icontrol_struct: public Inode{
    
protected:

    Icontrol_struct* next_struct = nullptr;

    void print_next(FILE* out_file) const;
    
public:
    
    Icontrol_struct() = default;
    Icontrol_struct(const Icontrol_struct&) = delete;
    virtual ~Icontrol_struct();

    Icontrol_struct& operator =(const Icontrol_struct&) = delete;
    
    void add_control_struct(Icontrol_struct* new_struct);
    void transfer_request(Irequest* cur_req);

    virtual void print_graphviz(FILE* out_file) const = 0;
    virtual void get_request(Irequest* cur_req) = 0;
};



//потом
class Overload_op final: public Icontrol_struct{

    char* op_name = nullptr;
    Initialization_list arr_of_initializations;
    Icontrol_struct* first_expr = nullptr;

public:

    void print_graphviz(FILE* out_file) const override{};
    Overload_op* copy_tree() const;
    void get_request(Irequest* cur_req) override{}
};

class Overload_set final: public Icontrol_struct{

    char* set_name = nullptr;
    Overload_op* first_overload = nullptr;

public:

    void add_operator(Overload_op* new_op){ } //должен прокидывать эту ссылку через цепочку операторов(first_overload)
    bool add_name(char* name){ return false; }
    void print_graphviz(FILE* out_file) const override{}
    void get_request(Irequest* cur_req) override{}
};

class Use final: public Icontrol_struct{

    char** arr_of_overloads = nullptr;

public:

    Use* copy_tree() const;
};
//потом


class Statement final: public Icontrol_struct{

    char* statement_name = nullptr;
    Initialization_list init_list;
    Icontent_of_statement* first_expr = nullptr;

public:

    Statement() = default;
    Statement(const Statement&) = delete;
    ~Statement();

    Statement& operator =(const Statement&) = delete;

    bool add_first_expr(Icontent_of_statement* new_first_expr);
    bool add_init_list(Initialization_list new_init_list);
    bool add_name(char* name);

    void print_name(FILE* out_file){};
    const Initialization_list& get_init_list_link(){};

    void print_graphviz(FILE* out_file) const override;
    void get_request(Irequest* cur_req) override;
};


//потом
class Asm_code final: public Icontrol_struct{

    char* code = nullptr;

public:

   //Asm_code* copy_tree() const;
};
//потом