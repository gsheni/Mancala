/*
 * Gaurav Sheni
 * CSC 371
 * Mancala - Program 1
 * September 9, 2014
 */
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <limits.h> // for int max and int min
#define MIN 1
#define MAX 0
//#define PRINT //for fun
#include <ctime>
#include <cmath>
#include <stdio.h>
#include <strings.h>
#include <algorithm>
#include <stdlib.h>
#include <ctype.h>
#include <cctype>
using namespace std;
void printboard(int board[]); // for printing the mancala board
int askforselection(int board[]); //asking the human to pick a choice of bin
bool checkboard(int board[]); // checking the board to make sure that it isn't over
bool takeMove(int board[], int spotinarray, int human_or_comp); //for actually making the move
int findacross(int ending); // find the bin number across
bool checkforone(int board[]); // check if there is only one move possible.
int findone(int board[]); // find the one possible move.
typedef struct move{ //a struct for holding the score of the board and the bin location of the causing bin movement
	int binNum;
	int score;
} Move;
Move minmax(int board[], int d, int maxD, int minOrMax, int alpha, int beta); //mini-max function with alpha, beta pruning.
int computerChooseMove(int board[], int maxD);// allow the computer to make a move.
int evalFunction(int board[], int minOrMax); // Evaluate the board based on who is currently playing.
int checkform(int board[], int choice);


int main(){
    srand(time(0)); // to get the time for the random number generator
    int m;// integer for move bin
    int turn;// for switching turns.
	int selected =  rand() % 2; //random number generator between 0 and 1
	cout << "Heads or tails?" << endl;
	cout << "For heads, enter 1" << endl;
	cout << "For tails, enter 0" << endl;
	int input; //get input
	cin >> input;
	while( input > 1 || input < 0) //make sure the input is valid. either 1 or 0
	{
		cout << "Invalid Input" << endl;
		cout << "Try Again" << endl;
		cin >> input;
	}
	if(selected == input){//if the match their input with the random number, then human goes first
		turn = 0;
	}
	else{ // comp goes first.
		turn = 1;
	}
	int maxDepth = 2;// how deep the searching for the best move should go.
	cout << "What would you like the depth to be?"<< endl;
	cout << "Might I suggest 4 or 5?"<< endl;
	cin >> maxDepth;
	while(maxDepth <= 0 ){ //make sure input for maxdepth is a valid number.
		cout << "Please select a value greater than 0 and less than 11." << endl;
		cin >> maxDepth;
	}
	int choiceofmove = -5; //for holding human move.
	int board[14] = {4,4,4,4,4,4,0,4,4,4,4,4,4,0}; //Initialize board with filled pieices.
	bool gameOver = false; // to see if the game is over, by looking if either top or bottom bins are empty.
	bool onlyone = false; // to see if there is only one move possible.
	bool doesntmatter = false;//not important for takingthemove, but used in eval function.
	int one;//incase there is only one move, this is to store the location of that one move.
	while (!gameOver){

		if (turn == MIN){// computer move
			//in order to make the computer more efficient, if there is only one move possible
			//there is not need to use minimax to find the move.
			onlyone = checkforone(board);
			if(onlyone){
				cout << "Only one move possible so forced to make" << endl;
				one = findone(board);//find location of move.
				m = one;//set the found location as the move.
			}
			else{
				m = computerChooseMove(board, maxDepth);//otherwise use the minmax function to find the best move, given a certain depth.
			}
			cout << "Computer moved at "<< m << ", resulting in board below "<< endl;
			doesntmatter = takeMove(board, m, MAX);
			printboard(board); //print the board.
			gameOver=checkboard(board); //see if the game is over
			if(gameOver){
				break;
			}
			else{
				turn = 0; //switch turn so that the human can go.
			}
		}
		else{ //human move.
			cout << "Your Move" << endl;
			printboard(board);
			choiceofmove = askforselection(board); //prompt user for legal move
			doesntmatter = takeMove(board,choiceofmove,MIN);
			printboard(board); //print the board.
			gameOver=checkboard(board); //see if the game is over
			if(gameOver){
				break;
			}
			else{
				turn = 1;
			}
		}
	}
	cout << "Game is over" << endl;
	if (board[6] > board[13]){
		cout << "Human wins!" << endl;
	}
	else if (board[13] > board[6]){
		cout << "Computer wins, as expected." << endl;
	}
	else {
		cout << "Game is a tie!" << endl;// incase there is a tie.
	}
	printboard(board);//print resulting board.
}

bool takeMove(int board[], int choice, int minOrmax){
	int avoid;//location to avoid, the other person's mancala
	if (MAX == minOrmax){ //comp playing
		avoid = 6;
	}
	else {//user playing
		avoid = 13;
	}
	int picked = board[choice]; //pick up the marbles and store them
	int next = choice + 1; //find the next bin number
	if(next == avoid){ //if this has to be avoided, be sure to change it
		if (avoid == 13){
			next = 0; //avoid comp mancala for human
		}
		else if( avoid == 6){
			next = 7;//avoid human mancala for comp
		}
	}
	board[choice] = 0; //empty bin just picked
	bool changed = false;//if the for loop runs into 13 or 6, this make sure that it doesn't increment twice.
	for( ; picked > 0 ; picked--){ //as long as the number of marbles picked up is less than 0 and decreasing with every iteration
		if(next == avoid){//after each increment, this check to make sure that the next bin number is not an avoider
			if (avoid == 13){
				changed = true; //next is where 1 marble will be placed. changed is true because it avoids incrementing twice
				next = 0; //since we are avoiding 13, the next bin is 0
			}
			else {
				changed = true; //next is where 1 marble will be placed. changed is true because it avoids incrementing twice
				next = 7;//since we are avoiding 6, the next bin is 7
			}
		}
		int currentinnext = board[next]; //get the current number of marbles in the next bin to be added 1
		board[next] = currentinnext + 1 ; // add one to the value of the marbles in the next bin
		int test = picked - 1; //to see if there will be no more marbles left to drop.
		if (test >= 0){
			if(changed){
				next++; //increment to next
				if(next == 14){ //make sure that the array loops
					next = 0;
				}
			}
			else if (picked == 1 && currentinnext == 0){
					break; //if last has only one to be dropped.
			}
			else{
				next++;
				if(next == 14){
					next = 0;
				}
			}
		}
	}
	int ending = next-1;// location of ending bin
	if (ending == -1){ // incase it ends after looping array
		ending = 13;
	}
	else if (ending == 7){
		ending = 6;
	}
	if (MAX == minOrmax && ending == 6){ //this is a special condition that had to be hardcoded
		ending = ending + 1;
	}
	if (MAX == minOrmax){ //Increment ending to be used later
		ending = ending + 1; //for comp
	}
	if (MIN == minOrmax){//Increment ending to be used later
		ending = ending + 1; //for human
	}
	if(ending != 13 || ending != 6){ // make sure that it doesn't find across if landed in mancala

		if (MIN == minOrmax && board[ending] == 1 && ending > -1 && ending < 6){ // for human
				int across = findacross(ending); // find across slot
				if (board[across] > 0){
					board[6] = board[6] + 1 + board[across]; // take the piece just landed
					board[ending] = 0;//empty bin on human side
					board[across] = 0; //empty bin across
					return true;//return true if across value was captured. used in eval
				}
				else{
					return false;
				}
		}
		if (MAX == minOrmax && board[ending] == 1 && ending > 6 && ending < 13){ // for comp
				int across = findacross(ending); // find across slot
				if (board[across] > 0){
					board[13] = board[13] + 1 + board[across]; // take the piece just landed
					board[ending] = 0; //empty bin on comp side
					board[across] = 0; //empty bin across
					return true;//return true if across value was captured. used in eval
				}
				else{
					return false;
				}
		}
	}
	else{
		return false;
	}
	return false;
}
Move minmax(int board[], int d, int maxD, int minOrMax, int alpha, int beta) {
//minOrMax is MAX if the computer is the player and MIN if the human is the player

	Move mTemp, m;
	int board2[14];//no need to fill board with values, will be changed later on
	bool check;//for checking if one side is empty
	bool doesntmatter;//return bool from takemove, for eval function and doens't matter here.
	check = checkboard(board);//check if one side is emtpy.
#ifdef PRINT
	cout << "Currently at depth of " << d << endl;
#endif
	if (check){ //if (one side has all empty bins)
		//return INT_MAX as the score if MAX is playing and INT_MIN if MIN is playing; bin number not important
		if (MIN == minOrMax){ // Computer is playing
			m.score = INT_MIN;
			m.binNum = -1; //not important.
			return m;
		}
		else if (MAX == minOrMax){
			m.score = INT_MAX;
			m.binNum = -1; //not important
			return m;
		}
	}
	else  if (d == maxD)  {
#ifdef PRINT
		cout << "At the max depth of "<< d << " so, evaluating now." << endl;
#endif
		m.score = evalFunction(board, minOrMax); //evaluate the current board state and give a score.
		m.binNum = -1; //doesnt matter
		//return a struct with the score; bin number not important
		return m;
	}
	else {
		if (minOrMax == MIN) { // MINIMizing player
			m.score= INT_MAX;
			m.binNum = -1;
			for (int i = 0; i <= 5; i++) { //user turn so only go through 6 bins on user side.
				if (board[i] != 0) {
					//Copy board into board2
					//for loop to copy board
					for(int j = 0; j < 14 ; j++){
						board2[j] = board[j];
					}
					doesntmatter = takeMove(board, i, minOrMax);  //change board state to next tentative move
#ifdef PRINT
					cout << "MIN is recursively calling minmax, with an alpha of " << alpha << " and m.score (beta) of " << m.score << endl;
#endif
					mTemp = minmax(board, d+1, maxD, MAX, alpha, m.score); //recursive call minmax with m.score(as beta) and alpha
					if( m.score <= alpha){  //if m.score is less than alpha
#ifdef PRINT
						cout << "m.score, which is " << m.score <<" is less than or equal to alpha, which is " << alpha << endl;
						cout << "**********So ALPHA cutting off, PRUNING, at MIN*********" << endl; //no need to search any further
#endif
						break; //so break out of for loop
					}
					if (mTemp.score < m.score) { //
#ifdef PRINT
						cout << "m.score, which is " << m.score <<" is greater than mTemp.score, which is " << mTemp.score << endl;
						cout << "Setting a new BIN number to " << i << endl;
#endif
						m.score = mTemp.score; //if the found score it less than the kept score, so update score kept.
						m.binNum = i; //bin number will be current index
						//less than because trying to minimize
					}

					//copy board2 back into board
					for(int k = 0; k < 14 ; k++){
						board[k] = board2[k];
					}
				}
			}
			return m;
		}
		else if (minOrMax == MAX) //comp
			{
			//Do this the same way as the previous case, changing the necessary variables and values to make it the flip-side, logically
			m.score = INT_MIN;
			m.binNum = -1;
			for (int i = 7; i <= 12; i++) {//comp turn so only go through 6 bins on user side.
				if (board[i] != 0) {
					for(int j = 0; j < 14 ; j++){
						board2[j] = board[j];
					}
					doesntmatter = takeMove(board, i, minOrMax);  //change board state to next tentative move
#ifdef PRINT
					cout << "MAX is recursively calling minmax, with an beta of " << beta << " and m.score (alpha) of " << m.score << endl;
#endif
					mTemp = minmax(board, d+1, maxD, MIN, m.score, beta); //recursive call minmax with m.score(as alpha) and beta
					if(m.score > beta){
#ifdef PRINT
						cout << "m.score, which is " << m.score <<" is greater than beta, which is " << beta << endl;
						cout << "**********BETA cutting off, PRUNING, at MAX*********" << endl; //no need to search any further
#endif
						break; //so break out of for loop
					}
					if (mTemp.score >= m.score) {
#ifdef PRINT
						cout << "m.score, which is " << m.score <<" is less than or equal to mTemp.score, which is " << mTemp.score << endl;
						cout << "Setting a new BIN number to " << i << endl;
#endif
						m.score = mTemp.score;//if the found score it greater than the kept score, so update score kept.
						m.binNum = i;//bin number will be current index
						//greater than because trying to maximize
					}
					///////////

					//copy board2 back into board
					for(int k = 0; k < 14; k++){
						board[k] = board2[k];
					}
				}
			}
			return m;
		}	//end of else if
	}	//end of else
}

int computerChooseMove(int board[], int maxD)
{
	Move m;
	int alpha = INT_MIN; // alpha = -infinity, from book for initial call
	int beta = INT_MAX; // beta = infinity, from book for initial call
#ifdef PRINT
	cout << "Initial Call for minmax just occurred" << endl;
#endif
	m = minmax(board, 0, maxD, MAX, alpha, beta); //initial call
	return m.binNum; //found the value of best bin
}

int evalFunction(int board[], int minOrMax){ //minORMax arent really needed since we are using evalfunction to benefit comp
	int board2[14];//for getting value lets copy board so it isn't messed up.
	bool checkempty;//check if it results in an empty bin location, if so then add the value of that.
	for(int j = 0; j < 14 ; j++){
		board2[j] = board[j]; //copy board 2
	}
	//get all the pieces on the human side, including mancala
	float humanside = (board[0] + board[1] + board[2] + board[3] + board[4] + board[5])*0.45; //multiply by a constant
	//I am multiplying by 0.45 because it gives more variation.
	//this is also why floating point numbers are used, later converted to an int
	float humanmancala = (board[6]) * 3.45; //multiple by a higher constant because these are guaranteed pieces, since in mancala already
	float human = humanside + humanmancala; // find total for human pieces
	//get all the pieces on the comp side, including mancala
	float compside = (board[7] + board[8] + board[9] + board[10] + board[11] + board[12])*0.45; //multiply by a constant
	//I am multiplying by 0.45 because it gives more variation.
	//this is also why floating point numbers are used, later converted to an int
	float compmancala = (board[13]) * 3.45;//multiple by a higher constant because these are guaranteed pieces, since in mancala already
	float comp = compside + compmancala; //find total for comp pieces
	float difference_board = comp - human; 	//find the difference between the two, negative doesn't make a difference.
	int valueofboard = int(difference_board + 0.5); //this basically rounds the floating point number to the nearest integer.
	for (int i = 0; i <= 5; i++) { //for each possible move for human
	checkempty = takeMove(board2, i, minOrMax); //see if you can land in an empty bin
		if(checkempty){ //if you can land in an empty bin
			int acrosslocation = findacross(i); //find the location of that
			int acrosspieces = board2[acrosslocation]; //find how many pieces are across
			//add the number of pieces across plus the one on your side to the vale of the board
			valueofboard = valueofboard + ((acrosspieces*0.5)*-1);//since we want the comp to not have to go to this state, we multiple by -1, adding negative
		}
	}
	for (int i = 7; i <= 12; i++) { //for each possible move for comp
	checkempty = takeMove(board2, i, minOrMax); //see if you can land in an empty bin
		if(checkempty){//if you can land in an empty bin
			int acrosslocation = findacross(i);//find the location of that
			int acrosspieces = board2[acrosslocation];//find how many pieces are across
			//add the number of pieces across plus the one on your side to the vale of the board
			valueofboard = valueofboard + ((acrosspieces*0.5)); //we want comp to go to this state, so multiply by constant only
		}
	}
	return valueofboard; //regardless of if you can land in an empty bin, give back the value of the board.
}


bool checkboard(int board[]){
	int bottomnumempty = 0; //see if bottom are empty
	for (int i = 0; i < 6;i++){
		if(board[i] == 0){
			bottomnumempty++; //check each one individual to see if 0
		}
	}
	if (bottomnumempty == 6){
		// if bottom are empty, then take all on comp side and deposit into comp mancala
		board[13] = board[13] + board[12] +board[11] +board[10] +board[9] + board[8] +board[7];
		for(int i = 7; i < 13; i++){
			board[i] = 0;//empty all on comp side
		}
		return true; //yes, its empty
	}
	int topnumempty = 0;
	for (int i = 7; i < 13;i++){//see if top are empty
		if(board[i] == 0){
			topnumempty++;//check each one individual to see if 0
		}
	}
	if (topnumempty == 6){
		// if top are empty, then take all on human side and deposit into human mancala
		board[6] = board[6] + board[5] +board[4] +board[3] +board[2] + board[1] +board[0];
		for(int i = 0; i < 6; i++){
			board[i] = 0;//empty all on human side
		}
		return true;//yes, its empty
	}
	return false;//no, its not empty, top 6 or bottom 6
}
bool checkforone(int board[]){
	int check = 0;
	for (int i = 7; i <= 12; i++) {
		if(board[i] > 0){
			check++; //for each possible move add one
		}
	}
	if(check == 1){ //if there is only one move possible.
		return true; //return true
	}
	else{
		return false; //else return false.
	}
}
int findone(int board[]){
	int check = 0;
	for (int i = 7; i <= 12; i++) { //go through all comp bins
		if(board[i] > 0){
			check = i ; //find the bin location of the one possible move for comp
			return check;
		}
	}
	return check;
}
int findacross(int ending){ //find across bin
	int across = -555;

	if (ending == 0){
		across = 12;
	}
	else if (ending == 1){
		across = 11;
	}
	else if (ending == 2){
		across = 10;
	}
	else if (ending == 3){
		across = 9;
	}
	else if (ending == 4){
		across = 8;
	}
	else if (ending == 5){
		across = 7;
	}
	else if (ending == 7){
		across = 5;
	}
	else if (ending == 8){
		across = 4;
	}
	else if (ending == 9){
		across = 3;
	}
	else if (ending == 10){
		across = 2;
	}
	else if (ending == 11){
		across = 1;
	}
	else if (ending == 12){
		across = 0;
	}
	return across;
}
int askforselection(int board[]){
	int selection;
	cout << "What move would you like to make? Your mancala is to the right." << endl;
	cout << "Please enter either 0,1,2,3,4 or 5." << endl;
	cin >> selection;
	while (selection > 5 || selection < 0 || board[selection]==0){ //make sure valid selection
		cout << "Wrong selection. Pit is either empty or out of range" << endl;
		cout << "What move would you like to make?" << endl;
		cout << "Please enter the slot number " << endl;
		cin >> selection;
	}
	return selection;
}

void printboard(int board[]){
	cout << "       |  12  |  11  |  10  |   9  |   8  |   7  |" << endl;
	cout << "________________________________________________________" << endl;
	cout << "|" << "      | (" << std::setfill('0') << std::setw(2) << board[12] << ") | " ;
	cout << "(" << std::setfill('0') << std::setw(2)<< board[11] ;
	cout << ") | " << "(" << std::setfill('0') << std::setw(2)<<  board[10] << ") | ";
	cout << "(" << std::setfill('0') << std::setw(2)<<  board[9] << ") | " ;
	cout << "(" << std::setfill('0') << std::setw(2)<<  board[8] << ") | " ;
	cout <<"(" << std::setfill('0') << std::setw(2) << board[7] << ") |      |" << endl;

	cout << "|      |‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|      |" << endl;

	cout << "|  " << std::setfill('0') << std::setw(2) << board[13] << "  |";
	cout << "                                         ";
	cout << "|  " << std::setfill('0') << std::setw(2) << board[6] << "  |" << endl;
	cout << "|      |_________________________________________|      |" << endl;

	cout << "|" << "      | (" << std::setfill('0') << std::setw(2) << board[0] << ") | " ;
	cout << "(" << std::setfill('0') << std::setw(2)<< board[1] ;
	cout << ") | " << "(" << std::setfill('0') << std::setw(2)<<  board[2] << ") | ";
	cout << "(" << std::setfill('0') << std::setw(2)<<  board[3] << ") | " ;
	cout << "(" << std::setfill('0') << std::setw(2)<<  board[4] << ") | " ;
	cout <<"(" << std::setfill('0') << std::setw(2) << board[5] << ") |	|" << endl;

	cout << "‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾" <<endl;
	cout << "       |   0  |   1  |   2  |   3  |   4  |   5  |" << endl;
	cout << "\n";
}

