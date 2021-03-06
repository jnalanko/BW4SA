#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include "utils_for_tests.h"

char* generate_random_string(char* alphabet, int length) {
	int alphabet_size = strlen(alphabet);
	char* string = calloc(length + 1, sizeof(char));

	for (int i = 0; i < length; i++) {
		string[i] = alphabet[rand() % alphabet_size];
	}
	return string;
}

int two_substrings_right_maximal(char* string, int string_length,
		int first_index, int second_index, int substr_length);

int list_contains_substr(char* string, test_substr* head, int start, int length);

int two_substrings_equal_not_same(char* string, int string_length,
		int first_index, int second_index, int substr_length) {

	if (first_index == second_index) {
		return 0;
	}
	if (first_index + substr_length > string_length
			|| second_index + substr_length > string_length) {

		return 0;
	}
	for (int i = 0; i < substr_length; i++) {

		if (string[first_index + i] != string[second_index + i]) {
			return 0;
		}
	}
	return 1;
}

int two_substrings_left_maximal(char* string, int string_length,
		int first_index, int second_index, int substr_length) {

	if (!two_substrings_equal_not_same(string, string_length, first_index,
			second_index, substr_length)) {
		return 0;
	}
	if (first_index == 0 || second_index == 0) {
		return 1;
	}
	if (string[first_index - 1] != string[second_index - 1]) {
		return 1;
	}
	return 0;
}

int two_substrings_right_maximal(char* string, int string_length,
		int first_index, int second_index, int substr_length) {

	if (!two_substrings_equal_not_same(string, string_length, first_index,
			second_index, substr_length)) {
		return 0;
	}
	if (first_index + substr_length >= string_length
			|| second_index + substr_length >= string_length) {
		return 1;
	}
	if (string[first_index + substr_length]
			!= string[second_index + substr_length]) {
		return 1;
	}
	return 0;
}

int is_substring_right_maximal(char* string, int string_length,
		int substr_start, int substr_length) {
	for (int i = 0; i + substr_length <= string_length; i++) {
		if (two_substrings_right_maximal(string, string_length, substr_start, i,
				substr_length)) {
			return 1;
		}
	}
	return 0;
}

int is_substring_max_repeat(char* string, int string_length, int substr_start,
		int substr_length) {

	int found = 0;
	for (int i = 0; i + substr_length <= string_length; i++) {
		if (two_substrings_right_maximal(string, string_length, substr_start, i,
				substr_length)) {
			found = 1;
			break;
		}
	}
	if(!found){
		return 0;
	}
	for (int i = 0; i + substr_length <= string_length; i++) {
			if (two_substrings_left_maximal(string, string_length,
							substr_start, i, substr_length)) {
				return 1;
			}
		}
	return 0;
}

void print_substr(char* string, int start_index, int length) {
	for (int i = 0; i < length; i++) {
		printf("%c", string[start_index + i]);
	}
}

test_substr* find_maximal_repeat_substrings(char* string, int threshold) {

	test_substr* current = calloc(1, sizeof(test_substr));
	test_substr* head = current;
	test_substr* new;

	int string_length = strlen(string);

	for (int length = threshold; length < string_length; length++) {
		for (int i = 0; i + length <= string_length; i++) {

			if (is_substring_max_repeat(string, string_length, i, length)) {

				new = calloc(1, sizeof(test_substr));
				new->start_index = i;
				new->length = length;
				current->next = new;
				current = new;
			}
		}
	}
	return head;
}
test_substr* find_right_maximal_substrings(char* string) {

	test_substr* current = calloc(1, sizeof(test_substr));
	test_substr* head = current;
	test_substr* new;

	int string_length = strlen(string);

	for (int length = 1; length < string_length; length++) {
		for (int i = 0; i + length <= string_length; i++) {

			if (is_substring_right_maximal(string, string_length, i, length)) {

				new = calloc(1, sizeof(test_substr));
				new->start_index = i;
				new->length = length;
				current->next = new;
				current = new;
			}
		}
	}
	return head;
}

int two_substrings_equal(char* string, int start1, int start2, int length) {
	for (int i = 0; i < length; i++) {
		if (string[start1 + i] != string[start2 + i]) {
			return 0;
		}
	}
	return 1;
}
int list_contains_substr(char* string, test_substr* head, int start, int length) {

	test_substr* current = head->next;
	while (current != NULL) {
		if (current->length == length) {
			if (current->start_index == start) {
				return 1;
			}
		}
		current = current->next;
	}
	return 0;
}

void print_substring_list(char* string, test_substr* head) {
	test_substr* current = head->next;
	while (current != NULL) {
		print_substr(string, current->start_index, current->length);
		current = current->next;
	}
}

void print_substring_indices(test_substr* head) {
	test_substr* current = head->next;
	while(current != NULL){
		printf("index: %d, length %d\n", current->start_index, current->length);
		current = current->next;
	}

}


int two_substrings_from_different_strings_equal(char* string1, char* string2,
		int string1_length, int string2_length, int string1_index, int string2_index, int substr_length)
{
	int i;

	if (string1_index + substr_length > string1_length || string2_index + substr_length > string2_length) {
		return 0;
	}
	
	for (i = 0; i < substr_length; i++) {
		if (string1[string1_index  + i] != string2[string2_index + i]) {
			return 0;
		}
	
	}
	
	return 1;

}


int are_two_substrings_mems(int index1, int index2, int length, char* string1,
		int str_length1, char* string2, int str_length2) {
	if (!two_substrings_from_different_strings_equal(string1, string2,
			str_length1, str_length2, index1, index2, length)) {
		return 0;
	}
	int left_okay = 0;
	int right_okay = 0;

	if (index1 == 0 || index2 == 0) {
		left_okay = 1;
	} else if (string1[index1 - 1] != string2[index2 - 1]) {
		left_okay = 1;
	}
	if (index1 + length == str_length1 || index2 + length == str_length2) {
		right_okay = 1;
	} else if (string1[index1 + length] != string2[index2 + length]) {
		right_okay = 1;
	}
	return left_okay & right_okay;
}

substring_pair* find_maximal_exact_matches(char* string1, char* string2, int threshold) {
	int length, i;

	substring_pair* current = calloc(1, sizeof(substring_pair));
	substring_pair* head = current;
	substring_pair* new;

	int string1_length = strlen(string1);
	int string2_length = strlen(string2);

	for (length = threshold; length <= string1_length; length++) {
		for (i = 0; i + length <= string1_length; i++) {

			for (int j = 0; j + length <= string2_length; j++) {

				if (are_two_substrings_mems(i, j, length, string1,
						string1_length, string2, string2_length)) {

					new = calloc(1, sizeof(substring_pair));
					new->index1 = i;
					new->index2 = j;
					new->length = length;
					current->next = new;
					current = new;

				}
			}
		}

	}
	return head;
}

int is_substring_unique(int index, int length, char* string, int str_length){
	int differs;
	for(int i = 0; i+length <= str_length; i++){
		if(i == index) continue;
		for(int j = 0; j < length; j++){
			if(string[i+j] != string[index+j]){
				differs = 1;
				break;
			}
		}
		if(!differs){
			return 0;
		}
		differs = 0;
	}
	return 1;
}

substring_pair* find_maximal_unique_matches(char* string1, char* string2, int threshold){

	substring_pair* current = calloc(1, sizeof(substring_pair));
	substring_pair* head = current;
	substring_pair* new;

	int str1length = strlen(string1);
	int str2length = strlen(string2);

	for(int length = threshold; length <= str1length; length++){
		for(int i = 0; i+length <= str1length; i++){
			for(int j = 0; j+length <= str2length; j++){
				if(are_two_substrings_mems(i, j, length, string1, str1length, string2, str2length) &&
						is_substring_unique(i, length, string1, str1length) &&
						is_substring_unique(j, length, string2, str2length)){
						new = calloc(1, sizeof(substring_pair));
						new->index1 = i;
						new->index2 = j;
						new->length = length;
						current->next = new;
						current = current->next;
				}
			}
		}
	}

	return head;
}


int two_substrings_equal2(char* first, int first_index, char* second, int second_index, int length)
{
	for(int i = 0; i < length; i++){
		if(first[first_index+i] != second[second_index+i]){
			return 0;
		}
	}
	return 1;
}

test_substr** find_common_substrings(char* first, char* second) {

	test_substr* dummy1 = calloc(1, sizeof(test_substr));
	test_substr* current1 = dummy1;

	test_substr* dummy2 = calloc(1, sizeof(test_substr));
	test_substr* current2 = dummy2;

	int shorter_length;
	if (strlen(first) > strlen(second)) {
		shorter_length = strlen(second);
	} else {
		shorter_length = strlen(first);
	}
	int first_length = strlen(first);
	int second_length = strlen(second);

	int found = 0;

	for (int length = 1; length <= shorter_length; length++) {
		for(int index1 = 0; index1 <= first_length-length; index1++) {

			for(int index2 = 0; index2 <= second_length-length; index2++){

				if(two_substrings_equal2(first, index1, second, index2, length)){

					found = 1;

					if(!list_contains_substr(second, dummy2, index2, length)){

						current2-> next = calloc(1, sizeof(test_substr));
						current2 = current2->next;
						current2->start_index = index2;
						current2->length = length;
					}
				}
			}
			if(found){
				current1-> next = calloc(1, sizeof(test_substr));
				current1 = current1->next;
				current1->start_index = index1;
				current1->length = length;
				found = 0;
			}
		}
	}


	test_substr** list = malloc(sizeof(test_substr*)*2);
	list[0] = dummy1;
	list[1] = dummy2;

	return list;
}

void print_substring_pairs(substring_pair* head, char* string){
	substring_pair* current = head->next;

	while(current != NULL){
		printf("pair [%d, %d], length %d \"",
				current->index1, current->index2, current->length);
		print_substr(string, current->index1, current->length);
		printf("\"\n");
		current = current->next;
	}
}

//int main(){
//
//	char* test2 = "dafkjdsaijdsaqrakkikoiranfdfkjhgfdskjhg";
//	char* test1 = "dsalkjhdsalkjhdsaurakkikoiraekjnbgfdkjh";
//	printf("%s\n", test1);
//	printf("%s\n", test2);
//	substring_pair* head = find_maximal_unique_matches(test1, test2, 1);
//	print_substring_pairs(head, test1);
//	return 0;
//}

