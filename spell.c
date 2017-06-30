#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char line[100];
char answer [9][20];

int getLine(int limit) //gets and sets the kanji to test
{
	srand(time(NULL)); 
	int lineNumber;
    lineNumber = limit;
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
	int i;
	int j;
		for ( i = 0 ; i < 8 ; i++ ) {
			for ( j = 0 ; j < 20 ; j++ ) {
				answer[i][j] = '\0'; // clears out the arrays after each stage
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
	return(0);
}

int main () 
{
	int i;
	int count = 0;
	int system(const char *command);
	for ( i=0 ; i < 2043 ; i++ ) {
		count ++;
		test(i);
		char buffer[128];
		snprintf(buffer, sizeof(buffer), "trans ja:ja %s | grep -m 1 \\(.*\\) | cut -d \"(\" -f 2 | cut -d \")\" -f 1 >> spell.txt", answer[1]);
		printf("processing \t %d/2042 \t %d%% : %s \n",count, 100*count/2042, answer[1]);
		system(buffer);
	}
}
