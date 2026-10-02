
# C++ WebAssembly Chess

A browser-based chess game built using C++, HTML, CSS, JavaScript and WebAssembly.

## About the Project

This project implements a chess rules engine in C++ and uses Emscripten to compile it into WebAssembly. JavaScript connects the engine to a browser-based chessboard.

## Features

- Standard chess piece movements
- Legal move validation
- Check and checkmate detection
- Stalemate detection
- Castling
- En passant
- Pawn promotion
- Interactive browser interface

## Tech Stack

- **C++** — Chess engine
- **Emscripten** — WebAssembly compilation
- **WebAssembly** — Browser execution
- **HTML** — Page structure
- **CSS** — Chessboard styling
- **JavaScript** — User interaction and engine integration

## Project Structure

```text
Chess/
├── cpp/
│   ├── Board.cpp
│   ├── Board.h
│   ├── Bindings.cpp
│   └── main.cpp
├── wasm/
│   ├── chess.js
│   └── chess.wasm
├── index.html
├── style.css
├── script.js
└── README.md
```

## How to Run

1. Clone this repository.
2. Open the project in VS Code.
3. Start a local web server, such as Live Server.
4. Open the provided local URL in your browser.

## Build WebAssembly

Install and activate the Emscripten SDK, then run this command from the `cpp` folder:

```bash
em++ Board.cpp Bindings.cpp -o ../wasm/chess.js \
-std=c++17 -lembind \
-sMODULARIZE=1 \
-sEXPORT_NAME=createChessModule \
-sALLOW_MEMORY_GROWTH=1
```

## Current Status

The chess engine and browser integration are under development. Some rules and browser interactions still require testing.

## Future Improvements

- Improve the chessboard interface
- Add move history
- Add a chess-playing AI
- Add a clock and player controls
- Improve automated testing

## Author

Developed as a C++ and WebAssembly learning project.
