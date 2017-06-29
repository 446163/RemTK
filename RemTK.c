#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char line[100];

int getLine(int limit) 
{
	srand(time(NULL)); 
	int lineNumber;
    lineNumber = rand() % (limit + 1);
	static const char filename[] = "heisig-data.txt";
	FILE *file = fopen(filename, "r");
	int count = 0;
	if ( file != NULL )
	{
	    while (fgets(line, sizeof line, file) != NULL)
	    {
	        if (count == lineNumber)
	        {
	    		fclose(file);
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
		printf("the heisig-data text file does not exist\n");
	}
	return(0);
}

int test(int range)
{
	int score = 0;
	int total = 0;
	char input[20] = " ";
	char answer [9][20];
	int i;
	int j;
	while (input[0] != '0') {
		for ( i = 0 ; i < 8 ; i++ ) {
			for ( j = 0 ; j < 20 ; j++ ) {
				answer[i][j] = '\0';
				input[j] = '\0';
			}
		}
		getLine(range);
		i = -1;
		int answerOne=0;
		int answerTwo=0;
		while ( answerOne < 7 ) {
			i++;
			if ( line[i] == ':' ) {
				answerOne ++;
				answerTwo = 0;
			} else {
				answer[answerOne][answerTwo] = line[i];
				answerTwo ++;
			}
		}
		printf("%s \t %d/%d\n", answer[1], score, total);
		fgets(input, 20, stdin);
		if ( input[0] == '\n' ) {
			printf("you got %d%% correct.\n", 100*score/total);
			return(1);
		}
		int fail = 0;
		for ( i = 0 ; i < 20 ; i ++ ){
			if ( input[i] != answer[2][i] && answer[2][i] != '\0') {
				fail = 1;
			}
		}
		total ++;
		if ( fail == 1 ) {
			input[strlen(input)-1] = 0; 
			printf("incorrect\n\'%s\'✗\n\'%s\'✓\n",input,  answer[2]);
		} else {
			score ++;
			printf("correct ✓ \n");
		}
	}
	return(1);
}

int main()
{
	char * testvar;
	printf("test up to number: ");
	int num, nitems;
	nitems = scanf("%d", &num);
	if (nitems == EOF) {
	} else if (nitems == 0) {
	} else {
		fgets(testvar, 2, stdin);
		test(num - 1);
	}
}
