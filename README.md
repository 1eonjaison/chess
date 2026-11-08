# C++ Chess Game

This project is a chess game developed using C++.

The game allows users to play chess against other registered players, guests or a computer opponent. It includes a user account system, an Elo rating system, a leaderboard and different computer difficulty levels.

---

## How It Works

When the program starts, the user can create an account or log in.

Once logged in, the user can choose between different game modes.

The game supports:

- **Registered Player** – Play against another registered user in a rated match.
- **Guest Player** – Play against another person without affecting Elo ratings.
- **Computer Opponent** – Play against an AI with Easy, Medium or Hard difficulty.
- **Legal Move Checking** – Checks whether moves follow chess rules and prevents illegal moves.
- **Special Moves** – Supports castling, en passant and pawn promotion.
- **Game Results** – Detects checkmate, stalemate and different draw conditions.
- **Move History** – Displays previous moves made during a game.
- **Undo and Restart** – Allows players to undo moves or restart unrated games.
- **Elo Rating System** – Updates player ratings after registered matches.
- **Leaderboard** – Ranks registered players based on their Elo ratings.

---

## Computer Opponent

The game includes three computer difficulty levels.

- **Easy** – Selects a random legal move.
- **Medium** – Uses the Minimax algorithm to search moves at a depth of 2.
- **Hard** – Uses the Minimax algorithm to search moves at a depth of 4.

The computer evaluates board positions using piece values and positional advantages.

Alpha-beta pruning is used to reduce the number of positions searched.

---

## User Account System

The program uses text files to store user accounts and game information.

Users can create an account with a username and password.

Each account starts with an Elo rating of 1000.

The system tracks each player's wins, losses, draws and total games played.

Registered matches update Elo ratings using a K-factor of 32, while guest and computer matches do not affect ratings.

The leaderboard displays the top 20 registered players.

---

## Project Aim

The aim of this project was to develop a chess game that combines game logic, computer algorithms and user account management.

It helped me improve my C++ programming skills and understand how algorithms, data structures, file handling and game development can be implemented in a single application.

The project also helped me gain experience with the Minimax algorithm, alpha-beta pruning and implementing chess rules.
