#pragma once
#include "Inode.h"
#include "Ioperand.h"

class Ioperator: public Inode{

protected:

    Inode* left = nullptr;
    Inode* right = nullptr;

    void print_nodes(FILE* out_file) const;

public: 

    Ioperator() = default;
    Ioperator(Ioperator* new_left, Ioperator* new_right);
    Ioperator(const Ioperator&) = delete;
    virtual ~Ioperator();

    Ioperator& operator =(const Ioperator&) = delete;

    void transfer_request_left(Irequest* cur_req);
    void transfer_request_right(Irequest* cur_req);

    virtual void print_graphviz(FILE* out_file) const = 0;
};



class Logical_or final: public Ioperator{

public:

    Logical_or(Ioperator* new_left, Ioperator* new_right): Ioperator(new_left, new_right){}
    Logical_or(const Logical_or&) = delete;
    ~Logical_or() = default;

    Logical_or& operator =(const Logical_or&) = delete;

    void print_graphviz(FILE* out_file) const override;
    void get_request(Irequest* cur_req) override;
};



class Logical_and final: public Ioperator{

public:

    Logical_and(Ioperator* new_left, Ioperator* new_right): Ioperator(new_left, new_right){}
    Logical_and(const Logical_and&) = delete;
    ~Logical_and() = default;

    Logical_and& operator =(const Logical_and&) = delete;

    void print_graphviz(FILE* out_file) const override;
    void get_request(Irequest* cur_req) override;
};



class Equality final: public Ioperator{

    bool equal = false;

public:

    Equality(Ioperator* new_left, bool is_equal, Ioperator* new_right);
    Equality(const Equality&) = delete;
    ~Equality() = default;

    Equality& operator =(const Equality&) = delete;

    void print_graphviz(FILE* out_file) const override;
    void get_request(Irequest* cur_req) override;
};



class Greater final: public Ioperator{

    bool and_equal = false;

public:

    Greater(Ioperator* new_left, bool is_equal, Ioperator* new_right);
    Greater(const Greater&) = delete;
    ~Greater() = default;

    Equality& operator =(const Equality&) = delete;

    void print_graphviz(FILE* out_file) const override;
    void get_request(Irequest* cur_req) override;
};



class Less final: public Ioperator{

    bool and_equal = false;

public:

    Less(Ioperator* new_left, bool is_equal, Ioperator* new_right);
    Less(const Less&) = delete;
    ~Less() = default;

    Less& operator =(const Less&) = delete;

    void print_graphviz(FILE* out_file) const override;
    void get_request(Irequest* cur_req) override;
};



class Bin_plus final: public Ioperator{

public:

    Bin_plus(Ioperator* new_left, Ioperator* new_right): Ioperator(new_left, new_right){}
    Bin_plus(const Bin_plus&) = delete;
    ~Bin_plus() = default;

    Bin_plus& operator =(const Bin_plus&) = delete;

    void print_graphviz(FILE* out_file) const override;
    void get_request(Irequest* cur_req) override;
};



class Bin_minus final: public Ioperator{

public:

    Bin_minus(Ioperator* new_left, Ioperator* new_right): Ioperator(new_left, new_right){}
    Bin_minus(const Bin_minus&) = delete;
    ~Bin_minus() = default;

    Bin_minus& operator =(const Bin_minus&) = delete;

    void print_graphviz(FILE* out_file) const override;
    void get_request(Irequest* cur_req) override;
};



class Mul final: public Ioperator{

public:

    Mul(Ioperator* new_left, Ioperator* new_right): Ioperator(new_left, new_right){}
    Mul(const Mul&) = delete;
    ~Mul() = default;

    Mul& operator =(const Mul&) = delete;

    void print_graphviz(FILE* out_file) const override;
    void get_request(Irequest* cur_req) override;
};



class Div final: public Ioperator{

public:

    Div(Ioperator* new_left, Ioperator* new_right): Ioperator(new_left, new_right){}
    Div(const Div&) = delete;
    ~Div() = default;

    Div& operator =(const Div&) = delete;

    void print_graphviz(FILE* out_file) const override;
    void get_request(Irequest* cur_req) override;
};



class Number_sign final: public Ioperator{

    bool less_than_zero = false;
    Ioperand* operand = nullptr;

public:

    explicit Number_sign(Ioperand* new_operand, bool is_less_than_zero);
    explicit Number_sign(Ioperator* new_right, bool is_less_than_zero);
    Number_sign(const Number_sign&) = delete;
    ~Number_sign() = default;

    Number_sign& operator =(const Number_sign&) = delete;

    void print_graphviz(FILE* out_file) const override;
    void get_request(Irequest* cur_req) override;
};


