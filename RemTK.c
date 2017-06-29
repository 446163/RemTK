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
	if ( file != NULL )
	{
	    while (fgets(line, sizeof line, file) != NULL)
	    {
	        if (count == lineNumber) // goes to the required line
	        {
	    		fclose(file); // reads the line into memory and then closes the file
				return(0);
	        }
	        else
	        {
	            count++;
	        }
	    }
	    fclose(file);
	}
	else
	{
		printf("the heisig-data text file does not exist\n"); // if the file is missing 
	}
	return(0);
}

int test(int range) // runs the actual test or quiz function 
{
	int score = 0;
	int total = 0;
	char input[20] = " "; // setting variables
	char answer [9][20];
	int i;
	int j;
	while (1) {
		for ( i = 0 ; i < 8 ; i++ ) {
			for ( j = 0 ; j < 20 ; j++ ) {
				answer[i][j] = '\0'; // clears out the arrays after each stage
				input[j] = '\0';
			}
		}
		getLine(range);
		i = -1;
		int answerOne=0; // resetting variables for the loop
		int answerTwo=0;
		while ( answerOne < 7 ) { // splits the random line in the file into a 2D array
			i++;
			if ( line[i] == ':' ) { // the elements of which are different information about the kanji
				answerOne ++;
				answerTwo = 0;
			} else {
				answer[answerOne][answerTwo] = line[i]; // see NOTE 1 for more infomation 
				answerTwo ++;
			}
		}
		printf("%s \t %d/%d\n", answer[1], score, total); // prints the kanji to test and the current score
		fgets(input, 20, stdin); // reading the answer
		if ( input[0] == '\n' ) { // to exit the test press enter with no input
			printf("you got %d%% correct.\n", 100*score/total); // the percentage of correct answers is shown
			return(1);
		}
		int fail = 0;
		for ( i = 0 ; i < 20 ; i ++ ){ // tests to see is the input exactly matches the answer from the file
			if ( input[i] != answer[2][i] && answer[2][i] != '\0') {
				fail = 1;
			}
		}
		total ++; // total (for the score) goes up when a test is done
		if ( fail == 1 ) {
			input[strlen(input)-1] = 0; // if you get the answer wrong then it will tell you 
			printf("incorrect\n\'%s\'✗\n\'%s\'✓\n",input,  answer[2]); // and give you the correct answer
		} else {
			score ++;
			printf("correct ✓ \n"); // if you get is right then you know and your score goes up
		}
	}
}

int main() // the main starting loop
{
	char * testvar; // used so crudley catch a stray newline
	printf("test up to number: "); // asks the level of the testing
	int num, nitems;
	nitems = scanf("%d", &num); // scans and tests the input, a bad input will end the program
	if (nitems == EOF) {
	} else if (nitems == 0) {
	} else {
		fgets(testvar, 2, stdin);
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
