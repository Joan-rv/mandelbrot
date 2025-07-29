#include <raylib.h>
#include <raymath.h>

#include <cassert>
#include <complex>
#include <vector>

static Color gradient(Color start, Color end, double point) {
    auto scale = [](auto s, auto e, auto p) { return s * (1.0 - p) + e * p; };
    return Color{
        (unsigned char)scale(start.r, end.r, point),
        (unsigned char)scale(start.g, end.g, point),
        (unsigned char)scale(start.b, end.b, point),
        (unsigned char)scale(start.a, end.a, point),
    };
}

static Color mandelbrot_color(std::complex<double> c) {
    constexpr int max_iters = 200;
    std::complex<double> z{0.0, 0.0};
    for (int i = 0; i < max_iters; i++) {
        if (z.real() * z.real() + z.imag() * z.imag() > 2 * 2) {
            Color start = {0x44, 0x01, 0x54, 0xff};
            Color end = {0xfd, 0xe7, 0x25, 0xff};
            return gradient(start, end, (double)i / max_iters);
        }
        z = z * z + c;
    }
    return Color{0x00, 0x00, 0x00, 0xff};
}

struct Size {
    int width, height;

    friend bool operator==(const Size &a, const Size &b) = default;
};

class Mandelbrot {
public:
    Mandelbrot(Size size);
    Size size() const;
    void size(Size size_);
    void draw(Vector2 origin) const;

private:
    void compute_();
    std::vector<Color> grid_;
    Size size_;
};

Mandelbrot::Mandelbrot(Size size) : size_(size) { compute_(); }
Size Mandelbrot::size() const { return size_; }
void Mandelbrot::size(Size size) {
    size_ = size;
    compute_();
}
void Mandelbrot::draw(Vector2 origin) const {
    for (int i = 0; i < size_.height; i++) {
        for (int j = 0; j < size_.width; j++) {
            DrawPixel(origin.x + j, origin.y + i, grid_[i * size_.width + j]);
        }
    }
}
void Mandelbrot::compute_() {
    assert(size_.width > 0);
    assert(size_.height > 0);
    grid_.resize(size_.width * size_.height);
    for (int i = 0; i < size_.height; i++) {
        for (int j = 0; j < size_.width; j++) {
            double x = (double)j / size_.width * (2.0 + 0.47) - 2.0;
            double y = (double)i / size_.height * (1.12 + 1.12) - 1.12;
            grid_[i * size_.width + j] = mandelbrot_color({x, y});
        }
    }
}

int main() {
    Size size{400, 400};
    Mandelbrot m(size);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(size.width, size.height, "mandelbrot");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        size.width = GetScreenWidth();
        size.height = GetScreenHeight();
        if (size != m.size()) {
            m.size(size);
        }

        BeginDrawing();

        ClearBackground(WHITE);
        m.draw({0, 0});

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
