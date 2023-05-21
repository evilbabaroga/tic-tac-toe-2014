#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <cstdlib>
#include <conio.h>
#include <ctype.h>
#include <locale>
#include <Windows.h>

using namespace std;

const int MAX = 21;
const int MAX_PLAYERS = 5;
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

class Player{
public:
	Player(){
		_name = "";
		_sign = '\0';
		_color = 0;
	}
	Player(string name, char sign, WORD color){
		_name = name;
		_sign = sign;
		_color = color;
	}

	void setName(string name){
		_name = name;
	}

	void setSign(char sign){
		_sign = sign;
	}

	char getSign(){
		return _sign;
	}

	string getName(){
		return _name;
	}

	WORD getColor(){
		return _color;
	}

	void setColor(WORD color){
		_color = color;
	}
private:
	string _name;
	char _sign;
	WORD _color;
};

class Board{
public:
	Board(){
		int _x = 0;
		int _y = 0;
		int winReq = 0;
	}
	Board(int x, int y, int winReq){
		_x = x;
		_y = y;
		_winReq = winReq;
		char a = ' ';
		int m = 1;
		boardLayout[0][0] = a;

		for (int i = 0; i <= 2 * _y + 2; i++){
			for (int j = 0; j <= 2 * _x + 3; j++){
				boardLayout[i][j] = ' ';
			}
		}

		for (int i = 2 * _y + 1; i >= 1; i--, m++){
			if (i % 2 == 0){
				boardLayout[i][2] = 48 + (m / 2);
			}
			if (i != 1 && i != 2 * _y + 1){
				boardLayout[i][3] = '|';
				boardLayout[i][(2 * _x) + 3] = '|';
			}
		}

		for (int j = 3; j <= 2 * _x + 3; j++){
			if (j % 2 == 0){
				boardLayout[2 * _y + 2][j] = 48 + (j - 2) / 2;
			}
			boardLayout[2 * _y + 2][0] = ' ';
			if (j != 3 && j != (2 * _x + 3)){
				boardLayout[1][j] = '-';
				boardLayout[(2 * _y) + 1][j] = '-';
			}
		}

		for (int i = 2; i <= 2 * _y; i++){
			for (int j = 4; j <= 2 * _x + 2; j++){
				if (j % 2 == 0 && i % 2 == 0){
					boardLayout[i][j] = ' ';
				}
				else if (j % 2 == 0 && i % 2 != 0){
					boardLayout[i][j] = '-';
				}
				else{
					boardLayout[i][j] = '|';
				}
			}
		}
	}

	void printBoard(){
		cout << endl;
		for (int i = 0; i <= 2 * _y + 2; i++){
			for (int j = 0; j <= 2 * _x + 3; j++){
				if (i == 2 * _y + 2 || j == 2){
					cout << boardLayout[i][j];
					SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
				else{
					cout << boardLayout[i][j];
				}
			}
			cout << endl;
		}
		cout << endl << endl;
	}

	void printBoard(Player players){
		cout << endl;
		for (int i = 0; i <= 2 * _y + 2; i++){
			for (int j = 0; j <= 2 * _x + 3; j++){
				if (i == 2 * _y + 2 || j == 2){
					cout << boardLayout[i][j];
					SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
				else if (boardLayout[i][j] == players.getSign()){
					SetConsoleTextAttribute(h, players.getColor());
					cout << boardLayout[i][j];
					SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
				else{
					cout << boardLayout[i][j];
				}
			}
			cout << endl;
		}
		cout << endl << endl;
	}

	void printBoard(Player players[], int numPlayers){
		cout << endl;
		for (int i = 0; i <= 2 * _y + 2; i++){
			for (int j = 0; j <= 2 * _x + 3; j++){
				bool doPaint = false;
				int z = 0;
				for (; z < numPlayers; z++){
					if (boardLayout[i][j] == players[z].getSign()){
						doPaint = true;
						break;
					}
				}
				if (i == 2 * _y + 2 || j == 2){
					cout << boardLayout[i][j];
					SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
				else if (doPaint){
					SetConsoleTextAttribute(h, players[z].getColor());
					cout << boardLayout[i][j];
					SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
				else{
					cout << boardLayout[i][j];
				}
			}
			cout << endl;
		}
		cout << endl << endl;
	}

	bool applySign(int x, int y, Player player){
		if (boardLayout[2 * _y + 2 - y][x + 2] != ' '){
			return false;
		}
		else {
			boardLayout[2 * _y + 2 - y][x + 2] = player.getSign();
			return true;
		}
	}

	bool checkBoard(Player player, string &winner){
		int countSigns = 0;
		int countAll = 0;
		for (int i = 2; i <= 2 * _y; i = i + 2){
			for (int j = 4; j <= 2 * _x + 2; j = j + 2){
				countAll++;
				if (boardLayout[i][j] != ' '){
					countSigns++;
				}
				if (boardLayout[i][j] == player.getSign()){
					if (boardLayout[i][j + 2 * (_winReq - 1)] == player.getSign()){
						int counter = 1;
						int k = j + 2 * (_winReq - 1);
						for (; k != j; k = k - 2){
							if (boardLayout[i][k] == player.getSign()){
								counter++;
								if (counter == _winReq){
									winner = player.getName();
									return true;
								}
							}
						}
					}
					if (boardLayout[i + 2 * (_winReq - 1)][j + 2 * (_winReq - 1)] == player.getSign()){
						int counter = 1;
						int k = j + 2 * (_winReq - 1);
						int l = i + 2 * (_winReq - 1);
						for (; k != j && l != i; k = k - 2, l = l - 2){
							if (boardLayout[l][k] == player.getSign()){
								counter++;
								if (counter == _winReq){
									winner = player.getName();
									return true;
								}
							}
						}
					}
					if (boardLayout[i + 2 * (_winReq - 1)][j] == player.getSign()){
						int counter = 1;
						int l = i + 2 * (_winReq - 1);
						for (; l != i; l = l - 2){
							if (boardLayout[l][j] == player.getSign()){
								counter++;
								if (counter == _winReq){
									winner = player.getName();
									return true;
								}
							}
						}
					}
					if (boardLayout[i + 2 * (_winReq - 1)][j - 2 * (_winReq - 1)] == player.getSign()){
						int counter = 1;
						int k = j - 2 * (_winReq - 1);
						int l = i + 2 * (_winReq - 1);
						for (; k != j && l != i; k = k + 2, l = l - 2){
							if (boardLayout[l][k] == player.getSign()){
								counter++;
								if (counter == _winReq){
									winner = player.getName();
									return true;
								}
							}
						}
					}
				}
			}
		}
		if (countSigns == countAll){
			winner = "tie";
			return true;
		}
		return false;
	}

private:
	char _x;
	char _y;
	int _winReq;
	char boardLayout[MAX][MAX];
};

void printAll(Player players[], int i, int numPlayers, bool b, bool b1, int X, int Y, int winReq);
void printHeader();
void printPlayers(Player players[], int playerIndex, int numPlayers);
void printDimensions(bool b, int X, int Y);
void printWinQuota(bool b, int winReq);
int getPlayers(Player (&players)[MAX_PLAYERS]);
void gridDimensions(Player players[MAX_PLAYERS], int numPlayers, int &X, int &Y);
int getWinReq(Player players[MAX_PLAYERS], int numPlayers, int X, int Y, int Max);
void game(Player players[], Board gameBoard, int numPlayers, int X, int Y, int winReq);
int getX(Board gameBoard, Player players[MAX_PLAYERS], Player player, int numPlayers, int winReq, int X, int Y);
int getY(Board gameBoard, Player players[MAX_PLAYERS], Player player, int numPlayers, int winReq, int X, int Y, int _X);
void stringToUpper(string &s);

int main(){
cout << "Hello World";
system("CLS");
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
bool playAgain = true;
while (playAgain == true){
	playAgain = false;
	int numPlayers = 0, X = 0, Y = 0, winReq = 0;
	char ch;
	string winner = "";
	bool gameOver = false;
	Player players[MAX_PLAYERS];

	printAll(players, -1, 0, false, false, 0, 0, 0);

	numPlayers = getPlayers(players);

	gridDimensions(players, numPlayers, X, Y);

	system("CLS");
	printAll(players, numPlayers - 1, numPlayers, true, false, X, Y, 0);

	winReq = getWinReq(players, numPlayers, X, Y, X > Y ? X : Y);

	system("CLS");
	printAll(players, numPlayers - 1, numPlayers, true, true, X, Y, winReq);

	for (int i = 0; i <= 12; i++){
		system("CLS");
		printAll(players, numPlayers - 1, numPlayers, true, true, X, Y, winReq);
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "\nGame ";
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "starts in ";
		cout << 3 - i / 4;
		cout << "... ";
		switch (i % 4){
		case 0:
			cout << "-";
			break;
		case 1:
			cout << "/";
			break;
		case 2:
			cout << "|";
			break;
		case 3:
			cout << "\\";
			break;
		}
	}

	system("CLS");
	printAll(players, numPlayers - 1, numPlayers, true, true, X, Y, winReq);

	Board gameBoard(X, Y, winReq);
	gameBoard.printBoard();

	game(players, gameBoard, numPlayers, X, Y, winReq);

	ch = _getch();
	ch = toupper(ch);

	if (ch == 'Y'){
		playAgain = true;
		system("CLS");
		printAll(players, numPlayers - 1, numPlayers, true, true, X, Y, winReq);
		gameBoard.printBoard();
		system("CLS");
	}
}
	return 0;
}

void printAll(Player players[], int i, int numPlayers, bool b, bool b1, int X, int Y, int winReq) {
	printHeader();
	printPlayers(players, i, numPlayers);
	printDimensions(b, X, Y);
	printWinQuota(b1, winReq);
}

void printHeader(){
	SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "\n----- ";
	SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "Welcome to TicTacToe ver 1.0 ";
	SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "----- \n";
	cout << "----- ";
	SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "Created by @evilbabaroga ";
	SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "----- \n";
	SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << endl;
}

void printPlayers(Player players[], int playerIndex, int numPlayers){
	if (playerIndex == -1){
		cout << "Enter the number of players (max " << MAX_PLAYERS << " ): ";
	}
	else{
		if (playerIndex != -1){
			cout << "Number of players: " << numPlayers << endl << endl;
		}
		for (int i = 0; i <= playerIndex; i++){
			cout << i + 1 << ".) Player: " << players[i].getName() << " ";
			SetConsoleTextAttribute(h, players[i].getColor());
			cout << players[i].getSign();
			SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << endl;
		}
	}
}

void printDimensions(bool b, int X, int Y){
	if (b){
		cout << "\nGrid dimensions: " << X << " x " <<  Y << endl;
	}
}

void printWinQuota(bool b, int winReq){
	if (b){
		cout << "Win quota: " << winReq << endl;
	}
}

int getPlayers(Player (&players)[MAX_PLAYERS]){
	int numPlayers;
	string name;
	char sign;
	bool valid = false;

	while (!valid)
	{
		valid = true;

		cin >> numPlayers;

		if (cin.fail()) {
			cin.clear(); //This corrects the stream.
			cin.ignore(); //This skips the left over stream data.
			cout << "\n  INVALID INPUT! Integers only!\n\n";
			system("PAUSE");
			system("CLS");
			printAll(players, -1, numPlayers, false, false, 0, 0, 0);
			valid = false; //The cin was not an integer so try again.
		}
		else if (numPlayers > 5 || numPlayers < 1){
			cin.clear(); //This corrects the stream.
			cin.ignore(); //This skips the left over stream data.
			cout << "\n  INVALID INPUT! Must be between 1 and 5.\n\n";
			system("PAUSE");
			system("CLS");
			printAll(players, -1, numPlayers, false, false, 0, 0, 0);
			valid = false; //The cin was not an integer so try again.
		}

	}
	system("CLS");
	printAll(players, -2, numPlayers, false, false, 0, 0, 0);

	for (int i = 0; i < numPlayers; i++){
		string check;

		if (i != 0){
			cout << endl;
		}

		cout << "Player " << i + 1 << endl << endl;
		cout << "  Enter name: ";
		valid = false;
		while (!valid)
		{
			valid = true;

			cin >> name;

			if (cin.fail()) {
				cin.clear(); //This corrects the stream.
				cin.ignore(); //This skips the left over stream data.
				cout << "\n  INVALID INPUT! Strings only!\n\n";
				system("PAUSE");
				system("CLS");
				printAll(players, -2, numPlayers, false, false, 0, 0, 0);
				cout << "Player " << i + 1 << endl << endl;
				cout << "  Enter name: ";
				valid = false; //The cin was not an integer so try again.
			}
			else {
				string name_check = name;
				stringToUpper(name_check);
				for (int j = 0; j <= i; j++){
					if (players[j].getName() == name_check){
						cin.clear(); //This corrects the stream.
						cin.ignore(); //This skips the left over stream data.
						cout << "\n Use a different name dork.\n\n";
						system("PAUSE");
						system("CLS");
						if (i == 0){
							printAll(players, -2, numPlayers, false, false, 0, 0, 0);
						}
						else {
							printAll(players, i - 1, numPlayers, false, false, 0, 0, 0);
							cout << endl;
						}
						cout << "Player " << i + 1 << endl << endl;
						cout << "  Enter name: ";
						valid = false; //The cin was not an integer so try again.
					}
				}
			}
		}
		players[i].setName(name);

		system("CLS");
		if (i == 0){
			printAll(players, -2, numPlayers, false, false, 0, 0, 0);
		}
		else {
			printAll(players, i - 1, numPlayers, false, false, 0, 0, 0);
			cout << endl;
		}
		cout << "Player " << i + 1 << endl << endl;
		cout << "  Name: " << players[i].getName() << endl;

		cout << "  Enter sign: ";

		valid = false;
		while (!valid){

			valid = true;

			cin >> check;

			if (check[1] != '\0' || check[0] == '\0' || check[0] == '\n' || check[0] == ' '
				|| check[0] == '1' || check[0] == '2' || check[0] == '3' || check[0] == '4'
				|| check[0] == '5' || check[0] == '6' || check[0] == '7' || check[0] == '8'
				|| check[0] == '9' || check[0] == '|' || check[0] == '-') {
				cout << "\n\n  INVALID INPUT! Chars only!\n\n";
				system("PAUSE");
				system("CLS");
				if (i == 0){
					printAll(players, -2, numPlayers, false, false, 0, 0, 0);
				}
				else {
					printAll(players, i - 1, numPlayers, false, false, 0, 0, 0);
					cout << endl;
				}
				cout << "Player " << i + 1 << endl << endl;
				cout << "  Name: " << players[i].getName() << endl;
				cout << "  Enter sign: ";
				valid = false; //The cin was not an integer so try again.
			}
			else {
				char sign = check[0];
				for (int j = 0; j <= i; j++){
					if ((int)players[j].getSign() == (int)sign){
						cin.clear(); //This corrects the stream.
						cin.ignore(); //This skips the left over stream data.
						cout << "\n\n Use a different symbol dork.\n\n";
						system("PAUSE");
						system("CLS");
						if (i == 0){
							printAll(players, -2, numPlayers, false, false, 0, 0, 0);
						}
						else {
							printAll(players, i - 1, numPlayers, false, false, 0, 0, 0);
							cout << endl;
						}
						cout << "Player " << i + 1 << endl << endl;
						cout << "  Name: " << players[i].getName() << endl;
						cout << "  Enter sign: ";
						valid = false; //The cin was not an integer so try again.
					}
				}
			}
		}

		system("CLS");
		if (i == 0){
			printAll(players, -2, numPlayers, false, false, 0, 0, 0);
		}
		else {
			printAll(players, i - 1, numPlayers, false, false, 0, 0, 0);
			cout << endl;
		}
		cout << "Player " << i + 1 << endl << endl;
		cout << "  Name: " << players[i].getName() << endl;
		cout << "  Sign: " << check[0] << endl;

		cout << "  Enter color (\"help\" for more): ";
		string check1;
		valid = false;
		while (!valid){

			valid = true;

			cin >> check1;
			stringToUpper(check1);

			
			if (check1 == "HELP"){
				cin.clear(); //This corrects the stream.
				cin.ignore(); //This skips the left over stream data.
				cout << "\n Supported colors:";

				SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << " yellow";
				SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
				cout << " magenta";
				SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << " red";
				SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
				cout << " cyan";
				SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				cout << " green";
				SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
				cout << " blue";
				SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				cout << " grey";
				SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);
				cout << " darkgrey";
				SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN);
				cout << " darkyellow";

				SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
				cout << "\n\n";
				system("PAUSE");
				system("CLS");
				if (i == 0){
					printAll(players, -2, numPlayers, false, false, 0, 0, 0);
				}
				else {
					printAll(players, i - 1, numPlayers, false, false, 0, 0, 0);
					cout << endl;
				}
				cout << "Player " << i + 1 << endl << endl;
				cout << "  Name: " << players[i].getName() << endl;
				cout << "  Sign: " << check[0] << endl;
				cout << "  Enter color (\"help\" for more): ";
				valid = false; //The cin was not an integer so try again.
			}
			else if (check1 != "GREEN" && check1 != "YELLOW" && check1 != "MAGENTA" && check1 != "RED" && check1 != "CYAN"
				&& check1 != "BLUE" && check1 != "GREY" && check1 != "DARKGREY" && check1 != "DARKYELLOW") {
				cin.clear(); //This corrects the stream.
				cin.ignore(); //This skips the left over stream data.
				cout << "\n  INVALID INPUT! Enter \"help\" for more.\n\n";
				system("PAUSE");
				system("CLS");
				if (i == 0){
					printAll(players, -2, numPlayers, false, false, 0, 0, 0);
				}
				else {
					printAll(players, i - 1, numPlayers, false, false, 0, 0, 0);
					cout << endl;
				}
				cout << "Player " << i + 1 << endl << endl;
				cout << "  Name: " << players[i].getName() << endl;
				cout << "  Sign: " << check[0] << endl;
				cout << "  Enter color (\"help\" for more): ";
				valid = false; //The cin was not an integer so try again.
			}
		}

		sign = check[0];
		stringToUpper(name);
		players[i].setName(name);
		players[i].setSign(sign);

		if (check1 == "GREEN") players[i].setColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		else if (check1 == "YELLOW") players[i].setColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		else if (check1 == "MAGENTA") players[i].setColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
		else if (check1 == "RED") players[i].setColor(FOREGROUND_INTENSITY | FOREGROUND_RED);
		else if (check1 == "CYAN") players[i].setColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
		else if (check1 == "GREEN") players[i].setColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		else if (check1 == "BLUE") players[i].setColor(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		else if (check1 == "DARKGREY") players[i].setColor(FOREGROUND_INTENSITY);
		else if (check1 == "GREY") players[i].setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		else if (check1 == "DARKYELLOW") players[i].setColor(FOREGROUND_RED | FOREGROUND_GREEN);

		cout << endl;

		system("CLS");
		printAll(players, i, numPlayers, false, false, 0, 0, 0);
	}

	cout << endl;
	return numPlayers;
}

void gridDimensions(Player players[MAX_PLAYERS], int numPlayers, int &X, int &Y){
	bool valid = false;

	cout << "Enter the dimensions of the board (max 9x9):\n\n";

	cout << "  X: ";
	while (!valid)
	{
		valid = true; //Assume the cin will be an integer.
		cin >> X;

		if ((X < 1) || (X > 9)){
			cin.clear(); //This corrects the stream.
			cin.ignore(); //This skips the left over stream data.
			cout << "\nMust be a digit between 1 and 9.\n\n";
			system("PAUSE");
			system("CLS");
			printAll(players, numPlayers - 1, numPlayers, false, false, X, Y, 0);
			cout << "\nEnter the dimensions of the board (max 9x9):\n\n";
			cout << "  X: ";
			valid = false; //The cin was not an integer so try again.
		}
	}

	cout << "  Y: ";
	valid = false;
	while (!valid)
	{
		valid = true; //Assume the cin will be an integer.
		cin >> Y;

		if (Y < 1 || Y > 9){
			cin.clear(); //This corrects the stream.
			cin.ignore(); //This skips the left over stream data.
			cout << "\nMust be a digit between 1 and 9.\n\n";
			system("PAUSE");
			system("CLS");
			printAll(players, numPlayers - 1, numPlayers, false, false, X, Y, 0);
			cout << "\nEnter the dimensions of the board (max 9x9):\n\n";
			cout << "  X: " << X << endl;
			cout << "  Y: ";
			valid = false; //The cin was not an integer so try again.
		}
	}


	cout << endl;
}

int getWinReq(Player players[MAX_PLAYERS], int numPlayers, int X, int Y, int Max){
	int winReq;
	bool valid = false;

	cout << "\nEnter the number of adjacent signs required to win (Max " << Max << "): ";

	while (!valid)
	{
		valid = true; //Assume the cin will be an integer.
		cin >> winReq;

		if (winReq < 1 || winReq > Max){
			cin.clear(); //This corrects the stream.
			cin.ignore(); //This skips the left over stream data.
			cout << "\nMust be between 1 and " << Max << ".\n\n";
			system("PAUSE");
			system("CLS");
			printAll(players, numPlayers - 1, numPlayers, true, false, X, Y, 0);
			cout << "\nEnter the number of adjacent signs required to win (Max " << Max << "): ";
			valid = false; //The cin was not an integer so try again.
		}
	}
	return winReq;
}

void game(Player players[], Board gameBoard, int numPlayers, int X, int Y, int winReq){
	string winner;
	bool gameOver = false;
	bool isGood;

	while (gameOver != true){
		for (int i = 0; i < numPlayers; i++){
			isGood = true;
			while (isGood = true){
				int _X = getX(gameBoard, players, players[i], numPlayers, winReq, X, Y);
				int _Y = getY(gameBoard, players, players[i], numPlayers, winReq, X, Y, _X / 2);
				isGood = gameBoard.applySign(_X, _Y, players[i]);
				if (isGood == false){
					printAll(players, numPlayers - 1, numPlayers, true, true, X, Y, winReq);
					gameBoard.printBoard(players, numPlayers);
					cout << "You can't do that man\n\n";
					continue;
				}
				else{
					break;
				}
			}
			system("CLS");
			printAll(players, numPlayers - 1, numPlayers, true, true, X, Y, winReq);
			gameBoard.printBoard(players, numPlayers);

			gameOver = gameBoard.checkBoard(players[i], winner);
			if (gameOver) break;
		}
	}

	if (winner == "tie"){
		cout << "\nIt's a tie :((((((((((((\n\nFIN\n\n";
	}
	else {
		WORD color;
		for (int i = 0; i < numPlayers; i++){
			if (players[i].getName() == winner){
				cout << "\nThe winner is: --- ";
				SetConsoleTextAttribute(h, players[i].getColor());
				cout << winner;
				SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
				cout << " --- VECNA MU SLAVA !!!\n\nFIN\n\n";
			}
		}
	}
	cout << "Play again? (y/n): ";
}

int getX(Board gameBoard, Player players[MAX_PLAYERS], Player player, int numPlayers, int winReq, int X, int Y){
	int _X;
	bool valid = false;

	cout << player.getName() << " ' ";
	SetConsoleTextAttribute(h, player.getColor());
    cout << player.getSign();
    SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << " ' " << ", enter X coordinate: ";

	while (!valid)
	{
		valid = true; //Assume the cin will be an integer.
		cin >> _X;

		if (_X < 1 || _X > X){
			cin.clear(); //This corrects the stream.
			cin.ignore(); //This skips the left over stream data.
			system("CLS");
			printAll(players, numPlayers - 1, numPlayers, true, true, X, Y, winReq);
			gameBoard.printBoard(players, numPlayers);
			cout << player.getName() << " ' ";
			SetConsoleTextAttribute(h, player.getColor());
			cout << player.getSign();
			SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << " ' " << ", enter X coordinate: ";
			valid = false; //The cin was not an integer so try again.
		}
	}

	return 2 * _X;
}

int getY(Board gameBoard, Player players[MAX_PLAYERS], Player player, int numPlayers, int winReq, int X, int Y, int _X){
	int _Y;
	bool valid = false;

	cout << player.getName() << " ' ";
	SetConsoleTextAttribute(h, player.getColor());
    cout << player.getSign();
    SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << " ' " << ", enter Y coordinate: ";

	while (!valid)
	{
		valid = true; //Assume the cin will be an integer.
		cin >> _Y;

		if (_Y < 1 || _Y > Y) {
			cin.clear(); //This corrects the stream.
			cin.ignore(); //This skips the left over stream data.
			system("CLS");
			printAll(players, numPlayers - 1, numPlayers, true, true, X, Y, winReq);
			gameBoard.printBoard(players, numPlayers);
			cout << player.getName() << " ' ";
			SetConsoleTextAttribute(h, player.getColor());
			cout << player.getSign();
			SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << " ' " << ", enter Y coordinate: " << _X << endl;
			valid = false; //The cin was not an integer so try again.
		}
	}
	system("CLS");

	return 2 * _Y;
}

void stringToUpper(string &s){
	for (unsigned int l = 0; l < s.length(); l++){
		s[l] = toupper(s[l]);
	}
}