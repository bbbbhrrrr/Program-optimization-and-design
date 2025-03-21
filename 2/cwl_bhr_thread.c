#include <stdio.h>
#include <immintrin.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define IMAGE_ROWS 1024
#define IMAGE_COLS 1024
#define KERNEL_ROWS 8
#define KERNEL_COLS 8
#define RESULT_ROWS (IMAGE_ROWS - KERNEL_ROWS + 1)
#define RESULT_COLS (IMAGE_COLS - KERNEL_COLS + 1)

typedef struct {
    const int* image;
    size_t image_rows;
    size_t image_cols;
    const int* kernel;
    size_t kernel_rows;
    size_t kernel_cols;
    int* result;
    size_t start_i;
    size_t end_i;
} ThreadArgs;

void* convolve_thread(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    const int* image = args->image;
    size_t image_rows = args->image_rows;
    size_t image_cols = args->image_cols;
    const int* kernel = args->kernel;
    size_t kernel_rows = args->kernel_rows;
    size_t kernel_cols = args->kernel_cols;
    int* result = args->result;
    size_t start_i = args->start_i;
    size_t end_i = args->end_i;

    int sum_array[8];
    size_t size= image_rows - kernel_rows ;
    for (size_t i = 0; i <= size; ++i) {
        for (size_t j = 0; j <= size; ++j) {
            // 初始化求和变量
            __m256i sum = _mm256_setzero_si256();
            // 对卷积核中的每个元素进行迭代
            for (size_t ki = 0; ki < kernel_rows; ++ki) {
                // 将图像的当前像素与卷积核的当前元素相乘，然后加到求和变量上
                // 加载数据到 SIMD 寄存
                __m256i image_vec = _mm256_loadu_si256((__m256i*)(image + (i + ki) * image_cols + j));
                __m256i kernel_vec = _mm256_loadu_si256((__m256i*)(kernel + ki * kernel_cols));
                __m256i prod = _mm256_mullo_epi32(image_vec, kernel_vec);
                sum = _mm256_add_epi32(sum, prod);
            }
            // 将求和结果存储在结果数组中
            int final_sum = 0;
            _mm256_storeu_si256((__m256i*)sum_array, sum);
            for (int k = 0; k < 8; k+=2) {
                sum_array[k] += sum_array[k + 1];
                final_sum += sum_array[k];
            }
            result[i * (size + 1) + j] = final_sum;
        }
    }
    pthread_exit(NULL);
}

void convolve_better(const int* image, size_t image_rows, size_t image_cols,
                     const int* kernel, size_t kernel_rows, size_t kernel_cols,
                     int* result) {
    const int num_threads = 4; // 设置线程数量
    pthread_t threads[num_threads];
    ThreadArgs thread_args[num_threads];

    size_t rows_per_thread = (image_rows - kernel_rows + 1) / num_threads;

    for (int t = 0; t < num_threads; ++t) {
        size_t start_i = t * rows_per_thread;
        size_t end_i = (t == num_threads - 1) ? (image_rows - kernel_rows) : (start_i + rows_per_thread - 1);
        printf("start %d  end %d\n", start_i, end_i);

        thread_args[t].image = image;
        thread_args[t].image_rows = image_rows;
        thread_args[t].image_cols = image_cols;
        thread_args[t].kernel = kernel;
        thread_args[t].kernel_rows = kernel_rows;
        thread_args[t].kernel_cols = kernel_cols;
        thread_args[t].result = result;
        thread_args[t].start_i = start_i;
        thread_args[t].end_i = end_i;

        pthread_create(&threads[t], NULL, convolve_thread, (void*)&thread_args[t]);
    }

    for (int t = 0; t < num_threads; ++t) {
        pthread_join(threads[t], NULL);
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
    convolve_better((int *)image, IMAGE_ROWS, IMAGE_COLS, (int *)kernel, KERNEL_ROWS, KERNEL_COLS, (int *)result);
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
