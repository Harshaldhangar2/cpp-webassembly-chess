#include <iostream>
#include <cmath>
#include <cctype>
#include "Board.h"

using namespace std;

Board::Board() {
    char initial[8][8] = {
        {'r','n','b','q','k','b','n','r'},
        {'p','p','p','p','p','p','p','p'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'P','P','P','P','P','P','P','P'},
        {'R','N','B','Q','K','B','N','R'}
    };

    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            squares[r][c] = initial[r][c];

    whiteTurn = true;

    whiteKingMoved = blackKingMoved = false;
    whiteRookAMoved = whiteRookHMoved = false;
    blackRookAMoved = blackRookHMoved = false;

    enPassantRow = -1;
    enPassantCol = -1;
}

void Board::display() {
    cout << "\n  a b c d e f g h\n";

    for (int r = 0; r < 8; r++) {
        cout << 8 - r << " ";

        for (int c = 0; c < 8; c++)
            cout << squares[r][c] << " ";

        cout << 8 - r << "\n";
    }

    cout << "  a b c d e f g h\n";
    cout << (whiteTurn ? "White's turn\n" : "Black's turn\n");
}

bool Board::isPathClear(int sr, int sc, int er, int ec) {
    int dr = (er > sr) - (er < sr);
    int dc = (ec > sc) - (ec < sc);

    int r = sr + dr;
    int c = sc + dc;

    while (r != er || c != ec) {
        if (squares[r][c] != '.')
            return false;

        r += dr;
        c += dc;
    }

    return true;
}

bool Board::isSquareAttacked(int r, int c, bool byWhite) {
    // Pawn attacks
    int pawnRow = r + (byWhite ? 1 : -1);
    char pawn = byWhite ? 'P' : 'p';

    if (pawnRow >= 0 && pawnRow < 8) {
        for (int dc : {-1, 1}) {
            int pc = c + dc;

            if (pc >= 0 && pc < 8 &&
                squares[pawnRow][pc] == pawn)
                return true;
        }
    }

    // Knight attacks
    const int knightMoves[8][2] = {
        {-2,-1}, {-2,1}, {-1,-2}, {-1,2},
        {1,-2}, {1,2}, {2,-1}, {2,1}
    };

    char knight = byWhite ? 'N' : 'n';

    for (const auto &m : knightMoves) {
        int nr = r + m[0];
        int nc = c + m[1];

        if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8 &&
            squares[nr][nc] == knight)
            return true;
    }

    // King attacks
    char king = byWhite ? 'K' : 'k';

    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;

            int nr = r + dr;
            int nc = c + dc;

            if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8 &&
                squares[nr][nc] == king)
                return true;
        }
    }

    // Rook, bishop and queen attacks
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;

            int nr = r + dr;
            int nc = c + dc;

            while (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                char piece = squares[nr][nc];

                if (piece != '.') {
                    bool sameColor = byWhite
                        ? (piece >= 'A' && piece <= 'Z')
                        : (piece >= 'a' && piece <= 'z');

                    if (sameColor) {
                        char p = static_cast<char>(tolower(piece));

                        if ((dr == 0 || dc == 0) &&
                            (p == 'r' || p == 'q'))
                            return true;

                        if (dr != 0 && dc != 0 &&
                            (p == 'b' || p == 'q'))
                            return true;
                    }

                    break;
                }

                nr += dr;
                nc += dc;
            }
        }
    }

    return false;
}

bool Board::isInCheck(bool white) {
    char king = white ? 'K' : 'k';

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (squares[r][c] == king)
                return isSquareAttacked(r, c, !white);
        }
    }

    return true;
}

bool Board::isPseudoLegalMove(int sr, int sc, int er, int ec) {
    if (sr < 0 || sr >= 8 || sc < 0 || sc >= 8 ||
        er < 0 || er >= 8 || ec < 0 || ec >= 8)
        return false;

    char piece = squares[sr][sc];
    char target = squares[er][ec];

    if (piece == '.' || (sr == er && sc == ec))
        return false;

    bool white = isupper(static_cast<unsigned char>(piece));

    if (white != whiteTurn)
        return false;

    if (target != '.') {
        bool targetWhite = isupper(static_cast<unsigned char>(target));

        if (white == targetWhite || target == 'K' || target == 'k')
            return false;
    }

    int dr = er - sr;
    int dc = ec - sc;
    int adr = abs(dr);
    int adc = abs(dc);

    char p = static_cast<char>(tolower(piece));

    switch (p) {
        case 'p': {
            int direction = white ? -1 : 1;
            int startRow = white ? 6 : 1;

            // Forward move
            if (dc == 0 && target == '.') {
                if (dr == direction)
                    return true;

                if (sr == startRow && dr == 2 * direction &&
                    squares[sr + direction][sc] == '.')
                    return true;
            }

            // Normal capture
            if (adc == 1 && dr == direction && target != '.')
                return true;

            // En passant
            if (adc == 1 && dr == direction && target == '.' &&
                er == enPassantRow && ec == enPassantCol) {
                int capturedRow = er + (white ? 1 : -1);
                char expectedPawn = white ? 'p' : 'P';

                return capturedRow >= 0 && capturedRow < 8 &&
                       squares[capturedRow][ec] == expectedPawn;
            }

            return false;
        }

        case 'n':
            return (adr == 2 && adc == 1) ||
                   (adr == 1 && adc == 2);

        case 'b':
            return adr == adc && isPathClear(sr, sc, er, ec);

        case 'r':
            return (dr == 0 || dc == 0) &&
                   isPathClear(sr, sc, er, ec);

        case 'q':
            return (adr == adc || dr == 0 || dc == 0) &&
                   isPathClear(sr, sc, er, ec);

        case 'k': {
            if (adr <= 1 && adc <= 1)
                return true;

            // Castling
            if (dr != 0 || adc != 2 || sc != 4)
                return false;

            if (white) {
                if (sr != 7 || whiteKingMoved)
                    return false;

                if (ec == 6) {
                    return !whiteRookHMoved &&
                           squares[7][7] == 'R' &&
                           squares[7][5] == '.' &&
                           squares[7][6] == '.' &&
                           !isInCheck(true) &&
                           !isSquareAttacked(7, 5, false) &&
                           !isSquareAttacked(7, 6, false);
                }

                if (ec == 2) {
                    return !whiteRookAMoved &&
                           squares[7][0] == 'R' &&
                           squares[7][1] == '.' &&
                           squares[7][2] == '.' &&
                           squares[7][3] == '.' &&
                           !isInCheck(true) &&
                           !isSquareAttacked(7, 3, false) &&
                           !isSquareAttacked(7, 2, false);
                }
            } else {
                if (sr != 0 || blackKingMoved)
                    return false;

                if (ec == 6) {
                    return !blackRookHMoved &&
                           squares[0][7] == 'r' &&
                           squares[0][5] == '.' &&
                           squares[0][6] == '.' &&
                           !isInCheck(false) &&
                           !isSquareAttacked(0, 5, true) &&
                           !isSquareAttacked(0, 6, true);
                }

                if (ec == 2) {
                    return !blackRookAMoved &&
                           squares[0][0] == 'r' &&
                           squares[0][1] == '.' &&
                           squares[0][2] == '.' &&
                           squares[0][3] == '.' &&
                           !isInCheck(false) &&
                           !isSquareAttacked(0, 3, true) &&
                           !isSquareAttacked(0, 2, true);
                }
            }

            return false;
        }
    }

    return false;
}

bool Board::isLegalMove(int sr, int sc, int er, int ec) {
    if (!isPseudoLegalMove(sr, sc, er, ec))
        return false;

    char piece = squares[sr][sc];
    char target = squares[er][ec];
    bool white = isupper(static_cast<unsigned char>(piece));

    bool castle = (piece == 'K' || piece == 'k') &&
                  sr == er && abs(ec - sc) == 2;

    bool enPassant = (piece == 'P' || piece == 'p') &&
                     sc != ec && target == '.' &&
                     er == enPassantRow && ec == enPassantCol;

    int capturedRow = er + (piece == 'P' ? 1 : -1);
    char epCaptured = '.';

    if (enPassant) {
        epCaptured = squares[capturedRow][ec];
        squares[capturedRow][ec] = '.';
    }

    int rookFrom = ec == 6 ? 7 : 0;
    int rookTo = ec == 6 ? 5 : 3;
    char rook = '.';

    if (castle) {
        rook = squares[sr][rookFrom];
        squares[sr][rookTo] = rook;
        squares[sr][rookFrom] = '.';
    }

    squares[er][ec] = piece;
    squares[sr][sc] = '.';

    // Promotion is simulated as a queen for king-safety testing.
    if (piece == 'P' && er == 0) squares[er][ec] = 'Q';
    if (piece == 'p' && er == 7) squares[er][ec] = 'q';

    bool legal = !isInCheck(white);

    // Restore the original board
    squares[sr][sc] = piece;
    squares[er][ec] = target;

    if (castle) {
        squares[sr][rookFrom] = rook;
        squares[sr][rookTo] = '.';
    }

    if (enPassant)
        squares[capturedRow][ec] = epCaptured;

    return legal;
}

void Board::updateCastlingRights(
    int sr, int sc, int er, int ec, char captured
) {
    char piece = squares[er][ec];

    if (piece == 'K') whiteKingMoved = true;
    if (piece == 'k') blackKingMoved = true;

    if (sr == 7 && sc == 0 && piece == 'R')
        whiteRookAMoved = true;
    if (sr == 7 && sc == 7 && piece == 'R')
        whiteRookHMoved = true;
    if (sr == 0 && sc == 0 && piece == 'r')
        blackRookAMoved = true;
    if (sr == 0 && sc == 7 && piece == 'r')
        blackRookHMoved = true;

    // Rights are lost if an original rook is captured.
    if (er == 7 && ec == 0 && captured == 'R')
        whiteRookAMoved = true;
    if (er == 7 && ec == 7 && captured == 'R')
        whiteRookHMoved = true;
    if (er == 0 && ec == 0 && captured == 'r')
        blackRookAMoved = true;
    if (er == 0 && ec == 7 && captured == 'r')
        blackRookHMoved = true;
}

bool Board::movePiece(int sr, int sc, int er, int ec) {
    if (!isLegalMove(sr, sc, er, ec)) {
        cout << "Invalid move!\n";
        return false;
    }

    char piece = squares[sr][sc];
    char captured = squares[er][ec];

    bool enPassant = (piece == 'P' || piece == 'p') &&
                     sc != ec && captured == '.' &&
                     er == enPassantRow && ec == enPassantCol;

    if (enPassant) {
        int capturedRow = er + (piece == 'P' ? 1 : -1);
        captured = squares[capturedRow][ec];
        squares[capturedRow][ec] = '.';
    }

    bool castle = (piece == 'K' || piece == 'k') &&
                  sr == er && abs(ec - sc) == 2;

    // Every move expires the previous en passant opportunity.
    enPassantRow = -1;
    enPassantCol = -1;

    squares[er][ec] = piece;
    squares[sr][sc] = '.';

    if (castle) {
        int rookFrom = ec == 6 ? 7 : 0;
        int rookTo = ec == 6 ? 5 : 3;

        squares[sr][rookTo] = squares[sr][rookFrom];
        squares[sr][rookFrom] = '.';
    }

    // Create a new en passant opportunity after a double pawn move.
    if (piece == 'P' && sr == 6 && er == 4) {
        enPassantRow = 5;
        enPassantCol = ec;
    } else if (piece == 'p' && sr == 1 && er == 3) {
        enPassantRow = 2;
        enPassantCol = ec;
    }

    // Ask for a promotion piece.
    if ((piece == 'P' && er == 0) ||
        (piece == 'p' && er == 7)) {

        char choice;

        cout << "Promote to (Q/R/B/N): ";
        cin >> choice;
        choice = static_cast<char>(
            toupper(static_cast<unsigned char>(choice))
        );

        if (choice != 'Q' && choice != 'R' &&
            choice != 'B' && choice != 'N') {
            cout << "Invalid choice. Queen selected.\n";
            choice = 'Q';
        }

        if (piece == 'p')
            choice = static_cast<char>(tolower(choice));

        squares[er][ec] = choice;
    }

    updateCastlingRights(sr, sc, er, ec, captured);
    whiteTurn = !whiteTurn;

    if (isInCheck(whiteTurn))
        cout << (whiteTurn ? "White" : "Black") << " is in check!\n";

    return true;
}

bool Board::hasLegalMove(bool white) {
    bool previousTurn = whiteTurn;
    whiteTurn = white;

    for (int sr = 0; sr < 8; sr++) {
        for (int sc = 0; sc < 8; sc++) {
            char piece = squares[sr][sc];

            if (piece == '.')
                continue;

            bool pieceWhite =
                isupper(static_cast<unsigned char>(piece));

            if (pieceWhite != white)
                continue;

            for (int er = 0; er < 8; er++) {
                for (int ec = 0; ec < 8; ec++) {
                    if (isLegalMove(sr, sc, er, ec)) {
                        whiteTurn = previousTurn;
                        return true;
                    }
                }
            }
        }
    }

    whiteTurn = previousTurn;
    return false;
}

bool Board::isGameOver() {
    if (hasLegalMove(whiteTurn))
        return false;

    if (isInCheck(whiteTurn)) {
        cout << "Checkmate! "
             << (whiteTurn ? "Black" : "White")
             << " wins!\n";
    } else {
        cout << "Stalemate! The game is a draw.\n";
    }

    return true;
}
char Board::getPiece(int r, int c) const {
    if (r < 0 || r >= 8 || c < 0 || c >= 8)
        return '.';

    return squares[r][c];
}