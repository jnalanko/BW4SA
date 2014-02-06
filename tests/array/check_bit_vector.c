/*
 * check_bit_vector.c
 *
 *  Created on: 6.2.2014
 *      Author: maxsandb
 */

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <check.h>
#include <math.h>
#include "../../include/utils.h"
#include "../../include/bit_vector.h"

int contains(unsigned int* arr, unsigned int arr_size, unsigned int key) {
	for (int i = 0; i < arr_size; ++i)
		if (arr[i] == key)
			return 1;

	return 0;
}

START_TEST (test_create_bit_vector) {
	struct bit_vec* vector;
	unsigned int length = 430;

	//allocate vector, checking for null
	ck_assert((vector = calloc(1, sizeof(bit_vector))) != 0);

	//init vector, checking for null
	ck_assert(init_bit_vector(vector, length) != 0);

	length = ((length + 31) / 32);

	//check vector variables
	ck_assert(vector->length == length);
	ck_assert(vector->mark_bit != 0);
	ck_assert(vector->unmark_bit != 0);

	//check internal vector
	ck_assert(vector->vector != 0);

	for (int i = 0; i < length; ++i)
		ck_assert(vector->vector[i] == 0);

	free_bit_vector(vector);
}
END_TEST

START_TEST (test_create_faulty_vector) {
	struct bit_vec* vector;
	unsigned int length = 0;

	//allocate vector, checking for null
	ck_assert((vector = calloc(1, sizeof(bit_vector))) != 0);

	//init vector, hoping for null
	ck_assert(init_bit_vector(vector, length) == 0);

	//try and init null, hoping for null
	ck_assert(init_bit_vector(0, length) == 0);

	free_bit_vector(vector);
}
END_TEST

START_TEST (test_bit_marking) {
	struct bit_vec* vector;
	unsigned int length = 38 * 32, ind_amount = ceil(length / 20), random, mask;
	unsigned int* indices = calloc(ind_amount, sizeof(unsigned int));

	//initialize random seed
	srand(time(NULL));

	//allocate and init vector
	vector = calloc(1, sizeof(bit_vector));
	init_bit_vector(vector, length);

	//mark bits at random indices
	for (int i = 0; i < ind_amount; ++i) {
		do {
			random = rand() % length;
		} while (contains(indices, ind_amount, random));

		vector->mark_bit(vector, random);
		indices[i] = random;
	}

	//check that bits have been correctly marked
	for (int i = 0; i < ind_amount; ++i) {
		mask = (1 << (indices[i] % 32));
		ck_assert((vector->vector[indices[i] / 32] & mask) == mask);
	}

	free_bit_vector(vector);
}
END_TEST

START_TEST (test_bit_unmarking) {
	struct bit_vec* vector;
	unsigned int length = 14 * 32, ind_amount = ceil(length / 20), random, mask;
	unsigned int* indices = calloc(ind_amount, sizeof(unsigned int));
	unsigned int half = ind_amount / 2;

	//initialize random seed
	srand(time(NULL));

	//allocate and init vector
	vector = calloc(1, sizeof(bit_vector));
	init_bit_vector(vector, length);

	//mark bits at random indices
	for (int i = 0; i < ind_amount; ++i) {
		do {
			random = rand() % length;
		} while (contains(indices, ind_amount, random));

		vector->mark_bit(vector, random);
		indices[i] = random;
	}

	//unmark half of the marked bits
	for (int i = 0; i < half; ++i)
		vector->unmark_bit(vector, indices[i]);

	//check that only half of the bits have been unmarked, and that the rest
	//remain unchanged
	for (int i = 0; i < ind_amount; ++i) {
		mask = (1 << (indices[i] % 32));

		if (i < half)
			ck_assert((vector->vector[indices[i] / 32] & mask) == 0);
		else
			ck_assert((vector->vector[indices[i] / 32] & mask) == mask);
	}

	free_bit_vector(vector);
}
END_TEST

START_TEST (test_mark_checking) {
	struct bit_vec* vector;
	unsigned int length = 56 * 32, ind_amount = ceil(length / 20), random, mask;
	unsigned int* indices = calloc(ind_amount, sizeof(unsigned int));

	//initialize indices to 0
	for (int i = 0; i < ind_amount; ++i)
		indices[i] = 0;

	//initialize random seed
	srand(time(NULL));

	//allocate and init vector
	vector = calloc(1, sizeof(bit_vector));
	init_bit_vector(vector, length);

	//mark bits at random indices and check if function finds bits unmarked before
	//marking
	for (int i = 0; i < ind_amount; ++i) {
		do {
			random = rand() % length;
		} while (contains(indices, ind_amount, random));

		ck_assert(vector->is_bit_marked(vector, random) == 0);
		vector->mark_bit(vector, random);
		indices[i] = random;
	}

	//check that function finds marked bits
	for (int i = 0; i < ind_amount; ++i) {
		mask = (1 << (indices[i] % 32));
		ck_assert(((vector->vector[indices[i] / 32] & mask) >> (indices[i] % 32)) ==
				vector->is_bit_marked(vector, indices[i]));
	}

	free_bit_vector(vector);
}
END_TEST

START_TEST (test_rank) {
	struct bit_vec* vector;
	unsigned int length = 503 * 32, ind_amount = ceil(length / 30), random;
	unsigned int* indices = calloc(ind_amount, sizeof(unsigned int));

	//initialize indices to 0
	for (int i = 0; i < ind_amount; ++i)
		indices[i] = 0;

	//initialize random seed
	srand(time(NULL));

	//allocate and init vector
	vector = calloc(1, sizeof(bit_vector));
	init_bit_vector(vector, length);

	//test that rank doesn't find any marked bits
	ck_assert(vector->rank(vector, vector->length * 32) == 0);

	//mark bits at random indices
	for (int i = 0; i < ind_amount; ++i) {
		do {
			random = rand() % length;
		} while (contains(indices, ind_amount, random));

		vector->mark_bit(vector, random);
		indices[i] = random;
	}

	//check that rank returns all marked bits
	ck_assert(vector->rank(vector, vector->length * 32) == ind_amount);

	free_bit_vector(vector);
}
END_TEST

Suite* array_suite(void) {
	Suite* suite = suite_create("Array");

	TCase* tc_bit_vector = tcase_create("Bit Vector");
	tcase_add_test (tc_bit_vector, test_create_bit_vector);
	tcase_add_test (tc_bit_vector, test_create_faulty_vector);
	tcase_add_test (tc_bit_vector, test_bit_marking);
	tcase_add_test (tc_bit_vector, test_bit_unmarking);
	tcase_add_test (tc_bit_vector, test_mark_checking);
	tcase_add_test (tc_bit_vector, test_rank);
	suite_add_tcase (suite, tc_bit_vector);

	return suite;
}

int main (void) {
	int number_failed;
	Suite* suite = array_suite();
	SRunner* suite_runner = srunner_create(suite);
	srunner_run_all(suite_runner, CK_NORMAL);
	number_failed = srunner_ntests_failed(suite_runner);
	srunner_free(suite_runner);

	return (number_failed == 0) ? 0 : 1;
}