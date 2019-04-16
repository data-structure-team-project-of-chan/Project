/*

  Authors (group members): Siyu Fu, Talal Al Mahrizi, Gabrielle Heughins
  Email addresses of group members: fsiyu2018@my.fit.edu,talmahrizi2017@my.fit.edu gheughins2017@my.fit.edu
  Group name:f

  Course:CSE 2010
  Section:14

  Description of the overall algorithm:
This project was to create a hangman player which
Initially took in a dictionary of words
Guess a letter 
Check to see if the letter is in the word
Allow only 6 wrong answers
Give back accuracy, time and memory


*/
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
int accuracy;
int total_words=0;
char guessed_letter;

/*Functions Prototypes*/
void create_file(int length_of_word,char *word_file);
void init_hangman_player(char* word_file);
void quicksort(letter_freqency *arr, int l, int r);
void initialize_map(int word_lenght);
void current_word(char current_word[], bool is_new_word, int hidden_word_length);
char guess_hangman_player(char* current_word, bool is_new_word);
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
	index_map--;

  }else
  {
		int hidden_word_length = strlen(current_word);
		initialize_map(hidden_word_length); 
		quicksort(map, 0, 25);
		index_map = 25;
		for(int i = 0; i < hidden_word_length; i++)
		{
			current_word[i] = '_';
		}
		current_word[hidden_word_length] = '\0';
  	guess = map[index_map].letter;
  }
  guessed_letter = guess;
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
	/*array of letters, to pass them to map array*/
	char letters[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
	for(int i = 0; i < 26; i++)
	{
		map[i].letter = letters[i]; //pass the letters from letters array to map.letter in the same order
		map[i].frequency = frequence_all[word_lenght - 1][i]; //pass the frequency from the array frequence_all[word_lenght - 1][i], to map.frequency in the same order. word_lenght - 1 becuase we only have to use the words of same length as hiddin word 
	}

}
/*
void current_word(char current_word[], bool is_new_word, int hidden_word_length)
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

*/
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
	int size = sizeof(current_word)/sizeof(current_word[0]);
	int hit = 0;
	for(int i = 0; i < size; i++)
		{
			if(current_word[i] == guessed_letter)
			{
				current_word[i] = guessed_letter;
				hit++;
			}
		}
	if(hit ==0){
		is_correct_guess = 0;
	}else{
		is_correct_guess=1;
	}
}