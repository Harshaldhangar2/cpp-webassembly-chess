#include <iostream>
#include "Board.h"

using namespace std;

int main() {
    Board chessboard;

    cout << "===== C++ CHESS GAME =====\n";
    cout << "Enter moves using row and column numbers.\n";
    cout << "Example: e2 to e4 = 6 4 4 4\n";
    cout << "Enter -1 to quit.\n";

    while (true) {
        chessboard.display();

        if (chessboard.isGameOver())
            break;

        int sr, sc, er, ec;

        cout << "\nEnter move (startRow startCol endRow endCol): ";

        if (!(cin >> sr))
            break;

        if (sr == -1) {
            cout << "Game ended.\n";
            break;
        }

        if (!(cin >> sc >> er >> ec)) {
            cout << "Invalid input. Exiting.\n";
            break;
        }

        if (chessboard.movePiece(sr, sc, er, ec)) {
            if (chessboard.isGameOver()) {
                chessboard.display();
                break;
            }
        }
    }

    return 0;
}