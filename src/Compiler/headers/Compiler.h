#pragma once

#include "Irequest.h"
#include "../../Lexer/headers/Inode.h"
#include "../../Lexer/headers/Icontent_of_statement.h"
#include "../../Lexer/headers/Icontrol_struct.h"
#include "../../Lexer/headers/Ioperand.h"
#include "../../Lexer/headers/Ioperator.h"
#include "../../Parcer/headers/Parcer.h"


class Asm_code_req: public Irequest{


};

class Compiler{


public:

    Compiler(Icontrol_struct* new_syntax_tree);

};