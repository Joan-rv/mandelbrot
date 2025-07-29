#include <complex>
#include <raylib.h>
#include <vector>

bool in_mandelbrot_set(std::complex<double> c) {
    constexpr int max_iters = 200;
    std::complex<double> z{0.0, 0.0};
    for (int i = 0; i < max_iters; i++) {
        if (std::abs(z) > 2) {
            return false;
        }
        z = z * z + c;
    }
    return true;
}

int main() {
    constexpr int width = 400;
    constexpr int height = 400;
    std::vector<bool> points(width * height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double x = (double)j / width * (2.0 + 0.47) - 2.0;
            double y = (double)i / height * (1.12 + 1.12) - 1.12;
            points[width * i + j] = in_mandelbrot_set({x, y});
        }
    }

    InitWindow(width, height, "mandelbrot");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(WHITE);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (points[width * i + j]) {
                    DrawPixel(j, i, BLACK);
                }
            }
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
