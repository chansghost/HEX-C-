#include "help_functions.h"
#include <fstream>



bool if_whitespace(char sign) {
    if (sign < 33) {
        return true;
    }
    return false;
}

void create_board(char** board, int size, int lines) {
    for (int i = 0; i < lines - 2; i++) {
        board[i] = new char[size];
        for (int j = 0; j < size; j++) {
            board[i][j] = '.';
        }
    }


}

void copy_board(int** board, int size, int lines) {

    //jak hitnie size to zmniejszaæ
    int fieldcount = 0;
    for (int i = 0; i < lines - 2; i++) {

        if (i > (size - 1)) {
            fieldcount--;
        }
        else {
            fieldcount++;
        }
        board[i] = new int[size];
        for (int j = 0; j < size; j++) {
            board[i][j] = -1;
        }

        for (int j = 0; j < fieldcount; j++) {
            board[i][j] = 0;
        }

    }
}


void del_boardt(int** board, int lines) {
    if (board != nullptr) {
        for (int i = 0; i < lines - 2; i++) {
            if (board[i] != nullptr) {
                delete[] board[i];
            }
        }
        delete[] board;
    }
}
int board_size(int spaces_count) {
    int size;
    size = (spaces_count - 1) / 3;
    size += 1;
    return size;
}

bool not_command(char sign) {
    if (sign > 64 && sign < 91 || sign == 95 || sign == 49 || sign == 50) {
        return true;
    }
    return false;
}