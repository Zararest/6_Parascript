#pragma once
#include "Inode.h"

class Icontrol_struct: public Inode{
    
protected:

    Icontrol_struct* next_struct = nullptr;

    void print_next() const{

        FILE* out_file = fopen(GRAPH_PATH, "a");

        if (next_struct != nullptr){

            fprintf(out_file, "\"%p\" -> \"%p\"\n", this, next_struct);
            fprintf(out_file, "\"%p\" -> \"%p\"\n", this, next_struct);
            next_struct->print_graphviz();
        }

        fclose(out_file);
    }

public:

    void add_control_struct(Icontrol_struct* new_struct); //тоже пробрасывает через цепочку, но теперь через control_struct

    virtual void print_graphviz() const;
    virtual ~Icontrol_struct(){

        delete next_struct;
    };
};

//потом
class Overload_op final: public Icontrol_struct{

    char* op_name = nullptr;
    Initialization_list arr_of_initializations;
    Icontrol_struct* first_expr = nullptr;

public:

    void print_graphviz() const override;
    Overload_op* copy_tree() const;
};

void Overload_op::print_graphviz() const{

    FILE* out_file = fopen(GRAPH_PATH, "a");

}

class Overload_set final: public Icontrol_struct{

    char* set_name = nullptr;
    Overload_op* first_overload = nullptr;

public:

    void add_operator(Overload_op* new_op); //должен прокидывать эту ссылку через цепочку операторов(first_overload)
    bool add_name(char* name);
    Overload_set* copy_tree() const;
};

class Use final: public Icontrol_struct{

    char** arr_of_overloads = nullptr;

public:

    Use* copy_tree() const;
};
//потом

void print_type(FILE* out_file, DataType type){

    if (type == T_char){ fprintf(out_file, "char"); }
    if (type == T_int){ fprintf(out_file, "int"); }
    if (type == T_float){ fprintf(out_file, "float"); }
    if (type == T_arr){ fprintf(out_file, "arr"); }
    if (type == T_expr){ fprintf(out_file, "expr"); }
}

class Statement final: public Icontrol_struct{

    char* statement_name = nullptr;
    Initialization_list init_list;
    Icontent_of_statement* first_expr = nullptr;

public:

    ~Statement(){

        for (int i = 0; i < init_list.num_of_vars; i++){

            delete[] init_list.list_of_vars[i].name;
        }

        delete init_list.list_of_vars;
        delete[] statement_name;
        delete first_expr;
    }

    bool add_first_expr(Icontent_of_statement* new_first_expr);
    bool add_init_list(Initialization_list new_init_list);
    bool add_name(char* name);

    //Statement* copy_tree() const;

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");

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
            first_expr->print_graphviz();
        }

        fclose(out_file);

        print_next();
    }
};

//потом
class Asm_code final: public Icontrol_struct{

    char* code = nullptr;

public:

   //Asm_code* copy_tree() const;
};
//потом