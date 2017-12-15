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
	char stroke[2];
	int lesson;
} data;

typedef struct incorrecttemplate { // data type for showing mistakes in a test
	int count;
	char kanji [3];
	char keyword [20];
} incorrect;

typedef struct incorrectLessonTemplate { // data type for showing mistakes in a test
	int lesson;
	int count;
} incorrectLesson;

void clearInputBuffer( char c ) { // clears the read buffer of rouge characters
	while (c != '\n' && c != EOF) {
        c = getchar();
	}
}

data getLine(int limit, int offset, char lesson) { // function to return all data on a character in the data data type
	int lessonOffset[58][2] = {{1,15},{16,34},{35,54},{55,74},{75,98},{99,109},{110,133},{134,184},{185,206},{207,249},{250,264},{265,294},{295,320},{321,345},{346,376},{377,395},{396,422},{423,514},{515,545},{546,546},{547,548},{549,554},{555,620},{621,687},{688,829},{830,859},{860,958},{959,1023},{1024,1104},{1105,1124},{1125,1167},{1168,1206},{1207,1268},{1269,1305},{1306,1337},{1338,1390},{1391,1431},{1432,1497},{1498,1534},{1535,1596},{1597,1651},{1652,1711},{1712,1743},{1744,1777},{1778,1813},{1814,1846},{1847,1894},{1895,1914},{1915,1946},{1947,1970},{1971,1997},{1998,2025},{2026,2053},{2054,2077},{2078,2132},{2133,2162},{2163,2182},{2183,2201}};
	limit = limit - 1;
	char line[100]; // line maximum lengh
	data kanjidata;
	memset(kanjidata.keyword, '\0', sizeof kanjidata.keyword); // clears the data instance
	memset(kanjidata.kanji, '\0', sizeof kanjidata.kanji);
	memset(kanjidata.stroke, '\0', sizeof kanjidata.stroke);
	kanjidata.index = 0;
	kanjidata.lesson = 0;
	int i = 0;
	int lineNumber; 
	struct timeb tmb; // time used as seed for random selection 
	ftime(&tmb);
	if ( lesson == 'l' ) {
		lineNumber = ((tmb.millitm + time(NULL)) % (lessonOffset[limit][1]-lessonOffset[limit][0])) + lessonOffset[limit][0];
	} else {
    	lineNumber = (tmb.millitm + time(NULL)) % (limit + 1); // gets the random line in the range
	}
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
					kanjidata.stroke[j] = line[i];
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

void sort( incorrectLesson a[], int size ) {
	int i,j;
	incorrectLesson temp;
	for( i = 0; i < size; i++ ) {
		for( j = 0 ;j < size - 1; j++ ) {
			if( a[j].count > a[j+1].count ) {
				temp = a[j];
				a[j] = a[j+1];
				a[j+1] = temp;
			}
		}
	}
}

int singleTest ( int high, int mode, char lesson ) { // function for testing when you only display one option (normal and swap)
	incorrect incorrect[high]; // initiation of the incorrect answer list
	incorrectLesson incorrectLesson[56];
	memset(incorrect, '\0', sizeof(incorrect));
	memset(incorrectLesson, '\0', sizeof(incorrectLesson));
	int loop = 1;
	int count = 0;
	int correct = 0;
	while ( loop ) {
		int i = 0;
		int incorrectloop = 1;
		int charCatch = 0;
		char testData = 'n';
		data kanjidata = getLine(high, 0, lesson); // fills kanjidata with all the information about a character
		if ( mode == 1 || mode == 2 ) { // selects between normal and swap mode
			printf("\n%s  %d/%d  \n\n:", kanjidata.keyword, correct, count);
		} else {
			printf("\n%s  %d/%d  \n\n:", kanjidata.kanji, correct, count);
		}
		char c = getchar(); 
		if ( c == '\n' ) { // if the user enters nothing then end the test
			i = 0;
			printf("you got %d%% correct \nyou got the following wrong:\n", 100*correct/count); // display correct percentage 
			while ( incorrect[i].kanji[0] != '\0' ) {
				printf(" %d | %.3s | %s \n", incorrect[i].count, incorrect[i].kanji, incorrect[i].keyword); // display all the incorrect answers with how many times they got it incorrect
				i++;
			}
			if ( lesson != 'l' ) {
				printf("Lesson breakdown: \n");
				i = 0;
				while ( incorrectLesson[i].lesson != '\0' ) {
					i++;
				}
				sort(incorrectLesson, i);
				i = 0;
				while ( incorrectLesson[i].lesson != '\0' ) {
					if ( incorrectLesson[i].lesson > 9 ) {
						printf(" %d |", incorrectLesson[i].lesson);
					} else {
						printf(" %d  |", incorrectLesson[i].lesson);
					}
					if ( incorrectLesson[i].count > 9 ) {
						printf(" %d |", incorrectLesson[i].count);
					} else {
						printf(" %d  |", incorrectLesson[i].count);
					}
					if ( 100/count*incorrectLesson[i].count > 9 ){
						printf(" %d%% | ", 100/(count-correct)*incorrectLesson[i].count);
					} else {
						printf(" %d%%  | ", 100/(count-correct)*incorrectLesson[i].count);
					}
					for ( int a = 0; a < 100/(count-correct)*incorrectLesson[i].count; a++ ) {
						printf("=");
					}
					printf("\n");
					i++;
				}
			}
			return(1); // return to the main function
		}
		while ( 1 ) { // read all data until new line is hit
			if ( mode == 1 ){
				testData = kanjidata.kanji[i];
			} else if ( mode == 2 ) {
				testData = kanjidata.stroke[i];
			} else {
				testData = kanjidata.keyword[i];
			}
			if ( testData == '\0' && c == '\n' ) {
				break;
			}
			if ( c != testData ) { // if a single char is incorrect
				charCatch = 1;
				break;
			}
			i++;
			c = getchar();
		}
		clearInputBuffer(c);
		c = '\0';
		count++;
		if ( charCatch == 0 ) { // if all chars are correct 
			printf("\nCorrect \n");
			correct++;
			incorrectloop = 0;
		} else { // if at least one char is incorrect
			if ( mode == 1 ){ // print the correct answer
				printf("\n%s is the correct answer \n", kanjidata.kanji);
			} else if ( mode == 2 ) {
				printf("\n%s is the correct answer: %s \n", kanjidata.stroke, kanjidata.kanji);
			} else {
				printf("\n%s is the correct answer \n", kanjidata.keyword);
			}
			i = 0;
			while ( incorrectloop ) { // add the information of the incorrect answer to the incorrect array
				if ( incorrect[i].kanji[0] == kanjidata.kanji[0] && incorrect[i].kanji[1] == kanjidata.kanji[1] && incorrect[i].kanji[2] == kanjidata.kanji[2]) {
					incorrect[i].count++;
					break;
				} else if ( incorrect[i].kanji[0] == '\0' ) {
					for ( int j = 0; j < 3; j++ ) { // add the kanji character
						incorrect[i].kanji[j] = kanjidata.kanji[j];
					}
					for ( int j = 0; j < 20; j ++ ) { // add the keyword
						incorrect[i].keyword[j] = kanjidata.keyword[j];
					}
					incorrect[i].count = 1;
					break;
				} else {
					i++;
				}
			}
			i = 0;
			while ( incorrectloop ) {
				if ( incorrectLesson[i].lesson == kanjidata.lesson ) {
					incorrectLesson[i].count++;
					incorrectloop = 0;
				} else if ( incorrectLesson[i].lesson == '\0' ) {
					incorrectLesson[i].lesson = kanjidata.lesson;
					incorrectLesson[i].count = 1;
					incorrectloop = 0;
				} else {
					i++;
				}
			}
		}
	}
	return(0);
}

int multiTest ( int high, char lesson ) { // function for when you are testing with multiple options (multi)
	incorrect incorrect[high];
	incorrectLesson incorrectLesson[56];
	memset(incorrect, '\0', sizeof(incorrect));
	memset(incorrectLesson, '\0', sizeof(incorrectLesson));
	int loop = 1;
	int count = 0;
	int correct = 0;
	while ( loop ) {
		int incorrectloop = 1;
		int i = 0;
		data kanjidata1 = getLine(high, 0, lesson);
		usleep(1500);
		data kanjidata2 = getLine(high, 0, lesson); // gets the 3 different characters and information to display
		usleep(1500);
		data kanjidata = getLine(high, 0, lesson); // this is the correct one
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
			if ( lesson != 'l' ) {
				printf("Lesson breakdown: \n");
				i = 0;
				while ( incorrectLesson[i].lesson != '\0' ) {
					i++;
				}
				sort(incorrectLesson, i);
				i = 0;
				while ( incorrectLesson[i].lesson != '\0' ) {
					if ( incorrectLesson[i].lesson > 9 ) {
						printf(" %d |", incorrectLesson[i].lesson);
					} else {
						printf(" %d  |", incorrectLesson[i].lesson);
					}
					if ( incorrectLesson[i].count > 9 ) {
						printf(" %d |", incorrectLesson[i].count);
					} else {
						printf(" %d  |", incorrectLesson[i].count);
					}
					if ( 100/count*incorrectLesson[i].count > 9 ){
						printf(" %d%% | ", 100/(count-correct)*incorrectLesson[i].count);
					} else {
						printf(" %d%%  | ", 100/(count-correct)*incorrectLesson[i].count);
					}
					for ( int a = 0; a < 100/(count-correct)*incorrectLesson[i].count; a++ ) {
						printf("=");
					}
					printf("\n");
					i++;
				}
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
					break;
				} else if ( incorrect[i].kanji[0] == '\0' ) {
					for ( int j = 0; j < 3; j++ ) { // adds incorrect kanji
						incorrect[i].kanji[j] = kanjidata.kanji[j];
					}
					for ( int j = 0; j < 20; j ++ ) { // adds incorrect keyword
						incorrect[i].keyword[j] = kanjidata.keyword[j];
					}
					incorrect[i].count = 1;
					break;
				} else {
					i++;
				}
			}
			i = 0;
			while ( incorrectloop ) {
				if ( incorrectLesson[i].lesson == kanjidata.lesson ) {
					incorrectLesson[i].count++;
					incorrectloop = 0;
				} else if ( incorrectLesson[i].lesson == '\0' ) {
					incorrectLesson[i].lesson = kanjidata.lesson;
					incorrectLesson[i].count = 1;
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
	char inp = '\0';
	int high = 0;
	char lesson = '\0';
	char c = '\0';
	printf("(m)ultiple choice \n(r)everse \n(s)troke count \n(n)ormal \n:"); // ask what mode
	inp = getchar(); // store selection
	clearInputBuffer(c);
	printf("(l)esson test \n(n)ormal test \n:");
	lesson = getchar();
	clearInputBuffer(c);
	if ( lesson == 'l' ) {
		printf("Test what lesson? \n:");
	} else {
		printf("Test up to? \n:"); // ask what index the user want to test up to
	}
	scanf("%d", &high); // read in the int
	clearInputBuffer(c);
	if ( inp == 'r' ) {
		singleTest(high, 1, lesson); // swap test
	} else if ( inp == 'm' ) {
		multiTest(high, lesson); // multi test
	} else if ( inp == 's' ) {
		singleTest(high, 2, lesson);
	} else {
		singleTest(high, 0, lesson); // normal test
	}
	printf("Do you want to test again? (y)es or (n)o\n:"); // when the test is over ask if they want to test again;
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
