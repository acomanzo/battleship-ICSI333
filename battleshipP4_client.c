/*
Initializes all variables for the game and creates the game board.
*/
char** initialize(char *stat, char let[], char num[], int *boats, char board[row][column], char **pt, struct node *h, struct node *t, struct node *nn) {
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
	pt = (char **)malloc(row * column * sizeof(char *));	//Allocates memory for the board
	for (int i = 0; i < row; i++) {		//Initializes rows
		for (int j = 0; j < column; j++) {		//Initializaes columns
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
	return pt;		//Return value of pt
}


/*
Accepts user input for a letter and number for coordinates to fire at.
*/
void input(char let[], char num[]) {
	printf("Enter a capital letter from A to J: \n");
	scanf("%s", let);		// Takes user input for a letter
	while (*let > 'J') {		// Runs if pointer to letter is greater than J
		printf("Not a valid letter. Enter another: \n");
		scanf("%s", let);	//Re-takes user input if first input was invalid
	}
	printf("Enter a number from 0 to 9: \n");
	scanf("%s", num);		// Takes user input for a number
	while (strlen(num) > 1) {		//Runs if number's length is greater than 1
		printf("Not a valid number. Enter another: \n");
		scanf("%s", num);		// Re-takes user input if first input was invalid
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
