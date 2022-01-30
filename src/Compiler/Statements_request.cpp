#include "headers/Compiler.h"

#include <cassert>

/**
 * @brief Без копирования имени!!!!
 * 
 * @param new_name 
 * @param num_of_ret_vars 
 */
Statement_info::Statement_info(char* new_name, int num_of_ret_vars){

    name = new_name;
    num_of_vars = num_of_ret_vars;
}

Statement_info::Statement_info(const Statement_info& old_obj){

    assert(old_obj.name != nullptr);
    int name_size = strlen(old_obj.name);
    assert(name_size > 0);

    name = new char[name_size];
    memcpy(name, old_obj.name, name_size);

    num_of_vars = old_obj.num_of_vars;
}

Statement_info::~Statement_info(){

    delete[] name;
}

bool Statement_info::cmp_name(const char* new_name) const{

    if (strcmp(new_name, name) == 0){

        return true;
    } else{

        return false;
    }
}

int Statement_info::get_number_of_ret_vars() const{

    return num_of_vars;
}


Statement_info_req::Statement_info_req(std::vector<Statement_info>& new_vector): statements(new_vector){}

void Statement_info_req::process_request(Statement* cur_node){
    
    int num_of_vars = cur_node->get_init_list_ptr()->num_of_vars;
    Statement_info* cur_info = new Statement_info(cur_node->get_name_copy(), num_of_vars);

    statements.push_back(*cur_info); //тут может память утекать

    cur_node->transfer_request(this);
}
