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

//尝试从内存访问、循环展开、SIMD和并行对此函数进行优化
void convolve_better(const int* image, size_t image_rows, size_t image_cols,
                     const int* kernel, size_t kernel_rows, size_t kernel_cols,
                     int* result) {
    for (size_t i = 0; i <= image_rows - kernel_rows; ++i) {
        for (size_t j = 0; j <= image_cols - kernel_cols; ++j) {
            __m256i sum = _mm256_setzero_si256();
            for (size_t k = 0; k < kernel_rows; ++k) {
              //读取image和kernel的值
              __m256i image_val = _mm256_loadu_si256((__m256i*)(image + (i+k) * image_cols + j));
              __m256i kernel_val = _mm256_loadu_si256((__m256i*)(kernel + k * kernel_cols));
              sum = _mm256_add_epi32(sum, _mm256_mullo_epi32(image_val, kernel_val));
              // print_m256i(image_val);
              // print_m256i(kernel_val);
              // print_m256i(sum);
              // printf("\n");
            }
            // printf("done...\n");
            int sum_array[8];
            _mm256_storeu_si256((__m256i*)sum_array, sum);
            int final_sum = 0;
            for (int k = 0; k < 8; k++) {
                final_sum += sum_array[k];
            }
            result[i * (image_cols - kernel_cols + 1) + j] = final_sum;
  }
}
}
void convolve_bhr(const int* image, size_t image_rows, size_t image_cols,
    const int* kernel, size_t kernel_rows, size_t kernel_cols,
    int* result) {
    // 对图像中的每个像素进行迭代
    for (size_t i = 0; i <= image_rows - kernel_rows; ++i) {
        for (size_t j = 0; j <= image_cols - kernel_cols; ++j) {
            // 初始化求和变量
            __m256i sum = _mm256_setzero_si256();
            // 对卷积核中的每个元素进行迭代
            for (size_t ki = 0; ki < kernel_rows; ++ki) {
                // 将图像的当前像素与卷积核的当前元素相乘，然后加到求和变量上
                const int* image_ptr = &image[(i + ki) * image_cols+j];
                const int* kernel_ptr = &kernel[ki * kernel_cols];
                // 加载数据到 SIMD 寄存
                __m256i image_vec = _mm256_loadu_si256((__m256i*)image_ptr);
                __m256i kernel_vec = _mm256_loadu_si256((__m256i*)kernel_ptr);
                __m256i prod = _mm256_mullo_epi32(image_vec, kernel_vec);
                sum = _mm256_add_epi32(sum, prod);
            }
            // 将求和结果存储在结果数组中
            sum=_mm256_hadd_epi32(sum, sum);
            sum=_mm256_hadd_epi32(sum, sum);
            sum = _mm256_hadd_epi32(sum, sum);
            _mm256_storeu_si256((__m256i*)(&result[i * (image_cols - kernel_cols + 1) + j]), sum);
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
    printf("[!] Image matrix:\n");
    for (int i = 0; i < IMAGE_ROWS; ++i) {
        for (int j = 0; j < IMAGE_COLS; ++j) {
            image[i][j] = (i * IMAGE_COLS + j) % 256; // Use a 0-255 pattern to fill the image
        }
    }
    struct timeval start, end;
    double interval;
    gettimeofday(&start, NULL);
    // convolve_better((int *)image, IMAGE_ROWS, IMAGE_COLS, (int *)kernel, KERNEL_ROWS, KERNEL_COLS, (int *)result);
    convolve_bhr((int *)image, IMAGE_ROWS, IMAGE_COLS, (int *)kernel, KERNEL_ROWS, KERNEL_COLS, (int *)result);
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
