#pragma once

#define GRAPH_PATH "../bin/graph.dot"

typedef int DataType;

enum Data_types{
    
    T_char = 0,
    T_int = 1,
    T_float = 2,
    T_arr = 3,
    T_expr = 4
};

struct Initialization_prot{

    char* name = nullptr;
    DataType type;
};

struct Initialization_list{

    int num_of_vars = 0;
    Initialization_prot* list_of_vars = nullptr;
};

class Inode{
    
public:
    
    virtual void print_graphviz() const = 0;
    //virtual Inode* copy_tree() const;
    virtual ~Inode(){};
};