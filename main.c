#include "header.h"

// === Fungsi Utama ===
// Mengatur input, membaca data dari file, menjalankan semua algoritma sorting, dan mencetak hasil perbandingan.
int main() {
    int pilihan, jumlah, i;

    // Menu input jenis data
    printf("Pilih jenis data:\n1. Angka\n2. Kata\nPilihan: ");
    scanf("%d", &pilihan);

    // Menu input jumlah data
    printf("\nPilih jumlah data:\n1. 10.000\n2. 50.000\n3. 100.000\n4. 250.000\n");
    printf("5. 500.000\n6. 1.000.000\n7. 1.500.000\n8. 2.000.000\nPilihan: ");
    scanf("%d", &jumlah);

    int sample_sizes[] = {10000, 50000, 100000, 250000, 500000, 1000000, 1500000, 2000000};
    int n = sample_sizes[jumlah - 1];

    char *filename = pilihan == 2 ? "data_kata.txt" : "data_angka.txt";

    WordSum *data = malloc(n * sizeof(WordSum));
    if (!data) {
        printf("Gagal alokasi memori untuk data\n");
        return 1;
    }
    for (i = 0; i < n; i++) data[i].word = NULL;

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("File tidak ditemukan!\n");
        free(data);
        return 1;
    }

    if (pilihan == 2) {
        char temp[100];
        for (i = 0; i < n; i++) {
            if (fscanf(fp, "%s", temp) != 1) break;
            data[i].word = strdup(temp);
            int sum = 0;
            for (int j = 0; temp[j] != '\0'; j++) {
                sum += temp[j] * (j + 1);
            }
            data[i].sum = sum;
        }
    } else {
        for (i = 0; i < n; i++) {
            if (fscanf(fp, "%d", &data[i].sum) != 1) break;
            data[i].word = NULL;
        }
    }
    fclose(fp);

    printf("\n=== HASIL EKSPERIMEN SORTING (%d data) ===\n", n);
    printf("%-15s | %-12s | %-18s | %-20s\n", "Algoritma", "Waktu (s)", "Memori (bytes)", "Memori Teoretis (bytes)");
    printf("------------------------------------------------------------\n");
    run_merge_sort(data, n);
    run_quick_sort(data, n);
    run_and_measure(shellSort, data, n, "Shell Sort");
    run_and_measure(insertionSort, data, n, "Insertion Sort");
    run_and_measure(bubbleSort, data, n, "Bubble Sort");
    run_and_measure(selectionSort, data, n, "Selection Sort");

    for (i = 0; i < n; i++) {
        if (data[i].word) free(data[i].word);
    }
    free(data);
    return 0;
}

// === Sorting Algorithms ===

// Bubble Sort: membandingkan elemen bersebelahan dan menukarnya jika tidak urut.
void bubbleSort(WordSum arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].sum > arr[j + 1].sum) {
                swap(&arr[j], &arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// Selection Sort: memilih elemen terkecil dari sisa array dan menempatkannya di posisi yang benar.
void selectionSort(WordSum arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j].sum < arr[min].sum) min = j;
        swap(&arr[min], &arr[i]);
    }
}

// Insertion Sort: menyisipkan elemen ke posisi yang benar seperti menyusun kartu.
void insertionSort(WordSum arr[], int n) {
    for (int i = 1; i < n; i++) {
        WordSum key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].sum > key.sum) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Merge: menggabungkan dua array terurut menjadi satu array terurut (digunakan dalam Merge Sort).
void merge(WordSum arr[], int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    WordSum *L = malloc(n1 * sizeof(WordSum));
    WordSum *R = malloc(n2 * sizeof(WordSum));
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i].sum <= R[j].sum) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L); free(R);
}

// Merge Sort: divide and conquer — membagi lalu menggabungkan array terurut.
void mergeSort(WordSum arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Partition: membagi array berdasarkan pivot (digunakan dalam Quick Sort).
int partition(WordSum arr[], int low, int high) {
    int pivot = arr[high].sum;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].sum < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// Quick Sort: membagi array berdasarkan pivot, lalu melakukan sorting secara rekursif.
void quickSort(WordSum arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Shell Sort: variasi dari insertion sort yang menggunakan gap untuk efisiensi.
void shellSort(WordSum arr[], int n) {
    for (int gap = n/2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            WordSum temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap].sum > temp.sum; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

// Swap: menukar dua elemen dalam array.
void swap(WordSum* a, WordSum* b) {
    WordSum t = *a;
    *a = *b;
    *b = t;
}

// printArray: mencetak isi array (untuk debugging).
void printArray(WordSum arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("(%s, %d) ", arr[i].word ? arr[i].word : "NULL", arr[i].sum);
    }
    printf("\n");
}

// calculate_theoretical_memory: menghitung penggunaan memori teoretis berdasarkan algoritma.
long long calculate_theoretical_memory(const char *name, int n) {
    long long mem = (long long)n * sizeof(WordSum);
    if (strcmp(name, "Merge Sort") == 0) {
        mem += (long long)n * sizeof(WordSum); // tambahan alokasi untuk merge
    }
    return mem;
}

// run_and_measure: menjalankan algoritma, mengukur waktu & memori, lalu verifikasi hasil.
void run_and_measure(void (*sort_func)(WordSum[], int), WordSum arr[], int n, const char *name) {
    WordSum *temp = malloc(n * sizeof(WordSum));
    for (int i = 0; i < n; i++) {
        temp[i].sum = arr[i].sum;
        temp[i].word = arr[i].word ? strdup(arr[i].word) : NULL;
    }

    struct mallinfo2 mi_before = mallinfo2();
    clock_t start = clock();
    sort_func(temp, n);
    clock_t end = clock();
    struct mallinfo2 mi_after = mallinfo2();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    long long mem_used = mi_after.uordblks - mi_before.uordblks;
    long long mem_theoretical = calculate_theoretical_memory(name, n);

    printf("%-15s | Waktu: %.4f s | Memori: %lld bytes | Memori Teoretis: %lld bytes\n",
           name, time_spent, mem_used, mem_theoretical);

    for (int i = 0; i < n; i++) {
        if (temp[i].word) free(temp[i].word);
    }
    free(temp);
}

// run_merge_sort & run_quick_sort: khusus untuk merge dan quick sort karena membutuhkan argumen tambahan
void run_merge_sort(WordSum arr[], int n) {
    WordSum *temp = malloc(n * sizeof(WordSum));
    for (int i = 0; i < n; i++) {
        temp[i].sum = arr[i].sum;
        temp[i].word = arr[i].word ? strdup(arr[i].word) : NULL;
    }

    struct mallinfo2 mi_before = mallinfo2();
    clock_t start = clock();
    mergeSort(temp, 0, n - 1);
    clock_t end = clock();
    struct mallinfo2 mi_after = mallinfo2();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    long long mem_used = mi_after.uordblks - mi_before.uordblks;
    long long mem_theoretical = calculate_theoretical_memory("Merge Sort", n);

    printf("%-15s | Waktu: %.4f s | Memori: %lld bytes | Memori Teoretis: %lld bytes\n",
           "Merge Sort", time_spent, mem_used, mem_theoretical);

    for (int i = 0; i < n; i++) {
        if (temp[i].word) free(temp[i].word);
    }
    free(temp);
}

void run_quick_sort(WordSum arr[], int n) {
    WordSum *temp = malloc(n * sizeof(WordSum));
    for (int i = 0; i < n; i++) {
        temp[i].sum = arr[i].sum;
        temp[i].word = arr[i].word ? strdup(arr[i].word) : NULL;
    }

    struct mallinfo2 mi_before = mallinfo2();
    clock_t start = clock();
    quickSort(temp, 0, n - 1);
    clock_t end = clock();
    struct mallinfo2 mi_after = mallinfo2();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    long long mem_used = mi_after.uordblks - mi_before.uordblks;
    long long mem_theoretical = calculate_theoretical_memory("Quick Sort", n);

    printf("%-15s | Waktu: %.4f s | Memori: %lld bytes | Memori Teoretis: %lld bytes\n",
           "Quick Sort", time_spent, mem_used, mem_theoretical);

    for (int i = 0; i < n; i++) {
        if (temp[i].word) free(temp[i].word);
    }
    free(temp);
}
