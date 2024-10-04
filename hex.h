#pragma once

class Hex {//jedna klasa to jedna plansza

protected:
    char** board;
    int** board_search;
    int size = 0, red = 0, blue = 0;
    //pomocnicze zmienne:
    int spaces = 0, lines = 0, line = 0;
    int end = 0;
    bool start = false;
    int field = -1;
    int count = 0;
    bool won = false;
    int win = -1;//-1 - nitk nie wygral, 0 - blue wygral, 1 - red wygral
    bool fakeadd = false;
    int fakeaddcount = 0;
    int takenX = -1, takenY = -1;


public:
    Hex();
    void Parser(char sign);
    //inne metody na kazde wymaganie

    //int GetF() const;
    //void SetF(int f);
    //int GetE() const;
    //void SetE(int e);

    bool dfs(int x, int y, char pawn);
    bool check_valid(int x, int y, char pawn);
    bool Win(int x, int y, char pawn);


    bool Game_over();
    bool initial_check_R();
    bool initial_check_B();
    void printgameover();
    void refreshGame_over();

    bool is_possible();
    void addpawn(char pawn, int y, int x);
    void removepawn(char pawn, int y, int x);
    bool is_possible_ext(char pawn);



    void naive(char pawn, int moves);
    bool can_win2(char pawn, int moves);
    void manage_enemy(char pawn, int add);

    //player-naive
    //enemy-player 2 ruchy wygrywajace

    void perfect(char pawn, int moves);
    bool enemy_player(char pawn);
    bool player_enemy_player(char pawn);
    bool enemy_player_enemy_player(char pawn);

    int GetSize() const;

    int GetR() const;
    void SetR(int red);


    int GetB() const;
    void SetB(int blue);

    bool is_done();

    bool check_correct();

    //bool is_possible();

    ~Hex();
};