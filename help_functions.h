#pragma once
#include <fstream>



bool if_whitespace(char sign);

void create_board(char** board, int size, int lines);

//void saveTextToFile(const std::string& filename, const std::string& text);

int board_size(int spaces_count);

bool not_command(char sign);

void copy_board(int** board, int size, int lines);

void del_boardt(int** board, int lines);