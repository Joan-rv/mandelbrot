#include <complex>
#include <raylib.h>
#include <vector>

Color gradient(Color start, Color end, double point) {
    auto scale = [](auto s, auto e, auto p) { return s * (1.0 - p) + e * p; };
    return Color{
        (unsigned char)scale(start.r, end.r, point),
        (unsigned char)scale(start.g, end.g, point),
        (unsigned char)scale(start.b, end.b, point),
        (unsigned char)scale(start.a, end.a, point),
    };
}

Color in_mandelbrot_set(std::complex<double> c) {
    constexpr int max_iters = 200;
    std::complex<double> z{0.0, 0.0};
    for (int i = 0; i < max_iters; i++) {
        if (std::abs(z) > 2) {
            Color start = {0x44, 0x01, 0x54, 0xff};
            Color end = {0xfd, 0xe7, 0x25, 0xff};
            return gradient(start, end, (double)i / max_iters);
        }
        z = z * z + c;
    }
    return Color{0x00, 0x00, 0x00, 0xff};
}

int main() {
    constexpr int width = 400;
    constexpr int height = 400;
    std::vector<Color> points(width * height);
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
                DrawPixel(j, i, points[width * i + j]);
            }
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
