#include <stdio.h>
#include <immintrin.h>
#include <stdlib.h>
#include <sys/time.h>

#define IMAGE_ROWS 1024
#define IMAGE_COLS 1024
#define KERNEL_ROWS 8
#define KERNEL_COLS 8
#define RESULT_ROWS (IMAGE_ROWS - KERNEL_ROWS + 1)
#define RESULT_COLS (IMAGE_COLS - KERNEL_COLS + 1)

void print_m256i(__m256i vec) {
    int values[8];
    _mm256_storeu_si256((__m256i*)values, vec);
    printf("Values: ");
    for (int i = 0; i < 8; ++i) {
        printf("%d ", values[i]);
    }
    printf("\n");
}

void convolve_avx2(const int* image, size_t image_rows, size_t image_cols,
    const int* kernel, size_t kernel_rows, size_t kernel_cols,
    int* result) {
    __m256i sum = _mm256_setzero_si256();
    __m256i image_vec, kernel_vec, prod;
    for (size_t i = 0; i <= image_rows - kernel_rows; ++i) {
        for (size_t j = 0; j <= image_cols - kernel_cols; ++j) {
            sum = _mm256_setzero_si256();
            for(int k = 0; k < 8 ;k++){
                //image_vec = _mm256_load_si256((__m256i*)&image[(i + k) * image_cols + j]);
                image_vec = _mm256_maskload_epi32(&image[(i + k) * image_cols + j], _mm256_setr_epi32(-1, -1, -1, -1, -1, -1, -1, -1));
                //kernel_vec = _mm256_load_si256((__m256i*)&kernel[k * kernel_cols]);
                kernel_vec = _mm256_maskload_epi32(&kernel[k * kernel_cols], _mm256_setr_epi32(-1, -1, -1, -1, -1, -1, -1, -1));
                prod = _mm256_mullo_epi32(image_vec, kernel_vec);
                sum = _mm256_add_epi32(sum, prod);
            }
            result[i * (image_cols - kernel_cols + 1) + j] += _mm256_extract_epi32(sum, 0);
            result[i * (image_cols - kernel_cols + 1) + j] += _mm256_extract_epi32(sum, 1);
            result[i * (image_cols - kernel_cols + 1) + j] += _mm256_extract_epi32(sum, 2);
            result[i * (image_cols - kernel_cols + 1) + j] += _mm256_extract_epi32(sum, 3);
            result[i * (image_cols - kernel_cols + 1) + j] += _mm256_extract_epi32(sum, 4);
            result[i * (image_cols - kernel_cols + 1) + j] += _mm256_extract_epi32(sum, 5);
            result[i * (image_cols - kernel_cols + 1) + j] += _mm256_extract_epi32(sum, 6);
            result[i * (image_cols - kernel_cols + 1) + j] += _mm256_extract_epi32(sum, 7);
        }
    }
}
int main() {
    int image[IMAGE_ROWS][IMAGE_COLS];
    int kernel[KERNEL_ROWS][KERNEL_COLS] = {
        {1, 0, -1, 0, 1, 0, -1, 0},
        {1, 0, -1, 0, 1, 0, -1, 0},
        {1, 0, -1, 0, 1, 0, -1, 0},
        {1, 0, -1, 0, 1, 0, -1, 0},
        {1, 0, -1, 0, 1, 0, -1, 0},
        {1, 0, -1, 0, 1, 0, -1, 0},
        {1, 0, -1, 0, 1, 0, -1, 0},
        {1, 0, -1, 0, 1, 0, -1, 0}
    };
    int result[RESULT_ROWS][RESULT_COLS];
    // Transpose the kernel matrix
    // Use a simple pattern to fill the image matrix
    for (int i = 0; i < IMAGE_ROWS; ++i) {
        for (int j = 0; j < IMAGE_COLS; ++j) {
            image[i][j] = (i * IMAGE_COLS + j) % 256; // Use a 0-255 pattern to fill the image
        }
    }
    struct timeval start, end;
    double interval;
    gettimeofday(&start, NULL);
    convolve_avx2((int *)image, IMAGE_ROWS, IMAGE_COLS, (int *)kernel, KERNEL_ROWS, KERNEL_COLS, (int *)result);
    // convolve_bhr((int *)image, IMAGE_ROWS, IMAGE_COLS, (int *)kernel, KERNEL_ROWS, KERNEL_COLS, (int *)result);
    gettimeofday(&end, NULL);
    interval = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;

    // Print the execution time
    printf("Convolution operation time: %f milliseconds\n", interval);
    printf("\n[!] Result matrix:\n");
    // Print the result matrix
    for (int i = 0; i < RESULT_ROWS; ++i) {
        for (int j = 0; j < RESULT_COLS; ++j) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}
