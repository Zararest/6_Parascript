#pragma once
#include "Inode.h"

class Ioperator: public Inode{

protected:

    Inode* left = nullptr;
    Inode* right = nullptr;

    void print_nodes(FILE* out_file) const{

        if (left != nullptr){

            fprintf(out_file, "\"%p\" -> \"%p\"\n", this, left);
            left->print_graphviz(out_file);
        }

        if (right != nullptr){

            fprintf(out_file, "\"%p\" -> \"%p\"\n", this, right);
            right->print_graphviz(out_file);
        }
    }

public: 

    Ioperator() = default;

    Ioperator(Ioperator* new_left, Ioperator* new_right){
        
        left = new_left;
        right = new_right;
    }
    
    virtual void print_graphviz(FILE* out_file) const = 0;

    virtual ~Ioperator(){

        delete left;
        delete right;
    };
};

/*
class Custom_op_with_ret final: public Ioperator{

    char* op_name = nullptr;
    Var** arr_of_vars = nullptr;

public:

    Custom_op_with_ret* copy_tree() const;
};
*/

class Logical_or final: public Ioperator{

public:

    Logical_or(Ioperator* new_left, Ioperator* new_right): Ioperator(new_left, new_right){}
    
    ~Logical_or(){}

    void print_graphviz(FILE* out_file) const override{

        fprintf(out_file, "\"%p\" [label = \"||\" fillcolor=green]\n", this);

        print_nodes(out_file);
    }
};

class Logical_and final: public Ioperator{

public:

    Logical_and(Ioperator* new_left, Ioperator* new_right): Ioperator(new_left, new_right){}

    ~Logical_and(){}

    void print_graphviz(FILE* out_file) const override{

        fprintf(out_file, "\"%p\" [label = \"&&\" fillcolor=green]\n", this);

        print_nodes(out_file);
    }
};

class Equality final: public Ioperator{

    bool equal = false;

public:

    Equality(Ioperator* new_left, bool is_equal, Ioperator* new_right): Ioperator(new_left, new_right){

        equal = is_equal;
    }

    ~Equality(){}

    void print_graphviz(FILE* out_file) const override{

        if (equal){

            fprintf(out_file, "\"%p\" [label = \"==\" fillcolor=green]\n", this);
        } else{

            fprintf(out_file, "\"%p\" [label = \"!=\" fillcolor=green]\n", this);
        }

        print_nodes(out_file);
    }
};

class Greater final: public Ioperator{

    bool and_equal = false;

public:

    Greater(Ioperator* new_left, bool is_equal, Ioperator* new_right): Ioperator(new_left, new_right){

        and_equal = is_equal;
    }  

    ~Greater(){}

    void print_graphviz(FILE* out_file) const override{

        if (and_equal){

            fprintf(out_file, "\"%p\" [label = \">=\" fillcolor=green]\n", this);
        } else{

            fprintf(out_file, "\"%p\" [label = \">\" fillcolor=green]\n", this);
        }

        print_nodes(out_file);
    }
};

class Less final: public Ioperator{

    bool and_equal = false;

public:

    Less(Ioperator* new_left, bool is_equal, Ioperator* new_right): Ioperator(new_left, new_right){

        and_equal = is_equal;
    }

    ~Less(){}

    void print_graphviz(FILE* out_file) const override{

        if (and_equal){

            fprintf(out_file, "\"%p\" [label = \"<=\" fillcolor=green]\n", this);
        } else{

            fprintf(out_file, "\"%p\" [label = \"<\" fillcolor=green]\n", this);
        }

        print_nodes(out_file);
    }
};

class Bin_plus final: public Ioperator{

public:

    Bin_plus(Ioperator* new_left, Ioperator* new_right): Ioperator(new_left, new_right){}

    ~Bin_plus(){}

    void print_graphviz(FILE* out_file) const override{

        fprintf(out_file, "\"%p\" [label = \"...+...\" fillcolor=green]\n", this);

        print_nodes(out_file);
    }
};

class Bin_minus final: public Ioperator{

public:

    Bin_minus(Ioperator* new_left, Ioperator* new_right): Ioperator(new_left, new_right){}

    ~Bin_minus(){}

    void print_graphviz(FILE* out_file) const override{

        fprintf(out_file, "\"%p\" [label = \"...-...\" fillcolor=green]\n", this);

        print_nodes(out_file);
    }
};

class Mul final: public Ioperator{

public:

    Mul(Ioperator* new_left, Ioperator* new_right): Ioperator(new_left, new_right){}

    ~Mul(){}

    void print_graphviz(FILE* out_file) const override{

        fprintf(out_file, "\"%p\" [label = \"...*...\" fillcolor=green]\n", this);

        print_nodes(out_file);
    }
};

class Div final: public Ioperator{

public:

    Div(Ioperator* new_left, Ioperator* new_right): Ioperator(new_left, new_right){}

    ~Div(){}

    void print_graphviz(FILE* out_file) const override{

        fprintf(out_file, "\"%p\" [label = \".../...\" fillcolor=green]\n", this);

        print_nodes(out_file);
    }
};

class Number_sign final: public Ioperator{

    bool less_than_zero = false;
    Ioperand* operand = nullptr;

public:

    explicit Number_sign(Ioperand* new_operand, bool is_less_than_zero){

        operand = new_operand;
        less_than_zero = is_less_than_zero;
    }

    explicit Number_sign(Ioperator* new_right, bool is_less_than_zero){

        right = new_right;
        less_than_zero = is_less_than_zero;
    }

    ~Number_sign(){}

    void print_graphviz(FILE* out_file) const override{

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
};


