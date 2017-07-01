#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char line[100];

int getLine(int limit) //gets and sets the kanji to test
{
	srand(time(NULL)); 
	int lineNumber;
    lineNumber = rand() % (limit + 1); // gets the random line in the range
	static const char filename[] = "heisig-data.txt";
	FILE *file = fopen(filename, "r"); // opens the test file
	int count = 0;
	if ( file != NULL ) {
	    while ( fgets ( line, sizeof line, file ) != NULL ) {
	        if ( count == lineNumber ) { // goes to the required line
	    		fclose(file); // reads the line into memory and then closes the file
				return(0);
	        } else {
	            count++;
	        }
	    }
	    fclose(file);
	} else {
		printf("the heisig-data text file does not exist\n"); // if the file is missing 
	}
	return(0);
}

int test ( int range ) // runs the actual test or quiz function 
{
	char errors[range][3][20]; // used to keep track of the errors that the payer makes
	int score = 0;
	int total = 0;
	char input[20] = " "; // setting variables
	char answer [10][20];
	int i;
	int j;
	for ( i = 0 ; i < range ; i++ ) { // sets the errors array to cleared as they are only used once
		for ( j = 0 ; j < 20 ; j++ ) {
			errors[i][0][j] = '\0';
			errors[i][1][j] = 0;
			errors[i][2][j] = '\0';
		}
	}
	while ( 1 ) {
		for ( i = 0 ; i < 9 ; i++ ) {
			for ( j = 0 ; j < 20 ; j++ ) {
				answer[i][j] = '\0'; // clears out the arrays after each stage
				input[j] = '\0';
			}
		}
		getLine(range);
		i = 0;
		int answerOne = 0; // resetting variables for the loop
		int answerTwo = 0;
		while ( answerOne < 9 && line[i] != '\0') { // splits the random line in the file into a 2D array
			if ( line[i] == ':' ) { // the elements of which are different information about the kanji
				answerOne ++;
				answerTwo = 0;
			} else {
				answer[answerOne][answerTwo] = line[i]; // see NOTE 1 for more infomation 
				answerTwo ++;
			}
			i++;
		}
		printf("%s \t %d/%d\n \t %s", answer[1], score, total, answer[8]); // prints the kanji to test and the current score
		fgets(input, 20, stdin); // reading the answer
		if ( input[0] == '\n' ) { // to exit the test press enter with no input
			printf("you got %d%% correct.\nerror summary:\n", 100*score/total); // the percentage of correct answers is shown
			for ( i = 0 ; i < range ; i++ ) { // runs through all options for the errors array
				if ( errors[i][0][0] != '\0' ) { // only prints ones that have a value
						printf("%d : %s : %s \n", errors[i][1][0], errors[i][0], errors[i][2]); // shows the user what they got wrong, what the right answer is and how many times they got it wrong during the test
				}
			}
			return(1);
		}
		int fail = 0;
		for ( i = 0 ; i < 20 ; i++ ){ // tests to see is the input exactly matches the answer from the file
			if ( input[i] != answer[4][i] && answer[4][i] != '\0') {
				fail = 1;
			}
		}
		total ++; // total (for the score) goes up when a test is done
		if ( fail == 1 ) {
			input[strlen(input)-1] = 0; // if you get the answer wrong then it will tell you 
			printf("incorrect\n\'%s\'✗\n\'%s\'✓\n",input,  answer[4]); // and give you the correct answer
			int errorLoop = 1;
			int errorCount = 0;
			while ( errorLoop == 1 ) {
				if ( (errors[errorCount][0][0] == answer[1][0] && errors[errorCount][0][1] == answer[1][1] && errors[errorCount][0][1] == answer[1][1]) || errors[errorCount][1][0] == '\0' ){ // adds the kanji to the list if you got it wrong, and increments the number
					errorLoop = 0;
					errors[errorCount][0][0] = answer[1][0];
					errors[errorCount][0][1] = answer[1][1];
					errors[errorCount][0][2] = answer[1][2];
					errors[errorCount][1][0] ++;
					for ( i = 0 ; i < 20 ; i++ ) {
						errors[errorCount][2][i] = answer[2][i];
					}
					errorLoop = 0;
				}
				errorCount ++;
			}
		} else {
			score ++;
			printf("correct ✓ \n"); // if you get is right then you know and your score goes up
		}
	}
}

int main() // the main starting loop, that is basically the menu
{
	char testvar = '\0'; // used so crudley catch a stray newline
	printf("test up to number: "); // asks the level of the testing
	int num;
	int nitems;
	nitems = scanf("%d", &num); // scans and tests the input, a bad input will end the program
	if (nitems == EOF) {
	} else if (nitems == 0) {
	} else {
		fgets(&testvar, 2, stdin);
		test(num - 1); // calls the test with the level that is required
	}
}

/*
 *functions:
 * 	main - the starting function
 * 		input: 
 * 			none
 * 		output:
 * 			none
 * 		effects:
 * 			calls the test function and starts the program
 *
 * 	test - the main body of the program that tests the user
 * 		input:
 * 			range: 
 *				how far to test the user
 *		output:
 *			1 when finished
 *		effects:
 *			calls Getline
 *			displays the kanji
 *			compares input to answer
 *			awards points
 *	Getline - gets a line from the file containing all the infomation and reads it in to the program
 *		input: 
 * 			limit:
 *  			how far to read through the file as a mamimum according to the skill level of the user
 *  	output:
 *  		none
 *  	effects:
 *  		sets the global varible 'line' to have all the infomation to be tested
 *  		
 *NOTE 1:
 * 	answer[9] = [heisig number][kanji][keyword in the 3rd edition][keyword in the 4th edition][keyword in the 5th edition][stroke count][index ordinal][lesson number][empty for error catching]
 * 	additional information is stored here for future stuff to add
 */
