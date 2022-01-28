#include "headers/Lexer.h"

#include <cstring>
#include <cassert>

Icontrol_struct* Lexer::get_syntax_tree(){

    root = get_block_of_code();
}

/**
 * @brief Сдвигает указатель.
 * 
 * @param req_token 
 * @param message 
 * @return true 
 * @return false 
 */
bool Lexer::require(const char* req_token, const char* message){

    if (req_token == nullptr){

        if (cur_pos != tokens.end()){

            fprintf(stderr, "Error:%s: |%s| in line %i\n", message, cur_pos->lexem, cur_pos->line_number);
            exit(0);
            return false;
        } else{

            return true;
        }
    }

    if (strcmp(cur_pos->lexem, req_token) == 0){

        cur_pos++;
        return true;
    } else{

        fprintf(stderr, "Error:%s: in line %i |%s| was expected ", message, cur_pos->line_number, req_token);
        fprintf(stderr, "but %s was received\n", cur_pos->lexem);
        exit(0);
        return false;
    }
}

bool Lexer::check_end_of_tokens(){

    if (cur_pos == tokens.end()){

        fprintf(stderr, "Error:end_of_file: lexem was expected, but end of file was reached\n");
        exit(0);
    }

    return true;
}

void Lexer::error(char* message){

    if (cur_pos == tokens.end()){

        fprintf(stderr, "Error:%s [end of file]\n");
    } else{

        fprintf(stderr, "Error:%s [%i]\n", cur_pos->line_number);
    }

    exit(0);
}


Icontrol_struct* Lexer::get_block_of_code(){

    Icontrol_struct* ret_node = nullptr;
    Icontrol_struct* tmp_node = nullptr;
    bool flag = false;

    while (cur_pos != tokens.end()){

        if ((tmp_node = get_overload_set()) != nullptr){

            flag = true;

            if (ret_node == nullptr){

                ret_node = tmp_node;
            } else {

                ret_node->add_control_struct(tmp_node);
            }
        }

        if (((tmp_node = get_statement()) != nullptr) && !flag){

            flag = true;
            
            if (ret_node == nullptr){

                ret_node = tmp_node;
            } else {

                ret_node->add_control_struct(tmp_node);
            }
        }

        if (((tmp_node = get_use()) != nullptr) && !flag){

            flag = true;
            
            if (ret_node == nullptr){

                ret_node = tmp_node;
            } else {

                ret_node->add_control_struct(tmp_node);
            }
        }

        if (!flag){

            require(nullptr, "undefined lexem");
        }

        flag = false;
    }
    
    return ret_node;
}

Overload_set* Lexer::get_overload_set(){

    if (cur_pos == tokens.end()){ return nullptr; }
    if (strcmp(cur_pos->lexem, "Overload_set") != 0){ return nullptr; }

    Overload_set* ret_node = new Overload_set;
    Overload_op* tmp_node = nullptr;

    cur_pos++; 
    check_end_of_tokens();

    require("{", "overload_set");
    check_end_of_tokens();

    if (!ret_node->add_name(get_statement_name())){ error("expected overload set name"); }

    require("}", "overload_set");
    check_end_of_tokens();
    
    while (strcmp(cur_pos->lexem, "end") != 0){

        tmp_node = get_overload_op();

        if (tmp_node != nullptr){

            ret_node->add_operator(tmp_node);
        } else{

            error("expected overload operator");
        }

        check_end_of_tokens();
    }

    cur_pos++;
        
    return ret_node;
}

Statement* Lexer::get_statement(){

    if (cur_pos == tokens.end()){ return nullptr; }
    if (strcmp(cur_pos->lexem, "{") != 0){ return nullptr; }

    Statement* ret_node = new Statement;

    cur_pos++;
    check_end_of_tokens();

    if (!ret_node->add_name(get_statement_name())){ error("expected statement name"); }

    require("}", "statement");
    check_end_of_tokens();

    require("<", "statement");
    check_end_of_tokens();

    ret_node->add_init_list(get_init_list());
    ret_node->add_first_expr(get_stat_content());

    require(">", "statement");
    check_end_of_tokens();

    return ret_node;
}

bool is_letter(char cur_symbol){

    if (cur_symbol >= 'a' && cur_symbol <= 'z'){ return true; }
    if (cur_symbol >= 'A' && cur_symbol <= 'Z'){ return true; }

    return false;
}

bool is_number(char cur_symbol){

    if (cur_symbol >= '0' && cur_symbol <= '9'){ return true; }

    return false;
}

/**
 * @brief Тут выделяется память под имя.
 * 
 * @return char* 
 */
char* Lexer::get_statement_name(){

    if (cur_pos == tokens.end()){ return nullptr; }
    assert(cur_pos->size > 1);

    char* lexem = cur_pos->lexem;

    if (!is_letter(lexem[0])){ return nullptr; }

    for (int i = 0; i < cur_pos->size - 1; i++){

        if (!is_letter(lexem[i])
            && (!is_number(lexem[i])
                || lexem[i] != '_')){ 
            
            return nullptr; 
        }
    }
    
    char* new_name = new char[cur_pos->size];
}

/**
 * @brief Сразу выкидывает ошибку
 * 
 * @return DataType 
 */
DataType Lexer::get_type(){

    if (cur_pos == tokens.end()){ error("expected variable type"); }

    if (strcmp("char", cur_pos->lexem) == 0){ return T_char; }
    if (strcmp("int", cur_pos->lexem) == 0){ return T_int; }
    if (strcmp("float", cur_pos->lexem) == 0){ return T_float; }
    if (strcmp("arr", cur_pos->lexem) == 0){ return T_arr; }
    if (strcmp("expr", cur_pos->lexem) == 0){ return T_expr; }

    error("incorrect type name");
    return -1;
}

/**
 * @brief Сразу выкидывает ошибку
 * 
 * @return char* 
 */
char* Lexer::get_var_name(){

    char* ret_val = get_statement_name();

    if (ret_val == nullptr){

        error("incorrect variable name");
    }

    return ret_val;
}

/**
 * @brief В get_statement_name() выделяется память под переменную.
 * 
 * @param init_list 
 * @param new_var_type 
 * @param new_var_name 
 */
void add_new_var(Initialization_list& init_list, DataType new_var_type, char* new_var_name){

    Initialization_prot* new_list = new Initialization_prot[init_list.num_of_vars + 1];

    new_list[init_list.num_of_vars].type = new_var_type;
    new_list[init_list.num_of_vars].name = new_var_name;

    if (init_list.num_of_vars != 0){

        memcpy(new_list, init_list.list_of_vars, sizeof(Initialization_prot) * init_list.num_of_vars);
    }

    init_list.num_of_vars++;
    delete[] init_list.list_of_vars;

    init_list.list_of_vars = new_list;
}

/**
 * @brief Выкидывает ошибку если встретился конец
 * 
 * @return Initialization_list 
 */
Initialization_list Lexer::get_init_list(){

    if (cur_pos == tokens.end()){ error("expected Initialization list"); }
    if (strcmp(cur_pos->lexem, "[") != 0){ return Initialization_list{0, nullptr}; }

    int num_of_vars = 0;
    char* new_var_name = nullptr;
    DataType new_var_type;
    Initialization_list ret_val;

    cur_pos++;
    check_end_of_tokens();

    while (strcmp(cur_pos->lexem, "]") != 0){ 

        new_var_type = get_type();
        new_var_name = get_var_name();

        add_new_var(ret_val, new_var_type, new_var_name);

        check_end_of_tokens();

        if (strcmp(cur_pos->lexem, "]") != 0){

            require(",", "init_list");
        }

        check_end_of_tokens();
    }

    cur_pos++;

    return ret_val;
}

Icontent_of_statement* Lexer::get_stat_content(){

    Icontent_of_statement* ret_node = nullptr;
    Icontent_of_statement* tmp_node = nullptr;
    bool found_expr = false;

    do{
        found_expr = false;

        if (((tmp_node = get_if()) != nullptr) && !found_expr){

            found_expr = true;

            if (ret_node == nullptr){

                ret_node = tmp_node;
            } else{

                ret_node->add_content_of_statement(tmp_node);
            }
        }

        if (((tmp_node = get_cycle()) != nullptr) && !found_expr){

            found_expr = true;

            if (ret_node == nullptr){

                ret_node = tmp_node;
            } else{

                ret_node->add_content_of_statement(tmp_node);
            }
        }

        if (((tmp_node = get_assign()) != nullptr) && !found_expr){

            found_expr = true;

            if (ret_node == nullptr){

                ret_node = tmp_node;
            } else{

                ret_node->add_content_of_statement(tmp_node);
            }
        }

        if (((tmp_node = get_call()) != nullptr) && !found_expr){

            found_expr = true;

            if (ret_node == nullptr){

                ret_node = tmp_node;
            } else{

                ret_node->add_content_of_statement(tmp_node);
            }
        }

        /*if (((tmp_node = get_cust_op()) != nullptr) && !found_expr){

            found_expr = true;

            if (ret_node == nullptr){

                ret_node = tmp_node;
            } else{

                ret_node->add_content_of_statement(tmp_node);
            }
        }*/ //потом надо добавить

    } while(found_expr);

    return ret_node;
}

/**
 * @brief Отсутствие выражения всегда 0. 
 * 
 * @return If* 
 */
If* Lexer::get_if(){

    if (cur_pos == tokens.end()){ return nullptr; }
    if (strcmp(cur_pos->lexem, "if") != 0){ return nullptr; }

    If* ret_val = new If;
    Call* tmp_node = nullptr;

    cur_pos++;
    check_end_of_tokens();

    require("(", "if_statement");
    check_end_of_tokens();

    ret_val->add_condition(get_expr());

    require(")", "if_statement");
    check_end_of_tokens();
     
    require("{", "if_statement");
    check_end_of_tokens();

    while (strcmp(cur_pos->lexem, "}") != 0){

        tmp_node = get_call();

        if (tmp_node == nullptr){ 

            error("if_statement: function call was expected");
        }

        ret_val->add_new_call(tmp_node);

        check_end_of_tokens();

        if (strcmp(cur_pos->lexem, "}") != 0){

            require(",", "init_list");
        }

        check_end_of_tokens();
    }

    cur_pos++;

    return ret_val;
}

/**
 * @brief Проверяет через одну лексему и возвращает nullptr.
 * 
 * @return Assign* 
 */
Assign* Lexer::get_assign(){

    if (cur_pos == tokens.end()){ return nullptr; }
    cur_pos++;
    if ((cur_pos != tokens.end()) && (strcmp(cur_pos->lexem, "=") != 0)){ return nullptr; }
    cur_pos--;

    Assign* ret_val = new Assign;
    Var* new_var = nullptr;
    Ioperator* new_expr = nullptr;

    new_var = get_var();

    require("=", "assign was expected");
    check_end_of_tokens();

    new_expr = get_expr();

    if (new_expr == nullptr){

        error("assign: R_value was expected");
    }

    ret_val->add_l_value(new_var);
    ret_val->add_r_value(new_expr);

    return ret_val;
}   

Cycle* Lexer::get_cycle(){

    if (cur_pos == tokens.end()){ return nullptr; }
    if (strcmp(cur_pos->lexem, "while") != 0){ return nullptr; }
    cur_pos++;

    Cycle* ret_val = new Cycle;
    Ioperator* new_condition = nullptr;
    Call* new_call = nullptr;

    require("(", "while");
    check_end_of_tokens();

    new_condition = get_expr();

    if (new_condition == nullptr){

        error("while: expression was expected");
    }

    require(")", "while");
    check_end_of_tokens();

    new_call = get_call();

    if (new_condition == nullptr){

        error("while: call was expected");
    }

    ret_val->add_condition(new_condition);
    ret_val->add_loop_body(new_call);

    return ret_val;
}   

Call* Lexer::get_call(){

    if (cur_pos == tokens.end()){ return nullptr; }
    cur_pos++;
    if ((cur_pos != tokens.end()) && (strcmp(cur_pos->lexem, "-") != 0)){ return nullptr; }
    cur_pos--;

    Call* ret_val = new Call;
    char* statment_name = nullptr;
    
    statment_name = get_statement_name();

    if (statment_name == nullptr){

        error("statment_call: name was expected");
    }

    require("-", "call");
    require(">", "call");
    require("[", "call");
    check_end_of_tokens();

    while (strcmp(cur_pos->lexem, "]") != 0){

        ret_val->add_new_var(get_var());

        check_end_of_tokens();
        
        if (strcmp(cur_pos->lexem, "]") != 0){

            require(",", "call_list");
        }

        check_end_of_tokens();
    }
    
    ret_val->add_name(statment_name);
    cur_pos++;

    return ret_val;
}

Var* Lexer::get_var(){

    if (cur_pos == tokens.end()){ return nullptr; }

    char* var_name = nullptr;
    bool is_inner = false;

    if (cur_pos->lexem == "inner"){

        is_inner = true;
        cur_pos++;
        check_end_of_tokens();
    } 

    var_name = get_var_name();

    return new Var(var_name, is_inner);
}

/**
 * @brief Возвращает nullptr если трабылы.
 * 
 * @return Ioperator* 
 */
Ioperator* Lexer::get_expr(){

    if (cur_pos == tokens.end()){ return nullptr; }

    Ioperator* ret_val = nullptr;
    Ioperator* right = nullptr;

    ret_val = get_and();

    if (ret_val == nullptr){ return nullptr; }
    if (cur_pos == tokens.end()){ return ret_val; }

    while ((cur_pos != tokens.end()) && (strcmp(cur_pos->lexem, "|") == 0)){

        cur_pos++;
        require("|", "or_expr");
        check_end_of_tokens();

        right = get_and();

        if (right == nullptr){

            error("or operand was expected");
        }

        ret_val = new Logical_or(ret_val, right);
    }

    return ret_val;
}

Ioperator* Lexer::get_and(){

    if (cur_pos == tokens.end()){ return nullptr; }

    Ioperator* ret_val = nullptr;
    Ioperator* right = nullptr;

    ret_val = get_equality();

    if (ret_val == nullptr){ return nullptr; }
    if (cur_pos == tokens.end()){ return ret_val; }

    while ((cur_pos != tokens.end()) && (strcmp(cur_pos->lexem, "&") == 0)){

        cur_pos++;
        require("&", "and_expr");
        check_end_of_tokens();

        right = get_equality();

        if (right == nullptr){

            error("and operand was expected");
        }

        ret_val = new Logical_and(ret_val, right);
    }

    return ret_val;
}

Ioperator* Lexer::get_equality(){

    if (cur_pos == tokens.end()){ return nullptr; }

    Ioperator* ret_val = nullptr;
    Ioperator* right = nullptr;
    bool equal = false;
    int tmp = 0;

    ret_val = get_relation();

    if (ret_val == nullptr){ return nullptr; }
    if (cur_pos == tokens.end()){ return ret_val; }

    while ((cur_pos != tokens.end()) 
        && ((tmp = strcmp(cur_pos->lexem, "!")) == 0 || strcmp(cur_pos->lexem, "=") == 0)){
        
        cur_pos++;
        
        require("=", "equality");

        if (tmp != 0){

            equal = true;
        }

        right = get_relation();

        if (right == nullptr){

            error("relation operand was expected");
        }

        ret_val = new Equality(ret_val, equal, right);

        equal = false;
    }

    return ret_val;
}

Ioperator* Lexer::get_relation(){

    if (cur_pos == tokens.end()){ return nullptr; }

    Ioperator* ret_val = nullptr;
    Ioperator* right = nullptr;
    bool greater = false;
    bool equal = false;
    int tmp = 0;

    ret_val = get_plus_minus();

    if (ret_val == nullptr){ return nullptr; }
    if (cur_pos == tokens.end()){ return ret_val; }

    while ((cur_pos != tokens.end()) 
        && ((tmp = strcmp(cur_pos->lexem, "<")) == 0 || strcmp(cur_pos->lexem, ">") == 0)){
        
        cur_pos++;
        check_end_of_tokens();

        if (strcmp(cur_pos->lexem, "=") == 0){
            
            equal = true;
            cur_pos++;
            check_end_of_tokens();
        }

        if (tmp != 0){

            greater = true;
        }

        right = get_plus_minus();

        if (right == nullptr){

            error("relation operand was expected");
        }

        if (greater){

            ret_val = new Greater(ret_val, equal, right);   
        } else{

            ret_val = new Less(ret_val, equal, right);
        }

        greater = false;
        equal = false;
    }

    return ret_val;
}

Ioperator* Lexer::get_plus_minus(){
    
    if (cur_pos == tokens.end()){ return nullptr; }

    Ioperator* ret_val = nullptr;
    Ioperator* right = nullptr;
    int tmp = 0;

    ret_val = get_mul_div();

    if (ret_val == nullptr){ return nullptr; }
    if (cur_pos == tokens.end()){ return ret_val; }

    while ((cur_pos != tokens.end()) 
        && ((tmp = strcmp(cur_pos->lexem, "+")) == 0 || strcmp(cur_pos->lexem, "-") == 0)){
        
        cur_pos++;
        check_end_of_tokens();

        right = get_mul_div();

        if (right == nullptr){

            error("plus or minus was expected");
        }

        if (tmp == 0){

            ret_val = new Bin_plus(ret_val, right);   
        } else{

            ret_val = new Bin_minus(ret_val, right);
        }
    }

    return ret_val;
}

Ioperator* Lexer::get_mul_div(){
    
    if (cur_pos == tokens.end()){ return nullptr; }

    Ioperator* ret_val = nullptr;
    Ioperator* right = nullptr;
    int tmp = 0;

    ret_val = get_number_sign();

    if (ret_val == nullptr){ return nullptr; }
    if (cur_pos == tokens.end()){ return ret_val; }

    while ((cur_pos != tokens.end()) 
        && ((tmp = strcmp(cur_pos->lexem, "*")) == 0 || strcmp(cur_pos->lexem, "/") == 0)){
        
        cur_pos++;
        check_end_of_tokens();

        right = get_number_sign();

        if (right == nullptr){

            error("mul or div minus was expected");
        }

        if (tmp == 0){

            ret_val = new Mul(ret_val, right);   
        } else{

            ret_val = new Div(ret_val, right);
        }
    }

    return ret_val;
}

/**
 * @brief Знак может быть и у скобок. Этот класс оборачивает все операнды.(в том числе и скобки)
 * 
 * @return Ioperator* 
 */
Ioperator* Lexer::get_number_sign(){

    if (cur_pos == tokens.end()){ return nullptr; }

    Ioperator* ret_val = nullptr;
    Inode* right = nullptr;
    int tmp = 0;

    if (strcmp(cur_pos->lexem, "(") == 0){

        cur_pos++;
        check_end_of_tokens();

        ret_val = get_expr();

        require(")", "nested_expressions");

        return ret_val;
    }

    if ((tmp = strcmp(cur_pos->lexem, "+")) == 0 || strcmp(cur_pos->lexem, "-") == 0){

        right = get_operand_or_custom_op();

        if (right == nullptr){

            error("opearnd was expected");
        }

        if (tmp != 0){ 

            return new Number_sign(right, false);
        }
    }

    return new Number_sign(right, true);
}

Inode* Lexer::get_operand_or_custom_op(){

    if (cur_pos == tokens.end()){ return nullptr; }

    Inode* ret_val = nullptr;

    if ((strtod(cur_pos->lexem, nullptr) != 0) || (strcmp(cur_pos->lexem, "0") == 0)){ 

        return new Num(strtod(cur_pos->lexem, nullptr));
    }

    return get_var();
}

