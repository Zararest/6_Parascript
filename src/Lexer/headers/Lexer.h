#pragma once
#include "Icontent_of_statement.h"
#include "Icontrol_struct.h"
#include "Ioperand.h"
#include "Ioperator.h"
#include "../../Parcer/headers/Parcer.h"

typedef int DataType;

enum Data_types{
    
    T_char = 0,
    T_int = 1,
    T_float = 2,
    T_arr = 3,
    T_expr = 4
};

struct Initialization_list{

    int num_of_vars = 0;
    Initialization_prot* list_of_vars = nullptr;
};

struct Initialization_prot{

    char* name = nullptr;
    DataType type;
};

class Inode{

    Inode* prev = nullptr;
    
public:
    
    virtual void link(Inode* new_node);//должно проводится связывание с этим узлом через prev
    virtual void print_graphviz(FILE* outp_file);
    virtual Inode* copy_tree() const;
    virtual ~Inode(){};
};

class Lexer{

    Icontrol_struct* root = nullptr;
    std::list<lexem>& tokens;
    std::list<lexem>::iterator cur_pos;

    Icontrol_struct* get_block_of_code(); //+
    Use* get_use();//потом
    Statement* get_statement();//+
        char* get_statement_name();//+
        DataType get_type();//+
        char* get_var_name();//+
        Initialization_list get_init_list();//+
        Var* get_var();//+
    Icontent_of_statement* get_stat_content();//+
        If* get_if();//+
        Assign* get_assign();//+
        Custom_operator* get_cust_op();//потом
        Cycle* get_cycle();//+
        Call* get_call();//+     //надо добавить возможность вызывать по константам
    Ioperator* get_expr();//+
        Ioperator* get_and();//+
        Ioperator* get_equality();//+
        Ioperator* get_relation();//+
        Ioperator* get_plus_minus();//+
        Ioperator* get_mul_div();//+
        Ioperator* get_number_sign();//+
        Inode* get_operand_or_custom_op();//+
    Overload_set* get_overload_set();//+//потом
        Overload_op* get_overload_op();//потом
        char* get_overload_name();//потом
        Asm_code* get_asm_code();  //потом                  

    void error(char* message);
    bool require(const char* req_token, const char* message);
    bool check_end_of_tokens();
public:

    Lexer(std::list<lexem>& new_tokens): tokens(new_tokens){

        cur_pos = tokens.begin();
    }
    Lexer(const Lexer&) = delete;
    Lexer(Lexer&&) = delete;
    ~Lexer() = default;

    Lexer& operator =(const Lexer&) = delete;
    Lexer& operator =(Lexer&&) = delete;

    Icontrol_struct* get_syntax_tree();
    void dump_graphviz(FILE* outp_file);
};





    

    



