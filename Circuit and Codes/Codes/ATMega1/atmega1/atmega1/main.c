 
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
 
 volatile int select[]  = {0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
 0x80,0x90,0xa0,0xb0,0xc0,0xd0,0xe0,0xf0};

 volatile int curX = 1, curY = 4;
 volatile int oldX = 1, oldY = 4;


 volatile int collision = 0;

 volatile int type;

 volatile int version = 0, oldVersion = 0;

 volatile int occupied[17][9];

 volatile int psCnt = 0;

 int dd = 8, oldDd = 8;

 int getRow(int x) {
	 int row = 0;
	 //for (int i = 1; i <= 8; i++) row = row | (occupied[x][i] << (i-1));
	 for (int i = 1; i <= 8; i++) row = row | (occupied[x][i] << (8-i));
	 return row;
 }

 int getCol(int x){
	 int col = 0;
	 for (int i = 1; i <= 8; i++) col = col | (occupied[i][x] << (i-1));
	 return col;
 }


 void drawPixel(int r, int c){
	 occupied[r][c] = 1;
 }

 int drawT(int r, int c){
	 if (version % 4 == 0){
		 if (r > 16 || c < 2 || c > 7) return 0;
		 if (occupied[r][c] == 0 && occupied[r][c-1] == 0 &&
		 occupied[r][c+1] == 0 && occupied[r-1][c] == 0){
			 drawPixel(r,c);
			 drawPixel(r,c-1);
			 drawPixel(r,c+1);
			 drawPixel(r-1,c);
			 return 1;
		 }
	 }
	 else if (version % 4 == 1){
		 if (r > 15 || c < 1 || c > 7) return 0;
		 if (occupied[r][c] == 0 && occupied[r-1][c] == 0 &&
		 occupied[r][c+1] == 0 && occupied[r+1][c] == 0){
			 drawPixel(r,c);
			 drawPixel(r-1,c);
			 drawPixel(r,c+1);
			 drawPixel(r+1,c);
			 return 1;
		 }
	 }
	 else if (version % 4 == 2){
		 if (r > 15 || c < 2 || c > 7) return 0;
		 if (occupied[r][c] == 0 && occupied[r][c-1] == 0 &&
		 occupied[r][c+1] == 0 && occupied[r+1][c] == 0){
			 drawPixel(r,c);
			 drawPixel(r,c-1);
			 drawPixel(r,c+1);
			 drawPixel(r+1,c);
			 return 1;
		 }
	 }
	 else if (version % 4 == 3){
		 if (r > 15 || c < 2 || c > 8) return 0;
		 if (occupied[r][c] == 0 && occupied[r-1][c] == 0 &&
		 occupied[r][c-1] == 0 && occupied[r+1][c] == 0){
			 drawPixel(r,c);
			 drawPixel(r-1,c);
			 drawPixel(r,c-1);
			 drawPixel(r+1,c);
			 return 1;
		 }
	 }
	 return 0;
 }

 int draw1(int r, int c){
	 if (version % 2 == 0){
		 if (r > 16 || c < 2 || c > 7) return 0;
		 if (occupied[r][c] == 0 && occupied[r][c-1] == 0 &&
		 occupied[r][c+1] == 0){
			 drawPixel(r,c);
			 drawPixel(r,c-1);
			 drawPixel(r,c+1);
			 return 1;
		 }
	 }
	 else if (version % 2 == 1){
		 if (r > 15 || c < 1 || c > 8) return 0;
		 if (occupied[r][c] == 0 && occupied[r-1][c] == 0 &&
		 occupied[r+1][c] == 0){
			 drawPixel(r,c);
			 drawPixel(r-1,c);
			 drawPixel(r+1,c);
			 return 1;
		 }
	 }
	 return 0;
 }

 int drawS(int r, int c){
	 if (r > 16 || c < 1 || c > 7) return 0;
	 if (occupied[r][c] == 0 && occupied[r][c+1] == 0 &&
	 occupied[r-1][c] == 0 && occupied[r-1][c+1] == 0){
		 drawPixel(r,c);
		 drawPixel(r,c+1);
		 drawPixel(r-1,c);
		 drawPixel(r-1,c+1);
		 return 1;
	 }
	 return 0;
 }

 int drawL(int r, int c){
	 if (version % 4 == 0) {
		 if (r > 16 || c < 1 || c > 7) return 0;
		 if (occupied[r][c] == 0 && occupied[r][c+1] == 0 &&
		 occupied[r-1][c] == 0 && occupied[r-2][c] == 0){
			 drawPixel(r,c);
			 drawPixel(r,c+1);
			 drawPixel(r-1,c);
			 drawPixel(r-2,c);
			 return 1;
		 }
	 }
	 else if (version % 4 == 1) {
		 if (r > 15 || c < 1 || c > 6) return 0;
		 if (occupied[r][c] == 0 && occupied[r][c+1] == 0 &&
		 occupied[r][c+2] == 0 && occupied[r+1][c] == 0){
			 drawPixel(r,c);
			 drawPixel(r,c+1);
			 drawPixel(r,c+2);
			 drawPixel(r+1,c);
			 return 1;
		 }
	 }
	 else if (version % 4 == 2) {
		 if (r > 14 || c < 2 || c > 8) return 0;
		 if (occupied[r][c] == 0 && occupied[r][c-1] == 0 &&
		 occupied[r+1][c] == 0 && occupied[r+2][c] == 0){
			 drawPixel(r,c);
			 drawPixel(r,c-1);
			 drawPixel(r+1,c);
			 drawPixel(r+2,c);
			 return 1;
		 }
	 }
	 else if (version % 4 == 3) {
		 if (r > 16 || c < 3 || c > 8) return 0;
		 if (occupied[r][c] == 0 && occupied[r][c-1] == 0 &&
		 occupied[r][c-2] == 0 && occupied[r-1][c] == 0){
			 drawPixel(r,c);
			 drawPixel(r,c-1);
			 drawPixel(r,c-2);
			 drawPixel(r-1,c);
			 return 1;
		 }
	 }
	 return 0;

 }

 int draw(int r, int c, int type){

	 //cli();
	 if (type == 1) return drawT(r,c);
	 else if (type == 2) return draw1(r,c);
	 else if (type == 3) return drawS(r,c);
	 else if (type == 4) return drawL(r,c);
	 //sei();
 }

 void removeShapes(int r, int c, int type){
	 //cli();
	 if (type == 1) {
		 if (version % 4 == 0){
			 occupied[r][c]   = 0;
			 occupied[r][c-1] = 0;
			 occupied[r][c+1] = 0;
			 occupied[r-1][c] = 0;
		 }
		 else if (version % 4 == 1){
			 occupied[r][c]   = 0;
			 occupied[r-1][c] = 0;
			 occupied[r][c+1] = 0;
			 occupied[r+1][c] = 0;
		 }
		 else if (version % 4 == 2){
			 occupied[r][c]   = 0;
			 occupied[r][c-1] = 0;
			 occupied[r][c+1] = 0;
			 occupied[r+1][c] = 0;
		 }
		 else if (version % 4 == 3){
			 occupied[r][c]   = 0;
			 occupied[r-1][c] = 0;
			 occupied[r][c-1] = 0;
			 occupied[r+1][c] = 0;
		 }
	 }
	 else if (type == 2) {
		 if (version % 2 == 0){
			 occupied[r][c]   = 0;
			 occupied[r][c-1] = 0;
			 occupied[r][c+1] = 0;
		 }
		 else if (version % 2 == 1){
			 occupied[r][c]   = 0;
			 occupied[r-1][c] = 0;
			 occupied[r+1][c] = 0;
		 }
	 }
	 else if (type == 3) {
		 occupied[r][c]     = 0;
		 occupied[r][c+1]   = 0;
		 occupied[r-1][c]   = 0;
		 occupied[r-1][c+1] = 0;
	 }
	 else if (type == 4) {
		 if (version % 4 == 0){
			 occupied[r][c]   = 0;
			 occupied[r][c+1] = 0;
			 occupied[r-1][c] = 0;
			 occupied[r-2][c] = 0;
		 }
		 else if (version % 4 == 1){
			 occupied[r][c]   = 0;
			 occupied[r][c+1] = 0;
			 occupied[r][c+2] = 0;
			 occupied[r+1][c] = 0;
		 }
		 else if (version % 4 == 2){
			 occupied[r][c]   = 0;
			 occupied[r][c-1] = 0;
			 occupied[r+1][c] = 0;
			 occupied[r+2][c] = 0;
		 }
		 else if (version % 4 == 3){
			 occupied[r][c]   = 0;
			 occupied[r][c-1] = 0;
			 occupied[r][c-2] = 0;
			 occupied[r-1][c] = 0;
		 }
	 }
	 //sei();
 }

 void drawBoard(){  /////k = 60 and j = 20
	 cli();
	 int i,j,k,p;
	 int valArr[16];

	 for (p = 0; p < 16; p++) valArr[p] = getRow(p+1);
	 for(k = 0; k < 160; k++){
		 for(i = 1; i <= 16; i++){
			 int val = valArr[i-1];
			 for (j = 1; j <= 5; j++){
				 // change 20 to increase speed 
				 PORTA = select[i-1];
				 PORTC = val;
			 }
		 }
	 }
	 sei();
 }

 ISR(INT2_vect)
 {
	 removeShapes(curX,curY,type);
	 version = (version + 1)%4;
	 draw(curX,curY,type);


 }

 ISR(INT1_vect)
 {
	 //right shift
	 psCnt++;

	 removeShapes(curX, curY, type);
	 oldY = curY;
	 //oldX = curX;
	 //if (psCnt <= 2) curX--;
	 //else psCnt = 0;

	 curY--;
	 //curX--;
	 if (draw(curX,curY,type) == 0) {
		 curY = oldY;
		 //curX = oldX;
		 draw(curX,curY,type);
	 }
 }

 ISR(INT0_vect)
 {  ///left shift
	 
	 removeShapes(curX, curY, type);
	 oldY = curY;

	 curY++;
	 if (draw(curX,curY,type) == 0) {
		 curY = oldY;
		 draw(curX,curY,type);
	 }
	 

	 

 }

 ISR(TIMER1_OVF_vect)
 {
	 TimerOverflow++;	/* Increment Timer Overflow count */
 }

 int clearMatchedRow(){
	 int i,j,k,p;
	 int matched = 0;

	 for (i = 16; i >= 1; i--){
		 if (getRow(i) == 255) {
			 for (j = i-1; j >= 1; j--){
				 for (k = 1; k <= 8; k++){
					 occupied[j+1][k] = occupied[j][k];
				 }
			 }
			 for (p = 1; p <= 8; p++){
				 occupied[1][p] = 0;
			 }
			 i++;
			 matched = 1;
			 PORTD=0b00000001;
			 _delay_ms(500);
			 PORTD=0b00000000;
		 }
	 }
	 return matched;
 }

 void gamePlay(){

	 //srand(time(0));
	 if (curX == 1) type = (rand() % 4) + 1;
	 
	 
	 if (draw(curX, curY, type) == 0) {
		 draw(oldX, curY, type);
		 collision = 1;
	 }
	 oldX = curX;

	 drawBoard();

	 if(collision == 0) {
		 removeShapes(curX, curY, type);
		 curX++;
	 }
	 else {
		 curX = 1;
		 curY = 4;
		 collision = 0;
	 }



	 if (clearMatchedRow() == 1) drawBoard();

 }


 void initInterrupt()
 {
	 GICR =  1<<INT1 | 1<<INT0 | 1<<INT2;
	 //GICR = (1<<7)|(1<<6)|(1<<5);
	 MCUCR = 1 <<ISC11 | 1 << ISC01;
	 //MCUCSR &= ~(1<<ISC2);
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
	 memset(occupied, 0, sizeof(occupied));

	 DDRB = 0x00;
	 DDRA = 0xFF;
	 DDRD = 0x01;

	 initInterrupt();

	 sei();
	 //TIMSK = (1 << TOIE1);	/* Enable Timer1 overflow interrupts */
	 //TCCR1A = 0;
		srand(time(0));
	 while(1){

		 gamePlay();
		 

	 }

 }
