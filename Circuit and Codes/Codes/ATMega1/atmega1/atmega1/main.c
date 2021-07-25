
#include "stdio.h"
#include <stdlib.h>
#include "string.h"
#include <avr/io.h>
#include "avr/interrupt.h"
#define F_CPU 1000000
#include "util/delay.h"
#include <time.h>
#define  Trigger_pin	PD0
int TimerOverflow = 0;

volatile int scroll_down[]  = {0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
0x80,0x90,0xa0,0xb0,0xc0,0xd0,0xe0,0xf0};
volatile int curX = 1, curY = 4,oldX = 1, oldY = 4;
volatile int collision = 0;
volatile int type;
volatile int version = 0;
volatile int board[17][9];
int blockStore[17][9];


int getRow(int x) {
	int row = 0;
	for (int i = 1; i <= 8; i++) row = row | (board[x][i] << (8-i));
	return row;
}

int getCol(int x){
	int col = 0;
	for (int i = 1; i <= 8; i++) col = col | (board[i][x] << (i-1));
	return col;
}


void lightLED(int r, int c){
	board[r][c] = 1;
}

int makeT(int r, int c){
	if (version % 4 == 0){
		if (r > 16 || c < 2 || c > 7) return 0;
		if (board[r][c] == 0 && board[r][c-1] == 0 &&
		board[r][c+1] == 0 && board[r-1][c] == 0){
			lightLED(r,c);
			lightLED(r,c-1);
			lightLED(r,c+1);
			lightLED(r-1,c);
			return 1;
		}
	}
	else if (version % 4 == 1){
		if (r > 15 || c < 1 || c > 7) return 0;
		if (board[r][c] == 0 && board[r-1][c] == 0 &&
		board[r][c+1] == 0 && board[r+1][c] == 0){
			lightLED(r,c);
			lightLED(r-1,c);
			lightLED(r,c+1);
			lightLED(r+1,c);
			return 1;
		}
	}
	else if (version % 4 == 2){
		if (r > 15 || c < 2 || c > 7) return 0;
		if (board[r][c] == 0 && board[r][c-1] == 0 &&
		board[r][c+1] == 0 && board[r+1][c] == 0){
			lightLED(r,c);
			lightLED(r,c-1);
			lightLED(r,c+1);
			lightLED(r+1,c);
			return 1;
		}
	}
	else if (version % 4 == 3){
		if (r > 15 || c < 2 || c > 8) return 0;
		if (board[r][c] == 0 && board[r-1][c] == 0 &&
		board[r][c-1] == 0 && board[r+1][c] == 0){
			lightLED(r,c);
			lightLED(r-1,c);
			lightLED(r,c-1);
			lightLED(r+1,c);
			return 1;
		}
	}
	return 0;
}

int makeBAR(int r, int c){
	if (version % 2 == 0){
		if (r > 16 || c < 2 || c > 7) return 0;
		if (board[r][c] == 0 && board[r][c-1] == 0 &&
		board[r][c+1] == 0){
			lightLED(r,c);
			lightLED(r,c-1);
			lightLED(r,c+1);
			return 1;
		}
	}
	else if (version % 2 == 1){
		if (r > 15 || c < 1 || c > 8) return 0;
		if (board[r][c] == 0 && board[r-1][c] == 0 &&
		board[r+1][c] == 0){
			lightLED(r,c);
			lightLED(r-1,c);
			lightLED(r+1,c);
			return 1;
		}
	}
	return 0;
}

int makeSQUARE(int r, int c){
	if (r > 16 || c < 1 || c > 7) return 0;
	if (board[r][c] == 0 && board[r][c+1] == 0 &&
	board[r-1][c] == 0 && board[r-1][c+1] == 0){
		lightLED(r,c);
		lightLED(r,c+1);
		lightLED(r-1,c);
		lightLED(r-1,c+1);
		return 1;
	}
	return 0;
}

int makeL(int r, int c){
	if (version % 4 == 0) {
		if (r > 16 || c < 1 || c > 7) return 0;
		if (board[r][c] == 0 && board[r][c+1] == 0 &&
		board[r-1][c] == 0 && board[r-2][c] == 0){
			lightLED(r,c);
			lightLED(r,c+1);
			lightLED(r-1,c);
			lightLED(r-2,c);
			return 1;
		}
	}
	else if (version % 4 == 1) {
		if (r > 15 || c < 1 || c > 6) return 0;
		if (board[r][c] == 0 && board[r][c+1] == 0 &&
		board[r][c+2] == 0 && board[r+1][c] == 0){
			lightLED(r,c);
			lightLED(r,c+1);
			lightLED(r,c+2);
			lightLED(r+1,c);
			return 1;
		}
	}
	else if (version % 4 == 2) {
		if (r > 14 || c < 2 || c > 8) return 0;
		if (board[r][c] == 0 && board[r][c-1] == 0 &&
		board[r+1][c] == 0 && board[r+2][c] == 0){
			lightLED(r,c);
			lightLED(r,c-1);
			lightLED(r+1,c);
			lightLED(r+2,c);
			return 1;
		}
	}
	else if (version % 4 == 3) {
		if (r > 16 || c < 3 || c > 8) return 0;
		if (board[r][c] == 0 && board[r][c-1] == 0 &&
		board[r][c-2] == 0 && board[r-1][c] == 0){
			lightLED(r,c);
			lightLED(r,c-1);
			lightLED(r,c-2);
			lightLED(r-1,c);
			return 1;
		}
	}
	return 0;

}

int makeBLOCK(int r, int c, int type){
	if (type == 1) return makeT(r,c);
	else if (type == 2) return makeBAR(r,c);
	else if (type == 3) return makeSQUARE(r,c);
	else if (type == 4) return makeL(r,c);
	
}

void removeBLOCKS(int r, int c, int type){
	
	if (type == 1) {
		if (version % 4 == 0){
			board[r][c]   = 0;
			board[r][c-1] = 0;
			board[r][c+1] = 0;
			board[r-1][c] = 0;
		}
		else if (version % 4 == 1){
			board[r][c]   = 0;
			board[r-1][c] = 0;
			board[r][c+1] = 0;
			board[r+1][c] = 0;
		}
		else if (version % 4 == 2){
			board[r][c]   = 0;
			board[r][c-1] = 0;
			board[r][c+1] = 0;
			board[r+1][c] = 0;
		}
		else if (version % 4 == 3){
			board[r][c]   = 0;
			board[r-1][c] = 0;
			board[r][c-1] = 0;
			board[r+1][c] = 0;
		}
	}
	else if (type == 2) {
		if (version % 2 == 0){
			board[r][c]   = 0;
			board[r][c-1] = 0;
			board[r][c+1] = 0;
		}
		else if (version % 2 == 1){
			board[r][c]   = 0;
			board[r-1][c] = 0;
			board[r+1][c] = 0;
		}
	}
	else if (type == 3) {
		board[r][c]     = 0;
		board[r][c+1]   = 0;
		board[r-1][c]   = 0;
		board[r-1][c+1] = 0;
	}
	else if (type == 4) {
		if (version % 4 == 0){
			board[r][c]   = 0;
			board[r][c+1] = 0;
			board[r-1][c] = 0;
			board[r-2][c] = 0;
		}
		else if (version % 4 == 1){
			board[r][c]   = 0;
			board[r][c+1] = 0;
			board[r][c+2] = 0;
			board[r+1][c] = 0;
		}
		else if (version % 4 == 2){
			board[r][c]   = 0;
			board[r][c-1] = 0;
			board[r+1][c] = 0;
			board[r+2][c] = 0;
		}
		else if (version % 4 == 3){
			board[r][c]   = 0;
			board[r][c-1] = 0;
			board[r][c-2] = 0;
			board[r-1][c] = 0;
		}
	}
	
}

void makeBOARD(){  /////k = 60 and j = 20
	cli();
	int i,j,k,p;
	int valArr[16];

	for (p = 0; p < 16; p++) valArr[p] = getRow(p+1);
	for(k = 0; k < 160; k++){

		for(i = 1; i <= 16; i++){
			int val = valArr[i-1];
			for (j = 1; j <= 20; j++){
				// change 20 to increase speed
				PORTA = scroll_down[i-1];
				PORTC = val;
			}
		}

		
	}
	
	sei();
}

ISR(INT2_vect)
{
	removeBLOCKS(curX,curY,type);
	version = (version + 1)%4;
	makeBLOCK(curX,curY,type);


}

ISR(INT1_vect)
{

	removeBLOCKS(curX, curY, type);
	oldY = curY;


	curY--;
	
	if (makeBLOCK(curX,curY,type) == 0) {
		curY = oldY;
		
		makeBLOCK(curX,curY,type);
	}
}

ISR(INT0_vect)
{  ///left shift
	
	removeBLOCKS(curX, curY, type);
	oldY = curY;

	curY++;
	if (makeBLOCK(curX,curY,type) == 0) {
		curY = oldY;
		makeBLOCK(curX,curY,type);
	}
}


int clearMatchedRow(){
	int i,j,k,p;
	int matched = 0;

	for (i = 16; i >= 1; i--){
		if (getRow(i) == 255) {
			for (j = i-1; j >= 1; j--){
				for (k = 1; k <= 8; k++){
					board[j+1][k] = board[j][k];
				}
			}
			for (p = 1; p <= 8; p++){
				board[1][p] = 0;
			}
			/* send scores*/
			i++;
			matched = 1;
			PORTD=0b00000001;
			_delay_ms(50);
			PORTD=0b00000000;
		}
	}
	return matched;
}

void Play(){

	if(isBoardful()==1) memset(board,0,sizeof(board));
	
	if (curX == 1) {
		type = (rand() % 4) + 1;
		version=0;
	}
	
	
	if (makeBLOCK(curX, curY, type) == 0) {
		makeBLOCK(oldX, curY, type);
		collision = 1;
	}
	oldX = curX;

	makeBOARD();

	if(collision == 0) {
		removeBLOCKS(curX, curY, type);
		curX++;
	}
	else {
		curX = 1;
		curY = 4;
		collision = 0;
	}



	if (clearMatchedRow() == 1) makeBOARD();

}

int isBoardful(){
	if(type==1 && (board[1][4]==1 || board[1][5]==1 || board[1][3]==1) ) return 1;
	else if(type==2 && (board[1][4]==1 || board[1][5]==1 || board[1][3]==1) ) return 1;
	else if((type==3 || type==4) && (board[1][4]==1 || board[1][5]==1 ) ) return 1;
	else return 0;
}


void initInterrupt()
{
	GICR =  1<<INT1 | 1<<INT0 | 1<<INT2;
	MCUCR = 1 <<ISC11 | 1 << ISC01;
	MCUCSR = 0x00;
	sei();
}

void checkLeftBoundary(){
	if (type == 1){
		if (version % 4 == 0 && curY < 2) curY = 2;
		else if (version % 4 == 1 && curY < 1) curY = 1;
		else if (version % 4 == 2 && curY < 2) curY = 2;
		else if (version % 4 == 3 && curY < 2) curY = 2;
	}
	else if (type == 2){
		if (version % 2 == 0 && curY < 2) curY = 2;
		else if (version % 2 == 1 && curY < 1) curY = 1;
	}
	else if (type == 3){
		if (curY < 1) curY = 1;
	}
	else if (type == 4){
		if (version % 4 == 0 && curY < 1) curY = 1;
		else if (version % 4 == 1 && curY < 1) curY = 1;
		else if (version % 4 == 2 && curY < 2) curY = 2;
		else if (version % 4 == 3 && curY < 3) curY = 3;
	}
}

void checkRightBoundary(){
	if (type == 1){
		if (version % 4 == 0 && curY > 7) curY = 7;
		else if (version % 4 == 1 && curY > 7) curY = 7;
		else if (version % 4 == 2 && curY > 7) curY = 7;
		else if (version % 4 == 3 && curY > 8) curY = 8;
	}
	else if (type == 2){
		if (version % 2 == 0 && curY > 7) curY = 7;
		else if (version % 2 == 1 && curY > 8) curY = 8;
	}
	else if (type == 3){
		if (curY > 7) curY = 7;
	}
	else if (type == 4){
		if (version % 4 == 0 && curY > 7) curY = 7;
		else if (version % 4 == 1 && curY > 6) curY = 6;
		else if (version % 4 == 2 && curY > 8) curY = 8;
		else if (version % 4 == 3 && curY > 8) curY = 8;
	}
}



int main(void)
{
	memset(board, 0, sizeof(board));
	memset(blockStore, 0, sizeof(blockStore));

	DDRB = 0x00;
	DDRA = 0xFF;
	DDRD = 0x01;

	initInterrupt();

	sei();
	srand(time(0));
	while(1){
		Play();
	}

}
