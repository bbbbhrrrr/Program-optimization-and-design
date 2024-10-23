#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define IMAGE_ROWS 14
#define IMAGE_COLS 14
#define KERNEL_ROWS 8
#define KERNEL_COLS 8
#define RESULT_ROWS (IMAGE_ROWS - KERNEL_ROWS + 1)
#define RESULT_COLS (IMAGE_COLS - KERNEL_COLS + 1)

void convolve(const int *image, size_t image_rows, size_t image_cols,
              const int *kernel, size_t kernel_rows, size_t kernel_cols,
              int *result) {
    for (size_t i = 0; i <= image_rows - kernel_rows; ++i) {
        for (size_t j = 0; j <= image_cols - kernel_cols; ++j) {
            int sum = 0;
            for (size_t ki = 0; ki < kernel_rows; ++ki) {
                for (size_t kj = 0; kj < kernel_cols; ++kj) {
                    sum += image[(i + ki) * image_cols + (j + kj)] * kernel[ki * kernel_cols + kj];
                }
            }
            result[i * (image_cols - kernel_cols + 1) + j] = sum;
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

    // Use a simple pattern to fill the image matrix
    for (int i = 0; i < IMAGE_ROWS; ++i) {
        for (int j = 0; j < IMAGE_COLS; ++j) {
            image[i][j] = (i * IMAGE_COLS + j) % 256; // Use a 0-255 pattern to fill the image
        }
    }

    struct timeval start, end;
    double interval;
    gettimeofday(&start, NULL);
    convolve((int *)image, IMAGE_ROWS, IMAGE_COLS, (int *)kernel, KERNEL_ROWS, KERNEL_COLS, (int *)result);
    gettimeofday(&end, NULL);
    interval = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;

    // Print the execution time
    printf("Convolution operation time: %f milliseconds\n", interval);

    // Print the result matrix
    for (int i = 0; i < RESULT_ROWS; ++i) {
        for (int j = 0; j < RESULT_COLS; ++j) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}
