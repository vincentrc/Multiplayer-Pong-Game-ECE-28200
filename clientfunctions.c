//clientfunctions.c
#include "clientfunctions.h"
#include <stdlib.h>
#include <termios.h>
#include <curses.h>
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>

//Draws updated paddle location
void drawpaddles(int paddle, int paddle2)
{
	move(paddle-3, 8);
	addstr("|");
	move(paddle-2, 8);
	addstr("|");
	move(paddle-1, 8);
	addstr("|");
	move(paddle, 8);
	addstr("|");
	move(paddle+1, 8);
	addstr("|");
	move(paddle+2, 8);
	addstr("|");
	move(paddle+3, 8);
	addstr("|");
	
	move(paddle2-3, 72);
	addstr("|");
	move(paddle2-2, 72);
	addstr("|");
	move(paddle2-1, 72);
	addstr("|");
	move(paddle2, 72);
	addstr("|");
	move(paddle2+1, 72);
	addstr("|");
	move(paddle2+2, 72);
	addstr("|");
	move(paddle2+3, 72);
	addstr("|");
	move(LINES-1,COLS-1);
}

//Removes old paddle location
void erasepaddles(int paddle, int paddle2)
{
	move(paddle-3, 8);
	addstr(" ");
	move(paddle-2, 8);
	addstr(" ");
	move(paddle-1, 8);
	addstr(" ");
	move(paddle, 8);
	addstr(" ");
	move(paddle+1, 8);
	addstr(" ");
	move(paddle+2, 8);
	addstr(" ");
	move(paddle+3, 8);
	addstr(" ");
	
	move(paddle2-3, 72);
	addstr(" ");
	move(paddle2-2, 72);
	addstr(" ");
	move(paddle2-1, 72);
	addstr(" ");
	move(paddle2, 72);
	addstr(" ");
	move(paddle2+1, 72);
	addstr(" ");
	move(paddle2+2, 72);
	addstr(" ");
	move(paddle2+3, 72);
	addstr(" ");
}

//Termination function
void quit(int signum)
{
	terminalSetup(0);
	endwin();
	exit(0);
}

//Sets the terminal the the appropriate settings
void terminalSetup(int input)
{
	struct termios info;


	//Gets terminal attributes
	if ( tcgetattr(0,&info) == -1 )        
                perror("Error");

	if(input == 1)
	{
		//Turns off canonical mode
		info.c_lflag &= ~ICANON;
	}
	else
	{
		//Turns on canonical mode
		info.c_lflag |= ICANON;
	}


	//Sets new terminal settings
	if ( tcsetattr(0,TCSANOW,&info) == -1 ) 
               perror("Error");
}
