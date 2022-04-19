#include <iostream>
#include <fstream>
#include <iomanip>
#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>
#include <thread>
using namespace std;
HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);//fontsize management to winner screen

// Variables
string checker[24] = { "2Y","  ","  ","  ","  ","5X",
                     "  ","3X","  ","  ","  ","5Y",
                     "5X","  ","  ","  ","3Y","  ",
                     "5Y","  ","  ","  ","  ","2X"}; //start from L1 to L2- Starting position
int roundInt = 1;
int broken_checker[2] = { 0,0 };
int dice_roll[2] = { 6,6 };
char turn,opponent;

//Gamification
void fontsize(int, int); 
void music3(); void music2(); void music1(); void music();
void table();

//File Management
void logFile();
void saveData();
void loadData();

//Menu Components
void menu();
void loadGame();
void newGame();

// Game Mechanism
void roundTurn();
void rollDice();
void moveChecker(int move_count,int *used_roll);
void moveControl(int move);
void brokenCheckerReturn(int* move_count, int* used_roll);
int furtherChecker();
int coordinate(string move);
void decrease(int move);
void finish();

//Game Decision
bool error(int move,int dice_num);
bool isAllCheckersInHomeBoard();
bool isGameEnd();

int main()
{
    system("color fa");
    menu();
    return 0;
}

void music3() { //error sound
    Beep(1568, 200);
    Beep(1175, 1000);
}
void music2() { // stone sound
    Beep(493, 300);
    Beep(698, 150);
    Beep(493, 300);
    Beep(587, 200);

}
void music1() { //main menu music
    Beep(440, 200);
    Beep(440, 200);
    Beep(329, 300);
    Beep(329, 200);
    Beep(349, 200);
    Beep(349, 150);
    Beep(329, 400);
    Sleep(500);
    Beep(293, 300);
    Beep(293, 300);
    Beep(349, 250);
    Beep(349, 150);
    Beep(329, 700);
}
void music() { //Game over music
    Beep(1568, 200);
    Beep(1568, 200);
    Beep(1568, 200);
    Beep(1245, 1400);
    Beep(1397, 200);
    Beep(1397, 200);
    Beep(1397, 200);
    Beep(1175, 1400);

}


void menu()
{
    thread t(music1);
    system("cls");
    fontsize(20, 20);
    cout<<"\t**********THE BACKGAMMON GAME*************";

    cout<<endl<<endl<<"\t\t\t  MENU\t\t"<<endl<<endl;
    cout<<"\t1.New Game   \t2.Load Game   \t3.Exit ";
    
    switch (_getch()){
    case '1':
        newGame();
        break;
    case '2': 
        loadGame();
        break;
    case '3': exit(0);
        break;
    case 27: exit(0);
        break;
    
    default:
        system("cls");
        cout<<endl<<"Enter 1 to 6 only";
        cout<<endl<<" Enter any key";
        _getch();
        menu();
    }
    t.join();
}
void fontsize(int a, int b) { 
    PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();
    lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
    lpConsoleCurrentFontEx->dwFontSize.X = a;
    lpConsoleCurrentFontEx->dwFontSize.Y = b;
    SetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
}
void newGame() {
    thread t(music1);
    t.join();
    system("cls");
    
    startroll:
    Sleep(500);
    rollDice();
    cout <<endl<< "X's start roll:" << dice_roll[0]<<endl;
    Sleep(1000);
    cout << "Y's start roll:" << dice_roll[1] << endl;
    
    if (dice_roll[0] > dice_roll[1]) {
        turn = 'X';
        opponent = 'Y';
    }
    else if (dice_roll[1] > dice_roll[0]) {
        turn = 'Y';
        opponent = 'X';
    }
    else {
        cout << "Roll's equal...Rolls once again ...";
        goto startroll;
    }
    cout << turn << " is starting the Game!";

    cout<<endl<<" Enter any key to start";
    
    ofstream outfile;
    outfile.open("log.dat");
    outfile << dice_roll[0] << endl << dice_roll[1] << endl;
    outfile.close();

    _getch();

    rollDice();
    roundTurn();

}

void loadGame() {
    
    loadData();
    roundTurn();
}

void table() { //Printing the Table

    ofstream outfile;
    outfile.open("Table.dat");
    

    //PRINTING THE TABLE
    const char DV = 178;
    string length(78, DV);

    const char DVout = '#';
    string lengthOut(78, DVout);
    
    /// ////////////////////////////////
    cout << endl << "\t" << turn << "'s TURN";
    cout <<endl<< "   ";

    outfile << endl << "\t" << roundInt << ".TURN AND " << turn << "'s PLAY";
    outfile << endl << "   ";
    for (int i = 65; i < 77; i++) {
        cout <<DV<<"  "<<(char)i<<"  ";
        outfile<< DVout << "  " << (char)i << "  ";
        if (i == 70) {
            outfile << DVout << DVout;
            cout << DV << DV;
        }
    }
    outfile << DVout << endl;
    cout <<DV<< endl;
    outfile << lengthOut;
    cout << length;
    
  ////////////////////////////////
   
    cout << endl << "   ";
    outfile << endl << "   ";
    for (int i = 65; i < 77; i++) {
        cout << DV << "     ";
        outfile << DVout << "     ";
        if (i == 70) {
            outfile << DVout << DVout;
            cout << DV << DV;
        }
    }
    outfile << DVout << endl;
    cout <<DV<< endl;
    
    ////////////////////////
  
    cout << " 1 ";
    outfile << " 1 ";
    for (int i = 11; i >=0; i--) {
        cout << DV << " "+ checker[i]+"  ";
        outfile << DVout << " " + checker[i] + "  ";
        if (i == 6) {
            cout << DV << DV;
            outfile << DVout << DVout;
        }
    }
    cout << DV << endl;
    outfile << DVout << endl;
    
    //////////////////////////
   
    cout << "   ";
    outfile << "   ";
    for (int i = 65; i < 77; i++) {
        cout << DV << "     ";
        outfile<< DVout << "     ";
        if (i == 70) {
            cout << DV << DV; 
            outfile << DVout << DVout;
        }
    }
    cout << DV << endl;
    outfile << DVout << endl;

    cout << length;
    outfile << lengthOut;
    
    /// //////////////////////////////////////////
   
    cout <<endl<< "   " << DV << "     BROKEN      " << DV << "                 " << DV << DV <<DV<< "                 " <<DV<< "     BROKEN      " << DV;
    cout << endl << "   " << DV << "    FLAKE OF     " << DV << "  DICE ONE= "<<dice_roll[0]<<"    " << DV << DV << DV << "  DICE TWO= "<<dice_roll[1]<<"    " << DV << "    FLAKE OF     " << DV;
    cout << endl << "   " << DV << "      X= "<<broken_checker[0]<<"       " << DV << "                 " << DV << DV << DV << "                 " << DV << "      Y= " << broken_checker[1] << "       ";
    cout << DV << endl;
    outfile << endl << "   " << DVout << "     BROKEN      " << DVout << "                 " << DVout << DVout << DVout << "                 " << DVout << "     BROKEN      " << DVout;
    outfile << endl << "   " << DVout << "    FLAKE OF     " << DVout << "  DICE ONE= " << dice_roll[0] << "    " << DVout << DVout << DVout << "  DICE TWO= " << dice_roll[1] << "    " << DVout << "    FLAKE OF     " << DVout;
    outfile << endl << "   " << DVout << "      X= " << broken_checker[0] << "       " << DVout << "                 " << DVout << DVout << DVout << "                 " << DVout << "      Y= " << broken_checker[1] << "       ";
    outfile << DVout << endl;
    /// //////////////////////////////////////////
    cout << length;
    outfile << lengthOut;
    ////////////////////////////////

    cout << endl << "   ";
    outfile << endl << "   ";
    for (int i = 65; i < 77; i++) {
        cout << DV << "     ";
        outfile << DVout << "     ";
        if (i == 70) {
            cout << DV << DV; 
            outfile << DVout << DVout;
        }
    }
    cout << DV << endl;
    outfile << DVout << endl;

    ////////////////////////

    cout << " 2 ";
    outfile << " 2 ";
    for (int i = 12; i<24; i++) {
        cout << DV << " " + checker[i] + "  ";
        outfile << DVout << " " + checker[i] + "  ";
        if (i == 17) { 
            cout << DV << DV; 
            outfile << DVout << DVout;
        }
    }
    cout << DV << endl;
    outfile << DVout << endl;
    ////////////////////////////////

    cout << "   ";
    outfile << "   ";
    for (int i = 65; i < 77; i++) {
        cout << DV << "     ";
        outfile << DVout << "     ";
        if (i == 70) {
            cout << DV << DV;
            outfile << DVout << DVout;
        }
    }

    cout << DV << endl;
    cout << length;
    outfile << DVout << endl;
    outfile << lengthOut;
    outfile << endl << endl << endl;
    outfile.close();


}

void rollDice() {
    srand(time(NULL));
    dice_roll[0] = rand() % 6 + 1;
    dice_roll[1] = rand() % 6 + 1;
}

void roundTurn() {
    int move_count = 2;
    int used_roll=-1;
    system("cls");
    saveData();

    //set board color for each player
    if (turn == 'X') {
        system("color fa");
    }
    else if (turn == 'Y') {
        system("color f4");
    }
    
    
    
    logFile();
    table();

    
    
    cout << endl << "\t Dice rolls are:" << dice_roll[0] <<" and "<< dice_roll[1];
    
    if (dice_roll[0] == dice_roll[1]) { move_count=4; }
    for (int i = 0; i < move_count; i++)
    {
        
        if (broken_checker[turn - 'X'] > 0) { brokenCheckerReturn(&move_count, &used_roll); }
        
        else if(!isGameEnd()){
            moveChecker(i,&used_roll);   
        }
        system("cls");
        table();
        
    }
    
    
    Sleep(1000);
    
    
    if (isGameEnd()) {
        finish();
    }
    else {
        rollDice();
        roundInt++;
        char temp = turn;
        turn = opponent;
        opponent = temp;
        roundTurn();
    }
}

void logFile() { //writes player's datas in logfile for each round 
    
    ofstream outfile("log.dat", std::ofstream::app);
    outfile <<roundInt<<":"<< turn << " " << dice_roll[0] << " " << dice_roll[1]<<endl;

    outfile.close();
}

void loadData() { //set variables 
    string data;
    ifstream infile;
    infile.open("load.dat");
    for (int i = 0; i < 24; i++) {
        getline(infile, data, '\n');
        checker[i] = data;
    }
    infile >> turn;
    infile >> opponent;
    infile >> broken_checker[0] ;
    infile >> broken_checker[1] ;
    infile >> dice_roll[0] ;
    infile >> dice_roll[1] ;
    infile >> roundInt ;
    infile.close();

}

void saveData() { //writes variables in datafile
    ofstream outfile;
    outfile.open("load.dat");
    for (int i = 0; i < 24; i++) {
        outfile << checker[i] << endl;
    }
    outfile << turn << endl;
    outfile << opponent << endl;
    outfile << broken_checker[0] << endl;
    outfile << broken_checker[1] << endl;
    outfile << dice_roll[0] << endl;
    outfile << dice_roll[1] << endl;
    outfile << roundInt << endl;
    outfile.close();
}

int coordinate(string move) {// string to int for array declaration
    if (move == "L2") return 23;
    else if (move == "K2") return 22;
    else if (move == "J2") return 21;
    else if (move == "I2") return 20;
    else if (move == "H2") return 19;
    else if (move == "G2") return 18;
    else if (move == "F2") return 17;
    else if (move == "E2") return 16;
    else if (move == "D2") return 15;
    else if (move == "C2") return 14;
    else if (move == "B2") return 13;
    else if (move == "A2") return 12;
    else if (move == "A1") return 11;
    else if (move == "B1") return 10;
    else if (move == "C1") return 9;
    else if (move == "D1") return 8;
    else if (move == "E1") return 7;
    else if (move == "F1") return 6;
    else if (move == "G1") return 5;
    else if (move == "H1") return 4;
    else if (move == "I1") return 3;
    else if (move == "J1") return 2;
    else if (move == "K1") return 1;
    else if (move == "L1") return 0;
    else return -1;
}

void brokenCheckerReturn(int *move_count,int *used_roll) {
    int dest1,dest2;
    cout << endl;
    if (turn == 'X') {
        dest1 = 24 - dice_roll[0];
        dest2 = 24 - dice_roll[1];
        
    }
    else if (turn == 'Y') {
        dest1 = dice_roll[0] - 1;
        dest2 = dice_roll[1] - 1;
    }
    if ((checker[dest1][0] > '1' && checker[dest1][1] == opponent) && (checker[dest2][0] > '1' && checker[dest2][1] == opponent)) {
        cout << "There is no possible move"<<endl<<"Game is resuming";
        Sleep(1000);
        cout << ".";
        Sleep(1000);
        cout << ".";
        Sleep(1000);
        cout << ".";
        *move_count = 0;
    }
    //If there is a forced move
    else if (checker[dest1][0] > '1' && checker[dest1][1] == opponent) {
        moveControl(dest2);
        broken_checker[turn - 'X']--;
        *used_roll = 1;
    }
    else if (checker[dest2][0] > '1' && checker[dest2][1] == opponent) {
        moveControl(dest1);
        broken_checker[turn - 'X']--;
        *used_roll = 0;
    }

    else if(dice_roll[0]!= dice_roll[1]&& broken_checker[turn-'X']==1) {
        cout << "Select dice for to enter the broken Checker(1-2)";
        switch (_getch())
        {
        case '1':
            moveControl(dest1);
            broken_checker[turn - 'X']--;
            *used_roll = 0;
            break;
        case '2':
            moveControl(dest2);
            broken_checker[turn - 'X']--;
            *used_roll = 1;
            break;

        
        default:
            cout<<endl<<"Enter 1 or 2 only";
            cout<<endl<<" Enter any key";
            _getch();
            break;
        }
        
    }
    else if (dice_roll[0] == dice_roll[1] && broken_checker[turn - 'X'] == 1) {
        moveControl(dest2);
        broken_checker[turn - 'X']--;
    }
    else {
        moveControl(dest1);
        broken_checker[turn - 'X']--;
        moveControl(dest2);
        broken_checker[turn - 'X']--;
    }

   
}

void moveChecker(int dice_num,int *used_roll){
    
    string move;
    int move_num,dest;
    if (dice_roll[0] != dice_roll[1]&& *used_roll==-1) {
        dice_select:
        cout << endl << "\t Select dice for move checker(1-2)";
        switch (_getch())
        {
        case '1':
            dice_num = 0;
            *used_roll = 0;
            break;
        case '2':
            dice_num = 1;
            *used_roll = 1;
            break;
        default:
            cout << endl << "Enter 1 or 2 only";
            cout << endl << " Enter any key";
            _getch();
            goto dice_select;
            break;
        }
    }
    else if (*used_roll != -1) {
        if (*used_roll == 0) dice_num = 1;
        else if (*used_roll == 1)dice_num = 0;
    }
    else if(dice_num>1)
        dice_num -= 2;
    
    movelabel:
    cout << endl << "\t Select checkers bar to move "<<dice_roll[dice_num]<<"(e.g. E1):";
    cin >> move;
    
    move_num = coordinate(move);
    
    dest = move_num + dice_roll[dice_num] - (2 * dice_roll[dice_num] * ('Y' - turn));

    if (isAllCheckersInHomeBoard()) {
        int dice=furtherChecker(); 
        if (dice < dice_roll[dice_num]) {
            dest = move_num + dice - (2 * dice * ('Y' - turn));
            
        }
    }

    

    if (error(move_num, dest)) {
        thread x(music3);
        x.join();
        goto movelabel;
    }
       
    
    decrease(move_num);
    thread t(music2);
    moveControl(dest);
    t.join();
}

void moveControl(int dest) {
    if (isAllCheckersInHomeBoard) {
        if (turn == 'X'&&dest<0) {
            return;
        }
        if (turn == 'Y' && dest >23) {
            return;
        }
    }
    if (checker[dest][0] == '1' && checker[dest][1] == opponent) {
        cout << opponent << "'s checker has been broken";
        checker[dest][1] = turn;
        broken_checker[opponent - 'X']++;

    }
    else if (checker[dest] == "  ") {
        checker[dest][0] = '1';
        checker[dest][1] = turn;
    }
    else if (checker[dest][1] == turn) {
        checker[dest][0]++;
    }
}

void decrease(int move) {
    if (checker[move][0] == '1')
        checker[move] = "  ";

    else
        checker[move][0]--;
}

bool error(int move,int dest) {
    if (move == -1) {
        cout << "Invalid move, Coordinate does not exist";
        return true;
    }
    else if (checker[move][1] != turn) {
        cout << "Invalid move, "<<turn<<"'s Checker is not here";
        return true;
    }
    else if ((dest > 24 || dest < 0) && !isAllCheckersInHomeBoard()) {
        cout << "Invalid move, " << turn << "'s Checker does not move there, End of the Table";
        return true;
    }
    else if (checker[dest][0] > '1' && checker[dest][1] == opponent) {
        cout << "Invalid move, Opponent's Gate is here";
        return true;
    }
    else if (isAllCheckersInHomeBoard()&&(dest<-1||dest>24)) {
        cout << "Invalid move, Move further checker";
        return true;
    }
    else {
        return false;
    }

}

bool isAllCheckersInHomeBoard(){
    int temp=0;
    if (turn == 'X') {
        for (int i = 23; i>=6; i--) {
            if (checker[i][1] == 'X' || broken_checker[turn - 'X'] > 0) {
                temp=1;
                break;
            }
        }
        if (temp == 0)return true;
    }
    else if (turn == 'Y') {
        for (int i = 0; i < 18; i++) {
            if (checker[i][1] == 'Y' || broken_checker[turn - 'X'] > 0) {
                temp=1;
                break;
            }
        }
        if (temp == 0)return true;
    }
     return false;
}

int furtherChecker() {
    int num=6;
    if (turn == 'X') {
        for (int i = 6; i >0;i--) {
            if (checker[i-1][1] == 'X') {
                num = i;
                break;
            }
        }
    }
    else if (turn == 'Y') {
        for (int i = 18; i <24; i++) {
            if (checker[i][1] == 'Y') {
                num = 24-i;
                break;
            }
        }
    }
    return num;
}

bool isGameEnd() {
    int temp = 0;

    for (int i = 0; i < 24; i++) {
        if (turn == 'X') {
            if (checker[i][1] == 'X') {
                temp = 1;
                break;
            }
        }
        else if (turn == 'Y') {
            if (checker[i][1] == 'Y') {
                temp=1;
                break;
            }
        } 
    }
    
        
    if (temp == 0)return true;
    
    else return false;
}

void finish() {
    system("cls");
    thread t(music);
    fontsize(60, 60);
    cout <<endl<< "    WINNER " << turn;
    t.join();
}