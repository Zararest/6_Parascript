#pragma once
#include "Inode.h"
#include "Ioperand.h"
#include "Ioperator.h"

class Icontent_of_statement: public Inode{

protected: 

    Icontent_of_statement* next_expr = nullptr;

    void print_next() const{

        FILE* out_file = fopen(GRAPH_PATH, "a");

        if (next_expr != nullptr){

            fprintf(out_file, "\"%p\" -> \"%p\"\n", this, next_expr);
            fprintf(out_file, "\"%p\" -> \"%p\"\n", this, next_expr);
            next_expr->print_graphviz();
        }

        fclose(out_file);
    }

public: 

    void add_content_of_statement(Icontent_of_statement* new_content){

        if (next_expr != nullptr){

            next_expr->add_content_of_statement(new_content);
        } else{

            next_expr = new_content;
        }
    }

    virtual ~Icontent_of_statement(){

        delete next_expr;
    };

    virtual void print_graphviz() const = 0;
};

class Call final: public Icontent_of_statement{

    char* statement_name = nullptr;
    Var** arr_of_vars = nullptr;
    int num_of_vars = 0;

public:

    ~Call(){

        for (int i = 0; i < num_of_vars; i++){

            delete arr_of_vars[i];
        }

        delete[] arr_of_vars;
        delete[] statement_name;
    }

    void add_name(char* new_name){

        delete[] statement_name;

        statement_name = new_name;
    }

    void add_new_var(Var* new_var){   //должен чекать на nullptr

        if (new_var == nullptr){ return; }

        Var** new_arr_of_vars = new Var*[num_of_vars + 1];

        new_arr_of_vars[num_of_vars] = new_var;

        if (num_of_vars > 0){

            memcpy(new_arr_of_vars, arr_of_vars, sizeof(Var*) * num_of_vars);
        }

        num_of_vars++;
        delete[] arr_of_vars;

        arr_of_vars = new_arr_of_vars;
    } 

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");

        fprintf(out_file, "\"%p\" [label = \"Call:%s\" fillcolor=lightblue]\n", this, statement_name);

        for (int i = 0; i < num_of_vars; i++){
            
            fprintf(out_file, "\"%p\" -> \"%p\"\n", this, arr_of_vars[i]);
            arr_of_vars[i]->print_graphviz();
        }

        fclose(out_file);

        print_next();
    }
};

class If final: public Icontent_of_statement{

    Ioperator* condition = nullptr; //тут может быть и нестандартный оператор
    Call** arr_of_calls = nullptr;
    int num_of_calls = 0;

public:

    ~If(){

        for (int i = 0; i < num_of_calls; i++){

            delete arr_of_calls[i];
        }

        delete[] arr_of_calls;
    }

    void add_condition(Ioperator* new_condition){
        
        delete condition;

        condition = new_condition;
    }

    void add_new_call(Call* new_call){

        if (new_call == nullptr){ return; }

        Call** new_arr_of_calls = new Call*[num_of_calls + 1];

        new_arr_of_calls[num_of_calls] = new_call;

        if (num_of_calls > 0){

            memcpy(new_arr_of_calls, arr_of_calls, sizeof(Var*) * num_of_calls);
        }

        num_of_calls++;
        delete[] arr_of_calls;

        arr_of_calls = new_arr_of_calls;
    }

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");

        fprintf(out_file, "\"%p\" [label = \"If\" fillcolor=lightblue]\n", this);

        for (int i = 0; i < num_of_calls; i++){
            
            fprintf(out_file, "\"%p\" -> \"%p\"\n", this, arr_of_calls[i]);
            arr_of_calls[i]->print_graphviz();
        }

        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, condition);
        condition->print_graphviz();

        fclose(out_file);

        print_next();
    }
};

class Cycle final: public Icontent_of_statement{

    Ioperator* condition = nullptr;
    Call* loop_body = nullptr;

public:

    ~Cycle(){

        delete condition;
        delete loop_body;
    }

    void add_condition(Ioperator* new_condition){

        delete condition;

        condition = new_condition;
    }

    void add_loop_body(Call* new_loop_body){

        delete loop_body;

        loop_body = new_loop_body;
    }

    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");

        fprintf(out_file, "\"%p\" [label = \"While\" fillcolor=lightblue]\n", this);

        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, loop_body);
        loop_body->print_graphviz();

        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, condition);
        condition->print_graphviz();

        fclose(out_file);

        print_next();
    }
};

//потом
class Custom_operator final: public Icontent_of_statement{

    char* op_name = nullptr;
    Var** arr_of_vars = nullptr;

public:

    ~Custom_operator(){}
};
//потом
    
class Assign final: public Icontent_of_statement{

    Var* l_value = nullptr;
    Ioperator* r_value = nullptr;

public:

    ~Assign(){

        delete l_value;
        delete r_value;
    }

    void add_l_value(Var* new_l_value){

        delete l_value;

        l_value = new_l_value;
    }

    void add_r_value(Ioperator* new_r_value){

        delete r_value;

        r_value = new_r_value;
    }
    
    void print_graphviz() const override{

        FILE* out_file = fopen(GRAPH_PATH, "a");

        fprintf(out_file, "\"%p\" [label = \"=\" fillcolor=lightblue]\n", this);

        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, l_value);
        l_value->print_graphviz();

        fprintf(out_file, "\"%p\" -> \"%p\"\n", this, r_value);
        r_value->print_graphviz();

        fclose(out_file);

        print_next();
    }
};