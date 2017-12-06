#include <sys/timeb.h> 
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

typedef struct datatemplate { // data type for all the information of a character
	char keyword [20];
	char kanji [3];
	int index;
	int stroke;
	int lesson;
} data;

typedef struct incorrecttemplate { // data type for showing mistakes in a test
	int count;
	char kanji [3];
	char keyword [20];
} incorrect;

void clearInputBuffer( char c ) { // clears the read buffer of rouge characters
	while (c != '\n' && c != EOF) {
        c = getchar();
	}
}

data getLine(int limit) { // function to return all data on a character in the data data type
	limit = limit - 1;
	char line[100]; // line maximum lengh
	data kanjidata;
	memset(kanjidata.keyword, '\0', sizeof kanjidata.keyword); // clears the data instance
	memset(kanjidata.kanji, '\0', sizeof kanjidata.kanji);
	kanjidata.index = 0;
	kanjidata.stroke = 0; // sets all value int he data instance to 0
	kanjidata.lesson = 0;
	int i = 0;
	int lineNumber; 
	struct timeb tmb; // time used as seed for random selection 
	ftime(&tmb);
    lineNumber = (tmb.millitm + time(NULL))  % (limit + 1); // gets the random line in the range
	static const char filename[] = "new-heisig-data.txt";
	FILE *file = fopen(filename, "r"); // opens the test file
	int count = 0;
	if ( file != NULL ) { // if the file exists
	    while ( fgets ( line, sizeof line, file ) != NULL ) {
	        if ( count == lineNumber ) { // goes to the required line
				int j = 0;
				while ( line[i] != ';' ) { // reads the keyword into the data instance
					kanjidata.keyword[j] = line[i];
					i++;
					j++;
				}
				i = i+2;
				j = 0;
				while ( line[i] != ';' ) { // reads the kanji character into the data instance
					kanjidata.kanji[j] = line[i];
					i++;
					j++;
				}
				i = i+2;
				j = 0;
				while ( line[i] != ';') { // reads the index number into the data instance and converts it to an int
					kanjidata.index = (10*kanjidata.index)+(line[i]-'0');
					i++;
					j++;
				}
				i = i+2;
				j = 0;
				while ( line[i] != ';') { // reads the stroke number into the data instance and converts it to an int

					kanjidata.stroke = 10*kanjidata.stroke+line[i]-'0';
					i++;
					j++;
				}
				i = i+2;
				j = 0;
				while ( line[i] != '\n') { // reads the lesson number into the data instance and converts it to an int
					kanjidata.lesson = 10*kanjidata.lesson+line[i]-'0';
					i++;
					j++;
				}
	    		fclose(file); // reads the line into memory and then closes the file
				return(kanjidata); // returns the data instance to the call
	        } else {
	            count++;
	        }
	    }
	    fclose(file);
	} else {
		printf("the heisig-data text file does not exist\n"); // if the file is missing 
	}
	return(kanjidata); // returns an empty instance if no file found
}

int singleTest ( int high, int mode ) { // function for testing when you only display one option (normal and swap)
	incorrect incorrect[high]; // initiation of the incorrect answer list
	memset(incorrect, '\0', sizeof(incorrect[high]));
	int loop = 1;
	int count = 0;
	int correct = 0;
	while ( loop ) {
		int i = 0;
		int incorrectloop = 1;
		int charCatch = 0;
		char testData = '\0';
		data kanjidata = getLine(high); // fills kanjidata with all the information about a character
		if ( mode ) { // selects between normal and swap mode
			printf("\n%s \t %d/%d \n\n:", kanjidata.keyword, correct, count);
		} else {
			printf("\n%s \t %d/%d \n\n:", kanjidata.kanji, correct, count);
		}
		char c = getchar(); 
		if ( c == '\n' ) { // if the user enters nothing then end the test
			i = 0;
			printf("you got %d%% correct \nyou got the following wrong:\n", 100*correct/count); // display correct percentage 
			while ( incorrect[i].kanji[0] != '\0' ) {
				printf(" %d | %.3s | %s \n", incorrect[i].count, incorrect[i].kanji, incorrect[i].keyword); // display all the incorrect answers with how many times they got it incorrect
				i++;
			}
			return(1); // return to the main function
		}
		while ( c != '\n' ) { // read all data until new line is hit
			if ( mode ){
				testData = kanjidata.kanji[i];
			} else {
				testData = kanjidata.keyword[i];
			}
			if ( c != testData ) { // if a single char is incorrect
				charCatch = 1;
			}
			i++;
			c = getchar();
		}
		count++;
		if ( charCatch == 0 ) { // if all chars are correct 
			printf("\nCorrect \n");
			correct++;
			incorrectloop = 0;
		} else { // if at least one char is incorrect
			if ( mode ){ // print the correct answer
				printf("\n%s is the correct answer \n", kanjidata.kanji);
			} else {
				printf("\n%s is the correct answer \n", kanjidata.keyword);
			}
			i = 0;
			while ( incorrectloop ) { // add the information of the incorrect answer to the incorrect array
				if ( incorrect[i].kanji[0] == kanjidata.kanji[0] && incorrect[i].kanji[1] == kanjidata.kanji[1] && incorrect[i].kanji[2] == kanjidata.kanji[2]) {
					incorrect[i].count++;
					incorrectloop = 0;
				} else if ( incorrect[i].kanji[0] == '\0' ) {
					for ( int j = 0; j < 3; j++ ) { // add the kanji character
						incorrect[i].kanji[j] = kanjidata.kanji[j];
					}
					for ( int j = 0; j < 20; j ++ ) { // add the keyword
						incorrect[i].keyword[j] = kanjidata.keyword[j];
					}
					incorrect[i].count = 1;
					incorrectloop = 0;
				} else {
					i++;
				}
			}
		}
	}
	return(0);
}

int multiTest ( int high ) { // function for when you are testing with multiple options (multi)
	incorrect incorrect[high];
	memset(incorrect, '\0', sizeof(incorrect));
	int loop = 1;
	int count = 0;
	int correct = 0;
	while ( loop ) {
		int incorrectloop = 1;
		int i = 0;
		data kanjidata1 = getLine(high);
		usleep(1500);
		data kanjidata2 = getLine(high); // gets the 3 different characters and information to display
		usleep(1500);
		data kanjidata = getLine(high); // this is the correct one
		struct timeb tmb; 
		ftime(&tmb);
		int choice = (tmb.millitm % 3) + 1; // select what space to place the correct one in
		printf("\n%s \t %d/%d\n", kanjidata.keyword, correct, count); // display the keyword
		if ( choice == 1 ) { // place the correct kanji in the first place
			printf("\n[%s]\t[%s]\t[%s]\n", kanjidata.kanji, kanjidata2.kanji, kanjidata1.kanji);
		} else if ( choice == 2 ) { // place the correct kanji in the second place
			printf("\n[%s]\t[%s]\t[%s]\n", kanjidata1.kanji, kanjidata.kanji, kanjidata2.kanji);
		} else if (choice == 3 ){ // place the correct kanji in the third place
			printf("\n[%s]\t[%s]\t[%s]\n", kanjidata2.kanji, kanjidata1.kanji, kanjidata.kanji);
		} 
		printf(" 1 \t 2 \t 3\n\n:"); // display the reference numbers
		char c = getchar();
		if ( c == '\n' ) { // when the user enters nothing then stop testing
			i = 0;
			printf("you got %d%% correct \nyou got the following wrong:\n", 100*correct/count); // displays correct percentage
			while ( incorrect[i].kanji[0] != '\0' ) {
				printf(" %d | %.3s | %s \n", incorrect[i].count, incorrect[i].kanji, incorrect[i].keyword); // displays all of the incorrect answers
				i++;
			}
			return(1);
		}
		count++;
		if ( c-'0' == choice ) { // compares input (char) to choice (int)
			printf("\nCorrect \n"); // if the user gets it right
			correct++;
			incorrectloop = 0;
		} else {
			printf("\n%d[%s] is the correct answer \n", choice, kanjidata.kanji);
		}
			i = 0;
			while ( incorrectloop ) { // adds incorrect answers to list
				if ( incorrect[i].kanji[0] == kanjidata.kanji[0] && incorrect[i].kanji[1] == kanjidata.kanji[1] && incorrect[i].kanji[2] == kanjidata.kanji[2]) {
					incorrect[i].count++;
					incorrectloop = 0;
				} else if ( incorrect[i].kanji[0] == '\0' ) {
					for ( int j = 0; j < 3; j++ ) { // adds incorrect kanji
						incorrect[i].kanji[j] = kanjidata.kanji[j];
					}
					for ( int j = 0; j < 20; j ++ ) { // adds incorrect keyword
						incorrect[i].keyword[j] = kanjidata.keyword[j];
					}
					incorrect[i].count = 1;
					incorrectloop = 0;
				} else {
					i++;
				}
			}
		clearInputBuffer(c);
	}
	return(0);
}

int main() { // starts the appropriate testing and contains the menu
	srand(time(0)); // use the time as a seed for randomness
	char inp;
	char c = '0';
	int high = 0;
	printf("(m)ultiple choice \n(S)wap \n(n)ormal \n:"); // ask what mode
	inp = getchar(); // store selection
	printf("Test up to? \n:"); // ask what index the user want to test up to
	scanf("%d", &high); // read in the int
	clearInputBuffer(c);
	if ( inp == 's' ) {
		singleTest(high, 1); // swap test
	} else if ( inp == 'm' ) {
		multiTest(high); // multi test
	} else {
		singleTest(high, 0); // normal test
	}
	printf("Do you want to test again? (y)es or (n)o\n:"); // when the test is over ask if they want to test again
	inp = '\0';
	inp = getchar();
	clearInputBuffer(inp);
	if ( inp == 'y' ) {
		main(); // if yes, run the program again 
		return 0;
	}
}

/*
 * functions:
 * 	clearInputBuffer
 * 		input
 * 			char c - to clear currenly read input as well
 * 		output
 * 			nothing
 * 		process
 * 			consumes all of the input buffer for the program, eliminating rouge newlines mostly
 * 	getLine
 * 		input
 * 			high - the distance through the kanji you want to go (index number)
 * 		output
 * 			data struct containing all the data for a character
 * 		process
 * 			reads the text file and sorts all the sections into the data struct
 * 	singleTest
 * 		input
 * 			high - needed to call getLine with the appropriate kanji depth
 * 		output
 * 			int - used to end the function when the test is done
 * 		process
 * 			shows the user a single output, user has to input the corresponding data, keeps track of correct percentage and incorrect characters
 * 	multiTest
 * 		input
 * 			high - the distance through the kanji you want to go (index number)
 * 		output
 * 			int - used to end the function when the test is done
 * 		process
 * 			shows the user 3 different kanji and a keyword as an output, user inputs the location of the correct kanji, keeps track of correct percentage and incorrect characters
 * 	main
 * 		input
 * 			none
 * 		output
 * 			int - program ran successfully or not
 * 		process
 * 			provided a menu to lauch the selected testing type and allows retesting without exiting the program
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
