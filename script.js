const boardElement = document.getElementById("board");
const statusElement = document.getElementById("status");
const resetButton = document.getElementById("reset");

const pieces = {
    K: "♔", Q: "♕", R: "♖", B: "♗", N: "♘", P: "♙",
    k: "♚", q: "♛", r: "♜", b: "♝", n: "♞", p: "♟"
};

let engine = null;
let selected = null;
let whiteTurn = true;
let gameEnded = false;

async function initializeChess() {
    try {
        engine = await createChessModule();
        renderBoard();
        updateStatus("White's turn");
    } catch (error) {
        console.error(error);
        updateStatus("Could not load the chess engine.");
    }
}

function updateStatus(message) {
    statusElement.textContent = message;
}

function renderBoard() {
    if (!engine) return;

    const board = engine.getBoard();
    boardElement.innerHTML = "";

    for (let r = 0; r < 8; r++) {
        for (let c = 0; c < 8; c++) {
            const square = document.createElement("button");
            square.type = "button";
            square.className =
                "square " + ((r + c) % 2 === 0 ? "light" : "dark");

            square.setAttribute("aria-label", `Square ${String.fromCharCode(97 + c)}${8 - r}`);

            if (selected && selected.r === r && selected.c === c) {
                square.classList.add("selected");
            }

            const piece = board[r][c];

            if (piece !== ".") {
                square.textContent = pieces[piece] || piece;
                square.classList.add(
                    piece === piece.toUpperCase()
                        ? "piece-white"
                        : "piece-black"
                );
            }

            square.addEventListener("click", () => {
                handleSquareClick(r, c);
            });

            boardElement.appendChild(square);
        }
    }
}

function handleSquareClick(r, c) {
    if (!engine || gameEnded) return;

    const board = engine.getBoard();
    const piece = board[r][c];

    if (!selected) {
        if (piece === ".") {
            updateStatus("Select a piece first.");
            return;
        }

        const isWhitePiece = piece === piece.toUpperCase();

        if (isWhitePiece !== whiteTurn) {
            updateStatus("Select a piece belonging to the current player.");
            return;
        }

        selected = { r, c };
        renderBoard();
        updateStatus("Select a destination square.");
        return;
    }

    // Select another piece belonging to the same player.
    if (piece !== "." &&
        (piece === piece.toUpperCase()) === whiteTurn) {
        selected = { r, c };
        renderBoard();
        updateStatus("Select a destination square.");
        return;
    }

    const from = selected;
    selected = null;

    const success = engine.makeMove(from.r, from.c, r, c);

    if (!success) {
        renderBoard();
        updateStatus("Illegal move. Try again.");
        return;
    }

    whiteTurn = !whiteTurn;
    renderBoard();

    gameEnded = engine.gameOver();

    if (gameEnded) {
        updateStatus("Game over. Start a new game to play again.");
    } else {
        updateStatus(whiteTurn ? "White's turn" : "Black's turn");
    }
}

resetButton.addEventListener("click", () => {
    if (!engine) return;

    engine.resetGame();
    selected = null;
    whiteTurn = true;
    gameEnded = false;

    renderBoard();
    updateStatus("White's turn");
});

initializeChess();