#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <malloc.h>

// Struktur untuk menyimpan pasangan kata dan nilai sum
typedef struct {
    char *word; // NULL untuk kasus angka
    int sum;    // Nilai angka atau sum dari kata
} WordSum;

// Prototipe fungsi sorting
void bubbleSort(WordSum arr[], int n);
void selectionSort(WordSum arr[], int n);
void insertionSort(WordSum arr[], int n);
void mergeSort(WordSum arr[], int l, int r);
void quickSort(WordSum arr[], int low, int high);
void shellSort(WordSum arr[], int n);

// Prototipe fungsi utilitas
void merge(WordSum arr[], int l, int m, int r);
int partition(WordSum arr[], int low, int high);
void swap(WordSum* a, WordSum* b);
void printArray(WordSum arr[], int size);
long long calculate_theoretical_memory(const char *name, int n);
void run_and_measure(void (*sort_func)(WordSum[], int), WordSum arr[], int n, const char *name);
void run_merge_sort(WordSum arr[], int n);
void run_quick_sort(WordSum arr[], int n);

#endif