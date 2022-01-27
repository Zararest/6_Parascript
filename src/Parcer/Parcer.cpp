#include "headers/Parcer.h"

#include <cstring>

#define SIZE_OF_BUF 16

bool Parcer::find_in_separs(char cur_char){

    for (int i = 0; i < NUM_OF_SEPARS; i++){

        if (arr_of_separs[i] == cur_char){

            return true;
        }
    }

    return false;
}

bool Parcer::find_in_useless(char cur_char){

    for (int i = 0; i < NUM_OF_USELESS; i++){

        if (useless_separs[i] == cur_char){

            return true;
        }
    }

    return false;
}

int Parcer::parse_file(FILE* input_file, std::list<lexem>& list_of_lexem){

    if (input_file == nullptr){

        fprintf(stderr, "Parcer::parse_file can't read file\n");
        return 0;
    }

    char buff[SIZE_OF_BUF] = "";
    char sub_buff[SIZE_OF_BUF] = "";
    int buff_size = 0, lexem_size = 0, buff_begin = 0, num_of_lexems = 0, cur_line = 0;

    while ((buff_size = fread(buff + buff_begin, 1, SIZE_OF_BUF - buff_begin, input_file)) > 0){

        for (int i = 0; i < buff_size + buff_begin; i++){

            if (buff[i] == '\n'){ cur_line++; }

            if (find_in_separs(buff[i])){

                if (lexem_size > 0){

                    lexem new_lexem;
                    new_lexem.lexem = new char[lexem_size + 1];
                    new_lexem.size = lexem_size + 1;
                    new_lexem.type = info;
                    new_lexem.line_number = cur_line;

                    memcpy(new_lexem.lexem, buff + i - lexem_size, lexem_size);
                    new_lexem.lexem[lexem_size] = '\0';

                    list_of_lexem.push_back(new_lexem);
                    num_of_lexems++;
                }

                if (!find_in_useless(buff[i])){

                    lexem new_separ;
                    new_separ.lexem = new char[2];
                    new_separ.size = 2;
                    new_separ.type = separ;
                    new_separ.line_number = cur_line;

                    new_separ.lexem[0] = buff[i];
                    new_separ.lexem[1] = '\0';

                    list_of_lexem.push_back(new_separ);
                    num_of_lexems++;
                }

                lexem_size = 0;
            } else{

                lexem_size++;
            }
        }

        if (lexem_size == SIZE_OF_BUF){

            fprintf(stderr, "Parcer::parse_file lexem is too big\n");
            exit(0);
        }
        
        if (lexem_size > 0){

            memcpy(sub_buff, buff + buff_begin + buff_size - lexem_size, lexem_size);
            memcpy(buff, sub_buff, lexem_size);
        }

        buff_begin = lexem_size;
        lexem_size = 0;
    }

    if (lexem_size > 0){

        lexem new_lexem;
        new_lexem.lexem = new char[lexem_size + 1];
        new_lexem.size = lexem_size + 1;
        new_lexem.type = info;
        new_lexem.line_number = cur_line;

        memcpy(new_lexem.lexem, buff, lexem_size);
        new_lexem.lexem[lexem_size] = '\0';

        list_of_lexem.push_back(new_lexem);
        num_of_lexems++;
    }

    return num_of_lexems;
}


