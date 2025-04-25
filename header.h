#ifndef HEADER_H
#define HEADER_H

// Header dasar untuk fungsi input/output dan utilitas umum
#include <stdio.h>      // untuk printf, scanf, dll
#include <stdlib.h>     // untuk malloc, free, dll
#include <string.h>     // untuk strdup, strcmp, dll
#include <stdbool.h>    // untuk tipe boolean
#include <time.h>       // untuk pengukuran waktu (clock)
#include <malloc.h>     // untuk mallinfo2 (pengukuran memori)

// Struktur utama untuk menyimpan data
// word digunakan untuk data kata (string), sum untuk nilai angka atau hash dari kata
typedef struct {
    char *word; // NULL untuk kasus angka
    int sum;    // Nilai angka atau hasil kalkulasi nilai dari kata
} WordSum;

// === Prototipe Fungsi Sorting ===

// Bubble Sort: urutkan elemen dengan membandingkan pasangan berurutan
void bubbleSort(WordSum arr[], int n);

// Selection Sort: pilih elemen minimum dan tukar dengan posisi saat ini
void selectionSort(WordSum arr[], int n);

// Insertion Sort: sisipkan elemen ke posisi yang tepat dalam array terurut
void insertionSort(WordSum arr[], int n);

// Merge Sort: algoritma rekursif divide-and-conquer
void mergeSort(WordSum arr[], int l, int r);

// Quick Sort: algoritma efisien berbasis partisi dan rekursi
void quickSort(WordSum arr[], int low, int high);

// Shell Sort: pengembangan dari insertion sort dengan gap antar elemen
void shellSort(WordSum arr[], int n);


// === Prototipe Fungsi Utilitas ===

// Fungsi merge untuk penggabungan dua bagian array (digunakan oleh Merge Sort)
void merge(WordSum arr[], int l, int m, int r);

// Fungsi partisi untuk pembagian array (digunakan oleh Quick Sort)
int partition(WordSum arr[], int low, int high);

// Menukar dua elemen dalam array
void swap(WordSum* a, WordSum* b);

// Mencetak isi array (digunakan untuk debugging)
void printArray(WordSum arr[], int size);

// Menghitung penggunaan memori secara teoretis berdasarkan algoritma
long long calculate_theoretical_memory(const char *name, int n);

// Menjalankan algoritma sorting dan mengukur performa (waktu dan memori)
void run_and_measure(void (*sort_func)(WordSum[], int), WordSum arr[], int n, const char *name);

// Wrapper untuk Merge Sort agar kompatibel dengan run_and_measure
void run_merge_sort(WordSum arr[], int n);

// Wrapper untuk Quick Sort agar kompatibel dengan run_and_measure
void run_quick_sort(WordSum arr[], int n);

#endif
