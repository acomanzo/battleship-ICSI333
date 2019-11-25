/*
ICSI 333 Programming at the Hardware-Software Interface
Fall 2019
Friday 1:40pm
Evan Poon and Tony Comanzo
001324907 and 001381954

Program is that of the game Battleship. Game board is created and ships
are randomly placed onto the board. Prompts user for letter and number
input for coordinates. Prints whether or not it's a hit or a miss. Runs
until all available hit-spots are marked off. Creates a log file of all
taken and lists coordinates fired at, whether it was a hit or miss, and,
if it was a hit, what type of ship was hit.
*/

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

#define PORT "3490"  // the port users will be connecting to
#define BACKLOG 10   // how many pending connections queue will hold
#define MAXDATASIZE 100 // max number of bytes we can get at once

/*
Defines structure of node and elements of it
*/
struct node {
	char ycoord;		//Letter (row)
	char xcoord;		//Number (column)
	char hitormiss[7];	//Hit or miss
	char type[15];		//Ship type (if hit)
	struct node *next;	//Pointer to next node
};
struct node *head, *tail;		//Initialize pointers to head and tail of linked list

void sigchld_handler(int s) {
	(void)s; // quiet unused variable warning
	// waitpid() might overwrite errno, so we save and restore it
	int save_errno = errno;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = save_errno;
}

// get sockaddr, IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa) {
	// if the sockaddr is IP4
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void communicate_with_client(int sockfd) {
	/*char buff[MAX];
	int n;
	printf("tony");
	// initiate loop for chat
	while(1) {
		bzero(buff, MAX);

		// read the message
		read(sockfd, buff, sizeof(buff));
		// print buffer which contains the client contents
		printf("From client: %s\n To client: ", buff);
		bzero(buff, MAX);
		n = 0;
		// copy server message in the buffer
		while ((buff[n++] = getchar()) != '\n');

		// and send that buffer to client
		write(sockfd, buff, sizeof(buff));
	}*/
}

// open a socket to listen to the specified port. Print the address and port.
// Wait for a connection.
int connect_to_client() {
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	int numbytes;
  char buf[MAXDATASIZE];
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; // connector's address information
  socklen_t sin_size;
  struct sigaction sa;
  int yes = 1;
  char s[INET6_ADDRSTRLEN];
  int rv;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP
  if ((rv = getaddrinfo("127.0.0.1", PORT, &hints, &servinfo)) != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
  }
  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
    sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }

  freeaddrinfo(servinfo); // all done with this structure

  if (p == NULL)  {
    fprintf(stderr, "server: failed to bind\n");
    exit(1);
  }

  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  printf("server: waiting for connections...\n");

  while(1) {  // main accept() loop
    sin_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
      perror("accept");
      continue;
    }

    inet_ntop(their_addr.ss_family,
      get_in_addr((struct sockaddr *)&their_addr),
      s, sizeof s);
    printf("server: got connection from %s\n", s);
		break;
    /*if (!fork()) { // this is the child process
      close(sockfd); // child doesn't need the listener
      if (send(new_fd, "Hello, world!", 13, 0) == -1)
        perror("send");
      close(new_fd);
      exit(0);
    }*/


  }
	if (send(new_fd, "Hello, World!", 13, 0) == -1)
		perror("send");
	printf("hey there\n");

	if ((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1) {
  	perror("recv");
    exit(1);
  }
	buf[numbytes] = '\0';
	printf("took %s\n", buf);
	//close(new_fd);  // parent doesn't need this
	//return 0;
	printf("HEY\n");
	return new_fd;
}

/*
Initializes all variables for the game and creates the game board.
*/
char** initialize(char *stat, char let[], char num[], int *boats, char board[ROWS][COLUMNS], char **pt, struct node *h, struct node *t, struct node *nn, int *socket_fd) {
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

	printf("whoa\n");
	*socket_fd = connect_to_client();
	printf("bye\n");
	//communicate_with_client(sockfd);

	return pt;		//Return value of pt
}

/*
Updates the state of the coordinate to either hit or miss and checks if it was previously entered.
*/
void update(char *stat, char *let, char num[], int *boats, char board[ROWS][COLUMNS], struct node *h, struct node *nn) {
	int a = atoi(num);		//Converts value entered in number to integer
	int b;
	switch(*let) {		//Sets integer value of b based on value of letter
		case 'A':
			b = 0;
			break;
		case 'B':
			b = 1;
			break;
		case 'C':
			b = 2;
			break;
		case 'D':
			b = 3;
			break;
		case 'E':
			b = 4;
			break;
		case 'F':
			b = 5;
			break;
		case 'G':
			b = 6;
			break;
		case 'H':
			b = 7;
			break;
		case 'I':
			b = 8;
			break;
		case 'J':
			b = 9;
			break;
	}
	nn->ycoord = *let;		//New node takes input for letter as its y-coordinate
	nn->xcoord = *num;		//New node takes input for number as its x-coordinate
	if((board[b][a] == 'C' || board[b][a] == 'B' || board[b][a] == 'R' || board[b][a] == 'S' || board[b][a] == 'D')) { // Checks if pointer to letter and number are a vowel and even number
		switch(board[b][a]) {		//Decide what type of ship the new node has hit
			case 'C':
				strcpy(nn->type, "Carrier");
				break;
			case 'B':
				strcpy(nn->type, "Battleship");
				break;
			case 'R':
				strcpy(nn->type, "Cruiser");
				break;
			case 'S':
				strcpy(nn->type, "Submarine");
				break;
			case 'D':
				strcpy(nn->type, "Destroyer");
				break;
		}
		board[b][a] = hit;	//Changes character at this coordinate from '-' to 'X' (from blank to hit)
		*stat = hit;	//Sets character of state to hit
		*boats = *boats-1;	//Decrements count of ships if it's a hit
		strcpy(nn->hitormiss, "Hit");		//New node takes a hit value
	} else if(board[b][a] == '-') {		//Checks if value at this coordinate is blank
		board[b][a] = miss;		//Changes character at this coordinate from '-' to 'O' (from blank to miss)
		*stat = miss;	//Sets character of state to miss
		strcpy(nn->hitormiss, "Miss");		//New node takes a miss value
		strcpy(nn->type, "No ship");		//New node hasn't hit a ship
	} else if(board[b][a] == miss || board[b][a] == hit) {	//Checks is value at this coordinate is a hit or miss
		printf("You've already entered this coordinate.\n");
	}
}

/*
Prints the updated state of the coordinate.
*/
void printupdate(char *stat, int boats, char board[ROWS][COLUMNS]) {
	if(*stat == hit) {				//Checks if state is equal to hit
		printf("It's a HIT.\n");	//Prints if user hit a ship when state is 'X' (is hit)
		*stat = blank;				//Reverts state back to blank for next coordinate
	} else if(*stat == miss) {		//Checks if state is equal to miss
		printf("It's a MISS.\n");	//Prints if user misses a ship when state is 'O' (is miss)
		*stat = blank;
	}
	printf("Shots left to hit: %d\n", boats);	//Prints count of shots left to hit
	printf(" 0 1 2 3 4 5 6 7 8 9\n");	//Prints labels for columns
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			printf(" ");
			printf("%c", board[i][j]);	//Prints the values currently on the board
		}
		fputc('\n', stdout);
	}
}

/*
Inserts a new node at the end of a linked list.
*/
void insert_node(struct node **h, struct node **t, struct node *nn) {
	struct node *temp;		//Temporary node created
	if((temp = (struct node *)malloc(sizeof(struct node))) == NULL) {
		printf("Allocation failed.\n");
		exit(1);
	}
	temp->next = NULL;		//Initialize temp's next to null
	temp->ycoord = nn->ycoord;		//Temp's y-coordinate is coordinate of new node
	temp->xcoord = nn->xcoord;		//Temp's x-coordinate is coordinate of new node
	strcpy(temp->hitormiss, nn->hitormiss);		//Temp copies new node's hit or miss status
	strcpy(temp->type, nn->type);		//Temp copes new node's ship type hit
	if(*h == NULL) {
		*h = *t = temp;		//Makes head of linked list the temp node if head was NULL
	} else {
		(*t)->next = temp;		//Makes the tail's next node equal the temp node
		*t = (*t)->next;		//Makes the new tail equal the previous tail's next
	}
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

/*
Starts the program and runs functions.
*/
int main() {
	struct node *new_node;		//Node to take in data throughout Battleship
	new_node = (struct node *)malloc(sizeof(struct node));		//Allocates memory for new node
	char state;			//Initializes state to be blank
	char letter[3];		//Letter of coordinate user inputs
	char number[3];		//Number of coordinate user inputs
	int ships;				//Number of spots left to hit
	char gameboard[ROWS][COLUMNS];	//Initializes grid for board
	char **boardpt;				//Board pointer
	int socket_fd;
	int numbytes = -1;
	char col1 = 'A';
	char row1 = 'B';
	boardpt = initialize(&state, letter, number, &ships, gameboard, boardpt, head, tail, new_node, &socket_fd);
	while(ships != 0) {		// Runs until count of ships reaches 0
		//update(&state, letter, number, &ships, gameboard, head, new_node);
		//printupdate(&state, ships, gameboard);
		//insert_node(&head, &tail, new_node);

		if ((numbytes = recv(socket_fd, &col1, 1, 0)) == -1) {
	  	perror("recv");
	    exit(1);
	  }
		letter[numbytes] = '\0';
		printf("Received letter: %c...\n", col1);

		if ((numbytes = recv(socket_fd, &row1, 1, 0)) == -1) {
	  	perror("recv");
	    exit(1);
	  }
		number[numbytes] = '\0';
		printf("Received number: %c...\n", row1);

		ships--;
		numbytes = -1;
	}
	termination(boardpt, head, tail, new_node);
}
