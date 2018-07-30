//serverfunctions.c
#include "serverfunctions.h"
#include		<stdio.h>
#include        <sys/time.h>
#include        <signal.h>

#define playto 5

extern int xpos, ypos, xdir, ydir, paddle, paddle2, p1fd, p2fd;
extern int points1, points2, win1, win2, tailflag;
extern int tail1x, tail1y, tail2x, tail2y;

//Updates the gamestate
void update(int signum)
{
	//First check to see if someone has won already
	if(win1 == 1 || win2 == 1)	//If so, no need to change anything
		return;
	
	//Check for collision
	if((xpos >= 75) || (xpos <= 5))
		xdir *= -1;
	if((ypos >= 40) || (ypos <= 10))
		ydir *= -1;
	if((xpos == 9)&&((ypos == paddle)||(ypos == paddle-2)||(ypos == paddle-1)||(ypos == paddle+1)||(ypos == paddle+2)
		||(ypos == paddle+3)||(ypos == paddle-3)))
		xdir *= -1;
	if((xpos == 71)&&((ypos == paddle2)||(ypos == paddle2-2)||(ypos == paddle2-1)||(ypos == paddle2+1)||(ypos == paddle2+2)
		||(ypos == paddle2+3)||(ypos == paddle2-3)))
		xdir *= -1;
	if(xpos == 75)	//If player 1 scored
	{
		points1++;
		xpos = 45;
		ypos = 25;
		tailflag = 0;
	}
	if(xpos == 5)	//If player 2 scored
	{
		points2++;
		xpos = 45;
		ypos = 25;
		tailflag = 0;	
	}
	if(points1 >= playto)	//Check to see if someone has won now
		win1 = 1;
	if(points2 >= playto)
		win2 = 1;

	
	//Update locations
	tail2x = tail1x; tail2y = tail1y;
	tail1x = xpos; tail1y = ypos;
	xpos += xdir;
	ypos += ydir;
	
	//Keep track of tail length
	if(tailflag < 2)
	{
		tailflag++;
	}
}

//Ticker function
int set_ticker( int n_msecs )
{
        struct itimerval new_timeset;
        long    n_sec, n_usecs;

        n_sec = n_msecs / 1000 ;                /* int part     */
        n_usecs = ( n_msecs % 1000 ) * 1000L ;  /* remainder    */

        new_timeset.it_interval.tv_sec  = n_sec;        /* set reload       */
        new_timeset.it_interval.tv_usec = n_usecs;      /* new ticker value */
        new_timeset.it_value.tv_sec     = n_sec  ;      /* store this       */
        new_timeset.it_value.tv_usec    = n_usecs ;     /* and this         */

        return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
