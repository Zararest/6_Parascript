#pragma once

#include <vector>

#include "Irequest.h"
#include "../../Lexer/headers/Inode.h"
#include "../../Lexer/headers/Icontent_of_statement.h"
#include "../../Lexer/headers/Icontrol_struct.h"
#include "../../Lexer/headers/Ioperand.h"
#include "../../Lexer/headers/Ioperator.h"
#include "../../Parcer/headers/Parcer.h"

class Statement_info{

    char* name = nullptr;
    int num_of_vars = 0;

public:

    Statement_info(char* new_name, int num_of_ret_vars);
    Statement_info(const Statement_info&);
    ~Statement_info();

    Statement_info& operator =(const Statement_info&) = delete;

    int get_number_of_ret_vars() const;
    bool cmp_name(const char* new_name) const;
};

class Statement_info_req: public Irequest{

    std::vector<Statement_info>& statements;

public:

    Statement_info_req(std::vector<Statement_info>& new_vector);
    Statement_info_req(const Statement_info_req&) = delete;
    ~Statement_info_req() = default;

    void process_request(Statement* cur_node);
};


class Asm_code_req: public Irequest{

    FILE* asm_file = nullptr;

    std::vector<Statement_info> statements;

    int num_of_accesses = 0;
    bool first_statement = true;
    char* cur_statment_name = nullptr;
    const Initialization_list* cur_init_list = nullptr;

    void get_char(Call* cur_call);
    void get_int(Call* cur_call);
    void get_float(Call* cur_call);
    void put_char(Call* cur_call);
    void put_float(Call* cur_call);

    void complete_init_list(int num_of_empty_vars);
    void init_vars(const Initialization_list& init_list);
    void dump_vars(const Initialization_list& init_list);
    DataType get_var_type(char* var_name) const;
    void pass_vars(Call* cur_call);
    void get_vars(Call* cur_call);
    int get_number_of_ret_vars(char* statement_name) const;
    void init_first_statement(char* statement_name);

    void push_var(char* var_name);
    void pop_var(char* var_name);

public:

    Asm_code_req(FILE* new_asm_file, std::vector<Statement_info>& statments_info);
    Asm_code_req(const Asm_code_req&) = delete;
    ~Asm_code_req() override = default;

    Asm_code_req& operator =(const Asm_code_req) = delete;

    void process_request(Statement* cur_node);
    void process_request(If* cur_node);
    void process_request(Call* cur_node);
    void process_request(Assign* cur_node);
    void process_request(Cycle* cur_node);
    void process_request(Return_statement* cur_node);
    void process_request(Statements_return* cur_node);

    void process_request(Bin_minus* cur_node);
    void process_request(Bin_plus* cur_node);
    void process_request(Mul* cur_node);
    void process_request(Div* cur_node);
    void process_request(Less* cur_node);
    void process_request(Greater* cur_node);
    void process_request(Equality* cur_node);

    void process_request(Var* cur_node);
    void process_request(Num* cur_node);

    void process_request(Number_sign* cur_node);
};

class Compiler{

    Icontrol_struct* synt_tree_root = nullptr;

public:

    Compiler(Icontrol_struct* new_syntax_tree);
    Compiler(const Compiler&) = delete;
    virtual ~Compiler();

    Compiler& operator=(const Compiler&) = delete;

    void generate_asm_code(const char* out_file_name);
};