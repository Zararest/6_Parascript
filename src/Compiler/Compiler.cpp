#include "headers/Compiler.h"

Compiler::Compiler(Icontrol_struct* new_syntax_tree){

    synt_tree_root = new_syntax_tree;
}

Compiler::~Compiler(){

    delete synt_tree_root;
}

void Compiler::generate_asm_code(const char* out_file_name){

    FILE* asm_file = fopen(out_file_name, "w");

    if (asm_file == nullptr){

        fprintf(stderr, "Couldn't open file\n");
        return;
    }

    Asm_code_req new_req(asm_file);

    synt_tree_root->get_request(&new_req);

    fclose(asm_file);
}
