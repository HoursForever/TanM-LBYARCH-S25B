#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <math.h>

extern void imgCvtGrayDoubleToInt(size_t height, size_t width, const double *input, int *output);

void imgCvtGrayDoubleToInt_C(size_t height, size_t width, const double *input, int *output) {
    size_t total_pixels = height * width;
    for (size_t i = 0; i < total_pixels; i++) {
        output[i] = (int)lround(input[i] * 255.0);
    }
}


void generate_random_image(size_t total_pixels, double *img) {
    for (size_t i = 0; i < total_pixels; i++) {
        img[i] = (double)rand() / RAND_MAX;
    }
}


bool verify_correctness(size_t height, size_t width, const double *input) {
    size_t total = height * width;
    int *out_asm = (int *)malloc(total * sizeof(int));
    int *out_c = (int *)malloc(total * sizeof(int));

    imgCvtGrayDoubleToInt(height, width, input, out_asm);
    imgCvtGrayDoubleToInt_C(height, width, input, out_c);

    bool match = true;
    for (size_t i = 0; i < total; i++) {
        if (abs(out_asm[i] - out_c[i]) > 1) { 
            match = false;
            break;
        }
    }

    free(out_asm);
    free(out_c);
    return match;
}

void run_benchmark(size_t height, size_t width, int runs) {
    size_t total_pixels = height * width;
    double *input = (double *)malloc(total_pixels * sizeof(double));
    int *output = (int *)malloc(total_pixels * sizeof(int));

    if (!input || !output) {
        printf("Memory allocation failed for size %zux%zu\n", height, width);
        return;
    }

    generate_random_image(total_pixels, input);

    bool correct = verify_correctness(height, width, input);

    double total_time_ms = 0.0;

    LARGE_INTEGER frequency;
    LARGE_INTEGER start, end;

    QueryPerformanceFrequency(&frequency);

    for (int r = 0; r < runs; r++) {

    QueryPerformanceCounter(&start);

    imgCvtGrayDoubleToInt(height, width, input, output);

    QueryPerformanceCounter(&end);

    double elapsed_ms =
        (double)(end.QuadPart - start.QuadPart) * 1000.0 /
        (double)frequency.QuadPart;

    total_time_ms += elapsed_ms;
    }
    double avg_time_ms = total_time_ms / runs;

    printf("| %4zux%-4zu | %12zu | %8d | %16.6f ms | %s |\n",
           height, width, total_pixels, runs, avg_time_ms, correct ? "PASSED" : "FAILED");

    free(input);
    free(output);
}

int main() {
    srand((unsigned int)time(NULL));

    printf("=========================================================\n");
    printf("         GRAYSCALE DOUBLE TO UINT8 CONVERSION            \n");
    printf("=========================================================\n\n");

    printf("1. Running Prompt Example Test (3x4 Image):\n");
    size_t ex_h = 3, ex_w = 4;
    double example_input[12] = {
        0.25, 0.35, 0.45, 0.33,
        0.55, 0.65, 0.75, 0.33,
        0.85, 0.95, 0.15, 0.33
    };
    int example_output[12];

    imgCvtGrayDoubleToInt(ex_h, ex_w, example_input, example_output);

    printf("Input Float Image:\n");
    for (size_t i = 0; i < ex_h; i++) {
        for (size_t j = 0; j < ex_w; j++) {
            printf("%.2f ", example_input[i * ex_w + j]);
        }
        printf("\n");
    }

    printf("\nAssembly Converted Integer Image:\n");
    for (size_t i = 0; i < ex_h; i++) {
        for (size_t j = 0; j < ex_w; j++) {
            printf("%3d ", example_output[i * ex_w + j]);
        }
        printf("\n");
    }
    printf("\n");

    // -------------------------------------------------------------------------
    // 2. Performance Benchmark (30 Runs)
    // -------------------------------------------------------------------------
    printf("2. Performance Benchmark (Average over 30 runs):\n");
    printf("+-----------+--------------+----------+--------------------+----------+\n");
    printf("| Dimension | Total Pixels | Runs     | Avg Execution Time | Status   |\n");
    printf("+-----------+--------------+----------+--------------------+----------+\n");

    run_benchmark(10, 10, 30);
    run_benchmark(100, 100, 30);
    run_benchmark(1000, 1000, 30);

    printf("+-----------+--------------+----------+--------------------+----------+\n");

    return 0;
}