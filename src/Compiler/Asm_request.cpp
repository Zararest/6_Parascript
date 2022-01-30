#include "./headers/Compiler.h"

#include <cassert>

Asm_code_req::Asm_code_req(FILE* new_asm_file, std::vector<Statement_info>& statments_info): statements(statments_info){

    asm_file = new_asm_file;
}

void Asm_code_req::init_first_statement(char* statement_name){

    int num_of_vars = get_number_of_ret_vars(statement_name);

    complete_init_list(num_of_vars);
}

/**
 * @brief Если первое выражение, то прописывает его вызов.
 *    
 * @param cur_node 
 */
void Asm_code_req::process_request(Statement* cur_node){

    num_of_accesses++;

    cur_statment_name = cur_node->get_name_copy();
    bool this_is_first = false;

    if (first_statement){

        init_first_statement(cur_statment_name);

        fprintf(asm_file, "CALL %s\n\n", cur_statment_name);

        this_is_first = true;
        first_statement = false;
    }

    fprintf(asm_file, "function %s\n", cur_statment_name);

    cur_init_list = cur_node->get_init_list_ptr();

    /*if (stack_size < cur_init_list->num_of_vars){

        complete_init_list(cur_init_list->num_of_vars - stack_size);
    }*/

    init_vars(*cur_init_list);

    cur_node->transfer_request_fir_expr(this);

    dump_vars(*cur_init_list);

    fprintf(asm_file, "RET\n");

    delete[] cur_statment_name;
    cur_statment_name = nullptr;

    cur_node->transfer_request(this);

    if (this_is_first){

        fprintf(asm_file, "END\n");
    }
}

/**
 * @brief После выполнения Statement все его переменные записываются в стек. Их должен прочитать Call
 * 
 * @param init_list 
 */
void Asm_code_req::dump_vars(const Initialization_list& init_list){

    for (int i = init_list.num_of_vars - 1; i >= 0; i--){ //-----------------------------------------------------!!!

        push_var(init_list.list_of_vars[i].name);
    }
}

/**
 * @brief Использует W_A
 * 
 * @param num_of_empty_vars 
 */
void Asm_code_req::complete_init_list(int num_of_empty_vars){

    assert(num_of_empty_vars >= 0);
    if (num_of_empty_vars == 0){ return; }

    fprintf(asm_file, "\tMOV W_A 0\n");

    for (int i = 0; i < num_of_empty_vars; i++){

        fprintf(asm_file, "\tPUSH W_A\n");
    }
}

void Asm_code_req::init_vars(const Initialization_list& init_list){

    for (int i = init_list.num_of_vars - 1; i >= 0; i--){

        switch (init_list.list_of_vars[i].type){
        
            case T_char:
                fprintf(asm_file, "\t%s_%s DB\n", cur_statment_name, init_list.list_of_vars[i].name);
                fprintf(asm_file, "\tPOP B_A\n");
                fprintf(asm_file, "\tMOV %s_%s B_A\n", cur_statment_name, init_list.list_of_vars[i].name);
                break;

            case T_int:
                fprintf(asm_file, "\t%s_%s DD\n", cur_statment_name, init_list.list_of_vars[i].name);
                fprintf(asm_file, "\tPOP D_A\n");
                fprintf(asm_file, "\tMOV %s_%s D_A\n", cur_statment_name, init_list.list_of_vars[i].name);
                break;

            case T_float:
                fprintf(asm_file, "\t%s_%s DMAS 8\n", cur_statment_name, init_list.list_of_vars[i].name);
                fprintf(asm_file, "\tPOP P_F\n");
                fprintf(asm_file, "\tMOV %s_%s P_F\n", cur_statment_name, init_list.list_of_vars[i].name);
                break;

            case T_arr:
                fprintf(asm_file, "Эта функция еще не поддерживаается\n");
                break;

            case T_expr:
                fprintf(asm_file, "Эта функция еще не поддерживаается\n");
                break;
        }
    }
}

/**
 * @brief Проверка есть ли условие проходит по количеству вызовов. 
 * 
 * @param cur_node 
 */
void Asm_code_req::process_request(If* cur_node){
    
    num_of_accesses++;

    int prev_num_of_accesses = num_of_accesses;

    cur_node->transfer_request_condition(this);

    if (prev_num_of_accesses == num_of_accesses){

        fprintf(asm_file, "\tMOV D_E 0\n");
        fprintf(asm_file, "\tPUSH D_E");
    }

    fprintf(asm_file, "\tPOP D_E\n"); //возвращаемое значение выражения лежало на стеке

    int cur_file_pos = ftell(asm_file);

    for (int i = 0; i < cur_node->get_num_of_calls(); i++){

        fprintf(asm_file, "\tPUSH D_E\n");
        fprintf(asm_file, "\tMOV B_A %i\n", i);
        fprintf(asm_file, "\tPUSH B_A\n");  
        fprintf(asm_file, "\tCMP\n");
        fprintf(asm_file, "\tJL %i_%i_next\n", cur_file_pos, i);//следующий call
        
        cur_node->transfer_request_call(this, i);

        fprintf(asm_file, "\tJN %i_end\n", cur_file_pos); //прыжок в конец
        fprintf(asm_file, "@%i_%i_next:\n", cur_file_pos, i);  //метка для следующего вызова
    }

    fprintf(asm_file, "@%i_end:\n", cur_file_pos);

    cur_node->transfer_request(this); //вызов следующего выражения
}   

void Asm_code_req::get_char(Call* cur_call){
    
    char* var_name = cur_call->get_var_name_copy(0);
    
    fprintf(asm_file, "\tIN B_B\n");
    fprintf(asm_file, "\tMOV %s_%s B_B\n", cur_statment_name, var_name);
  
    delete[] var_name;
}

void Asm_code_req::get_int(Call* cur_call){
    
    char* var_name = cur_call->get_var_name_copy(0);
    
    fprintf(asm_file, "\tIN D_B\n");
    fprintf(asm_file, "\tMOV %s_%s D_B\n", cur_statment_name, var_name);
  
    delete[] var_name;
}

void Asm_code_req::get_float(Call* cur_call){
    
    char* var_name = cur_call->get_var_name_copy(0);
    
    fprintf(asm_file, "\tIN P_R\n");
    fprintf(asm_file, "\tMOV %s_%s P_R\n", cur_statment_name, var_name);
  
    delete[] var_name;
}

void Asm_code_req::put_char(Call* cur_call){
    
    char* var_name = cur_call->get_var_name_copy(0);
    
    fprintf(asm_file, "\tMOV D_B %s_%s\n", cur_statment_name, var_name);
    fprintf(asm_file, "\tPUSH D_B\n");
    fprintf(asm_file, "\tOUT_CHR\n");
  
    delete[] var_name;
}

void Asm_code_req::put_float(Call* cur_call){
    
    char* var_name = cur_call->get_var_name_copy(0);
    
    fprintf(asm_file, "\tMOV P_R %s_%s\n", cur_statment_name, var_name);
    fprintf(asm_file, "\tPUSH P_R\n");
    fprintf(asm_file, "\tOUT\n");
  
    delete[] var_name;
}

void Asm_code_req::process_request(Call* cur_node){

    num_of_accesses++;

    char* call_stat_name = cur_node->get_stat_name_copy();
    int num_of_vars = cur_node->get_num_of_vars();

    if (strcmp(call_stat_name, "get_char") == 0){ 

        get_char(cur_node); 
        cur_node->transfer_request(this); 
        return;
    }

    if (strcmp(call_stat_name, "get_int") == 0){ 
        
        get_int(cur_node); 
        cur_node->transfer_request(this); 
        return;
    }

    if (strcmp(call_stat_name, "get_float") == 0){ 
        
        get_float(cur_node); 
        cur_node->transfer_request(this); 
        return;
    }

    if (strcmp(call_stat_name, "put_char") == 0){ 
        
        put_char(cur_node); 
        cur_node->transfer_request(this); 
        return;
    }

    if (strcmp(call_stat_name, "put_float") == 0){ 
        
        put_float(cur_node); 
        cur_node->transfer_request(this); 
        return;
    }

    pass_vars(cur_node);

    fprintf(asm_file, "\tCALL %s\n", call_stat_name);

    get_vars(cur_node); //для переменных inner

    delete[] call_stat_name;
}

void Asm_code_req::get_vars(Call* cur_call){

    char* call_stat_name = cur_call->get_stat_name_copy();
    int num_of_call_vars = cur_call->get_num_of_vars();
    int num_of_ret_vars = get_number_of_ret_vars(call_stat_name);

    for (int i = num_of_ret_vars - 1; i >= 0; i--){

        if ((i < num_of_call_vars) && (cur_call->is_inner_var(i))){
            
            char* cur_var_name = cur_call->get_var_name_copy(i);
            assert(cur_var_name != nullptr);

            pop_var(cur_var_name);

            delete[] cur_var_name;
            cur_var_name = nullptr;

        } else{

            fprintf(asm_file, "\tPOP B_A\n");
        }
    }

    delete[] call_stat_name;
}

int Asm_code_req::get_number_of_ret_vars(char* statement_name) const{

    for (int i = 0; i < statements.size(); i++){

        if (statements[i].cmp_name(statement_name)){

            return statements[i].get_number_of_ret_vars();
        }
    }

    printf("error no statement\n");

    return -1;
}

void Asm_code_req::pass_vars(Call* cur_call){

    char* call_stat_name = cur_call->get_stat_name_copy();
    int num_of_vars = cur_call->get_num_of_vars();
    int num_of_statement_vars = get_number_of_ret_vars(call_stat_name);

    assert(num_of_vars <= num_of_statement_vars);

    for (int i = 0; i < num_of_vars; i++){

        char* cur_var_name = cur_call->get_var_name_copy(i);
        assert(cur_var_name != nullptr);

        push_var(cur_var_name);

        delete[] cur_var_name;
        cur_var_name = nullptr;
    }

    complete_init_list(num_of_statement_vars - num_of_vars);

    delete[] call_stat_name;
}

DataType Asm_code_req::get_var_type(char* var_name) const{

    int num_of_vars = cur_init_list->num_of_vars;

    for (int i = 0; i < num_of_vars; i++){

        if (strcmp(cur_init_list->list_of_vars[i].name, var_name) == 0){

            return cur_init_list->list_of_vars[i].type;
        }
    }

    return -1;
}

void Asm_code_req::process_request(Assign* cur_node){

    num_of_accesses++;

    char* var_name = cur_node->get_var_name_copy();

    cur_node->transfer_request_r_value(this);

    pop_var(var_name);

    delete[] var_name;

    cur_node->transfer_request(this);
}

void Asm_code_req::process_request(Cycle* cur_node){

    num_of_accesses++;

    int cur_file_pos = ftell(asm_file);
    int prev_num_of_accesses = num_of_accesses;

    fprintf(asm_file, "@%i_loop_start:\n", cur_file_pos);

    cur_node->transfer_request_condition(this);

    if (prev_num_of_accesses == num_of_accesses){

        fprintf(asm_file, "\tMOV D_E 0\n");
        fprintf(asm_file, "\tPUSH D_E\n");
    }

    fprintf(asm_file, "\tMOV D_E 1\n");
    fprintf(asm_file, "\tPUSH D_E\n");
    fprintf(asm_file, "\tCMP\n");
    fprintf(asm_file, "\tJL %i_end\n", cur_file_pos);
    fprintf(asm_file, "\tJG %i_end\n", cur_file_pos);

    cur_node->transfer_request_call(this);

    fprintf(asm_file, "\tJN %i_loop_start\n", cur_file_pos);
    fprintf(asm_file, "@%i_end:\n", cur_file_pos);

    cur_node->transfer_request(this);
}

void Asm_code_req::process_request(Return_statement* cur_node){

    num_of_accesses++;

    char* var_name = cur_node->get_var_name_copy();

    cur_node->transfer_request_call(this);

    /*if (stack_size == 0){ //тут нужна проверка

        fprintf(asm_file, "\tMOV D_E 0\n");
        fprintf(asm_file, "\tPUSH D_E\n");
        stack_size++;
    }*/

    pop_var(var_name); 

    delete[] var_name;

    cur_node->transfer_request(this);
}

void Asm_code_req::process_request(Statements_return* cur_node){

    num_of_accesses++;

    char* var_name = cur_node->get_var_name_copy();

    push_var(var_name);

    delete[] var_name;

    cur_node->transfer_request(this);
}

void Asm_code_req::push_var(char* var_name){

    switch (get_var_type(var_name)){

        case T_char:
            fprintf(asm_file, "\tMOV B_A %s_%s\n", cur_statment_name, var_name);
            fprintf(asm_file, "\tPUSH B_A\n");
            break;

        case T_int:
            fprintf(asm_file, "\tMOV D_A %s_%s\n", cur_statment_name, var_name);
            fprintf(asm_file, "\tPUSH D_A\n");
            break;

        case T_float:
            fprintf(asm_file, "\tMOV P_F %s_%s\n", cur_statment_name, var_name);
            fprintf(asm_file, "\tPUSH P_F\n");
            break;

        case T_arr:
            fprintf(asm_file, "Эта функция еще не поддерживаается\n");
            break;

        case T_expr:
            fprintf(asm_file, "Эта функция еще не поддерживаается\n");
            break;

        case -1:
            printf("undef type\n");
            exit(0);
            break;
    }
}

void Asm_code_req::pop_var(char* var_name){

    switch(get_var_type(var_name)){

        case T_char:
            fprintf(asm_file, "\tPOP B_A\n");
            fprintf(asm_file, "\tMOV %s_%s B_A\n", cur_statment_name, var_name);
            break;

        case T_int:
            fprintf(asm_file, "\tPOP D_A\n");
            fprintf(asm_file, "\tMOV %s_%s D_A\n", cur_statment_name, var_name);
            break;

        case T_float:
            fprintf(asm_file, "\tPOP P_F\n");
            fprintf(asm_file, "\tMOV %s_%s P_F\n", cur_statment_name, var_name);
            break;

        case T_arr:
            fprintf(asm_file, "Эта функция еще не поддерживаается\n");
            break;

        case T_expr:
            fprintf(asm_file, "Эта функция еще не поддерживаается\n");
            break;

        case -1:
            printf("undef type\n");
            exit(0);
            break;
    }
}

void Asm_code_req::process_request(Bin_minus* cur_node){

    num_of_accesses++;

    cur_node->transfer_request_left(this);
    cur_node->transfer_request_right(this);

    fprintf(asm_file, "\tSUB\n");
}

void Asm_code_req::process_request(Bin_plus* cur_node){

    num_of_accesses++;

    cur_node->transfer_request_left(this);
    cur_node->transfer_request_right(this);

    fprintf(asm_file, "\tADD\n");
}

void Asm_code_req::process_request(Mul* cur_node){

    num_of_accesses++;

    cur_node->transfer_request_left(this);
    cur_node->transfer_request_right(this);

    fprintf(asm_file, "\tMUL\n");
}

void Asm_code_req::process_request(Div* cur_node){

    num_of_accesses++;

    cur_node->transfer_request_left(this);
    cur_node->transfer_request_right(this);

    fprintf(asm_file, "\tDIV\n");
}

void Asm_code_req::process_request(Less* cur_node){

    num_of_accesses++;

    cur_node->transfer_request_left(this);
    cur_node->transfer_request_right(this);

    int cur_file_pos = ftell(asm_file);

    fprintf(asm_file, "\tCMP\n");
    fprintf(asm_file, "\tJL %i_zero\n", cur_file_pos);

    if (!cur_node->is_and_equal()){

        fprintf(asm_file, "\tJE %i_zero\n", cur_file_pos);
    }

    fprintf(asm_file, "\tMOV D_E 1\n");
    fprintf(asm_file, "\tPUSH D_E\n");
    fprintf(asm_file, "\tJN %i_end\n", cur_file_pos);

    fprintf(asm_file, "@%i_zero:\n", cur_file_pos);
    fprintf(asm_file, "\tMOV D_E 0\n");
    fprintf(asm_file, "\tPUSH D_E\n");

    fprintf(asm_file, "@%i_end:\n", cur_file_pos);
}

void Asm_code_req::process_request(Greater* cur_node){

    num_of_accesses++;

    cur_node->transfer_request_left(this);
    cur_node->transfer_request_right(this);

    int cur_file_pos = ftell(asm_file);

    fprintf(asm_file, "\tCMP\n");
    fprintf(asm_file, "\tJG %i_zero\n", cur_file_pos);

    if (!cur_node->is_and_equal()){

        fprintf(asm_file, "\tJE %i_zero\n", cur_file_pos);
    }

    fprintf(asm_file, "\tMOV D_E 1\n");
    fprintf(asm_file, "\tPUSH D_E\n");
    fprintf(asm_file, "\tJN %i_end\n", cur_file_pos);

    fprintf(asm_file, "@%i_zero:\n", cur_file_pos);
    fprintf(asm_file, "\tMOV D_E 0\n");
    fprintf(asm_file, "\tPUSH D_E\n");

    fprintf(asm_file, "@%i_end:\n", cur_file_pos);
}

void Asm_code_req::process_request(Equality* cur_node){

    num_of_accesses++;

    cur_node->transfer_request_left(this);
    cur_node->transfer_request_right(this);

    int cur_file_pos = ftell(asm_file);

    fprintf(asm_file, "\tCMP\n");

    if (cur_node->is_equal()){

        fprintf(asm_file, "\tJE %i_one\n", cur_file_pos);
        fprintf(asm_file, "\tMOV D_E 0\n");
        fprintf(asm_file, "\tPUSH D_E\n");
        fprintf(asm_file, "\tJN %i_end\n", cur_file_pos);

        fprintf(asm_file, "@%i_one:\n", cur_file_pos);
        fprintf(asm_file, "\tMOV D_E 1\n");
        fprintf(asm_file, "\tPUSH D_E\n");

    } else{

        fprintf(asm_file, "\tJE %i_zero\n", cur_file_pos);
        fprintf(asm_file, "\tMOV D_E 1\n");
        fprintf(asm_file, "\tPUSH D_E\n");
        fprintf(asm_file, "\tJN %i_end\n", cur_file_pos);

        fprintf(asm_file, "@%i_zero:\n", cur_file_pos);
        fprintf(asm_file, "\tMOV D_E 0\n");
        fprintf(asm_file, "\tPUSH D_E\n");
    }

    fprintf(asm_file, "@%i_end:\n", cur_file_pos);
}

void Asm_code_req::process_request(Var* cur_node){

    num_of_accesses++;

    char* var_name = cur_node->get_name_copy();

    push_var(var_name);

    delete[] var_name;
}

void Asm_code_req::process_request(Num* cur_node){

    num_of_accesses++;

    double cur_val = cur_node->get_value();

    fprintf(asm_file, "\tMOV P_R %lf\n", cur_val);
    fprintf(asm_file, "\tPUSH P_R\n");
}

void Asm_code_req::process_request(Number_sign* cur_node){

    num_of_accesses++;

    cur_node->transfer_request_operand(this);

    if (cur_node->is_less_than_zero()){

        fprintf(asm_file, "\tMOV B_A -1\n");
        fprintf(asm_file, "\tPUSH B_A\n");
        fprintf(asm_file, "\tMUL\n");
    }
}



