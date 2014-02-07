
#include <string.h>
#include <stdlib.h>
#include "../../include/c_array.h"
#include "../../include/backward_search.h"
/*
 * backward_search.c
 *
 *  Created on: 30.1.2014
 *      Author: lvapaaka
 */

int rank(const int index, const char c, const char* string){
	int i, count = 0;
	if(index <= 0){
		return 0;
	}

	for(i = index - 1; i >= 0; i--){
		if(string[i] == c){
			count++;
		}
	}
	return count;
}

int get_char_index(int* c_array, char* alphabet, char c);

Interval* backward_search(const char* bwt, const char* string)
{
	int i = 0;
	int j = strlen(bwt)-1;
	char* alphabet = get_alphabet(bwt);
	int* c_array = create_c_array(bwt);
	int k;
	for(k = strlen(string)-1; k>=0; k--){
		if(j<i){
			return NULL;
		}
		i = get_char_index(c_array, alphabet, string[k])+rank(i, string[k], bwt);
		j = get_char_index(c_array, alphabet, string[k])+rank(j+1, string[k], bwt)-1;
	}
	Interval* new_interval = malloc(sizeof(Interval));
	new_interval->i = i;
	new_interval->j = j;
	return new_interval;		
}

int get_char_index(int* c_array, char* alphabet, char c)
{
	int i;
	for(i=0;i<strlen(alphabet);i++){
		if(alphabet[i] == c){
			return c_array[i];
		}
	}
	return -1;
}
	