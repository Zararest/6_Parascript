#pragma once
#include "Inode.h"
#include "Icontent_of_statement.h"
#include "Icontrol_struct.h"
#include "Ioperand.h"
#include "Ioperator.h"
#include "../../Parcer/headers/Parcer.h"

class Lexer{

    Icontrol_struct* root = nullptr;
    std::list<lexem>& tokens;
    std::list<lexem>::iterator cur_pos;

    Icontrol_struct* get_block_of_code(); //+
    Use* get_use(){ return nullptr; };//потом
    Statement* get_statement();//+
        char* get_statement_name();//+
        DataType get_type();//+
        char* get_var_name();//+
        Initialization_list get_init_list();//+
        Var* get_var();//+
    Icontent_of_statement* get_stat_content();//+
        If* get_if();//+
        Assign* get_assign();//+
        Custom_operator* get_cust_op(){ return nullptr; };//потом
        Cycle* get_cycle();//+
        Call* get_call();//+     //надо добавить возможность вызывать по константам
    Ioperator* get_expr();//+
        Ioperator* get_and();//+
        Ioperator* get_equality();//+
        Ioperator* get_relation();//+
        Ioperator* get_plus_minus();//+
        Ioperator* get_mul_div();//+
        Ioperator* get_number_sign();//+
        Ioperand* get_operand_or_custom_op();//+
    Overload_set* get_overload_set();//+//потом
        Overload_op* get_overload_op(){ return nullptr; };//потом
        char* get_overload_name(){return nullptr;};//потом
        Asm_code* get_asm_code(){return nullptr;};  //потом                  

    void error(const char* message);
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
    void dump_graphviz();
};





    

    



