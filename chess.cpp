#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <cmath>
#include <cctype>
#include <climits>
using namespace std;

mt19937 generator(random_device{}());
struct Move{
    int from;
    int to;
    char promotion = ' ';
    bool ep = false;
    bool castle = false;
};

struct Game{
    char board[8][8];
    bool whiteTurn = true;
    bool kingMoved[2] = { false, false};
    bool rookMoved[2][2] = { { false, false}, { false, false}};
    int epRow = -1;
    int epCol = -1;
    int halfmoves = 0;
};

struct Account{
    int id = 0;
    string username;
    int rating = 1000;
};

string ask(string message){
    cout << message;
    string answer;
    getline(cin, answer);
    return answer;
}

string clean(string input){
    string result;
    for (char letter : input)
    {
        if (!isspace((unsigned char)letter))
        {
            result += (char)tolower((unsigned char)letter);
        }
    }
    return result;
}

struct SavedAccount{
    int id = 0;
    string username;
    string password;
    int rating = 1000;
    int wins = 0;
    int losses = 0;
    int draws = 0;
};

vector<SavedAccount> accounts;
string lowername(string name){
    for (char &letter : name)
    {
        letter = (char)tolower((unsigned char)letter);
    }
    return name;
}

bool loadaccounts(){
    ifstream file("accounts.txt");
    if (!file.is_open())
    {
        return true;
    }
    SavedAccount account;
    while (file >> account.id >> account.username >> account.password >> account.rating >> account.wins >> account.losses >> account.draws)
    {
        accounts.push_back(account);
    }
    return file.eof();
}

bool saveaccounts(){
    ofstream file("accounts.txt");
    if (!file.is_open())
    {
        cout << "Unable to save accounts.txt." << endl;
        return false;
    }
    for (SavedAccount account:  accounts)
    {
        file << account.id << " " << account.username << " " << account.password << " " << account.rating << " " << account.wins << " " << account.losses << " " << account.draws << endl;
    }
    return file.good();
}

int findaccount(string username){
    for (int i = 0; i <(int) accounts.size(); i++)
    {
        if (lowername(accounts[i].username) == lowername(username))
        {
            return i;
        }
    }
    return - 1;
}

int findid(int id){
    for (int i = 0; i <(int) accounts.size(); i++)
    {
        if (accounts[i].id == id)
        {
            return i;
        }
    }
    return - 1;
}

void createaccount(){
    string username = ask("\nNew username (or exit): ");
    if (username == "exit")
    {
        return;
    }
    if (username.size() < 3 || username.size() > 24)
    {
        cout << "Username must contain 3 to 24 characters." << endl;
        return;
    }
    for (char letter:  username)
    {
        if (!isalnum((unsigned char)letter) && letter != '_')
        {
            cout << "Use only letters, numbers or underscores." << endl;
            return;
        }
    }
    if (findaccount(username) != - 1)
    {
        cout << "Username already exists." << endl;
        return;
    }
    string password = ask("Password (at least 8 characters): ");
    if (password.size() < 8 || password.find(' ') != string::npos)
    {
        cout << "Use at least 8 characters and no spaces." << endl;
        return;
    }
    if (ask("Confirm password: ") != password)
    {
        cout << "Passwords do not match." << endl;
        return;
    }
    SavedAccount account;
    for (SavedAccount existing:  accounts)
    {
        account.id = max(account.id, existing.id);
    }
    account.id++;
    account.username = username;
    account.password = password;
    accounts.push_back(account);
    if (saveaccounts())
    {
        cout << "Account created successfully." << endl;
    }
}

Account login(){
    Account result;
    string username = ask("\nUsername (or exit): ");
    if (username == "exit")
    {
        return result;
    }
    int index = findaccount(username);
    if (index == - 1)
    {
        cout << "Account not found." << endl;
        return result;
    }
    for (int attempt = 0; attempt < 3; attempt++)
    {
        string password = ask("Password: ");
        if (password == accounts[index].password)
        {
            result.id = accounts[index].id;
            result.username = accounts[index].username;
            result.rating = accounts[index].rating;
            cout << "Login successful. Welcome, " << result.username << "!" << endl;
            return result;
        }
        cout << "Incorrect password. Attempts remaining: " << 2 - attempt << endl;
    }
    return result;
}

void leaderboard(){
    vector<SavedAccount> sorted = accounts;
    sort(sorted.begin(), sorted.end(),[](SavedAccount a, SavedAccount b)
    {
        if (a.rating != b.rating)
        {
            return a.rating > b.rating;
        }
        if (a.wins != b.wins)
        {
            return a.wins > b.wins;
        }
        return a.username < b.username;
    }
    );
    cout << "\nLeaderboard (Top 20)" << endl;
    cout << "Rank  Username                 Elo    W   L   D" << endl;
    for (int i = 0; i <(int) sorted.size() && i < 20; i++)
    {
        SavedAccount a = sorted[i];
        cout << i + 1 << ".    " << a.username << string(max(1, 25 -(int) a.username.size()), ' ') << a.rating << "   " << a.wins << "   " << a.losses << "   " << a.draws << endl;
    }
}

void statistics(Account account){
    int index = findid(account.id);
    if (index == - 1)
    {
        return;
    }
    SavedAccount a = accounts[index];
    cout << "\n" << a.username << "'s Statistics" << endl;
    cout << "Rating: " << a.rating << endl;
    cout << "Wins: " << a.wins << endl;
    cout << "Losses: " << a.losses << endl;
    cout << "Draws: " << a.draws << endl;
    cout << "Games: " << a.wins + a.losses + a.draws << endl;
}
int ratingof(int id)
{
    int index = findid(id);
    if (index == - 1)
    {
        return 1000;
    }
    return accounts[index].rating;
}

void updateaccount(int id, int change, double score){
    int index = findid(id);
    if (index == - 1)
    {
        return;
    }
    accounts[index].rating = max(100, accounts[index].rating + change);
    if (score == 1)
    {
        accounts[index].wins++;
    }
    else if (score == 0)
    {
        accounts[index].losses++;
    }
    else
    {
        accounts[index].draws++;
    }
}

void savegame(Account white, Account black, string mode, double whiteScore, bool rated){
    string result = whiteScore == 1 ?  "White wins":  whiteScore == 0 ?  "Black wins":  "Draw";
    ofstream file("games.txt", ios::app);
    if (!file.is_open())
    {
        cout << "Unable to save games.txt." << endl;
        return;
    }
    file << white.id << " " << black.id << " " << mode << " " << whiteScore << " " << rated << endl;
    file.close();
    if (rated && white.id && black.id)
    {
        int whiteRating = ratingof(white.id);
        int blackRating = ratingof(black.id);
        double expectedWhite = 1.0 /(1.0 + pow(10.0,(blackRating - whiteRating) / 400.0));
        int change =(int) round(32 *(whiteScore - expectedWhite));
        updateaccount(white.id, change, whiteScore);
        updateaccount(black.id, - change, 1 - whiteScore);
        cout << "Elo change: " << white.username << " " <<(change >= 0 ?  "+":  "") << change << ", " << black.username << " " <<(- change >= 0 ?  "+":  "") << - change << endl;
    }
    else
    {
        if (white.id)
        {
            updateaccount(white.id, 0, whiteScore);
        }
        if (black.id)
        {
            updateaccount(black.id, 0, 1 - whiteScore);
        }
    }
    if (saveaccounts())
    {
        cout << "Game saved. Result: " << result << endl;
    }
}

bool inside(int r, int c){
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}

bool iswhite(char p){
    return p >= 'A' && p <= 'Z';
}

bool isblack(char p){
    return p >= 'a' && p <= 'z';
}

bool samecolour(char a, char b){
    return a != '.' && b != '.' && iswhite(a) == iswhite(b);
}

void setupboard(Game& g){
    string back = "rnbqkbnr";
    for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			g.board[r][c] = '.';
		}
	}
    for (int c = 0; c < 8; c++){
        g.board[0][c] = back[c];
        g.board[1][c] = 'p';
        g.board[6][c] = 'P';
        g.board[7][c] =(char) toupper(back[c]);
    }
}

void showboard(Game g){
    cout << "\n    a b c d e f g h\n  +-----------------+\n";
    for (int r = 0; r < 8; r++)
    {
        cout << 8 - r << " | ";
        for (int c = 0; c < 8; c++){
			cout << g.board[r][c] << ' ';
		}
        cout << "| " << 8 - r << endl;
    }
    cout << "  +-----------------+\n    a b c d e f g h\nUppercase = White, lowercase = Black\n";
}

bool squareattacked(Game g, int row, int col, bool white)
{
    int direction = white ? -1 : 1;
    char pawn = white ? 'P' : 'p', knight = white ? 'N' : 'n', bishop = white ? 'B' : 'b';
    char rook = white ? 'R' : 'r', queen = white ? 'Q' : 'q', king = white ? 'K' : 'k';
    for (int side : {-1, 1})
    {
        if (inside(row - direction, col + side) && g.board[row - direction][col + side] == pawn)
        {
            return true;
        }
    }
    int nr[] = {-2, -2, -1, -1, 1, 1, 2, 2}, nc[] = {-1, 1, -2, 2, -2, 2, -1, 1};
    for (int i = 0; i < 8; i++)
    {
        if (inside(row + nr[i], col + nc[i]) && g.board[row + nr[i]][col + nc[i]] == knight)
        {
            return true;
        }
    }
    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1}, dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    for (int i = 0; i < 8; i++)
    {
        int r = row + dr[i], c = col + dc[i], distance = 1;
        while (inside(r, c))
        {
            char piece = g.board[r][c];
            if (piece != '.')
            {
                if (distance == 1 && piece == king)
                {
                    return true;
                }
                bool diagonal = dr[i] != 0 && dc[i] != 0;
                if (diagonal && (piece == bishop || piece == queen))
                {
                    return true;
                }
                if (!diagonal && (piece == rook || piece == queen))
                {
                    return true;
                }
                break;
            }
            r += dr[i];
            c += dc[i];
            distance++;
        }
    }
    return false;
}

bool kingincheck(Game g, bool white)
{
    char king = white ?  'K':  'k';
    for (int r = 0; r < 8; r++){
		for (int c = 0; c < 8; c++) {
			if (g.board[r][c] == king) {
				return squareattacked(g, r, c, ! white);
			}
		}
	}
    return true;
}

void addmove(vector<Move> & moves, Game g, int from, int to, bool ep = false, bool castle = false)
{
    int row = to / 8;
    char piece = g.board[from / 8][from % 8];
    if (tolower(piece) == 'p' &&(row == 0 || row == 7))
    {
        for (char promotion:  string("qrbn")) {
			moves.push_back({ from, to, promotion, ep, castle});
		}
    }
    else moves.push_back({ from, to, ' ', ep, castle});
}

vector<Move> possiblemoves(Game g)
{
    vector<Move> moves;
    for (int row = 0; row < 8; row++) for (int col = 0; col < 8; col++)
    {
        char piece = g.board[row][col];
        if (piece == '.' || iswhite(piece) != g.whiteTurn) {
			continue;
			}
		
        int from = row * 8 + col;
        char type =(char) tolower(piece);
        if (type == 'p')
        {
            int direction = g.whiteTurn ?  - 1:  1, next = row + direction;
            if (inside(next, col) && g.board[next][col] == '.')
            {
                addmove(moves, g, from, next * 8 + col);
                int two = row + 2 * direction;
                if (row ==(g.whiteTurn ?  6:  1) && g.board[two][col] == '.') {
					addmove(moves, g, from, two * 8 + col);
				}
            }
            for (int side:
            {
                - 1, 1
            }
            )
            {
                int c = col + side;
                if (!inside(next, c)) {
					continue;
				}
                char target = g.board[next][c];
                if (target != '.' && ! samecolour(piece, target) && tolower(target) != 'k') {
					addmove(moves, g, from, next * 8 + c);
				}
                if (next == g.epRow && c == g.epCol && target == '.' && g.board[row][c] ==(g.whiteTurn ?  'p':  'P')){
					addmove(moves, g, from, next * 8 + c, true);
				}
            }
        }
        else if (type == 'n')
        {
            int dr[] = { - 2, - 2, - 1, - 1, 1, 1, 2, 2}, dc[] = { - 1, 1, - 2, 2, - 2, 2, - 1, 1};
            for (int i = 0; i < 8; i++)
            {
                int r = row + dr[i], c = col + dc[i];
                if (inside(r, c) && ! samecolour(piece, g.board[r][c]) && tolower(g.board[r][c]) != 'k') {
					addmove(moves, g, from, r * 8 + c);
				}
            }
        }
        else
        {
            int dr[] = { - 1, - 1, - 1, 0, 0, 1, 1, 1}, dc[] = { - 1, 0, 1, - 1, 1, - 1, 0, 1};
            for (int i = 0; i < 8; i++)
            {
                bool diagonal = dr[i] != 0 && dc[i] != 0;
                if (type == 'b' && ! diagonal) {
					continue;
				}
                if (type == 'r' && diagonal) {
					continue;
				}
                int r = row + dr[i], c = col + dc[i];
                while (inside(r, c))
                {
                    char target = g.board[r][c];
                    if (samecolour(piece, target) || tolower(target) == 'k') {
						break;
					}
                    addmove(moves, g, from, r * 8 + c);
                    if (target != '.' || type == 'k') {
						break;
					}
                    r += dr[i];
                    c += dc[i];
                }
            }
            if (type == 'k')
            {
                int home = g.whiteTurn ?  7:  0, side = g.whiteTurn ?  0:  1;
                char rook = g.whiteTurn ?  'R':  'r';
                if (row == home && col == 4 && ! g.kingMoved[side] && ! kingincheck(g, g.whiteTurn))
                {
                    if (!g.rookMoved[side][1] && g.board[home][7] == rook && g.board[home][5] == '.' && g.board[home][6] == '.' && ! squareattacked(g, home, 5, ! g.whiteTurn) && ! squareattacked(g, home, 6, ! g.whiteTurn)) {
						addmove(moves, g, from, home * 8 + 6, false, true);
					}
                    if (!g.rookMoved[side][0] && g.board[home][0] == rook && g.board[home][1] == '.' && g.board[home][2] == '.' && g.board[home][3] == '.' && ! squareattacked(g, home, 3, ! g.whiteTurn) && ! squareattacked(g, home, 2, ! g.whiteTurn)) {
						addmove(moves, g, from, home * 8 + 2, false, true);
					}
                }
            }
        }
    }
    return moves;
}

Game makemove(Game g, Move m)
{
    int fr = m.from / 8, fc = m.from % 8, tr = m.to / 8, tc = m.to % 8;
    char piece = g.board[fr][fc], captured = g.board[tr][tc];
    g.board[fr][fc] = '.';
    g.board[tr][tc] = m.promotion == ' ' ?  piece:(g.whiteTurn ?(char) toupper(m.promotion):  m.promotion);
    if (m.ep) {
		g.board[fr][tc] = '.';
	}
    if (m.castle)
    {
        if (tc == 6)
        {
            g.board[tr][5] = g.board[tr][7];
            g.board[tr][7] = '.';
        }
        else
        {
            g.board[tr][3] = g.board[tr][0];
            g.board[tr][0] = '.';
        }
    }
    if (piece == 'K') {
		g.kingMoved[0] = true;
	}
    if (piece == 'k') {
		g.kingMoved[1] = true;
	}
    if (m.from == 56 || m.to == 56) {
		g.rookMoved[0][0] = true;
	}
    if (m.from == 63 || m.to == 63) {
		g.rookMoved[0][1] = true;
	}
    if (m.from == 0 || m.to == 0) {
		g.rookMoved[1][0] = true;
	}
    if (m.from == 7 || m.to == 7) {
		g.rookMoved[1][1] = true;
	}
    g.epRow = -1;
    g.epCol = -1;
    if (tolower(piece) == 'p' && abs(tr - fr) == 2)
    {
        g.epRow =(fr + tr) / 2;
        g.epCol = fc;
    }
    g.halfmoves =(tolower(piece) == 'p' || captured != '.' || m.ep) ?  0:  g.halfmoves + 1;
    g.whiteTurn = ! g.whiteTurn;
    return g;
}

vector<Move> legalmoves(Game g)
{
    vector<Move> result;
    for (Move m:  possiblemoves(g)) if (!kingincheck(makemove(g, m), g.whiteTurn)) {
		result.push_back(m);
	}
    return result;
}
string movetext(Move m)
{
    string s;
    s += char('a' + m.from % 8);
    s += char('8' - m.from / 8);
    s += char('a' + m.to % 8);
    s += char('8' - m.to / 8);
    if (m.promotion != ' ') s += m.promotion;
    return s;
}

int piecevalue(char p)
{
    p = (char)tolower(p);
    if (p == 'p') return 100;
    if (p == 'n') return 320;
    if (p == 'b') return 330;
    if (p == 'r') return 500;
    if (p == 'q') return 900;
    return 0;
}

int evaluateboard(Game g)
{
    int score = 0;
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            char p = g.board[r][c];
            if (p == '.') continue;
            int value = piecevalue(p);
            if (tolower(p) == 'p') value +=(iswhite(p) ? 6 - r: r - 1) * 5;
            if (tolower(p) == 'n' || tolower(p) == 'b') value +=(3 - abs(3 - c)) * 3;
            score += iswhite(p) ? value: - value;
        }
    }
    return g.whiteTurn ? score: - score;
}

int minimax(Game g, int depth, int alpha, int beta)
{
    vector<Move> moves = legalmoves(g);
    if (moves.empty()) return kingincheck(g, g.whiteTurn) ? - 100000 - depth: 0;
    if (g.halfmoves >= 100 || depth == 0) return g.halfmoves >= 100 ? 0: evaluateboard(g);
    int best = -1000000;
    for (Move m: moves)
    {
        int score = - minimax(makemove(g, m), depth - 1, - beta, - alpha);
        best = max(best, score);
        alpha = max(alpha, score);
        if (alpha >= beta) break;
    }
    return best;
}

Move computermove(Game g, int difficulty)
{
    vector<Move> moves = legalmoves(g);
    shuffle(moves.begin(), moves.end(), generator);
    if (difficulty == 1) return moves[0];
    int depth = difficulty == 2 ? 2: 4, best = -1000000;
    Move chosen = moves[0];
    for (Move m: moves)
    {
        int score = - minimax(makemove(g, m), depth - 1, - 1000000, 1000000);
        if (score > best)
        {
            best = score;
            chosen = m;
        }
    }
    return chosen;
}

string positionkey(Game g)
{
    string key;
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            key += g.board[r][c];
        }
    }
    key += g.whiteTurn ? 'W': 'B';
    for (int side = 0; side < 2; side++)
    {
        key += g.kingMoved[side] ? '1': '0';
        for (int rook = 0; rook < 2; rook++) key += g.rookMoved[side][rook] ? '1': '0';
    }
    bool canEp = false;
    for (Move m: legalmoves(g)) if (m.ep) canEp = true;
    if (canEp)
    {
        key += char('0' + g.epRow);
        key += char('0' + g.epCol);
    }
    return key;
}

int repetitioncount(const vector<Game> & states)
{
    string key = positionkey(states.back());
    int count = 0;
    for (Game g: states) if (positionkey(g) == key) count++;
    return count;
}

bool insufficientmaterial(Game g)
{
    int count = 0, bishops = 0, firstColour = -1;
    bool sameColour = true;
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            char p = (char)tolower(g.board[r][c]);
            if (p == '.' || p == 'k') continue;
            if (p != 'b' && p != 'n') return false;
            count++;
            if (p == 'b')
            {
                bishops++;
                int colour = (r + c) % 2;
                if (firstColour != - 1 && colour != firstColour) sameColour = false;
                firstColour = colour;
            }
        }
    }
    return count <= 1 ||(count == bishops && sameColour);
}

void showhistory(vector<string> history)
{
    cout << "\nMove History\n";
    if (history.empty()) cout << "No moves have been played.\n";
    for (int i = 0; i <(int) history.size(); i++)
    {
        if (i % 2 == 0) cout << i / 2 + 1 << ". ";
        cout << history[i] <<(i % 2 == 1 ? "\n": "  ");
    }
    cout << endl;
}

void showhelp()
{
    ifstream file("help.txt");
    if (!file.is_open())
    {
        cout << "\nThe help file could not be found.\n";
        return;
    }
    cout << "\n";
    string line;
    while (getline(file, line)) cout << line << endl;
    file.close();
    while (true)
    {
        if (clean(ask("\nType 'exit' to return: ")) == "exit") return;
        cout << "Please type 'exit'.\n";
    }
}

int choosedifficulty()
{
    while (true)
    {
        cout << "\nChoose Difficulty\n1. Easy\n2. Medium\n3. Hard\n4. Exit\n";
        string choice = ask("\nChoose: ");
        if (choice == "1") return 1;
        else if (choice == "2") return 2;
        else if (choice == "3") return 3;
        else if (choice == "4") return 0;
        else cout << "Invalid choice.\n";
    }
}

void playgame(Account white, Account black, string mode, int difficulty, bool rated, int humanSide)
{
    Game start;
    setupboard(start);
    vector<Game> states = {start};
    vector<string> history;
    cout << "\nType 'help' for instructions or 'exit' to leave the game at any time.\n";
    while (true)
    {
        Game g = states.back();
        showboard(g);
        vector<Move> moves = legalmoves(g);
        int repeats = repetitioncount(states);
        if (moves.empty())
        {
            bool check = kingincheck(g, g.whiteTurn);
            cout <<(check ? "\nCheckmate!\n": "\nStalemate!\n");
            savegame(white, black, mode, check ?(g.whiteTurn ? 0.0 : 1.0): 0.5, rated);
            return;
        }
        if (insufficientmaterial(g) || g.halfmoves >= 150 || repeats >= 5)
        {
            cout << "\nGame drawn automatically.\n";
            savegame(white, black, mode, 0.5, rated);
            return;
        }
        cout << "\n" <<(g.whiteTurn ? "White": "Black") << " to move";
        if (kingincheck(g, g.whiteTurn)) cout << " - CHECK";
        cout << endl;
        if (mode == "computer" &&(g.whiteTurn ? 1 : -1) != humanSide)
        {
            cout << "Computer is thinking...\n";
            Move m = computermove(g, difficulty);
            cout << "Computer plays: " << movetext(m) << endl;
            history.push_back(movetext(m));
            states.push_back(makemove(g, m));
            continue;
        }
        string choice = clean(ask("\nMove: "));
        if (choice == "exit")
        {
            if (rated)
            {
                cout << "Exiting a rated match counts as resignation.\n";
                savegame(white, black, mode, g.whiteTurn ? 0.0 : 1.0, rated);
            }
            else cout << "Game exited without recording a result.\n";
            return;
        }
        else if (choice == "help")
        {
            showhelp();
            continue;
        }
        else if (choice == "moves")
        {
            for (Move m: moves) cout << movetext(m) << " ";
            cout << endl;
            continue;
        }
        else if (choice == "history")
        {
            showhistory(history);
            continue;
        }
        else if (choice == "undo")
        {
            if (rated)
            {
                cout << "Undo is disabled in rated games.\n";
                continue;
            }
            if (states.size() == 1)
            {
                cout << "Nothing to undo.\n";
                continue;
            }
            do
            {
                states.pop_back();
                history.pop_back();
            }
            while (mode == "computer" && states.size() > 1 &&(states.back().whiteTurn ? 1 : -1) != humanSide);
            cout << "Move undone.\n";
            continue;
        }
        else if (choice == "restart")
        {
            if (rated)
            {
                cout << "Restart is disabled in rated games.\n";
                continue;
            }
            if (clean(ask("Restart this game? (yes/no): ")) == "yes")
            {
                states.assign(1, start);
                history.clear();
                cout << "Game restarted.\n";
            }
            continue;
        }
        else if (choice == "claim")
        {
            if (repeats >= 3 || g.halfmoves >= 100)
            {
                cout << "Draw claimed.\n";
                savegame(white, black, mode, 0.5, rated);
                return;
            }
            cout << "No draw is available to claim.\n";
            continue;
        }
        else if (choice == "resign")
        {
            if (clean(ask("Confirm resignation? (yes/no): ")) == "yes")
            {
                savegame(white, black, mode, g.whiteTurn ? 0.0 : 1.0, rated);
                return;
            }
            continue;
        }
        bool found = false;
        for (Move m: moves) if (movetext(m) == choice)
        {
            history.push_back(choice);
            states.push_back(makemove(g, m));
            found = true;
            break;
        }
        if (!found) cout << "Invalid or illegal move. Example: e2e4. Type 'moves' for legal moves.\n";
    }
}

void gamemenu(Account current)
{
    while (true)
    {
        cout << "\nChess Game\nLogged in as: " << current.username << "\n1. Player vs Registered Player\n2. Player vs Guest\n3. Player vs Computer\n4. My Statistics\n5. Leaderboard\n6. Help\n7. Logout\n8. Exit\n";
        string choice = ask("\nChoose: ");
        if (choice == "1")
        {
            cout << "\nSecond player login\n";
            Account second = login();
            if (!second.id) continue;
            if (second.id == current.id)
            {
                cout << "You cannot play against your own account.\n";
                continue;
            }
            playgame(current, second, "registered", 0, true, 0);
        }
        else if (choice == "2") playgame(current, Account{}, "guest", 0, false, 0);
        else if (choice == "3")
        {
            int difficulty = choosedifficulty();
            if (!difficulty) continue;
            string colour = ask("1. White\n2. Black\nChoose: ");
            if (colour == "1") playgame(current, Account{}, "computer", difficulty, false, 1);
            else if (colour == "2") playgame(Account{}, current, "computer", difficulty, false, - 1);
            else cout << "Invalid choice.\n";
        }
        else if (choice == "4") statistics(current);
        else if (choice == "5") leaderboard();
        else if (choice == "6") showhelp();
        else if (choice == "7")
        {
            cout << "Logged out.\n";
            return;
        }
        else if (choice == "8")
        {
            cout << "Goodbye!\n";
            exit(0);
        }
        else cout << "Invalid choice.\n";
    }
}

int main()
{
    if (!loadaccounts())
    {
        cout << "Unable to read accounts.txt." << endl;
        return 1;
    }
    while (true)
    {
        cout << "\nChess Game\n1. Login\n2. Create Account\n3. Leaderboard\n4. Help\n5. Exit\n";
        string choice = ask("\nChoose: ");
        if (choice == "1")
        {
            Account account = login();
            if (account.id) gamemenu(account);
        }
        else if (choice == "2") createaccount();
        else if (choice == "3") leaderboard();
        else if (choice == "4") showhelp();
        else if (choice == "5")
        {
            cout << "Goodbye!\n";
            break;
        }
        else cout << "Invalid choice.\n";
    }
    return 0;
}
