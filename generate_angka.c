#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void generate_random_numbers(const char *filename, int count, int max_value) {
FILE *fp = fopen(filename, "w");
if (!fp) {
perror("File tidak dapat dibuka");
return;
}
srand(time(NULL)); // Inisialisasi seed
for (int i = 0; i < count; i++) {
int num = rand() % max_value;
fprintf(fp, "%d\n", num);
}
fclose(fp);
}
int main() {
generate_random_numbers("data_angka.txt", 2000000, 2000000);
return 0;
}