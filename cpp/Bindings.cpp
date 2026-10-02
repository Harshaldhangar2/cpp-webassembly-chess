#include <emscripten/bind.h>
#include "Board.h"

using namespace emscripten;

static Board board;

val getBoard() {
    val result = val::array();

    for (int r = 0; r < 8; r++) {
        val row = val::array();

        for (int c = 0; c < 8; c++) {
            // The board array is private, so we'll expose it
            // through a getter in Board.h and Board.cpp.
            row.set(c, std::string(1, board.getPiece(r, c)));
        }

        result.set(r, row);
    }

    return result;
}

bool makeMove(int sr, int sc, int er, int ec) {
    return board.movePiece(sr, sc, er, ec);
}

bool gameOver() {
    return board.isGameOver();
}

void resetGame() {
    board = Board();
}

EMSCRIPTEN_BINDINGS(chess_module) {
    function("getBoard", &getBoard);
    function("makeMove", &makeMove);
    function("gameOver", &gameOver);
    function("resetGame", &resetGame);
}