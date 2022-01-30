#include "headers/Ioperator.h"

void Ioperator::transfer_request_left(Irequest* cur_req){

    if (left != nullptr){

        left->get_request(cur_req);
    } else{

        cur_req->terminal_request();
    }
}

void Ioperator::transfer_request_right(Irequest* cur_req){

    if (right != nullptr){

        right->get_request(cur_req);
    } else{

        cur_req->terminal_request();
    }
}

void Ioperator::print_nodes(FILE* out_file) const{

    if (left != nullptr){

        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, left);
        left->print_graphviz(out_file);
    }

    if (right != nullptr){

        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, right);
        right->print_graphviz(out_file);
    }
}

Ioperator::Ioperator(Ioperator* new_left, Ioperator* new_right){
        
    left = new_left;
    right = new_right;
}

Ioperator::~Ioperator(){

    delete left;
    delete right;
};



void Logical_or::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"||\" fillcolor=green]\n", this);

    print_nodes(out_file);
}

void Logical_or::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}



void Logical_and::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"&&\" fillcolor=green]\n", this);

    print_nodes(out_file);
}

void Logical_and::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}



Equality::Equality(Ioperator* new_left, bool is_equal, Ioperator* new_right): Ioperator(new_left, new_right){

    equal = is_equal;
}

void Equality::print_graphviz(FILE* out_file) const{

    if (equal){

        fprintf(out_file, "\"%p\" [label = \"==\" fillcolor=green]\n", this);
    } else{

        fprintf(out_file, "\"%p\" [label = \"!=\" fillcolor=green]\n", this);
    }

    print_nodes(out_file);
}

void Equality::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}



Greater::Greater(Ioperator* new_left, bool is_equal, Ioperator* new_right): Ioperator(new_left, new_right){

    and_equal = is_equal;
}  

void Greater::print_graphviz(FILE* out_file) const{

    if (and_equal){

        fprintf(out_file, "\"%p\" [label = \">=\" fillcolor=green]\n", this);
    } else{

        fprintf(out_file, "\"%p\" [label = \">\" fillcolor=green]\n", this);
    }

    print_nodes(out_file);
}

void Greater::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}



Less::Less(Ioperator* new_left, bool is_equal, Ioperator* new_right): Ioperator(new_left, new_right){

    and_equal = is_equal;
}

void Less::print_graphviz(FILE* out_file) const{

    if (and_equal){

        fprintf(out_file, "\"%p\" [label = \"<=\" fillcolor=green]\n", this);
    } else{

        fprintf(out_file, "\"%p\" [label = \"<\" fillcolor=green]\n", this);
    }

    print_nodes(out_file);
}

void Less::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}



void Bin_plus::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"...+...\" fillcolor=green]\n", this);

    print_nodes(out_file);
}

void Bin_plus::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}



void Bin_minus::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"...-...\" fillcolor=green]\n", this);

    print_nodes(out_file);
}

void Bin_minus::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}



void Mul::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \"...*...\" fillcolor=green]\n", this);

    print_nodes(out_file);
}

void Mul::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}



void Div::print_graphviz(FILE* out_file) const{

    fprintf(out_file, "\"%p\" [label = \".../...\" fillcolor=green]\n", this);

    print_nodes(out_file);
}

void Div::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}



Number_sign::Number_sign(Ioperand* new_operand, bool is_less_than_zero){

    operand = new_operand;
    less_than_zero = is_less_than_zero;
}

Number_sign::Number_sign(Ioperator* new_right, bool is_less_than_zero){

    right = new_right;
    less_than_zero = is_less_than_zero;
}

void Number_sign::print_graphviz(FILE* out_file) const{

    if (less_than_zero){

        fprintf(out_file, "\"%p\" [label = \"-...\" fillcolor=green]\n", this);
    } else{

        fprintf(out_file, "\"%p\" [label = \"+...\" fillcolor=green]\n", this);
    }

    if (operand != nullptr){
        
        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, operand);
        operand->print_graphviz(out_file);
    }

    print_nodes(out_file);
}

void Number_sign::get_request(Irequest* cur_req){ 

    cur_req->process_request(this);
}