#include "hangmanPlayer.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
/*struct of a letter and frequency, to map each letter with it's freqency = how many time the letter appears*/
typedef struct letter_freqency
{
	char letter;
	int frequency;

}letter_freqency;

/*global vairables*/
int frequence_all[24][26]={0};
letter_freqency *map; //map pointer, it's an array of 26 letters and their freqency
int index_map; //to keep track of what letters we checked from the map array
int incorrectGuess=0;
int *accuracy;
int total_words=0;
char *hidden;
/*Functions Prototypes*/
void create_file(int length_of_word,char *word_file);
void init_hangman_player(char* word_file);
void quicksort(letter_freqency *arr, int l, int r);
void initialize_map(int word_lenght);
void current_word(char current_word[], bool is_new_word, int hidden_word_length, char hidden_word[]);
char guess_hangman_player(char* current_word, bool is_new_word);
void get_random_hidden_word(FILE *hidden_word_file, char file_name[], char hidden_word[]);

int main()
{
	int i, j;
	char filename[24];
	char hidden_word_file_name[50]; //stores the name of the hidden words file
	char hidden_word[50]; //stores the hidden word
	int hidden_word_length; //stores the length of the hidden word
	strcpy(filename, "data/data.txt");

	init_hangman_player(filename);
	/*
	check:
	for(i = 0; i < 24; i++)
	{		
		for(j = 0; j < 26; j++)
			printf(" %d ", frequence_all[i][j]);
		printf("\n"); 
	}
	*/

	FILE *hidden_word_file; //pointer to hidden words file
	
	strcpy(hidden_word_file_name, "hidden_word.txt"); //copy the file name tohidden_word_file_name
	/*get random word from hidden_word file*/
	get_random_hidden_word(hidden_word_file, hidden_word_file_name, hidden_word);
	//check://printf("hidden word = %s\n", hidden_word); //hidden word
	hidden_word_length = strlen(hidden_word);
	//check://printf("hidden_word_length = %d\n", hidden_word_length); //hidden word length
	
	initialize_map(hidden_word_length); 
	quicksort(map, 0, 25);
	char currentWord[hidden_word_length+1];
	bool is_new_word = 0;
	current_word(currentWord, is_new_word, hidden_word_length, hidden_word);
	/*
	check:
	printf("before sorting:\n");
	for(int i = 0; i < 26; i++)
	{
		printf("letter = %c, frequency = %d\n",map[i].letter, map[i].frequency);
	}
	
	printf("After sorting:\n");
	for(int i = 0; i < 26; i++)
	{
		printf("letter = %c, frequency = %d\n",map[i].letter, map[i].frequency);
	}
	*/

}


void create_file(int length_of_word,char *word_file)
{
	char filename[12], word[30];
	if(length_of_word < 10)
	{
		filename[0]=length_of_word+'0';
		filename[1]='.';
		filename[2]='t';
		filename[3]='x';
		filename[4]='t';
		filename[5]='\0';		
	}
	if(length_of_word>=10)
	{
		filename[0]=(length_of_word /10.0+'0');
		filename[1]=(length_of_word %10+'0');		
		filename[2]='.';
		filename[3]='t';
		filename[4]='x';
		filename[5]='t';
		filename[6]='\0';			
	}
			
	FILE *fp,*fp1;
	fp = fopen(word_file,"r");
	fp1 = fopen(filename,"w");
	
	int i, record, j, count=0;

    while(fscanf(fp,"%s",word) != EOF)
	{
		for(j = 0; j < length_of_word; j++)
		{
			if(word[j] >= 'A' && word[j] <= 'Z')
				word[j] = tolower(word[j]);
		}	
		if(strlen(word) == length_of_word)
		{
			//printf("word is:%s",word);
			fprintf(fp1, "%s\n", word);
			for(j = 0; j < length_of_word; j++)
			{			
				record = word[j] - 'a';			
				frequence_all[length_of_word-1][record]++;			
			}
		}	
	}
	fclose(fp);
	fclose(fp1);
} 
void init_hangman_player(char* word_file)
{
	int i,word_length,word_total = 0,longest_word = -1;
	char word[64];
	FILE *word_dictionary;
	word_dictionary = fopen(word_file,"r");

	while(fscanf(word_dictionary, "%s", word) != EOF)
	{
		word_length = strlen(word);
		word_total++;
		if(longest_word < word_length)
			longest_word = word_length;

	}
	printf("longest length is %d\n",longest_word);	
	fclose(word_dictionary);
	for(i = 1; i <= longest_word; i++)
		create_file(i, word_file);
	
}
/*This function will return a guessed letter and depending on if is_new_word is true or false we pick a char*/
char guess_hangman_player(char* current_word, bool is_new_word)
{
  char guess = ' ';
  
  if(is_new_word == 0)
  {
	guess = map[index_map].letter;

  }else
  {
  	index_map = 25; //reseting index to equal 25, to pick the letter with highest freqency from map array of struct
  	guess = map[index_map].letter;
  }
  
  return guess; //guessed letter
}

// Function to run quicksort on an array of integers
// l is the leftmost starting index, which begins at 0
// r is the rightmost starting index, which begins at array length - 1

void quicksort(letter_freqency *arr, int l, int r)
{
    // Base case: No need to sort arrays of length <= 1
    if (l >= r)
    {
        return;
    }   
    // Choose pivot to be the last element in the subarray
    int pivot = arr[r].frequency;
    // Index indicating the "split" between elements smaller than pivot and 
    // elements greater than pivot
    int cnt = l;
    // Traverse through array from l to r
    for (int i = l; i <= r; i++)
    {
        // If an element less than or equal to the pivot is found...
        if (arr[i].frequency <= pivot)
        {
            // Then swap arr[cnt] and arr[i] so that the smaller element arr[i] 
            // is to the left of all elements greater than pivot
            //swap(&arr[cnt], &arr[i]);
            letter_freqency temp;

			temp.frequency = arr[cnt].frequency;
			temp.letter = arr[cnt].letter;

			arr[cnt].frequency = arr[i].frequency;
			arr[cnt].letter = arr[i].letter;

			arr[i].frequency = temp.frequency;
			arr[i].letter = temp.letter;

            // Make sure to increment cnt so we can keep track of what to swap
            // arr[i] with
            cnt++;
        }
    }
    
    // NOTE: cnt is currently at one plus the pivot's index 
    // (Hence, the cnt-2 when recursively sorting the left side of pivot)
    quicksort(arr, l, cnt-2); // Recursively sort the left side of pivot
    quicksort(arr, cnt, r);   // Recursively sort the right side of pivot
}
/*this function maps each letter and it's freqency = number of times the letter appears in the ditionary*/
void initialize_map(int word_lenght)
{
	map = (letter_freqency*) malloc(sizeof(letter_freqency) * 26); //struct array size 26, index for each letter
	/*array of letters, to pass the to map array*/
	char letters[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
	for(int i = 0; i < 26; i++)
	{
		map[i].letter = letters[i]; //pass the letters from letters array to map.letter in the same order
		map[i].frequency = frequence_all[word_lenght - 1][i]; //pass the frequency from the array frequence_all[word_lenght - 1][i], to map.frequency in the same order. word_lenght - 1 becuase we only have to use the words of same length as hiddin word 
	}

}

void current_word(char current_word[], bool is_new_word, int hidden_word_length, char hidden_word[])
{

	index_map = 25;
	for(int i = 0; i < hidden_word_length; i++)
	{
		current_word[i] = '_';
	}
	current_word[hidden_word_length] = '\0';
	char guessed_letter;
	int hit = 0, miss = 0, x = 0;
	int is_guessed_letter_in_hidden_word;
	while(hit != hidden_word_length && miss != 6)
	{
		guessed_letter = guess_hangman_player(current_word, is_new_word);
		printf("gussed letter: %c\n", guessed_letter);
		is_guessed_letter_in_hidden_word = 0;
		x = 0;
		for(int i = 0; i < hidden_word_length; i++)
		{
			if(hidden_word[i] == guessed_letter)
			{
				current_word[i] = guessed_letter;
				hit++;
				is_guessed_letter_in_hidden_word++;
				if(x == 0)
				{
					printf("hit!!\n");
					x++;
				}

			}
		}
		if(is_guessed_letter_in_hidden_word == 0)
		{
			miss++;
			if(x == 0)
			{
				printf("miss!!\n");
			}			
		}

		index_map--;

		printf("current word is: %s\n", current_word);
	}

	printf("hit = %d\tmiss = %d\n", hit, miss);
	if(hit == hidden_word_length)
	{
		printf("You won!!\nThe hiiden word is %s\n", hidden_word);
	}
	if(miss == 6)
	{
		printf("You lost!!\nThe hiiden word is %s\n", hidden_word);
	}

}

/*this function read and returns a random word from hidden_word_file*/
void get_random_hidden_word(FILE *hidden_word_file, char file_name[], char hidden_word[])
{
	srand(time(NULL)); //to get random numbers
	hidden_word_file = fopen(file_name, "r"); //open hidden_word_file to read
	int size_of_hidden_word_file = 0; //how many lines in the hidden_word_file
	/*to get the number of lines in hidden_word_file*/
	while(!feof(hidden_word_file)) //while not end of file
	{
		fscanf(hidden_word_file, "%s", hidden_word); 
		size_of_hidden_word_file++; //incrment the size of size_of_hidden_word_file
	}

	int random_number = rand() % size_of_hidden_word_file + 1; //get a random number from 1 to the number of lines in hidden_word_file 
	fseek(hidden_word_file, 0 , SEEK_SET); //move cureser to begging

	int i = 1; //counter to check if reached the line of random number
	while(!feof(hidden_word_file) && i <= random_number)
	{
		fscanf(hidden_word_file, "%s", hidden_word); //read a word from each line, last word will be the hidden word
		i++;
	}
	strcpy(hidden, hidden_word);
	//checks://printf("size_of_hidden_word_file = %d\n", size_of_hidden_word_file); //test the length of the hidden words list
	//checks://printf("random_number = %d\n", random_number); //test random number
	fclose(hidden_word_file); //close file


}

// feedback on the guessed letter
// is_correct_guess: true if the guessed letter is one of the letters in the hidden word
// current_word: partially filled or blank word
//   
// Case       is_correct_guess    current_word   
// a.         true                partial word with the guessed letter
//                                   or the whole word if the guessed letter was the
//                                   last letter needed
// b.         false               partial word without the guessed letter
void feedback_hangman_player(bool is_correct_guess, char* current_word)
{
	if(is_correct_guess == 1){
		if(strcmp(current_word, hidden)==0){
			accuracy= (int *)malloc((total_words+1)*sizeof(int));
			accuracy[total_words] = (1-(incorrectGuess/6))*100;
			total_words++;
		}
	}else{
		incorrectGuess++;
	}

}
