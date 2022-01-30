#include "headers/Icontent_of_statement.h"

#include <cassert>

void Icontent_of_statement::transfer_request(Irequest* cur_req){

    if (next_expr != nullptr){

        next_expr->get_request(cur_req);
    } else{

        cur_req->terminal_request();
    }
}

void Icontent_of_statement::print_next(FILE* out_file) const{

    if (next_expr != nullptr){

        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, next_expr);
        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, next_expr);
        next_expr->print_graphviz(out_file);
    }
}

void Icontent_of_statement::add_content_of_statement(Icontent_of_statement* new_content){

    if (next_expr != nullptr){

        next_expr->add_content_of_statement(new_content);
    } else{

        next_expr = new_content;
    }
}

Icontent_of_statement::~Icontent_of_statement(){

    delete next_expr;
}



Call::~Call(){

    for (int i = 0; i < num_of_vars; i++){

        delete arr_of_vars[i];
    }

    delete[] arr_of_vars;
    delete[] statement_name;
}

void Call::add_name(char* new_name){

    delete[] statement_name;
    statement_name = new_name;         
}

void Call::add_new_var(Var* new_var){

    if (new_var == nullptr){ return; }

    Var** new_arr_of_vars = new Var*[num_of_vars + 1];

    new_arr_of_vars[num_of_vars] = new_var;

    if (num_of_vars > 0){

        memcpy(new_arr_of_vars, arr_of_vars, sizeof(Var*) * num_of_vars);
    }

    num_of_vars++;
    delete[] arr_of_vars;

    arr_of_vars = new_arr_of_vars;
}

char* Call::get_stat_name_copy() const{

    assert(statement_name != nullptr);
    int name_size = strlen(statement_name);
    assert(name_size > 0);

    char* name_copy = new char[name_size];
    memcpy(name_copy, statement_name, name_size);

    return name_copy;
}

char* Call::get_var_name_copy(int num_of_var) const{

    if (num_of_var < num_of_vars){

        return arr_of_vars[num_of_var]->get_name_copy();
    } else{

        return nullptr;
    }
}

int Call::get_num_of_vars() const{

    return num_of_vars;
}

bool Call::is_inner_var(int num_of_var) const{

    if (num_of_var < num_of_vars){

        return arr_of_vars[num_of_var]->is_inner();
    } else{

        return false;
    }
}


void Call::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"Call:%s\" fillcolor=lightblue]\n", this, statement_name);

    for (int i = 0; i < num_of_vars; i++){
        
        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, arr_of_vars[i]);
        arr_of_vars[i]->print_graphviz(out_file);
    }

    print_next(out_file);
}

void Call::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}



Return_statement::~Return_statement(){

    delete ret_stat;
    delete dest_var;
}

void Return_statement::add_call(Call* new_call){

    delete ret_stat;
    ret_stat = new_call;
}

void Return_statement::add_var(Var* new_var){

    delete dest_var;
    dest_var = new_var;
}

char* Return_statement::get_var_name_copy() const{

    return dest_var->get_name_copy();
}

void Return_statement::transfer_request_call(Irequest* cur_req){

    ret_stat->get_request(cur_req);
}

void Return_statement::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"Return\" fillcolor=orange]\n", this);

    if (ret_stat != nullptr){

        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, ret_stat);
        ret_stat->print_graphviz(out_file);
    }

    if (dest_var != nullptr){

        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, dest_var);
        dest_var->print_graphviz(out_file);
    }

    print_next(out_file);
}

void Return_statement::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}



Statements_return::~Statements_return(){

    delete ret_var;
}

void Statements_return::add_var(Var* new_var){

    delete ret_var;
    ret_var = new_var;
}

char* Statements_return::get_var_name_copy() const{

    return ret_var->get_name_copy();
}

void Statements_return::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"Return_val\" fillcolor=pink]\n", this);

    if (ret_var != nullptr){

        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, ret_var);
        ret_var->print_graphviz(out_file);
    }

    print_next(out_file);
}

void Statements_return::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}



If::~If(){

    for (int i = 0; i < num_of_calls; i++){

        delete arr_of_calls[i];
    }

    delete[] arr_of_calls;
}

void If::add_condition(Ioperator* new_condition){
        
    delete condition;
    condition = new_condition;
}

void If::add_new_call(Call* new_call){

    if (new_call == nullptr){ return; }

    Call** new_arr_of_calls = new Call*[num_of_calls + 1];

    new_arr_of_calls[num_of_calls] = new_call;

    if (num_of_calls > 0){

        memcpy(new_arr_of_calls, arr_of_calls, sizeof(Var*) * num_of_calls);
    }

    num_of_calls++;
    delete[] arr_of_calls;

    arr_of_calls = new_arr_of_calls;
}

char* If::get_call_name_copy(int num_of_call) const{

    if (num_of_call < num_of_calls){

        return arr_of_calls[num_of_call]->get_stat_name_copy();
    } else{

        return nullptr;
    }
}

int If::get_num_of_calls(){

    return num_of_calls;
}

void If::transfer_request_condition(Irequest* cur_req){

    if (condition != nullptr){

        condition->get_request(cur_req);
    } else{

        cur_req->terminal_request();
    }
}

void If::transfer_request_call(Irequest* cur_req, int num_of_call){

    if (num_of_call < num_of_calls){

        arr_of_calls[num_of_call]->get_request(cur_req);
    } else{

        cur_req->terminal_request();
    }
}

void If::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"If\" fillcolor=lightblue]\n", this);

    for (int i = 0; i < num_of_calls; i++){
        
        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, arr_of_calls[i]);
        arr_of_calls[i]->print_graphviz(out_file);
    }

    fprintf(out_file, "\"%p\" -> \"%p\"\n", this, condition);
    condition->print_graphviz(out_file);

    print_next(out_file);
}

void If::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}



Cycle::~Cycle(){

    delete condition;
    delete loop_body;
}

void Cycle::add_condition(Ioperator* new_condition){

    delete condition;
    condition = new_condition;
}

void Cycle::add_loop_body(Call* new_loop_body){

    delete loop_body;
    loop_body = new_loop_body;
}

void Cycle::transfer_request_call(Irequest* cur_req){

    loop_body->get_request(cur_req);
}

void Cycle::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"While\" fillcolor=lightblue]\n", this);

    fprintf(out_file, "\"%p\" -> \"%p\"\n", this, loop_body);
    loop_body->print_graphviz(out_file);

    fprintf(out_file, "\"%p\" -> \"%p\"\n", this, condition);
    condition->print_graphviz(out_file);

    print_next(out_file);
}

void Cycle::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}


Assign::~Assign(){

    delete l_value;
    delete r_value;
}

void Assign::add_l_value(Var* new_l_value){

        delete l_value;
        l_value = new_l_value;
    }

void Assign::add_r_value(Ioperator* new_r_value){

        delete r_value;
        r_value = new_r_value;
}

char* Assign::get_var_name_copy() const{

    return l_value->get_name_copy();
}

void Assign::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"=\" fillcolor=lightblue]\n", this);

    fprintf(out_file, "\"%p\" -> \"%p\"\n", this, l_value);
    l_value->print_graphviz(out_file);

    fprintf(out_file, "\"%p\" -> \"%p\"\n", this, r_value);
    r_value->print_graphviz(out_file);

    print_next(out_file);
}

void Assign::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}
       