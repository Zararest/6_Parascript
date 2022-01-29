#include "headers/Ioperand.h"

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



Num::Num(double new_value){

    value = new_value;
}

void Num::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"%lf\" fillcolor=yellow]\n", this, value);
}



String::~String(){

    delete[] string;
}

void String::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"%s\" fillcolor=yellow]\n", this, string);
}