#include "header.h"

// ==== Implementasi fungsi sorting ====

int main() {
    int pilihan, jumlah, i;

    // Menu input jenis data
    printf("Pilih jenis data:\n1. Angka\n2. Kata\nPilihan: ");
    scanf("%d", &pilihan);

    // Menu input jumlah data
    printf("\nPilih jumlah data:\n1. 10.000\n2. 50.000\n3. 100.000\n4. 250.000\n");
    printf("5. 500.000\n6. 1.000.000\n7. 1.500.000\n8. 2.000.000\nPilihan: ");
    scanf("%d", &jumlah);

    // Daftar ukuran data sesuai pilihan
    int sample_sizes[] = {10000, 50000, 100000, 250000, 500000, 1000000, 1500000, 2000000};
    int n = sample_sizes[jumlah - 1];

    // Penentuan nama file
    char *filename = pilihan == 2 ? "data_kata.txt" : "data_angka.txt";

    // Alokasi memori untuk data
    WordSum *data = malloc(n * sizeof(WordSum));
    if (!data) {
        printf("Gagal alokasi memori untuk data\n");
        return 1;
    }
    for (i = 0; i < n; i++) data[i].word = NULL; // Inisialisasi

    // Membuka file input
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("File tidak ditemukan!\n");
        free(data);
        return 1;
    }

    // Membaca data dari file
    if (pilihan == 2) {
        char temp[100];
        for (i = 0; i < n; i++) {
            if (fscanf(fp, "%s", temp) != 1) {
                printf("Gagal membaca kata ke-%d\n", i + 1);
                break;
            }
            data[i].word = strdup(temp);
            if (!data[i].word) {
                printf("Gagal alokasi memori untuk kata\n");
                break;
            }
            int sum = 0;
            for (int j = 0; temp[j] != '\0'; j++) {
                sum += temp[j] * (j + 1);
            }
            data[i].sum = sum;
        }
    } else {
        for (i = 0; i < n; i++) {
            if (fscanf(fp, "%d", &data[i].sum) != 1) {
                printf("Gagal membaca angka ke-%d\n", i + 1);
                break;
            }
            data[i].word = NULL;
        }
    }
    fclose(fp);

    // Jalankan semua algoritma
    printf("\n=== HASIL EKSPERIMEN SORTING (%d data) ===\n", n);
    printf("%-15s | %-12s | %-18s | %-20s\n", "Algoritma", "Waktu (s)", "Memori (bytes)", "Memori Teoretis (bytes)");
    printf("------------------------------------------------------------\n");
    run_merge_sort(data, n);
    run_quick_sort(data, n);
    run_and_measure(shellSort, data, n, "Shell Sort");
    run_and_measure(insertionSort, data, n, "Insertion Sort");
    run_and_measure(bubbleSort, data, n, "Bubble Sort");
    run_and_measure(selectionSort, data, n, "Selection Sort");

    // Bebaskan memori
    for (i = 0; i < n; i++) {
        if (data[i].word) free(data[i].word);
    }
    free(data);
    return 0;
}

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

void selectionSort(WordSum arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j].sum < arr[min].sum) min = j;
        swap(&arr[min], &arr[i]);
    }
}

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

void merge(WordSum arr[], int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    WordSum *L = malloc(n1 * sizeof(WordSum));
    WordSum *R = malloc(n2 * sizeof(WordSum));
    if (!L || !R) {
        printf("Gagal alokasi memori di merge\n");
        free(L); free(R);
        return;
    }
    for (int i = 0; i < n1; i++) {
        L[i].sum = arr[l + i].sum;
        L[i].word = arr[l + i].word ? strdup(arr[l + i].word) : NULL;
    }
    for (int j = 0; j < n2; j++) {
        R[j].sum = arr[m + 1 + j].sum;
        R[j].word = arr[m + 1 + j].word ? strdup(arr[m + 1 + j].word) : NULL;
    }

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i].sum <= R[j].sum) {
            arr[k].sum = L[i].sum;
            if (arr[k].word) free(arr[k].word);
            arr[k].word = L[i].word ? strdup(L[i].word) : NULL;
            i++;
        } else {
            arr[k].sum = R[j].sum;
            if (arr[k].word) free(arr[k].word);
            arr[k].word = R[j].word ? strdup(R[j].word) : NULL;
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k].sum = L[i].sum;
        if (arr[k].word) free(arr[k].word);
        arr[k].word = L[i].word ? strdup(L[i].word) : NULL;
        i++;
        k++;
    }
    while (j < n2) {
        arr[k].sum = R[j].sum;
        if (arr[k].word) free(arr[k].word);
        arr[k].word = R[j].word ? strdup(R[j].word) : NULL;
        j++;
        k++;
    }
    for (int i = 0; i < n1; i++) if (L[i].word) free(L[i].word);
    for (int j = 0; j < n2; j++) if (R[j].word) free(R[j].word);
    free(L); free(R);
}

void mergeSort(WordSum arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

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

void quickSort(WordSum arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

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

void swap(WordSum* a, WordSum* b) {
    WordSum t = *a;
    *a = *b;
    *b = t;
}

void printArray(WordSum arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("(%s, %d) ", arr[i].word ? arr[i].word : "NULL", arr[i].sum);
    }
    printf("\n");
}

// Fungsi untuk menghitung memori teoretis
long long calculate_theoretical_memory(const char *name, int n) {
    long long mem = (long long)n * sizeof(WordSum);
    if (strcmp(name, "Merge Sort") == 0) {
        mem += (long long)n * sizeof(WordSum);
    }
    return mem;
}

// Fungsi untuk mengukur waktu dan memori
void run_and_measure(void (*sort_func)(WordSum[], int), WordSum arr[], int n, const char *name) {
    WordSum *temp = malloc(n * sizeof(WordSum));
    if (!temp) {
        printf("Gagal alokasi memori untuk temp\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        temp[i].word = arr[i].word ? strdup(arr[i].word) : NULL;
        temp[i].sum = arr[i].sum;
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
    printf("Debug: %s: mi_before.uordblks = %zu, mi_after.uordblks = %zu\n", 
           name, mi_before.uordblks, mi_after.uordblks);
    
    // Verifikasi sorting
    bool sorted = true;
    for (int i = 0; i < n - 1; i++) {
        if (temp[i].sum > temp[i + 1].sum) {
            sorted = false;
            break;
        }
    }
    printf("Debug: %s sorted correctly: %s\n", name, sorted ? "Yes" : "No");
    
    // Cetak beberapa elemen untuk verifikasi
    printf("First 5 elements: ");
    for (int i = 0; i < 5 && i < n; i++) {
        printf("(%s, %d) ", temp[i].word ? temp[i].word : "NULL", temp[i].sum);
    }
    printf("\nLast 5 elements: ");
    for (int i = n - 5; i < n && i >= 0; i++) {
        printf("(%s, %d) ", temp[i].word ? temp[i].word : "NULL", temp[i].sum);
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        if (temp[i].word) free(temp[i].word);
    }
    free(temp);
}

// Wrapper khusus merge sort
void run_merge_sort(WordSum arr[], int n) {
    WordSum *temp = malloc(n * sizeof(WordSum));
    if (!temp) {
        printf("Gagal alokasi memori untuk temp\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        temp[i].word = arr[i].word ? strdup(arr[i].word) : NULL;
        temp[i].sum = arr[i].sum;
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
    printf("Debug: Merge Sort: mi_before.uordblks = %zu, mi_after.uordblks = %zu\n", 
           mi_before.uordblks, mi_after.uordblks);
    
    // Verifikasi sorting
    bool sorted = true;
    for (int i = 0; i < n - 1; i++) {
        if (temp[i].sum > temp[i + 1].sum) {
            sorted = false;
            break;
        }
    }
    printf("Debug: Merge Sort sorted correctly: %s\n", sorted ? "Yes" : "No");
    
    // Cetak beberapa elemen untuk verifikasi
    printf("First 5 elements: ");
    for (int i = 0; i < 5 && i < n; i++) {
        printf("(%s, %d) ", temp[i].word ? temp[i].word : "NULL", temp[i].sum);
    }
    printf("\nLast 5 elements: ");
    for (int i = n - 5; i < n && i >= 0; i++) {
        printf("(%s, %d) ", temp[i].word ? temp[i].word : "NULL", temp[i].sum);
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        if (temp[i].word) free(temp[i].word);
    }
    free(temp);
}

// Wrapper khusus quick sort
void run_quick_sort(WordSum arr[], int n) {
    WordSum *temp = malloc(n * sizeof(WordSum));
    if (!temp) {
        printf("Gagal alokasi memori untuk temp\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        temp[i].word = arr[i].word ? strdup(arr[i].word) : NULL;
        temp[i].sum = arr[i].sum;
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
    printf("Debug: Quick Sort: mi_before.uordblks = %zu, mi_after.uordblks = %zu\n", 
           mi_before.uordblks, mi_after.uordblks);
    
    // Verifikasi sorting
    bool sorted = true;
    for (int i = 0; i < n - 1; i++) {
        if (temp[i].sum > temp[i + 1].sum) {
            sorted = false;
            break;
        }
    }
    printf("Debug: Quick Sort sorted correctly: %s\n", sorted ? "Yes" : "No");
    
    // Cetak beberapa elemen untuk verifikasi
    printf("First 5 elements: ");
    for (int i = 0; i < 5 && i < n; i++) {
        printf("(%s, %d) ", temp[i].word ? temp[i].word : "NULL", temp[i].sum);
    }
    printf("\nLast 5 elements: ");
    for (int i = n - 5; i < n && i >= 0; i++) {
        printf("(%s, %d) ", temp[i].word ? temp[i].word : "NULL", temp[i].sum);
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        if (temp[i].word) free(temp[i].word);
    }
    free(temp);
}
