/**
 * @file	utils.c
 * @brief	Implementation of the utility-algorithms.
 * @see		utils.h
 * @author	Max Sandberg (REXiator)
 * @bug		No known bugs.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/utils.h"

/**
 * @brief	Simple swap-function.
 *
 * Swaps the position of two entries in an array.
 *
 * @param	arr			Pointer to the array is to be manipulated.
 * @param	entry_size	Size of the entries that are to be swapped.
 * @param	i			Index of the first entry.
 * @param	j			Index of the second entry.
 * @see		utils.h#quick_sort
 * @author	Max Sandberg (REXiator)
 * @bug		No known bugs.
 */
void swap(void* arr, unsigned int entry_size, int i, int j) {
	void* tmp = malloc(entry_size);

	memcpy(tmp, (arr + (j * entry_size)), entry_size);
    memcpy((arr + (j * entry_size)), (arr + (i * entry_size)), entry_size);
    memcpy((arr + (i * entry_size)), tmp, entry_size);

    free(tmp);
}

/**
 * @brief	The implementation of the quicksort-algorithm.
 * @param	arr			Pointer to the array that is to be sorted.
 * @param	entry_size	Size of the individual entries in the array.
 * @param	a			Index to the first entry to be sorted.
 * @param	b			Index to the last entry to be sorted.
 * @see		utils.h#quick_sort
 * @author	Max Sandberg (REXiator)
 * @bug		No known bugs.
 */
void quicksort(void* arr, unsigned int entry_size, unsigned int a,
		unsigned int b) {

    if (a >= b)
        return;

    void* key = (arr + (a * entry_size));
    int i = a + 1, j = b;

    while (i < j) {
        while (i < j && memcmp((arr + (j * entry_size)), key, entry_size) >= 0)
        	--j;

        while (i < j && memcmp((arr + (i * entry_size)), key, entry_size) <= 0)
        	++i;

        if (i < j)
            swap(arr, entry_size, i, j);
    }

    if (memcmp((arr + (a * entry_size)), (arr + (i * entry_size)), entry_size) > 0) {
        swap(arr, entry_size, a, i);
        quicksort(arr, entry_size, a, i - 1);
        quicksort(arr, entry_size, i + 1, b);
    }

    // there is no left-hand-side
    else
        quicksort(arr, entry_size, a + 1, b);
}

void quick_sort(void* arr, unsigned int arr_size, unsigned int entry_size) {
	if (arr == 0 || arr_size == 0 || entry_size == 0)
		return;

	quicksort(arr, entry_size, 0, arr_size - 1);
}

void print_bits(unsigned int mask) {
	unsigned int size = sizeof(unsigned int);
	unsigned int max_pow = 1 << (size * 8 - 1);

	for (int i = 0; i < size * 8; ++i) {
		// print last bit and shift left.
	    printf("%u", mask & max_pow ? 1 : 0);
	    mask = mask << 1;
	}
}
