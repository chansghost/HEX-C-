#include <iostream>
#include "defines.h"
#include "help_functions.h"
#include "hex.h"

using namespace std;




int main() {
    char sign;
    Hex* hex = new Hex();
    int test = 0;
    string tescior = "";
    string command = "";
    bool command_done = false;
    int hexs = 1;
    int command_count = 0;
    while (cin.get(sign)) {
        if (sign != '\n') {
            if (!not_command(sign)) {
                if (hex->is_done()) {
                    command_count = 0;
                    delete hex;
                    hex = new Hex();
                    hexs += 1;
                }
                hex->Parser(sign);


            }
            else {
                command_done = true;
                command += sign;

                if (command == boardsize) {
                    cout << hex->GetSize() << endl;
                }
                else if (command == pawns) {
                    cout << (hex->GetR()) + (hex->GetB()) << endl;
                }
                else if (command == correct) {
                    if (hex->check_correct()) {
                        cout << "YES" << endl;
                    }
                    else cout << "NO" << endl;
                }

                else if (command == over) {
                    hex->Game_over();
                    hex->printgameover();
                }
                else if (command == possible) {
                    if (hex->is_possible()) {
                        cout << "YES" << endl;
                        //saveTextToFile("plik.txt", "YES");

                    }
                    else {
                        cout << "NO" << endl;

                    }
                }
                else if (command == red1naive || command == red2naive || command == blue1naive || command == blue2naive) {


                    if (command_count == 0) {

                        //saveTextToFile("plik.txt", tescior);
                        hex->naive('r', 1);
                        hex->naive('b', 1);
                        hex->naive('r', 2);
                        hex->naive('b', 2);
                        cout << endl;
                        command_count = 1;



                    }
                }
                else if (command == red1perfect || command == red2perfect || command == blue1perfect || command == blue2perfect) {
                    if (command_count == 0) {
                        hex->perfect('r', 1);
                        hex->perfect('b', 1);
                        hex->perfect('r', 2);
                        hex->perfect('b', 2);

                        cout << endl;
                        command_count = 1;
                        //saveTextToFile("plik.txt", "\n");




                    }
                }
                else {
                    command_done = false;
                }
                if (command_done) command = "";

            }
        }
    }

    delete hex;
    return 0;
}