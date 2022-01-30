#include "./headers/Compiler.h"

#include <cassert>

Asm_code_req::Asm_code_req(FILE* new_asm_file){

    asm_file = new_asm_file;
}

/**
 * @brief Если первое выражение, то прописывает его вызов.
 *    
 * @param cur_node 
 */
void Asm_code_req::process_request(Statement* cur_node){

    num_of_accesses++;

    cur_statment_name = cur_node->get_name_copy();

    if (first_statement){

        fprintf(asm_file, "CALL %s\n\n", cur_statment_name);

        first_statement = false;
    }

    fprintf(asm_file, "function %s\n", cur_statment_name);

    cur_init_list = cur_node->get_init_list_ptr();

    if (stack_size < cur_init_list->num_of_vars){

        complite_init_list(cur_init_list->num_of_vars - stack_size);
    }

    init_vars(*cur_init_list);

    cur_node->transfer_request_fir_expr(this);

    dump_vars(*cur_init_list);

    fprintf(asm_file, "RET\n");

    delete[] cur_statment_name;
    cur_statment_name = nullptr;

    cur_node->transfer_request(this);
}

/**
 * @brief После выполнения Statement все его переменные записываются в стек. Их должен прочитать Call
 * 
 * @param init_list 
 */
void Asm_code_req::dump_vars(const Initialization_list& init_list){

    num_of_ret_vars = 0;

    for (int i = init_list.num_of_vars - 1; i >= 0; i--){

        switch (init_list.list_of_vars[i].type){
        
            case T_char:
                fprintf(asm_file, "\tMOV B_A %s_%s\n", cur_statment_name, init_list.list_of_vars[i].name);
                fprintf(asm_file, "\tPUSH B_A\n");
                break;

            case T_int:
                fprintf(asm_file, "\tMOV D_A %s_%s\n", cur_statment_name, init_list.list_of_vars[i].name);
                fprintf(asm_file, "\tPUSH D_A\n");
                break;

            case T_float:
                fprintf(asm_file, "\tMOV P_F %s_%s\n", cur_statment_name, init_list.list_of_vars[i].name);
                fprintf(asm_file, "\tPUSH P_F\n");
                break;

            case T_arr:
                fprintf(asm_file, "Эта функция еще не поддерживаается\n");
                break;

            case T_expr:
                fprintf(asm_file, "Эта функция еще не поддерживаается\n");
                break;
        }
        num_of_ret_vars++;
    }
}

/**
 * @brief Использует W_A
 * 
 * @param num_of_empty_vars 
 */
void Asm_code_req::complite_init_list(int num_of_empty_vars){

    fprintf(asm_file, "\tMOV W_A 0\n");

    for (int i = 0; i < num_of_empty_vars; i++){

        fprintf(asm_file, "\tPUSH W_A\n");
        stack_size++;
    }
}

void Asm_code_req::init_vars(const Initialization_list& init_list){

    for (int i = init_list.num_of_vars - 1; i >= 0; i--){

        assert(stack_size > 0);

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

        stack_size--;
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
        fprintf(asm_file, "\t@%i_%i_next\n", cur_file_pos, i);  //метка для следующего вызова
    }

    fprintf(asm_file, "\t@%i_end\n", cur_file_pos);

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

    int num_of_call_vars = cur_call->get_num_of_vars();

    for (int i = num_of_ret_vars - 1; i >= 0; i--){

        if ((i < num_of_call_vars) && (cur_call->is_inner_var(i))){
            
            char* cur_var_name = cur_call->get_var_name_copy(i);
            assert(cur_var_name != nullptr);

            switch (get_var_type(cur_var_name)){
        
                case T_char:
                    fprintf(asm_file, "\tPOP B_A\n");
                    fprintf(asm_file, "\tMOV %s_%s B_A\n", cur_statment_name, cur_var_name);
                    break;

                case T_int:
                    fprintf(asm_file, "\tPOP D_A\n");
                    fprintf(asm_file, "\tMOV %s_%s D_A\n", cur_statment_name, cur_var_name);
                    break;

                case T_float:
                    fprintf(asm_file, "\tPOP P_F\n");
                    fprintf(asm_file, "\tMOV %s_%s P_F\n", cur_statment_name, cur_var_name);
                    break;

                case T_arr:
                    fprintf(asm_file, "Эта функция еще не поддерживаается\n");
                    break;

                case T_expr:
                    fprintf(asm_file, "Эта функция еще не поддерживаается\n");
                    break;

                case -1:
                    printf("error 326\n");
                    break;
            }

            delete[] cur_var_name;
            cur_var_name = nullptr;

        } else{

            fprintf(asm_file, "\tPOP B_A\n");
        }

        stack_size--;
    }
}

void Asm_code_req::pass_vars(Call* cur_call){

    int num_of_vars = cur_call->get_num_of_vars();

    for (int i = 0; i < num_of_vars; i++){

        char* cur_var_name = cur_call->get_var_name_copy(i);
        assert(cur_var_name != nullptr);

        switch (get_var_type(cur_var_name)){

            case T_char:
                fprintf(asm_file, "\tMOV B_A %s_%s\n", cur_statment_name, cur_var_name);
                fprintf(asm_file, "\tPUSH B_A\n");
                break;

            case T_int:
                fprintf(asm_file, "\tMOV D_A %s_%s\n", cur_statment_name, cur_var_name);
                fprintf(asm_file, "\tPUSH D_A\n");
                break;

            case T_float:
                fprintf(asm_file, "\tMOV P_F %s_%s\n", cur_statment_name, cur_var_name);
                fprintf(asm_file, "\tPUSH P_F\n");
                break;

            case T_arr:
                fprintf(asm_file, "Эта функция еще не поддерживаается\n");
                break;

            case T_expr:
                fprintf(asm_file, "Эта функция еще не поддерживаается\n");
                break;

            case -1:
                printf("error 379\n");
        }

        delete[] cur_var_name;
        cur_var_name = nullptr;

        stack_size++;
    }
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
