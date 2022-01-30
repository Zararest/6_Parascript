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

    if (first_statement){

        fprintf(asm_file, "CALL ");
        cur_node->print_name(asm_file);
        fprintf(asm_file, "\n\n");

        first_statement = false;
    }

    fprintf(asm_file, "function ");
    cur_node->print_name(asm_file);
    fprintf(asm_file, "\n");

    const Initialization_list& init_list = cur_node->get_init_list_link();

    if (stack_size < init_list.num_of_vars){

        complite_init_list(init_list.num_of_vars - stack_size);
    }

    init_vars(init_list);

    cur_node->transfer_request(this);

    dump_vars(init_list);

    fprintf(asm_file, "RET");
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
                fprintf(asm_file, "\tMOV B_A %s\n", init_list.list_of_vars[i].name);
                fprintf(asm_file, "\tPUSH B_A\n");
                break;

            case T_int:
                fprintf(asm_file, "\tMOV D_A %s\n", init_list.list_of_vars[i].name);
                fprintf(asm_file, "\tPUSH D_A\n");
                break;

            case T_float:
                fprintf(asm_file, "\tMOV P_F %s\n", init_list.list_of_vars[i].name);
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
                fprintf(asm_file, "\t%s DB\n", init_list.list_of_vars[i].name);
                fprintf(asm_file, "\tPOP B_A\n");
                fprintf(asm_file, "\tMOV %s B_A\n", init_list.list_of_vars[i].name);
                break;

            case T_int:
                fprintf(asm_file, "\t%s DD\n", init_list.list_of_vars[i].name);
                fprintf(asm_file, "\tPOP D_A\n");
                fprintf(asm_file, "\tMOV %s D_A\n", init_list.list_of_vars[i].name);
                break;

            case T_float:
                fprintf(asm_file, "\t%s DMAS 8\n", init_list.list_of_vars[i].name);
                fprintf(asm_file, "\tPOP P_F\n");
                fprintf(asm_file, "\tMOV %s P_F\n", init_list.list_of_vars[i].name);
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

        fprintf(asm_file, "\tJN %i_end\n", cur_file_pos, i); //прыжок в конец
        fprintf(asm_file, "\t@%i_%i_next\n", cur_file_pos, i);  //метка для следующего вызова
    }

    fprintf(asm_file, "\t@%i_end\n", cur_file_pos);
}   


