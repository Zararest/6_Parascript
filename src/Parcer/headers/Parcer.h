#include <iostream>
#include <list>

#define NUM_OF_SEPARS 25
#define NUM_OF_USELESS 5

enum Lexem_type{

    separ = 1,
    info = 2
};

struct lexem{

    char* lexem = nullptr;
    int size = 0;
    int type = 0;
};

class Parcer{

    char arr_of_separs [NUM_OF_SEPARS] = {"+-=!()<>.,*/\n\t\0[]{};&|: "};
    char useless_separs [NUM_OF_USELESS] = {" \t\n\0"};

    bool find_in_separs(char cur_char);
    bool find_in_useless(char cur_char);
public: 

    int parse_file(FILE* input_file, std::list<lexem>& list_of_lexem);
};
