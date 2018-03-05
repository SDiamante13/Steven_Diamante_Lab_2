/* 
 * File:   Lab2main.c
 * Author: Steven Diamante
 *
 * Created on March 3, 2013, 3:07 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>
#include <plib.h>
#include "uart.h"
#include "delay.h"


// Cerebot board configuration
#pragma config ICESEL       = ICS_PGx1  // ICE/ICD Comm Channel Select
#pragma config DEBUG        = OFF       // Debugger Disabled for Starter Kit

#pragma config FNOSC        = PRIPLL	// Oscillator selection
#pragma config POSCMOD      = EC	// Primary oscillator mode
#pragma config FPLLIDIV     = DIV_2	// PLL input divider
#pragma config FPLLMUL      = MUL_20	// PLL multiplier
#pragma config FPLLODIV     = DIV_1	// PLL output divider
#pragma config FPBDIV       = DIV_8	// Peripheral bus clock divider
#pragma config FSOSCEN      = OFF	// Secondary oscillator enable

#define  COL1 0x08
#define  COL2 0x04
#define  COL3 0x02
#define  COL4 0x01
#define  ROW1 0x80
#define  ROW2 0x40
#define  ROW3 0x20
#define  ROW4 0x10

static unsigned char Keyboard[4][4] =  {{'1','2','3','A'},
                                        {'4','5','6','B'},
                                        {'7','8','9','C'},
                                        {'0','F','E','D'}};

unsigned char gameboard[4][4] = {{'_','_','_','_'},
                                {'_','_','_','_'},
                                {'_','_','_','_'},
                                {'_','_','_','_'}};

void Check(int);
void Check_Row0(int, int);
void Check_Middle(int,int);
void Check_Row3(int,int);

void Setup(){
mJTAGPortEnable(0);
mPORTESetPinsDigitalOut(COL1|COL2|COL3|COL4);
mPORTESetPinsDigitalIn(ROW1|ROW2|ROW3|ROW4);
PORTSetBits(IOPORT_E, COL1);
PORTSetBits(IOPORT_E, COL2);
PORTSetBits(IOPORT_E, COL3);
PORTSetBits(IOPORT_E, COL4);
}
void display_board(){
    int i, j;
    char Entry;
    for(i=0; i<4; i++)
   {
        for(j=0; j<4; j++)
        {
            Entry = gameboard[i][j];
            putcUART1(Entry);
          putcUART1(' ');
        }
        putcUART1('\n');
}
}

void clear_board(){
    int i, j;
    for(i=0; i<4; i++)
   {
        for(j=0; j<4; j++)
        {
            gameboard[i][j]= '_';
        }
}
}
int char_to_space(unsigned char Pbutton)
{
int invalid = 0;
if (Pbutton == '1')
{
if(gameboard[0][0]='_')
     gameboard[0][0]='x';
else
{
    UARTSendString("ILLEGAL MOVE!");
    invalid = 1;
}
}
else if(Pbutton == '2'){
if(gameboard[0][1]='_')
gameboard[0][1]='x';
else
    {
    UARTSendString("ILLEGAL MOVE!");
    invalid =1;
}
}
else if(Pbutton == '3'){
if(gameboard[0][2]='_')
gameboard[0][2]='x';
else
    {
    UARTSendString("ILLEGAL MOVE!");
    invalid =1;
}}
else if(Pbutton == 'A'){
if(gameboard[0][3]='_')
gameboard[0][3]='x';
else
{
UARTSendString("ILLEGAL MOVE!");
invalid =1;
}
}
else if(Pbutton == '4'){
if(gameboard[1][0]='_')
gameboard[1][0]='x';
else
{
    UARTSendString("ILLEGAL MOVE!");
    invalid =1;
}}
else if(Pbutton == '5'){
if(gameboard[1][1]='_')
gameboard[1][1]='x';
else
    {
    UARTSendString("ILLEGAL MOVE!");
    invalid =1;
}}
else if(Pbutton == '6'){
if(gameboard[1][2]='_'){
gameboard[1][2]='x';}
else
    {
    UARTSendString("ILLEGAL MOVE!");
    invalid =1;
}}
else if(Pbutton == 'B'){
if(gameboard[1][3]='_'){
gameboard[1][3]='x';}
else
    {
    UARTSendString("ILLEGAL MOVE!");
    invalid =1;
}}
else if(Pbutton == '7'){
if(gameboard[2][0]='_'){
gameboard[2][0]='x';}
else
    {
    UARTSendString("ILLEGAL MOVE!");
    invalid =1;
}}
else if(Pbutton == '8'){
if(gameboard[2][1]='_'){
gameboard[2][1]='x';}
else
    {
    UARTSendString("ILLEGAL MOVE!");
    invalid =1;
}}
else if(Pbutton == '9'){
if(gameboard[2][2]='_'){
gameboard[2][2]='x';}
else
    {
    UARTSendString("ILLEGAL MOVE!");
    invalid =1;
}}
else if(Pbutton == 'C'){
if(gameboard[2][3]='_'){
gameboard[2][3]='x';}
else
    {
    UARTSendString("ILLEGAL MOVE!");
    invalid =1;
}}
else if(Pbutton == '0'){
if(gameboard[3][0]='_'){
gameboard[3][0]='x';}
else
    {
    UARTSendString("ILLEGAL MOVE!");
    invalid =1;
}}
else if(Pbutton == 'F'){
if(gameboard[3][1]='_'){
gameboard[3][1]='x';}
else
    {
    UARTSendString("ILLEGAL MOVE!");
    invalid =1;
}}
else if(Pbutton == 'E'){
if(gameboard[3][2]='_'){
gameboard[3][2]='x';}
else
    {
    UARTSendString("ILLEGAL MOVE!");
    invalid =1;
}}
else if(Pbutton == 'D'){
if(gameboard[3][3]='_'){
gameboard[3][3]='x';}
else
    {
    UARTSendString("ILLEGAL MOVE!");
    invalid =1;
}}
else
  UARTSendString("I think someone won or it's a draw.");
return invalid;
}

unsigned char pull_col_read(){
int row = -1, col = -1;
while (row==-1)
{
PORTClearBits(IOPORT_E, COL1);
    if(!(PORTRead(IOPORT_E) & ROW1))
        { row = 0;
          col = 0;
          PORTSetBits(IOPORT_E, COL1);
        }
    else if (!(PORTRead(IOPORT_E) & ROW2))
        { row = 1;
          col = 0;
          PORTSetBits(IOPORT_E, COL1);
        }
    else if (!(PORTRead(IOPORT_E) & ROW3))
        { row = 2;
          col = 0;
          PORTSetBits(IOPORT_E, COL1);
        }
    else if(!(PORTRead(IOPORT_E) & ROW4))
        { row = 3;
          col = 0;
          PORTSetBits(IOPORT_E, COL1);
        }
    else{
        PORTSetBits(IOPORT_E, COL1);
    }
    PORTClearBits(IOPORT_E, COL2); // pull column 2 low
    if(!(PORTRead(IOPORT_E) & ROW1))
        { row = 0;
          col = 1;
          PORTSetBits(IOPORT_E, COL2);
        }
    else if (!(PORTRead(IOPORT_E) & ROW2))
        { row = 1;
          col = 1;
          PORTSetBits(IOPORT_E, COL2);
        }
    else if (!(PORTRead(IOPORT_E) & ROW3))
        { row = 2;
          col = 1;
          PORTSetBits(IOPORT_E, COL2);
        }
    else if(!(PORTRead(IOPORT_E) & ROW4))
        { row = 3;
          col = 1;
          PORTSetBits(IOPORT_E, COL2);
        }
    else{
        PORTSetBits(IOPORT_E, COL2);
    }
     PORTClearBits(IOPORT_E, COL3); // pull column 3 low
    if(!(PORTRead(IOPORT_E) & ROW1))
        { row = 0;
          col = 2;
          PORTSetBits(IOPORT_E, COL3);
        }
    else if (!(PORTRead(IOPORT_E) & ROW2))
        { row = 1;
          col = 2;
          PORTSetBits(IOPORT_E, COL3);
        }
    else if (!(PORTRead(IOPORT_E) & ROW3))
        { row = 2;
          col = 2;
          PORTSetBits(IOPORT_E, COL3);
        }
    else if(!(PORTRead(IOPORT_E) & ROW4))
        { row = 3;
          col = 2;
          PORTSetBits(IOPORT_E, COL3);
        }
    else{
        PORTSetBits(IOPORT_E, COL3);
    }
      PORTClearBits(IOPORT_E, COL4); // pull column 4 low
    if(!(PORTRead(IOPORT_E) & ROW1))
        { row = 0;
          col = 3;
          PORTSetBits(IOPORT_E, COL4);
        }
    else if (!(PORTRead(IOPORT_E) & ROW2))
        { row = 1;
          col = 3;
          PORTSetBits(IOPORT_E, COL4);
        }
    else if (!(PORTRead(IOPORT_E) & ROW3))
        { row = 2;
          col = 3;
          PORTSetBits(IOPORT_E, COL4);
        }
    else if(!(PORTRead(IOPORT_E) & ROW4))
        { row = 3;
          col = 3;
          PORTSetBits(IOPORT_E, COL4);
        }
    else{
        PORTSetBits(IOPORT_E, COL4);
    }
}
DelayMs(20);
return Keyboard[row][col];
}

void Scan(int row){
if(row == 0)
{
int c = 0;
Scan_Row0(row, c);
}

else if(row==1)
{
int c=0;
Scan_Middle(row,c);
}

else if(row==2)
{
int c=0;
Scan_Middle(row,c);
}
else if(row==3)
{
int c=0;
Scan_Row3(row,c);
}
else
{
printf("Who won?\n");
}
}

void Scan_Row0(int row, int col){

if (gameboard[row][0]== 'x')
{
if (gameboard[row][col+1]=='_')
{
gameboard[row][col+1]='o';
}
else if (gameboard[row+1][col]=='_')
{
gameboard[row+1][col]='o';
}
else if (gameboard[row+1][col+1]=='_')
{
gameboard[row+1][col+1]='o';
}
else //if spots are already filled around it, go to next col
{
col++;
Scan_Row0(row, col);
}
}
else if (gameboard[row][1] == 'x')
{
if (gameboard[row][col+1]=='_')
{
gameboard[row][col+1]='o';
}
else if (gameboard[row+1][col]=='_')
{
gameboard[row+1][col]='o';
}
else if (gameboard[row+1][col+1]=='_')
{
gameboard[row+1][col+1]='o';
}
else if (gameboard[row][col-1]=='_')
{
gameboard[row][col-1]='o';
}
else if (gameboard[row+1][col-1]=='_')
{
gameboard[row+1][col-1]='o';
}
else
{
col++;
Scan_Row0(row, col);
}
}
else if (gameboard[row][2]== 'x' && col==2)
{
if (gameboard[row][col+1]!='x'||'o')
{
gameboard[row][col+1]='o';
}
else if (gameboard[row+1][col]=='_')
{
gameboard[row+1][col]='o';
}
else if (gameboard[row+1][col+1]=='_')
{
gameboard[row+1][col+1]='o';
}
else if (gameboard[row][col-1]=='_')
{
gameboard[row][col-1]='o';
}
else if (gameboard[row+1][col-1]=='_')
{
gameboard[row+1][col-1]='o';
}
else
{
col++;
Scan_Row0(row, col);
}
}
else if (gameboard[row][3]== 'x')
{
if (gameboard[row][col-1]=='_')
{
gameboard[row][col-1]='o';
}
else if (gameboard[row+1][col]=='_')
{
gameboard[row+1][col]='o';
}
else if (gameboard[row+1][col-1]=='_')
{
gameboard[row+1][col-1]='o';
}
else
{
col++; //col will equal 4
Scan_Row0(row, col);//go back to CheckRow0()
}
}
else
{
row++; //if row0 is filled with X's and O's
col=0; //reset col
Check(row);
}
}

void Scan_Middle(int row, int col){

if (gameboard[row][0]== 'x')
{
if (gameboard[row][col+1]=='_')
{
gameboard[row][col+1]='o';
}
else if (gameboard[row+1][col]=='_')
{
gameboard[row+1][col]='o';
}
else if (gameboard[row+1][col+1]=='_')
{
gameboard[row+1][col+1]='o';
}
else if (gameboard[row-1][col]=='_')
{
gameboard[row-1][col]='o';
}
else if (gameboard[row-1][col-1]=='_')
{
gameboard[row-1][col-1]='o';
}

else //if spots are already filled around it, go to next col
{
col++;
Scan_Middle(row, col);
}
}
else if (gameboard[row][1]== 'x')
{
if (gameboard[row][col+1]=='_')
{
gameboard[row][col+1]='o';
}
else if (gameboard[row+1][col]=='_')
{
gameboard[row+1][col]='o';
}
else if (gameboard[row+1][col+1]=='_')
{
gameboard[row+1][col+1]='o';
}
else if (gameboard[row-1][col]=='_')
{
gameboard[row-1][col]='o';
}
else if (gameboard[row-1][col-1]=='_')
{
gameboard[row-1][col-1]='o';
}
else if (gameboard[row+1][col-1]=='_')
{
gameboard[row+1][col-1]='o';
}
else if (gameboard[row-1][col-1]=='_')
{
gameboard[row-1][col-1]='o';
}
else if (gameboard[row][col-1]=='_')
{
gameboard[row][col-1]='o';
}
else //if spots are already filled around it, go to next col
{
col++;
Scan_Middle(row, col);
}
}
else if (gameboard[row][2]== 'x')
{
if (gameboard[row][col+1]=='_')
{
gameboard[row][col+1]='o';
}
else if (gameboard[row+1][col]=='_')
{
gameboard[row+1][col]='o';
}
else if (gameboard[row+1][col+1]=='_')
{
gameboard[row+1][col+1]='o';
}
else if (gameboard[row-1][col]=='_')
{
gameboard[row-1][col]='o';
}
else if (gameboard[row-1][col-1]=='_')
{
gameboard[row-1][col-1]='o';
}
else if (gameboard[row+1][col-1]=='_')
{
gameboard[row+1][col-1]='o';
}
else if (gameboard[row-1][col-1]=='_')
{
gameboard[row-1][col-1]='o';
}
else if (gameboard[row][col-1]=='_')
{
gameboard[row][col-1]='o';
}
else 
{
col++;
Scan_Middle(row, col);
}
}
else if (gameboard[row][3]== 'x')
{
if (gameboard[row][col-1]=='_')
{
gameboard[row][col-1]='o';
}
else if (gameboard[row+1][col]=='_')
{
gameboard[row+1][col]='o';
}
else if (gameboard[row+1][col-1]=='_')
{
gameboard[row+1][col+1]='o';
}
else if (gameboard[row-1][col]=='_')
{
gameboard[row-1][col]='o';
}
else if (gameboard[row-1][col-1]=='_')
{
gameboard[row-1][col-1]='o';
}
else //if spots are already filled around it, go to next col
{
col++;
Scan_Middle(row, col);
}

}
else
{
row++; //if row0 is filled with X's and O's
col=0; //reset col
Scan(row);
}
}

void Scan_Row3(int row, int col){
if (gameboard[row][0]== 'x')
{
if (gameboard[row][col+1]=='_')
{
gameboard[row][col+1]='o';
}
else if (gameboard[row-1][col]=='_')
{
gameboard[row-1][col]='o';
}
else if (gameboard[row-1][col+1]=='_')
{
gameboard[row-1][col+1]='o';
}
else //if spots are already filled around it, go to next col
{
col++;
Scan_Row3(row, col);
}
}
else if (gameboard[row][1]== 'x')
{
if (gameboard[row][col+1]=='_')
{
gameboard[row][col+1]='o';
}
else if (gameboard[row-1][col]=='_')
{
gameboard[row-1][col]='o';
}
else if (gameboard[row-1][col-1]=='_')
{
gameboard[row-1][col-1]='o';
}
else if (gameboard[row][col-1]=='_')
{
gameboard[row][col-1]='o';
}
else if (gameboard[row-1][col+1]=='_')
{
gameboard[row-1][col+1]='o';
}
else
{
col++;
Scan_Row3(row, col);
}
}
else if (gameboard[row][2]== 'x')
{
if (gameboard[row][col+1]=='_')
{
gameboard[row][col+1]='o';
}
else if (gameboard[row-1][col]=='_')
{
gameboard[row-1][col]='o';
}
else if (gameboard[row-1][col-1]=='_')
{
gameboard[row-1][col-1]='o';
}
else if (gameboard[row][col-1]=='_')
{
gameboard[row][col-1]='o';
}
else if (gameboard[row-1][col+1]=='_')
{
gameboard[row-1][col+1]='o';
}
else
{
col++;
Scan_Row3(row, col);
}}
else if (gameboard[row][3]== 'x')
{
if (gameboard[row][col-1]=='_')
{
gameboard[row][col-1]='o';
}
else if (gameboard[row-1][col]=='_')
{
gameboard[row-1][col]='o';
}
else if (gameboard[row-1][col-1]=='_')
{
gameboard[row-1][col-1]='o';
}
else
{
col++; //col will equal 4
Scan_Row3(row, col);//go back to CheckRow0()
}
}
else
{
row++; //if row0 is filled with x's and o's
col=0; //reset col
Scan(row);
}
}

int Who_Won(int counter){
    int winner;
    if((gameboard[0][0]=='x' && gameboard[0][1]=='x' && gameboard[0][2]=='x' &&
    gameboard[0][3]=='x') || (gameboard[1][0]=='x' && gameboard[1][1]=='x' && gameboard[1][2]=='x'
    && gameboard[1][3]=='x') || (gameboard[2][0]=='x' && gameboard[2][1]=='x' && gameboard[2][2]=='x'
    && gameboard[2][3]=='x') || (gameboard[3][0]=='x' && gameboard[3][1]=='x' && gameboard[3][2]=='x' && gameboard[3][3]=='x') ||
    (gameboard[0][0]=='x' && gameboard[1][0]=='x' && gameboard[2][0]=='x' && gameboard[3][0]=='x') || (gameboard[0][1]=='x' && gameboard[1][1]=='x' && gameboard[2][1]=='x' &&
    gameboard[3][1]=='x') || (gameboard[0][2]=='x' && gameboard[1][2]=='x' && gameboard[2][2]=='x' &&
    gameboard[3][2]=='x') || (gameboard[0][3]=='x' && gameboard[1][3]=='x' && gameboard[2][3]=='x' &&
    gameboard[3][3]=='x') || (gameboard[0][0]=='x' && gameboard[1][1]=='x' && gameboard[2][2]=='x' &&
    gameboard[3][3]=='x') || (gameboard[0][3]=='x' && gameboard[1][2]=='x' && gameboard[2][1]=='x' &&
    gameboard[3][0]=='x'))
        winner = 1;
    else if((gameboard[0][0]=='o' && gameboard[0][1]=='o' && gameboard[0][2]=='o' &&
    gameboard[0][3]=='o') || (gameboard[1][0]=='o' && gameboard[1][1]=='o' && gameboard[1][2]=='o'
    && gameboard[1][3]=='o') || (gameboard[2][0]=='o' && gameboard[2][1]=='o' && gameboard[2][2]=='o'
    && gameboard[2][3]=='o') || (gameboard[3][0]=='o' && gameboard[3][1]=='o' && gameboard[3][2]=='o' && gameboard[3][3]=='x') ||
    (gameboard[0][0]=='o' && gameboard[1][0]=='o' && gameboard[2][0]=='o' &&
    gameboard[3][0]=='o') || (gameboard[0][1]=='o' && gameboard[1][1]=='o' && gameboard[2][1]=='o' &&
    gameboard[3][1]=='o') || (gameboard[0][2]=='o' && gameboard[1][2]=='o' && gameboard[2][2]=='o' &&
    gameboard[3][2]=='o') || (gameboard[0][3]=='o' && gameboard[1][3]=='o' && gameboard[2][3]=='o' &&
    gameboard[3][3]=='o') || (gameboard[0][0]=='o' && gameboard[1][1]=='o' && gameboard[2][2]=='o' &&
    gameboard[3][3]=='o') || (gameboard[0][3]=='o' && gameboard[1][2]=='o' && gameboard[2][1]=='o' &&
    gameboard[3][0]=='o'))
        winner = 2;
    return winner;
    if(counter == 7 && winner==0)
        winner = 3;
}
unsigned char key;
int invalid;

enum modes{

	USER,				// It's the user's turn in this mode
	PC,				// It's the PC's turn in this mode
	//PLAYER_WIN,			// Player has won
	//PC_WIN,                         // PC has won
        //CAT					 // It's a draw!
	};
               enum modes mode;
int main() {
int counter = 0;
//int code_count = 0;
//char pass[5];
int winner=0;
DelayInit();
Setup();
int r=0;
UARTSendString("Cerebot Tic Tac Toe\n");
UARTSendString("Enter code to start: ");
//while(pass != "1CC5E"){
//while(code_count<5)
//{
 //pass[code_count] = pull_col_read();
 //key = pull_col_read();
 //putcUART1(key);
 //code_count++;
// }}
putcUART1('\n');
display_board();
putcUART1('\n');
mode = USER;
while(1){
switch(mode){
	case USER:				// Make a valid move
               invalid = 0;
               UARTSendString("Enter your move:\n");
               key = pull_col_read();
               invalid = char_to_space(key);
               if(invalid == 1)
               {
                 mode = USER;
                 break;
               }
                    display_board();
		    mode = PC;
                    break;
case PC:
DelayMs(2000);
UARTSendString("\nMy move:\n");
putcUART1('\n');
Scan(r);
display_board();
counter++;
winner=Who_Won(counter);
if (winner==1){
UARTSendString("You Win!!!\n");
putcUART1('\n');
counter=0;
clear_board();
mode = USER;
break;
}
else if(winner==2){
UARTSendString("I Win!\n");
putcUART1('\n');
counter=0;
clear_board();
mode = USER;
break;
}
else if(winner==3){
UARTSendString("It's a draw!\n");
counter=0;
clear_board();
mode = USER;
break;
}
mode = USER;
break;
}
}
putcUART1('\n');
    return (EXIT_SUCCESS);
}

