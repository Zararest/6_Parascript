#pragma once
#include "Lexer.h"

class Ioperator: public Inode{

    Inode* left = nullptr;
    Inode* right = nullptr;

public: 

    Ioperator(Ioperator* new_left, Ioperator* new_right);
    virtual void print_graphviz(FILE* outp_file);
    virtual Inode* copy_tree() const;
    virtual ~Ioperator(){};
};

class Custom_op_with_ret final: public Ioperator{

    char* op_name = nullptr;
    Var** arr_of_vars = nullptr;

public:

    Custom_op_with_ret* copy_tree() const;
};

class Logical_or final: public Ioperator{

public:

    Logical_or(Ioperator* new_left, Ioperator* new_right);
};

class Logical_and final: public Ioperator{

public:

    Logical_and(Ioperator* new_left, Ioperator* new_right);
};

class Equality final: public Ioperator{

    bool equal = false;

public:

    Equality(Ioperator* new_left, bool is_equal, Ioperator* new_right);
};

class Greater final: public Ioperator{

    bool and_equal = false;

public:

    Greater(Ioperator* new_left, bool is_equal, Ioperator* new_right);    
};

class Less final: public Ioperator{

    bool and_equal = false;

public:

    Less(Ioperator* new_left, bool is_equal, Ioperator* new_right);  
};

class Bin_plus final: public Ioperator{

public:

    Bin_plus(Ioperator* new_left, Ioperator* new_right);
};

class Bin_minus final: public Ioperator{

public:

    Bin_minus(Ioperator* new_left, Ioperator* new_right);
};

class Mul final: public Ioperator{

public:

    Mul(Ioperator* new_left, Ioperator* new_right);
};

class Div final: public Ioperator{

public:

    Div(Ioperator* new_left, Ioperator* new_right);
};

class Number_sign final: public Ioperator{

    bool less_than_zero = false;

public:

    Number_sign(Inode* new_right, bool is_less_than_zero);
};


