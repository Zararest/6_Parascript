#include "headers/Icontrol_struct.h"

void Icontrol_struct::print_next(FILE* out_file) const{

    if (next_struct != nullptr){

        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, next_struct);
        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, next_struct);
        next_struct->print_graphviz(out_file);
    }
}

void Icontrol_struct::add_control_struct(Icontrol_struct* new_struct){   //тоже пробрасывает через цепочку, но теперь через control_struct

    if (next_struct != nullptr){

        next_struct->add_control_struct(next_struct);
    } else{

        next_struct = new_struct;
    }
} 

Icontrol_struct::~Icontrol_struct(){

    delete next_struct;
};



Statement::~Statement(){

    delete_initial_list(init_list);
    delete[] statement_name;
    delete first_expr;
}

bool Statement::add_first_expr(Icontent_of_statement* new_first_expr){

    if (new_first_expr == nullptr){ return false; }

    delete first_expr;
    first_expr = new_first_expr;

    return true;
}

/**
 * @brief Всегда возвращает true.
 * 
 * @param new_init_list 
 * @return true 
 * @return false 
 */
bool Statement::add_init_list(Initialization_list new_init_list){

    delete_initial_list(init_list);
    new_init_list = init_list;

    return true;
}

bool Statement::add_name(char* name){

    if (name == nullptr){ return false; }

    delete[] statement_name;
    statement_name = name;

    return true;
}

void Statement::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"Statement:%s[", this, statement_name);

    if (init_list.num_of_vars > 0){

        print_type(out_file, init_list.list_of_vars[0].type);
        fprintf(out_file, " %s", init_list.list_of_vars[0].name);
    }

    for (int i = 1; i < init_list.num_of_vars; i++){

        fprintf(out_file, ", ");
        print_type(out_file, init_list.list_of_vars[i].type);
        fprintf(out_file, " %s", init_list.list_of_vars[i].name);
    }

    fprintf(out_file, "]\" fillcolor = red]\n");

    if (first_expr != nullptr){

        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, first_expr);
        first_expr->print_graphviz(out_file);
    }

    print_next(out_file);
}