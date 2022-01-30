#pragma once

#include "Irequest.h"
#include "../../Lexer/headers/Inode.h"
#include "../../Lexer/headers/Icontent_of_statement.h"
#include "../../Lexer/headers/Icontrol_struct.h"
#include "../../Lexer/headers/Ioperand.h"
#include "../../Lexer/headers/Ioperator.h"
#include "../../Parcer/headers/Parcer.h"


class Asm_code_req: public Irequest{

    FILE* asm_file = nullptr;

    int num_of_accesses = 0;
    int num_of_ret_vars = 0;
    int stack_size = 0;
    bool first_statement = true;
    char* cur_statment_name = nullptr;
    const Initialization_list* cur_init_list = nullptr;

    void get_char(Call* cur_call);
    void get_int(Call* cur_call);
    void get_float(Call* cur_call);
    void put_char(Call* cur_call);
    void put_float(Call* cur_call);

    void complite_init_list(int num_of_empty_vars);
    void init_vars(const Initialization_list& init_list);
    void dump_vars(const Initialization_list& init_list);
    DataType get_var_type(char* var_name) const;
    void pass_vars(Call* cur_call);
    void get_vars(Call* cur_call);

public:

    Asm_code_req(FILE* new_asm_file);
    Asm_code_req(const Asm_code_req&) = delete;
    ~Asm_code_req() override = default;

    Asm_code_req& operator =(const Asm_code_req) = delete;

    void process_request(Statement* cur_node);
    void process_request(If* cur_node);
    void process_request(Call* cur_node);
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