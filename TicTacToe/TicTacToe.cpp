#include <iostream>
#include <chrono>
#include <random>
#include <stdlib.h>
using namespace std;
using namespace chrono;

int getRandom(int min, int max) {
    const static auto seed = system_clock::now().time_since_epoch().count();
    static mt19937_64 generator(seed);
    uniform_int_distribution<int32_t> dis(min, max);
    return dis(generator);
}

enum symbols : char {
    s_cross = 'X',
    s_zero = 'O',
    s_empty = '_'
};

enum game_progress {
    in_progress,
    player1_won,
    player2_won,
    draw    
};

struct game_coord{
    short x { 0 };
    short y { 0 };
};

struct game{
    const short size_field = 3;
    symbols** field {nullptr};
    game_progress status{ in_progress };
    symbols player1;
    symbols player2;
    short turn{ 0 };
};

void __fastcall clear_field() {
    system("cls");
}

void __fastcall init_game(game& g) {
    g.field = new symbols* [g.size_field];
    for (short i = 0; i < g.size_field; i++) 
        g.field[i] = new symbols [g.size_field];
    
    for (short x = 0; x < g.size_field; x++) 
        for (short y = 0; y < g.size_field; y++) 
            g.field[x][y] = s_empty;
        
    if (getRandom(0, 10) > 5) {
        g.player1 = s_cross;
        g.player2 = s_zero;
        g.turn = 0;
    }
    else { 
        g.player1 = s_zero;
        g.player2 = s_cross;
        g.turn = 1;
    };
}

void __fastcall print_field(const game& g) {
    cout << "    ";
    for (short x = 0; x < g.size_field; x++) 
        cout << x + 1 << " | ";
    
    cout << endl;
    
    for (short y = 0; y < g.size_field; y++) {
        cout << y + 1 << " | ";
        for (short x = 0; x < g.size_field; x++)
            cout << g.field[x][y] << " | ";
        
        cout << endl;
   }
    cout << g.player1 << " - player 1\n" << g.player2 << " - player2\n";
}

void __fastcall deinit_game(game& g) {
    for (short i = 0; i < g.size_field; i++) 
        delete[] g.field[i];
    
    delete[] g.field;
    g.field = nullptr;
}

game_coord __fastcall get_coord_player1(game& g) {
    game_coord c{ 0 };
    do {
        cout << "Enter X: ";
        cin >> c.x;
        cout << "Enter Y: ";
        cin >> c.y;
        c.x--;
        c.y--;
    } while (c.x > 2 || c.y > 2 || g.field[c.x][c.y] != s_empty);
    return c;
}

game_coord __fastcall get_coord_player2(game& g) {
    game_coord c{ 0 };
    do {
        cout << "Enter X: ";
        cin >> c.x;
        cout << "Enter Y: ";
        cin >> c.y;
        c.x--;
        c.y--;
    } while (c.x > 2 || c.y > 2 || g.field[c.x][c.y] != s_empty);
    return c;
}

game_progress __fastcall win_lose_draw(game& g) {
    for (short y = 0; y < g.size_field; y++) {
        if (g.field[0][y] == g.field[1][y] && g.field[0][y] == g.field[2][y]) {
            if (g.field[0][y] == g.player1)
                return player1_won;
            if (g.field[0][y] == g.player2)
                return player2_won;
        }
    }
    for (short x = 0; x < g.size_field; x++){
        if (g.field[x][0] == g.field[x][1] && g.field[x][0] == g.field[x][2]) {
            if (g.field[x][0] == g.player1)
                return player1_won;
            if (g.field[x][0] == g.player2)
                return player2_won;
        }
    }
    if (g.field[0][0] == g.field[1][1] && g.field[0][0] == g.field[2][2]) {
        if (g.field[0][0] == g.player1)
            return player1_won;
        if (g.field[0][0] == g.player2)
            return player2_won;
    }
    if (g.field[0][2] == g.field[1][1] && g.field[0][2] == g.field[2][0]) {
        if (g.field[2][0] == g.player1) 
            return player1_won;
        if (g.field[2][0] == g.player2) 
            return player2_won;       
    }
    bool drw{ true };
    for (short x = 0; x < g.size_field; x++) {
        for (short y = 0; y < g.size_field; y++) {
            if (g.field[x][y] == s_empty) {
                drw = false;
                break;
            }
        }
        if (drw == false) 
            break;    
    }
    if (drw) 
        return draw;
    
    return in_progress;
}

void __fastcall results(const game& g) //Подводит результаты по игре
{
    if (g.status == player1_won)
        std::cout << "Player 1 won\n";

    if (g.status == player2_won)
        std::cout << "Player 2 won\n";
    
    if (g.status == draw)
        std::cout << "Draw\n";
}

int main(void){
    game g;
    init_game(g);
    print_field(g);
    do {
        if (g.turn % 2 == 0) {
            game_coord c = get_coord_player1(g);
            g.field[c.x][c.y] = g.player1;
        }
        else {
            game_coord c = get_coord_player2(g);
            g.field[c.x][c.y] = g.player2;
        }
        g.turn++;
        clear_field();
        print_field(g);
        g.status = win_lose_draw(g);
    } while (g.status == in_progress);

    deinit_game(g);
    results(g);
}