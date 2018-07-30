//client.c
#include       <stdlib.h>
#include       <stdio.h>
#include       <sys/types.h>
#include       <sys/socket.h>
#include       <netinet/in.h>
#include	   <sys/select.h>
#include       <netdb.h>
#include       <string.h>
#include	   <curses.h>
#include	   <signal.h>
#include  	   <errno.h>
#include	   "clientfunctions.h"

int xpos, ypos, paddle, paddle2, sock_id, points1, points2;
char player;

int main(int ac, char *av[])
{
	struct sockaddr_in  servadd;       
    struct hostent      *hp;            
    int    sock_fd, activity, maxfd, tailflag;
	int	   tail1x, tail1y, tail2x, tail2y;
	FILE   *sock_fp;
    char   message[256], score1[10], score2[10], c;             
    int    messlen, x;
	char   *tok;
	fd_set readfds, writefds;

	//Setup
	xpos = 11; ypos = 11;
	paddle = 23; paddle2 = 23;

	//Get a socket
	if((sock_id = socket( AF_INET, SOCK_STREAM, 0 )) == -1) 
		perror( "socket" );
		
	//Setup server connection
	bzero( &servadd, sizeof( servadd ) );
	if((hp = gethostbyname( av[1] )) == NULL)	//get host ip
		perror(av[1]);
	bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(atoi(av[2]));	//Port num
	servadd.sin_family = AF_INET ;			//Socket type

	//Request to be added to the game
	if( connect(sock_id,(struct sockaddr *)&servadd, sizeof(servadd)) !=0)
		perror( "connect" );
	if(write(sock_id, "join", strlen("join")) != strlen("join"))
		perror("Join Write");
	if((messlen = read(sock_id, message, BUFSIZ)) < 0)
		perror("Client join read");
	message[messlen] = '\0';
	
	//Wait for player 2 if player 1
	if(strcmp(message, "1") == 0)
	{
		printf("\n\nWaiting for an opponent...\n");
		if((messlen = read(sock_id, message, BUFSIZ)) < 0)
			perror("Client wait read");
		message[messlen] = '\0';
		printf("\n%s, Player 1!\n", message);
	}
	

	//Setup screen for game
	initscr();
	noecho();
	terminalSetup(1);
	signal(SIGINT, quit);

	//Draws the border for the game
	for(x=4;x<=76;x++)	//Top and bottom
	{
		move(9, x);
		addstr("*");
		move(41, x);
		addstr("*");
	}
	for(x=9;x<=41;x++)	//Left and right
	{
		move(x, 4);
		addstr("*");
		move(x, 76);
		addstr("*");
	}

	//Loop for asking server for gamestate
	while(1)
	{	
		//Setup for select
		FD_ZERO(&readfds); FD_ZERO(&writefds); //Clear socket sets
		FD_SET(sock_id, &writefds);	//Add server to write set
		FD_SET(0, &readfds);		//Add stdin to read set
		if(sock_id > 0)
			maxfd = sock_id;
		else
			maxfd = 0;


		//See which fds are ready 
		activity = select(maxfd+1, &readfds, &writefds, NULL, 0);
		if (activity < 0 && errno != EINTR)
			perror("select error");

		
		//If user entered something:
		if(FD_ISSET(0, &readfds))
		{
			if((messlen = read(0, message, 255)) < 0)
				perror("stdin read");
			c = message[0];

			//Move paddle up
			if(c == 'o')
			{
				//Tell the server
				if(write(sock_id, "up", strlen("up")) != strlen("up"))
					perror("Client paddle up write");
			}
			//Move paddle down
			else if(c == 'l')
			{
				//Tell the server
				if(write(sock_id, "down", strlen("down")) != strlen("down"))
					perror("Client paddle down write");
			}
		}

		
		//Ask for gamestate
		if(write(sock_id, "state", strlen("state")) != strlen("state"))
			perror("Client gamestate request write");

		//Read in gamestate
		if((messlen = read(sock_id, message, BUFSIZ)) < 0)
			perror("Client gamestate read");
		message[messlen] = '\0';

		//First check to see if someone won the game, and display if they did
		if(strcmp(message, "win1") == 0)
		{
			move(8, 32);
			addstr("Press Q to quit");
			move(24, 10);
			addstr("Winner!!!!");
			points1++;
			sprintf(score1, "Score: %d", points1);
			move(8, 8);
			addstr(score1);
			move(LINES-1,COLS-1);
			refresh();
			while(c = getch())
			{
				if(c == 'q')
				{
					terminalSetup(0);
					endwin();
					exit(0);
				}
			}
		}
		else if(strcmp(message, "win2") == 0)
		{
			move(8, 32);
			addstr("Press Q to quit");
			move(24, 60);
			addstr("Winner!!!!");
			points2++;
			sprintf(score2, "Score: %d", points2);
			move(8, 64);
			addstr(score2);
			move(LINES-1,COLS-1);
			refresh();
			while(c = getch())
			{
				if(c == 'q')
				{
					terminalSetup(0);
					endwin();
					exit(0);
				}
			}
		}
		else	//If nobody won the game, generate updated graphics
		{
			//Update positions according to received packet
			tok = strtok(message, ",");
			xpos = atoi(tok);
			tok = strtok(NULL, ",");
			ypos = atoi(tok);
			tok = strtok(NULL, ",");
			paddle = atoi(tok);
			tok = strtok(NULL, ",");
			paddle2 = atoi(tok);
			tok = strtok(NULL, ",");
			points1 = atoi(tok);
			tok = strtok(NULL, ",");
			points2 = atoi(tok);
			tok = strtok(NULL, ",");
			tail1x = atoi(tok);
			tok = strtok(NULL, ",");
			tail1y = atoi(tok);
			tok = strtok(NULL, ",");
			tail2x = atoi(tok);
			tok = strtok(NULL, ",");
			tail2y = atoi(tok);
			tok = strtok(NULL, ",");
			tailflag = atoi(tok);

			//Updates screen
			sprintf(score1, "Score: %d", points1);
			move(8, 8);
			addstr(score1);
			sprintf(score2, "Score: %d", points2);
			move(8, 64);
			addstr(score2);
			move(ypos, xpos);
			addstr("0");

			//Draw tail if appropriate
			if(tailflag > 1)
			{
				move(tail1y, tail1x);
				addstr("o");
				move(tail2y, tail2x);
				addstr(".");
			}
			else if(tailflag == 1)
			{
				move(tail1y, tail1x);
				addstr("o");
			}

			//Draws paddle location
			drawpaddles(paddle, paddle2);

			refresh();

			//Erase old locations
			move(ypos, xpos);	
			addstr(" ");
			move(tail1y, tail1x);
			addstr(" ");
			move(tail2y, tail2x);
			addstr(" ");
			erasepaddles(paddle, paddle2);
		}	
	}
}
