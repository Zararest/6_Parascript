#pragma once
#include "Icontent_of_statement.h"
#include "Icontrol_struct.h"
#include "Ioperand.h"
#include "Ioperator.h"
#include "../../Parcer/headers/Parcer.h"

typedef int DataType;

enum Data_types{

};

struct Initialization_prot{

    char* name;
    DataType type;
};

class Inode{

public:

    virtual void print_graphviz(FILE* outp_file);
    virtual Inode* copy_tree() const;
    virtual ~Inode(){};
};

class Lexer{

    Icontrol_struct* root = nullptr;
    std::list<lexem>& tokens;

public:


};





    

    



