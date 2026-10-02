#ifndef BOARD_H
#define BOARD_H

class Board {
private:
    char squares[8][8];
    bool whiteTurn;

    bool whiteKingMoved, blackKingMoved;
    bool whiteRookAMoved, whiteRookHMoved;
    bool blackRookAMoved, blackRookHMoved;

    int enPassantRow, enPassantCol;

    bool isPathClear(int sr, int sc, int er, int ec);
    bool isSquareAttacked(int r, int c, bool byWhite);
    bool isInCheck(bool white);
    bool isPseudoLegalMove(int sr, int sc, int er, int ec);
    bool isLegalMove(int sr, int sc, int er, int ec);
    bool hasLegalMove(bool white);

    void updateCastlingRights(
        int sr, int sc, int er, int ec, char captured
    );

public:
    Board();

    void display();
    bool movePiece(int sr, int sc, int er, int ec);
    bool isGameOver();
    char getPiece(int r, int c) const;
};

#endif