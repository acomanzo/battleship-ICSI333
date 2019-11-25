#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define hit 'X'				//Defines 'X' as value of hit
#define miss 'O'			//Defines 'O' as value of miss
#define blank '-'			//Defines '-' as value of blank
#define COLUMNS 10			//Defines number of columns
#define ROWS 10				//Defines number of rows

#define PORT "3490" // the port client will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once

struct node {
	char ycoord;		//Letter (row)
	char xcoord;		//Number (column)
	char hitormiss[7];	//Hit or miss
	char type[15];		//Ship type (if hit)
	struct node *next;	//Pointer to next node
};
struct node *head, *tail;		//Initialize pointers to head and tail of linked list

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void communicate_with_server(int sockfd) {
  /*char buff[MAX];
  int n;
	printf("brandonß");
  while(1) {
    bzero(buff, sizeof(buff));
    printf("Enter the string : ");
    n = 0;
    while ((buff[n++] = getchar()) != '\n');
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("From server : %s", buff);
    if ((strncmp(buff, "exit", 4)) == 0) {
      printf("Client exit...\n");
      break;
    }
  }*/
}

int connect_to_server(int *argc, char ***argv) {
	int sockfd, numbytes;
  char buf[MAXDATASIZE];
  struct addrinfo hints, *servinfo, *p;
  int rv;
  char s[INET6_ADDRSTRLEN];
  if (*argc != 2) {
    fprintf(stderr,"usage: client hostname\n");
    exit(1);
  }
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if ((rv = getaddrinfo((*argv)[1], PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and connect to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
      p->ai_protocol)) == -1) {
      perror("client: socket");
      continue;
    }

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      perror("client: connect");
      close(sockfd);
      continue;
    }

    break;
  }


  if (p == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    return 2;
  }

  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
  printf("client: connecting to %s\n", s);
  freeaddrinfo(servinfo); // all done with this structure
  if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
  	perror("recv");
    exit(1);
  }
	buf[numbytes] = '\0';
  printf("client: received '%s'\n",buf);

	if (send(sockfd, "poop", 4, 0) == -1)
		perror("send");
	printf("Sent poop;)\n");
  //close(sockfd);

  return sockfd;
}

/*
Initializes all variables for the game and creates the game board.
*/
char** initialize(char *stat, char let[], char num[], int *boats, char board[ROWS][COLUMNS], char **pt, struct node *h, struct node *t, struct node *nn, int *argc, char ***argv, int *socket_fd) {
	h = NULL;		//Initialize head to null
	t = NULL;		//Initialize tail to null
	nn = NULL;		//Initialize a new node to null
	h = (struct node *)malloc(sizeof(struct node));		//Allocates memory for head of linked list
	t = (struct node *)malloc(sizeof(struct node));		//Allocates memory for tail of linked list
	*stat = blank;	//Initializes state to be blank
	let = " ";		//Letter of coordinate user inputs
	num = " ";		//Number of coordinate user inputs
	*boats = 17;	//Number of spots left to hit
	srand(time(NULL));
	pt = (char **)malloc(ROWS * COLUMNS * sizeof(char *));	//Allocates memory for the board
	for (int i = 0; i < ROWS; i++) {		//Initializes rows
		for (int j = 0; j < COLUMNS; j++) {		//Initializaes columns
			board[i][j] = blank;	//Initializes value of whole board
		}
	}
	int direction;		//Direction of the boat (horizontal or vertical)
	int rowst;			//Start of ship on this row
	int columnst;		//Start of ship on this column
	int shipnumber;		//Ship number/type
	bool ship1 = false;		//False until Carrier is placed
	bool ship2 = false;		//False until Battleship is placed
	bool ship3 = false;		//False until Cruiser is placed
	bool ship4 = false;		//False unitl Submarine is placed
	bool ship5 = false;		//False until Destroyer is placed
	for(shipnumber = 1; shipnumber < 6; shipnumber++) {
		direction = rand() % 2;			//Determines direction randomly
		if(direction == 0) {			//Ship placed vertically
			if(shipnumber == 1) {	//Ship number 1 is a carrier (5 spaces)
				while(ship1 != true) {		//Runs until Carrier is placed
					columnst = rand() % 10;		//Column start position randomly chosen
					rowst = rand() % 10;		//Row start position randomly chosen
					if((rowst+1 < 10 && rowst+2 < 10 && rowst+3 < 10 && rowst+4 < 10) &&
            		(rowst-1 >= 0 && rowst-2 >= 0 && rowst-3 >= 0 && rowst-4 >= 0)) {
						if((board[rowst][columnst] == '-' && board[rowst+1][columnst] == '-' && board[rowst+2][columnst] == '-' && board[rowst+3][columnst] == '-' && board[rowst+4][columnst] == '-')) {
							board[rowst][columnst] = 'C';
							board[rowst+1][columnst] = 'C';
							board[rowst+2][columnst] = 'C';
							board[rowst+3][columnst] = 'C';
							board[rowst+4][columnst] = 'C';
							ship1 = true;
						}else if((board[rowst][columnst] == '-' && board[rowst-1][columnst] == '-' && board[rowst-2][columnst] == '-' && board[rowst-3][columnst] == '-' && board[rowst-4][columnst] == '-')) {
							board[rowst][columnst] = 'C';
							board[rowst-1][columnst] = 'C';
							board[rowst-2][columnst] = 'C';
							board[rowst-3][columnst] = 'C';
							board[rowst-4][columnst] = 'C';
							ship1 = true;
						}
					}
				}
			}else if(shipnumber == 2) {		//Ship number 2 is a battleship (4 spaces)
				while(ship2 != true) {		//Runs until Battleship is placed
					columnst = rand() % 10;		//Column start position randomly chosen
					rowst = rand() % 10;		//Row start position randomly chosen
					if((rowst+1 < 10 && rowst+2 < 10 && rowst+3 < 10) &&
            		(rowst-1 >= 0 && rowst-2 >= 0 && rowst-3 >= 0)) {
						if((board[rowst][columnst] == '-' && board[rowst+1][columnst] == '-' && board[rowst+2][columnst] == '-' && board[rowst+3][columnst] == '-')) {
							board[rowst][columnst] = 'B';
							board[rowst+1][columnst] = 'B';
							board[rowst+2][columnst] = 'B';
							board[rowst+3][columnst] = 'B';
							ship2 = true;
						}else if((board[rowst][columnst] == '-' && board[rowst-1][columnst] == '-' && board[rowst-2][columnst] == '-' && board[rowst-3][columnst] == '-')) {
							board[rowst][columnst] = 'B';
							board[rowst-1][columnst] = 'B';
							board[rowst-2][columnst] = 'B';
							board[rowst-3][columnst] = 'B';
							ship2 = true;
						}
					}
				}
			}else if(shipnumber == 3) {	//Ship number 3 is a cruiser (3 spaces)
				while(ship3 != true) {		//Runs until Cruiser is placed
					columnst = rand() % 10;		//Column start position randomly chosen
					rowst = rand() % 10;		//Row start position randomly chosen
					if((rowst+1 < 10 && rowst+2 < 10) &&
            		(rowst-1 >= 0 && rowst-2 >= 0)) {
						if((board[rowst][columnst] == '-' && board[rowst+1][columnst] == '-' && board[rowst+2][columnst] == '-')) {
							board[rowst][columnst] = 'R';
							board[rowst+1][columnst] = 'R';
							board[rowst+2][columnst] = 'R';
							ship3 = true;
						}else if((board[rowst][columnst] == '-' && board[rowst-1][columnst] == '-' && board[rowst-2][columnst] == '-')) {
							board[rowst][columnst] = 'R';
							board[rowst-1][columnst] = 'R';
							board[rowst-2][columnst] = 'R';
							ship3 = true;
						}
					}
				}
			}else if(shipnumber == 4) {	//Ship number 4 is a submarine (3 spaces)
				while(ship4 != true) {		//Runs until Submarine is placed
					columnst = rand() % 10;		//Column start position randomly chosen
					rowst = rand() % 10;		//Row start position randomly chosen
					if((rowst+1 < 10 && rowst+2 < 10) &&
            		(rowst-1 >= 0 && rowst-2 >= 0)) {
						if((board[rowst][columnst] == '-' && board[rowst+1][columnst] == '-' && board[rowst+2][columnst] == '-')) {
							board[rowst][columnst] = 'S';
							board[rowst+1][columnst] = 'S';
							board[rowst+2][columnst] = 'S';
							ship4 = true;
						}else if((board[rowst][columnst] == '-' && board[rowst-1][columnst] == '-' && board[rowst-2][columnst] == '-')) {
							board[rowst][columnst] = 'S';
							board[rowst-1][columnst] = 'S';
							board[rowst-2][columnst] = 'S';
							ship4 = true;
						}
					}
				}
			}else if(shipnumber == 5) {	//Ship number 5 is a destroyer (2 spaces)
				while(ship5 != true) {		//Runs until Destroyer is placed
					columnst = rand() % 10;		//Column start position randomly chosen
					rowst = rand() % 10;		//Row start position randomly chosen
					if((rowst+1 < 10) && (rowst-1 >= 0)) {
						if((board[rowst][columnst] == '-' && board[rowst+1][columnst] == '-')) {
							board[rowst][columnst] = 'D';
							board[rowst+1][columnst] = 'D';
							ship5 = true;
						}else if((board[rowst][columnst] == '-' && board[rowst-1][columnst] == '-')) {
							board[rowst][columnst] = 'D';
							board[rowst-1][columnst] = 'D';
							ship5 = true;
						}
					}
				}
			}
		}else if(direction == 1) {		//Ship placed horizontally
			if(shipnumber == 1) {	//Ship number 1 is a carrier (5 spaces)
				while(ship1 != true) {		//Runs until Carrier is placed
					columnst = rand() % 10;		//Column start position randomly chosen
					rowst = rand() % 10;		//Row start position randomly chosen
					if((columnst+1 < 10 && columnst+2 < 10 && columnst+3 < 10 && columnst+4 < 10) &&
            		(columnst-1 >= 0 && columnst-2 >= 0 && columnst-3 >= 0 && columnst-4 >= 0)) {
						if((board[rowst][columnst] == '-' && board[rowst][columnst+1] == '-' && board[rowst][columnst+2] == '-' && board[rowst][columnst+3] == '-' && board[rowst][columnst+4] == '-')) {
							board[rowst][columnst] = 'C';
							board[rowst][columnst+1] = 'C';
							board[rowst][columnst+2] = 'C';
							board[rowst][columnst+3] = 'C';
							board[rowst][columnst+4] = 'C';
							ship1 = true;
						}else if((board[rowst][columnst] == '-' && board[rowst][columnst-1] == '-' && board[rowst][columnst-2] == '-' && board[rowst][columnst-3] == '-' && board[rowst][columnst-4] == '-')){
							board[rowst][columnst] = 'C';
							board[rowst][columnst-1] = 'C';
							board[rowst][columnst-2] = 'C';
							board[rowst][columnst-3] = 'C';
							board[rowst][columnst-4] = 'C';
							ship1 = true;
						}
					}
				}
			}else if(shipnumber == 2) {		//Ship number 2 is a battleship (4 spaces)
				while(ship2 != true) {		//Runs until Battleship is placed
					columnst = rand() % 10;		//Column start position randomly chosen
					rowst = rand() % 10;		//Row start position randomly chosen
					if((columnst+1 < 10 && columnst+2 < 10 && columnst+3 < 10) &&
            		(columnst-1 >= 0 && columnst-2 >= 0 && columnst-3 >= 0)) {
						if((board[rowst][columnst] == '-' && board[rowst][columnst+1] == '-' && board[rowst][columnst+2] == '-' && board[rowst][columnst+3] == '-')) {
							board[rowst][columnst] = 'B';
							board[rowst][columnst+1] = 'B';
							board[rowst][columnst+2] = 'B';
							board[rowst][columnst+3] = 'B';
							ship2 = true;
						}else if((board[rowst][columnst] == '-' && board[rowst][columnst-1] == '-' && board[rowst][columnst-2] == '-' && board[rowst][columnst-3] == '-')){
							board[rowst][columnst] = 'B';
							board[rowst][columnst-1] = 'B';
							board[rowst][columnst-2] = 'B';
							board[rowst][columnst-3] = 'B';
							ship2 = true;
						}
					}
				}
			}else if(shipnumber == 3) {	//Ship number 3 is a cruiser (3 spaces)
				while(ship3 != true) {		//Runs until Cruiser is placed
					columnst = rand() % 10;		//Column start position randomly chosen
					rowst = rand() % 10;		//Row start position randomly chosen
					if((columnst+1 < 10 && columnst+2 < 10) &&
            		(columnst-1 >= 0 && columnst-2 >= 0)) {
						if((board[rowst][columnst] == '-' && board[rowst][columnst+1] == '-' && board[rowst][columnst+2] == '-')) {
							board[rowst][columnst] = 'R';
							board[rowst][columnst+1] = 'R';
							board[rowst][columnst+2] = 'R';
							ship3 = true;
						}else if((board[rowst][columnst] == '-' && board[rowst][columnst-1] == '-' && board[rowst][columnst-2] == '-')){
							board[rowst][columnst] = 'R';
							board[rowst][columnst-1] = 'R';
							board[rowst][columnst-2] = 'R';
							ship3 = true;
						}
					}
				}
			}else if(shipnumber == 4) {	//Ship number 4 is a submarine (3 spaces)
				while(ship4 != true) {		//Runs until Submarine is placed
					columnst = rand() % 10;		//Column start position randomly chosen
					rowst = rand() % 10;		//Row start position randomly chosen
					if((columnst+1 < 10 && columnst+2 < 10) &&
            		(columnst-1 >= 0 && columnst-2 >= 0)) {
						if((board[rowst][columnst] == '-' && board[rowst][columnst+1] == '-' && board[rowst][columnst+2] == '-')) {
							board[rowst][columnst] = 'S';
							board[rowst][columnst+1] = 'S';
							board[rowst][columnst+2] = 'S';
							ship4 = true;
						}else if((board[rowst][columnst] == '-' && board[rowst][columnst-1] == '-' && board[rowst][columnst-2] == '-')){
							board[rowst][columnst] = 'S';
							board[rowst][columnst-1] = 'S';
							board[rowst][columnst-2] = 'S';
							ship4 = true;
						}
					}
				}
			}else if((shipnumber == 5 && board[rowst][columnst] == '-')) {	//Ship number 5 is a destroyer (2 spaces)
				while(ship5 != true) {		//Runs until Destroyer is placed
					columnst = rand() % 10;		//Column start position randomly chosen
					rowst = rand() % 10;		//Row start position randomly chosen
					if((columnst+1 < 10) && (columnst-1 >= 0)) {
						if((board[rowst][columnst] == '-' && board[rowst][columnst+1] == '-')) {
							board[rowst][columnst] = 'D';
							board[rowst][columnst+1] = 'D';
							ship5 = true;
						}else if((board[rowst][columnst] == '-' && board[rowst][columnst-1] == '-')){
							board[rowst][columnst] = 'D';
							board[rowst][columnst-1] = 'D';
							ship5 = true;
						}
					}
				}
			}
		}
	}

	*socket_fd = connect_to_server(argc, argv);
	printf("bye\n");
	//communicate_with_server(sockfd);

	return pt;		//Return value of pt
}


/*
Accepts user input for a letter and number for coordinates to fire at.
*/
void input(char let[], char num[]) {
	printf("Enter a capital letter from A to J: \n");
	scanf("%s", let);		// Takes user input for a letter
	fflush(stdout);
	while (*let > 'J') {		// Runs if pointer to letter is greater than J
		printf("Not a valid letter. Enter another: \n");
		scanf("%s", let);	//Re-takes user input if first input was invalid
		fflush(stdout);
	}
	printf("Enter a number from 0 to 9: \n");
	scanf("%s", num);		// Takes user input for a number
	fflush(stdout);
	while (strlen(num) > 1) {		//Runs if number's length is greater than 1
		printf("Not a valid number. Enter another: \n");
		scanf("%s", num);		// Re-takes user input if first input was invalid
		fflush(stdout);
	}
	printf("You fired at coordinate: %s%s\n", let, num);		//Prints coordinates fired at
}

/*
Terminates the game and clears the board.
*/
void termination(char **pt, struct node *h, struct node *t, struct node *nn) {
	FILE *log;		//Pointer to file
	if((log = fopen("log.txt", "w")) == NULL) {
		printf("Can't open.\n");
	} else {
		fprintf(log, "LOG OF MOVES\n---------------------------------\n");
		while(h != NULL) {
			fprintf(log, "Fired at %c%c. %s - %s\n", h->ycoord, h->xcoord, h->hitormiss, h->type);		//Prints value of head into the file
			h = h->next;		//Moves head to next node in linked list
		}
		fprintf(log, "END OF GAME\n");
	}
	printf("You sunk all my Battleships!\n");	//Prints when game ends
	free(pt);		//Clears memory for the board
	free(h);		//Clears memory for the head of linked list
	free(t);		//Clears memory for the tail of linked list
	free(nn);		//Clears memory for the new node of linked list
}

/**
	Prompt's the user to enter a letter A through J that indicates the column
	of the coordinate that the user wants to shoot. Returns an integer value
	to represent that column.
**/
char get_col_coordinate(struct node *node_ptr) {
	// a char to represent the column letter
	char col;
	// a string to hold in the input
	char input[5];
	printf("Enter a column letter (A-J).");
	// an int to hold the length of the user's input
	int len = 0;

	fgets(input, 5, stdin);
	len = strlen(input);
	//flush the input buffer of extra characters
	/*if(len > 4) {
		while ((getchar()) != '\n');
	}*/
	while ((getchar()) != '\n');
	// if the user doesn't enter a single letter, print error
	if(len != 2) {
		printf("Invalid input. Try again by entering a single letter.\n");
		return get_col_coordinate(node_ptr);
	}

	// an int to hold the output
	char output = 0;
	col = input[0];
	switch(col) {
		case 'A' :
		case 'a' :
			node_ptr->xcoord = 'A';
			output = 1;
			break;
		case 'B' :
		case 'b' :
			node_ptr->xcoord = 'B';
			output = 2;
			break;
		case 'C' :
		case 'c' :
			node_ptr->xcoord = 'C';
			output = 3;
			break;
		case 'D' :
		case 'd' :
			node_ptr->xcoord = 'D';
			output = 4;
			break;
		case 'E' :
		case 'e' :
			node_ptr->xcoord = 'E';
			output = 5;
			break;
		case 'F' :
		case 'f' :
			node_ptr->xcoord = 'F';
			output = 6;
			break;
		case 'G' :
		case 'g' :
			node_ptr->xcoord = 'G';
			output = 7;
			break;
		case 'H' :
		case 'h' :
			node_ptr->xcoord = 'H';
			output = 8;
			break;
		case 'I' :
		case 'i' :
			node_ptr->xcoord = 'I';
			output = 9;
			break;
		case 'J' :
		case 'j' :
			node_ptr->xcoord = 'J';
			output = 10;
			break;
		default :
			printf("Invalid input. Try again with a letter A through J.\n");
			output = get_col_coordinate(node_ptr);
			break;
	}

	//node_ptr->col = output;
	return col;
}

/**
	Prompt's the user to enter a number 0 through 9 that indicates the row
	of the coordinate that the user wants to shoot. Returns an integer value
	to represent that row.
**/
char get_row_coordinate(struct node *node_ptr) {
	// an int to represent the row
	int rowww = 0;
	// a char to hold the user input
	char input[5];
	printf("Enter a row number (0-9).\n");
	// an int to hold the length of the user's input
	int len = 0;

	fgets(input, 5, stdin);
	len = strlen(input);
	// flush the input buffer of extra characters
	if(len >= 4) {
		while ((getchar()) != '\n');
	}
	// if the user doesn't enter a single digit number, print error
	if(len != 2) {
		printf("Invalid input. Try again by entering a single digit.\n");
		return get_row_coordinate(node_ptr);
	}

	// an int to hold the output
	char output = 0;
	rowww = input[0];
	switch(rowww) {
		case '0' :
			output = 1;
			break;
		case '1' :
			output = 2;
			break;
		case '2' :
			output = 3;
			break;
		case '3' :
			output = 4;
			break;
		case '4' :
			output = 5;
			break;
		case '5' :
			output = 6;
			break;
		case '6' :
			output = 7;
			break;
		case '7' :
			output = 8;
			break;
		case '8' :
			output = 9;
			break;
		case '9' :
			output = 10;
			break;
		default :
			printf("Invalid input. Try again.\n");
			output = get_row_coordinate(node_ptr);
			break;
	}

	node_ptr->ycoord = output - 1;
	return rowww;
}

/*
Starts the program and runs functions.
*/
int main(int argc, char *argv[]) {
	struct node *new_node;		//Node to take in data throughout Battleship
	new_node = (struct node *)malloc(sizeof(struct node));		//Allocates memory for new node
	char state;			//Initializes state to be blank
	char letter[2];		//Letter of coordinate user inputs
	char number[2];		//Number of coordinate user inputs
	int ships;				//Number of spots left to hit
	char gameboard[ROWS][COLUMNS];	//Initializes grid for board
	char **boardpt;				//Board pointer
	int socket_fd;
	boardpt = initialize(&state, letter, number, &ships, gameboard, boardpt, head, tail, new_node, &argc, &argv, &socket_fd);
	while(ships != 0) {		// Runs until count of ships reaches 0
		*letter = get_col_coordinate(new_node);
		*number = get_row_coordinate(new_node);

		if (send(socket_fd, letter, 1, 0) == -1) {
			perror("send");
			exit(1);
		}
		printf("Sent letter: %c...\n", *letter);

		if (send(socket_fd, number, 1, 0) == -1) {
			perror("send");
			exit(1);
		}
		printf("Sent number: %c...\n", *number);

		ships--;
	}
	termination(boardpt, head, tail, new_node);
}
