#pragma once
#include "Inode.h"

class Ioperand: public Inode{

public: 
    
    Ioperand() = default;
    Ioperand(const Ioperand&) = delete;
    virtual ~Ioperand(){};
    
    Ioperand& operator =(const Ioperand&) = delete;

    virtual void print_graphviz(FILE* out_file) const = 0;
    virtual void get_request(Irequest* cur_req) = 0;
};



class Var final: public Ioperand{ 

    bool inner = false;
    char* name = nullptr;

public:

    Var(char* new_name, bool is_inner);
    Var(const Var&) = delete;
    ~Var();

    Var& operator =(const Var&) = delete;

    char* get_name_copy() const;
    bool is_inner() const;

    void print_graphviz(FILE* out_file) const override;
    void get_request(Irequest* cur_req) override;
};



class Num final: public Ioperand{

    double value = 0;

public:

    Num(double new_value);
    Num(const Num&) = delete;
    ~Num() = default;

    Num& operator =(const Num&) = delete;

    double get_value();

    void print_graphviz(FILE* out_file) const override;
    void get_request(Irequest* cur_req) override;
};



class String final: public Ioperand{

    char* string = nullptr;
    int size = 0;

public:

    String() = default;
    String(const String&) = delete;
    ~String();

    String& operator =(const String&) = delete;

    void print_graphviz(FILE* out_file) const override;
    void get_request(Irequest* cur_req) override;
};
