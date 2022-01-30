#include "headers/Ioperand.h"

#include <cassert>

/**
 * @brief Нет копирования имени
 * 
 * @param new_name 
 * @param is_inner 
 */
Var::Var(char* new_name, bool is_inner){

    name = new_name;
    inner = is_inner;
}

Var::~Var(){

    delete name;
}

void Var::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"%s\" fillcolor=yellow]\n", this, name);
}

char* Var::get_name_copy() const{

    assert(name != nullptr);
    int name_size = strlen(name) + 1;
    assert(name_size > 0);

    char* name_copy = new char[name_size];
    memcpy(name_copy, name, name_size);

    return name_copy;
}

bool Var::is_inner() const{

    return inner;
}

void Var::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}



Num::Num(double new_value){

    value = new_value;
}

double Num::get_value(){

    return value;
}

void Num::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"%lf\" fillcolor=yellow]\n", this, value);
}

void Num::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}



String::~String(){

    delete[] string;
}

void String::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"%s\" fillcolor=yellow]\n", this, string);
}

void String::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}