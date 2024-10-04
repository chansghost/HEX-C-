#include "hex.h"
#include "help_functions.h"
#include <iostream>

using namespace std;



Hex::Hex() {
    size = 0, red = 0, blue = 0;
    //pomocnicze zmienne:
    spaces = 0, lines = 0, line = 0;
    end = 0;
    start = false;
    field = -1;
    count = 0;
    board = nullptr;
    board_search = nullptr;
}


void Hex::Parser(char sign) {
    if (this->size == 0) {//rozwa¿anie 1 linijki, gdzie jeszcze nie ma zdeterminowanego size
        if (if_whitespace(sign)) {
            spaces++;
        }
        else {
            size = board_size(spaces);
            lines = (size * 2) + 1;
            board = new char* [lines - 2];
            create_board(board, size, lines);
        }
    }
    else {


        if (!start) {//jesli nie ma nas w polowie
            if (sign == '<')
            {
                start = true; //zapobieganie dodawania niepotrzebnych pól (pustych przed plansza) +przyœpieszenie(??)
                field += 1;
            }
            else if (line + 2 == lines) {//czy koniec planszy w next linijce
                if (sign == '-') {
                    count += 1;//liczenie "-"
                    if (count == 3) {
                        line += 1;
                    }
                }
            }
        }
        else {
            if (sign == '<') {
                end = 0;
                field += 1;
                count = 0;
            }

            //dodanie pionków
            else if (sign == 114 || sign == 98) {//kody ascii r i b
                board[line][field] = sign;
                if (sign == 114) red += 1;
                else if (sign == 98) blue += 1;
                //end += 1;
                count = 0;
            }
            //dodanie pustego pola
            else if (if_whitespace(sign)) {
                count += 1;// liczenie miejsc w polu (s¹ 3, a na 2 miejscu zwykle jest pionek)
                if (count == 2) {
                    board[line][field] = sign;
                }
            }



            //sprawdzanie czy linijka siê koñczy (za wyj¹tkiem œrodkow)
            else if (sign == '-') {
                end += 1;
            }

            if (field + 1 == size) {//sprawdzanie czy linijka sie konczy w srodku
                if (sign == '>') {
                    start = false;
                    line += 1;
                    count = 0;
                    field = -1;
                }
            }
            else if (end == 2) {//koniec linijki
                start = false;
                line += 1;
                //if (line == lines - 2) {
                end = 0;
                //}
                field = -1;
                count = 0;
            }
        }

    }


}

bool Hex::initial_check_B() {
    int leftborder = 0, rightborder = 0;
    for (int i = 0; i < size; i++) {
        if (board[i][i] == 'b') {
            rightborder += 1;
        }
        if (board[i + size - 1][0] == 'b') {
            leftborder += 1;
        }
    }
    if (rightborder > 0 && leftborder > 0) return true; //czy na obu krawedziach sa w ogole jakies pionki
    return false;
}

bool Hex::initial_check_R() {
    int leftborder = 0, rightborder = 0;
    for (int i = 0; i < size; i++) {
        if (board[i][0] == 'r') {
            rightborder += 1;
        }
        if (board[(size * 2) - 2 - i][i] == 'r') {//od dolu idzie
            leftborder += 1;
        }
    }
    if (rightborder > 0 && leftborder > 0) return true;
    return false;
}

bool Hex::check_valid(int x, int y, char pawn) {
    if (y < lines - 2 && y>-1 && x < size && x>-1) {//???
        if (board_search[y][x] == 0) {
            board_search[y][x] = 1;
            if (board[y][x] == pawn) {
                return true;
            }
        }
        //czy jest w granicach planszy, czy nie zosta³o odwiedzone, czy pole to nasz pionek
    }
    return false;
}

bool Hex::Win(int x, int y, char pawn) {
    if (pawn == 'r') {
        for (int i = 0; i < size; i++) {//prawa krawêdŸ tylko (dfs jest puszczany od lewej)

            if ((size * 2) - 2 - i == y && i == x) {//czy nasze coords pasuj¹ do drugiej krawedzi
                //cout << "YES RED" << endl;
                win = 1;
                won = true;
                return true;
            }
        }
        return false;
    }
    else {
        for (int i = 0; i < size; i++) {
            if (y == i && x == i) {//od gory sprawdzamy krawedz prawa
                //cout << "YES BLUE" << endl;
                win = 0;
                won = true;
                return true;
            }
        }
        return false;
    }
}

bool Hex::dfs(int x, int y, char pawn) {


    if (won == false) {// i jesli zosta³o jeszcze nie odwiedzone to pusc dfs
        if (check_valid(x, y, pawn)) {
            if (Win(x, y, pawn)) return true;

            //search po lini
            /*dfs(x - 1, y, pawn);
            dfs(x + 1, y, pawn);*/
            //jesli znajdujemy sie w dole planszy to to wszystko tak dziala:
            if (y < size) {
                dfs(x - 1, y - 2, pawn);
                dfs(x - 1, y - 1, pawn);
                if (y < size - 2) {
                    dfs(x, y - 1, pawn);

                    dfs(x + 1, y + 2, pawn);
                    dfs(x, y + 1, pawn);
                    dfs(x + 1, y + 1, pawn);
                }
                else if (y == size - 2) {
                    dfs(x, y - 1, pawn);

                    dfs(x, y + 2, pawn);
                    dfs(x, y + 1, pawn);
                    dfs(x + 1, y + 1, pawn);
                }
                else if (y == size - 1) {
                    dfs(x, y - 1, pawn);

                    dfs(x - 1, y + 2, pawn);
                    dfs(x, y + 1, pawn);
                    dfs(x - 1, y + 1, pawn);
                }
            }
            else if (y == size) {
                dfs(x, y - 2, pawn);
                dfs(x + 1, y - 1, pawn);
                dfs(x, y - 1, pawn);

                dfs(x - 1, y + 2, pawn);
                dfs(x, y + 1, pawn);
                dfs(x - 1, y + 1, pawn);
            }
            else {
                dfs(x + 1, y - 2, pawn);
                dfs(x + 1, y - 1, pawn);
                dfs(x, y - 1, pawn);

                dfs(x - 1, y + 2, pawn);
                dfs(x, y + 1, pawn);
                dfs(x - 1, y + 1, pawn);
            }


        }
        return false;
    }
    else {
        return true;
    }


}

void Hex::refreshGame_over() {
    del_boardt(board_search, lines);
    won = false;
    win = -1;
    board_search = new int* [lines - 2];
    copy_board(board_search, size, lines);
}

bool Hex::Game_over() {//przerobic na bool i ogarnac couts
    refreshGame_over();
    if (this->check_correct()) {
        if (red == 1 && size == 1) {
            win = 1;
            return true;
        }
        if (red == 0 && blue == 0) {
            return false;;
        }
        else {
            if (this->initial_check_R()) {
                for (int i = 0; i < size; i++) {
                    dfs(0, i, 'r');
                    if (won) {//w 1 kolumnie lewa krawedz
                        return true;
                    }
                }

            }
            if (win == -1) {
                refreshGame_over();
                if (this->initial_check_B()) {
                    for (int i = 0; i < size; i++) {
                        dfs(0, (size * 2) - 2 - i, 'b');
                        if (won) {
                            return true;
                        }
                    }
                }
            }
        }

    }
    return false;

}



void Hex::addpawn(char pawn, int y, int x) {
    if (pawn == 'r') {
        board[y][x] = 'r';
        red += 1;
    }
    else {
        blue += 1;
        board[y][x] = 'b';
    }
}

void Hex::removepawn(char pawn, int y, int x) {
    board[y][x] = ' ';
    if (pawn == 'r') {
        red--;
    }
    else {
        blue--;
    }
}

bool Hex::is_possible_ext(char pawn) {
    for (int i = 0; i < lines - 2; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == pawn) {

                removepawn(pawn, i, j);
                if (Game_over()) {
                    addpawn(pawn, i, j);
                    if (pawn == 'r' && win == 0) return false;
                }
                else {
                    addpawn(pawn, i, j);
                    return true;
                }
            }
        }
    }
    return false;
}

bool Hex::is_possible() {
    if (Game_over()) {

        if (win == 1) {
            if (red == blue) return false;
            return is_possible_ext('r');
        }
        else if (win == 0) {
            if (red > blue) return false;
            //printboard();
            return is_possible_ext('b');
        }
    }
    else if (check_correct()) {
        return true;
    }
    return false;

}

bool Hex::enemy_player(char pawn) {
    char enemy;
    if (pawn == 'r') enemy = 'b';
    else enemy = 'r';

    /*if (can_win2(enemy, 1)) {
            return false;
    }*/

    for (int i = 0; i < lines - 2; i++) {
        for (int j = 0; j < size; j++) {
            if (!(takenX == j && takenY == i)) {//czy juz szybciej nie sprawdzilismy tego pola
                if (board[i][j] == ' ') {
                    addpawn(pawn, i, j);
                    manage_enemy(pawn, 1);
                    if (Game_over()) {
                        manage_enemy(pawn, 0);
                        removepawn(pawn, i, j);
                        if (takenX != -1) {
                            return true;
                        }
                        else {
                            takenX = j;
                            takenY = i;
                            if (enemy_player(pawn)) return true;
                        }
                    }
                    manage_enemy(pawn, 0);
                    removepawn(pawn, i, j);
                }
            }
        }
    }
    return false;
}

bool Hex::player_enemy_player(char pawn) {
    for (int i = 0; i < lines - 2; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == ' ') {
                addpawn(pawn, i, j);
                manage_enemy(pawn, 1);
                if (enemy_player(pawn)) {
                    removepawn(pawn, i, j);
                    manage_enemy(pawn, 0);
                    return true;
                }
                removepawn(pawn, i, j);
                manage_enemy(pawn, 0);
            }

        }
    }
    return false;
}

bool Hex::enemy_player_enemy_player(char pawn) {
    char enemy;
    if (pawn == 'r') enemy = 'b';
    else enemy = 'r';

    for (int i = 0; i < lines - 2; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == ' ') {
                addpawn(enemy, i, j);
                manage_enemy(enemy, 1);
                if (player_enemy_player(pawn)) {
                    removepawn(enemy, i, j);
                    manage_enemy(enemy, 0);
                    return true;
                }
                removepawn(enemy, i, j);
                manage_enemy(enemy, 0);
            }

        }
    }
    return false;
}


void Hex::perfect(char pawn, int moves) {
    if (is_possible() && !Game_over()) {
        int empty_field = (size * size) - red - blue;
        if (empty_field != 0 && (red + blue) > ((size * size) / 3)) {
            if (moves == 1) {
                if (pawn == 'r') {
                    if (red == blue) {//nasza kolejka
                        if ((moves == 2 && empty_field >= 3) || moves == 1) {
                            if (can_win2('r', moves)) {//nasza kolej, czyli ruch taki sam jak z naiwnym
                                std::cout << "YES" << endl;
                                //saveTextToFile("plik.txt", "YES");
                                return;
                            }
                            else {
                                std::cout << "NO" << endl;
                                return;
                            }
                        }
                    }
                    else {
                        if ((moves == 2 && empty_field >= 4) || (moves == 1 && empty_field >= 2)) {

                            if (enemy_player(pawn)) {
                                std::cout << "YES" << endl;
                                //saveTextToFile("plik.txt", "YES");
                                return;
                            }
                            std::cout << "NO" << endl;
                            return;
                        }
                    }
                }

                else {
                    if (red > blue) {
                        if ((moves == 2 && empty_field >= 3) || moves == 1) {//nasza kolejka
                            if (can_win2('b', moves)) {
                                std::cout << "YES" << endl;
                                //saveTextToFile("plik.txt", "YES");
                                return;
                            }
                            else {
                                std::cout << "NO" << endl;
                                return;
                            }
                        }
                    }
                    else {
                        if ((moves == 2 && empty_field >= 4) || (moves == 1 && empty_field >= 2)) {

                            if (enemy_player(pawn)) {
                                std::cout << "YES" << endl;
                                //saveTextToFile("plik.txt", "YES");
                                return;
                            }
                            else {
                                std::cout << "NO" << endl;
                                return;
                            }
                        }
                    }
                }
            }
            else {//dwa ruchy
                if (pawn == 'r') {
                    if (red == blue) {//nasza kolejka
                        if ((moves == 2 && empty_field >= 3) || moves == 1) {
                            if (player_enemy_player(pawn)) {
                                std::cout << "YES" << endl;
                                return;
                            }
                            std::cout << "NO" << endl;
                            return;

                        }
                    }
                    else {
                        if ((moves == 2 && empty_field >= 4) || (moves == 1 && empty_field >= 2)) {
                            if (enemy_player_enemy_player(pawn)) {
                                std::cout << "YES" << endl;
                                return;
                            }
                            std::cout << "NO" << endl;
                            return;
                        }
                    }
                }

                else {
                    if (red > blue) {
                        if ((moves == 2 && empty_field >= 3) || moves == 1) {//nasza kolejka
                            if (player_enemy_player(pawn)) {
                                std::cout << "YES" << endl;
                                return;
                            }
                            std::cout << "NO" << endl;
                            return;

                        }

                    }
                    else {
                        if ((moves == 2 && empty_field >= 4) || (moves == 1 && empty_field >= 2)) {
                            if (enemy_player_enemy_player(pawn)) {
                                cout << "YES" << endl;
                                return;
                            }
                            std::cout << "NO" << endl;
                            return;
                        }
                    }
                }
            }
        }

    }
    std::cout << "NO" << endl;
}

void Hex::naive(char pawn, int moves) {
    if (is_possible() && !Game_over()) {
        int empty_field = (size * size) - red - blue;
        if (empty_field != 0) {
            if (pawn == 'r') {
                if (red == blue) {
                    if ((moves == 2 && empty_field >= 3) || moves == 1) {
                        if (can_win2('r', moves)) {
                            cout << "YES" << endl;
                            //saveTextToFile("plik.txt", "YES");
                            return;
                        }
                    }
                }
                else {
                    if ((moves == 2 && empty_field >= 4) || (moves == 1 && empty_field >= 2)) {
                        if (can_win2('r', moves)) {
                            cout << "YES" << endl;
                            //saveTextToFile("plik.txt", "YES");
                            return;
                        }
                    }
                }
            }
            else {
                if (red > blue) {
                    if ((moves == 2 && empty_field >= 3) || moves == 1) {
                        if (can_win2('b', moves)) {
                            cout << "YES" << endl;
                            //saveTextToFile("plik.txt", "YES");
                            return;
                        }
                    }
                }
                else {
                    if ((moves == 2 && empty_field >= 4) || (moves == 1 && empty_field >= 2)) {
                        if (can_win2('b', moves)) {
                            cout << "YES" << endl;
                            //saveTextToFile("plik.txt", "YES");
                            return;
                        }
                    }
                }
            }
        }

    }
    cout << "NO" << endl;
    //saveTextToFile("plik.txt", "NO");
}

void Hex::manage_enemy(char pawn, int add) {//int add indykuje czy dodajemy, czy usuwamy enemy count (1- add, 0- remove)
    if (add == 1 && !check_correct()) {
        if (pawn == 'r') blue++;
        else red++;
        fakeaddcount += 1;
    }
    else if (add == 0 && fakeaddcount > 0) {
        if (pawn == 'r') blue--;
        else red--;
        fakeaddcount--;
    }
}

bool Hex::can_win2(char pawn, int moves) {
    int move = 0;
    for (int i = 0; i < lines - 2; i++) {
        for (int j = 0; j < size; j++) {
            move = 0;
            if (board[i][j] == ' ') {
                addpawn(pawn, i, j);
                move += 1;
                manage_enemy(pawn, 1);
                if (Game_over()) {
                    //remove enemy count

                    if (moves == move) {
                        manage_enemy(pawn, 0);
                        removepawn(pawn, i, j);
                        return true;
                    }

                }
                else if (move != moves) {//zajdzie tylko jak mamy 2 ruchy i po nim ZAWSZE bedzie jeszcze tylko 1 can win
                    //if can_not win, enemy
                            //if can_win moves 1 (rekurencja)
                    if (can_win2(pawn, 1)) {
                        removepawn(pawn, i, j);
                        manage_enemy(pawn, 0);
                        return true;
                    }
                }
                manage_enemy(pawn, 0);
                removepawn(pawn, i, j);

            }
        }
    }
    return false;
}


void Hex::printgameover() {
    //redirectCoutToFile("output.txt");
    if (win == -1) {
        cout << "NO" << endl;
        //saveTextToFile("plik.txt", "NO");
    }
    else if (win == 1) {
        cout << "YES RED" << endl;
        //saveTextToFile("plik.txt", "YES RED");

    }
    else {
        cout << "YES BLUE" << endl;
        //saveTextToFile("plik.txt", "YES BLUE");
    }
}

bool Hex::is_done() {
    if (line + 1 == lines) {//jak aktualny counter linijek dotrze do calkowitej liczby lini planszy
        return true;
    }
    return false;
}

int Hex::GetSize() const {
    return size;
}

int Hex::GetR() const {
    return red;
}

void Hex::SetR(int red) {
    this->red = red;;
}

int Hex::GetB() const {
    return blue;
}

void Hex::SetB(int blue) {
    this->blue = blue;
}

bool Hex::check_correct() {
    if (red == blue || (blue + 1) == red) {
        return true;
    }
    return false;
}

Hex::~Hex() {
    for (int i = 0; i < lines - 2; i++) {
        if (board != nullptr && board[i] != nullptr) {
            delete[] board[i];
        }
        if (board_search != nullptr && board_search[i] != nullptr) {
            delete[] board_search[i];
        }
    }
    if (board != nullptr) {
        delete[] board;
    }
    if (board_search != nullptr) {
        delete[] board_search;
    }

}