#pragma once

class Call;
class Return_statement;
class Statements_return;
class If;
class Cycle;
class Assign;
class Statement;
class Var;
class Num;
class String;
class Logical_or;
class Logical_and;
class Equality;
class Statement;
class Greater;
class Less;
class Bin_plus;
class Bin_minus;
class Mul;
class Div;
class Number_sign;

class Irequest{

public:

    Irequest() = default;
    Irequest(const Irequest&) = default;
    virtual ~Irequest() = 0;

    virtual void process_request(Call* cur_node){}
    virtual void process_request(Return_statement* cur_node){}
    virtual void process_request(Statements_return* cur_node){}
    virtual void process_request(If* cur_node){}
    virtual void process_request(Cycle* cur_node){}
    virtual void process_request(Assign* cur_node){}

    //тут операторы
    virtual void process_request(Statement* cur_node){}

    virtual void process_request(Var* cur_node){}
    virtual void process_request(Num* cur_node){}
    virtual void process_request(String* cur_node){}

    virtual void process_request(Logical_or* cur_node){}
    virtual void process_request(Logical_and* cur_node){}
    virtual void process_request(Equality* cur_node){}
    virtual void process_request(Greater* cur_node){}
    virtual void process_request(Less* cur_node){}
    virtual void process_request(Bin_plus* cur_node){}
    virtual void process_request(Bin_minus* cur_node){}
    virtual void process_request(Mul* cur_node){}
    virtual void process_request(Div* cur_node){}
    virtual void process_request(Number_sign* cur_node){}
};

