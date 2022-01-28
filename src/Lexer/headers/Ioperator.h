#pragma once
#include "Inode.h"

class Ioperator: public Inode{

protected:

    Inode* left = nullptr;
    Inode* right = nullptr;

    void print_nodes() const{

        FILE* out_file = fopen(GRAPH_PATH, "a");

        if (left != nullptr){

            fprintf(out_file, "\"%p\" -> \"%p\"\n", this, left);
            left->print_graphviz();
        }

        if (right != nullptr){

            fprintf(out_file, "\"%p\" -> \"%p\"\n", this, right);
            right->print_graphviz();
        }

        fclose(out_file);
    }

public: 

    Ioperator(Ioperator* new_left, Ioperator* new_right);
    virtual void print_graphviz() const = 0;

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

    Logical_or(Ioperator* new_left, Ioperator* new_right);
    
    ~Logical_or(){}

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");
        fprintf(out_file, "\"%p\" [label = \"||\" fillcolor=green]\n", this);
        fclose(out_file);

        print_nodes();
    }
};

class Logical_and final: public Ioperator{

public:

    Logical_and(Ioperator* new_left, Ioperator* new_right);

    ~Logical_and(){}

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");
        fprintf(out_file, "\"%p\" [label = \"&&\" fillcolor=green]\n", this);
        fclose(out_file);

        print_nodes();
    }
};

class Equality final: public Ioperator{

    bool equal = false;

public:

    Equality(Ioperator* new_left, bool is_equal, Ioperator* new_right);

    ~Equality(){}

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");

        if (equal){

            fprintf(out_file, "\"%p\" [label = \"==\" fillcolor=green]\n", this);
        } else{

            fprintf(out_file, "\"%p\" [label = \"!=\" fillcolor=green]\n", this);
        }

        fclose(out_file);

        print_nodes();
    }
};

class Greater final: public Ioperator{

    bool and_equal = false;

public:

    Greater(Ioperator* new_left, bool is_equal, Ioperator* new_right);  

    ~Greater(){}

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");

        if (and_equal){

            fprintf(out_file, "\"%p\" [label = \">=\" fillcolor=green]\n", this);
        } else{

            fprintf(out_file, "\"%p\" [label = \">\" fillcolor=green]\n", this);
        }

        fclose(out_file);

        print_nodes();
    }
};

class Less final: public Ioperator{

    bool and_equal = false;

public:

    Less(Ioperator* new_left, bool is_equal, Ioperator* new_right);  

    ~Less(){}

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");

        if (and_equal){

            fprintf(out_file, "\"%p\" [label = \"<=\" fillcolor=green]\n", this);
        } else{

            fprintf(out_file, "\"%p\" [label = \"<\" fillcolor=green]\n", this);
        }

        fclose(out_file);

        print_nodes();
    }
};

class Bin_plus final: public Ioperator{

public:

    Bin_plus(Ioperator* new_left, Ioperator* new_right);

    ~Bin_plus(){}

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");
        fprintf(out_file, "\"%p\" [label = \"...+...\" fillcolor=green]\n", this);
        fclose(out_file);

        print_nodes();
    }
};

class Bin_minus final: public Ioperator{

public:

    Bin_minus(Ioperator* new_left, Ioperator* new_right);

    ~Bin_minus(){}

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");
        fprintf(out_file, "\"%p\" [label = \"...-...\" fillcolor=green]\n", this);
        fclose(out_file);

        print_nodes();
    }
};

class Mul final: public Ioperator{

public:

    Mul(Ioperator* new_left, Ioperator* new_right);

    ~Mul(){}

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");
        fprintf(out_file, "\"%p\" [label = \"...*...\" fillcolor=green]\n", this);
        fclose(out_file);

        print_nodes();
    }
};

class Div final: public Ioperator{

public:

    Div(Ioperator* new_left, Ioperator* new_right);

    ~Div(){}

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");
        fprintf(out_file, "\"%p\" [label = \".../...\" fillcolor=green]\n", this);
        fclose(out_file);

        print_nodes();
    }
};

class Number_sign final: public Ioperator{

    bool less_than_zero = false;

public:

    Number_sign(Inode* new_right, bool is_less_than_zero);

    ~Number_sign(){}

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");

        if (less_than_zero){

            fprintf(out_file, "\"%p\" [label = \"-...\" fillcolor=green]\n", this);
        } else{

            fprintf(out_file, "\"%p\" [label = \"+...\" fillcolor=green]\n", this);
        }
        
        fclose(out_file);

        print_nodes();
    }
};


