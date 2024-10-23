#include <immintrin.h>
#include <stdio.h>

void print_m256i(__m256i vec) {
    int values[8];
    _mm256_storeu_si256((__m256i*)values, vec);
    printf("Values: ");
    for (int i = 0; i < 8; ++i) {
        printf("%d ", values[i]);
    }
    printf("\n");
}

int main() {
    int array[8][8];

    // Fill the array with values 0 to 63
    int count = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            array[i][j] = count++;
        }
    }
    printf("Array:\n");
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
          printf("%d ", array[i][j]);
      }
      printf("\n");
    }

    // Store the first row of the array into __m256i parameter
    __m256i data = _mm256_loadu_si256((__m256i*)array[1]);

    // Print the stored data
    printf("Stored data: ");
    print_m256i(data);

    return 0;
}
