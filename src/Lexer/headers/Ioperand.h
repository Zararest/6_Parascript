#pragma once
#include "Inode.h"

class Ioperand: public Inode{

public: 
    
    Ioperand() = default;
    Ioperand(const Ioperand&) = delete;
    virtual ~Ioperand(){};
    
    Ioperand& operator =(const Ioperand&) = delete;

    virtual void print_graphviz(FILE* out_file) const = 0;
};



class Var final: public Ioperand{ 

    bool inner = false;
    char* name = nullptr;

public:

    Var(char* new_name, bool is_inner);
    Var(const Var&) = delete;
    ~Var();

    Var& operator =(const Var&) = delete;

    void print_graphviz(FILE* out_file) const override;
};



class Num final: public Ioperand{

    double value = 0;

public:

    Num(double new_value);
    Num(const Num&) = delete;
    ~Num() = default;

    Num& operator =(const Num&) = delete;

    void print_graphviz(FILE* out_file) const override;
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
};
