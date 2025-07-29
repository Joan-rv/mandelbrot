#include <algorithm>
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
    Mandelbrot(Size size, double zoom = 1.0);
    Size size() const;
    void size(Size size_);
    double zoom() const;
    void zoom(double zoom);
    void draw(Vector2 origin) const;

private:
    void compute_();
    std::vector<Color> grid_;
    Size size_;
    double zoom_;
};

Mandelbrot::Mandelbrot(Size size, double zoom) : size_(size), zoom_(zoom) {
    compute_();
}
Size Mandelbrot::size() const { return size_; }
void Mandelbrot::size(Size size) {
    size_ = size;
    compute_();
}
double Mandelbrot::zoom() const { return zoom_; }
void Mandelbrot::zoom(double zoom) {
    zoom_ = zoom;
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
            x /= zoom_;
            double y = (double)i / size_.height * (1.12 + 1.12) - 1.12;
            y /= zoom_;
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
        const float mouse_wheel_move = GetMouseWheelMove();
        if (mouse_wheel_move != 0.0f) {
            m.zoom(std::clamp(m.zoom() + 0.05 * mouse_wheel_move, 0.1, 3.0));
        }

        BeginDrawing();

        ClearBackground(WHITE);
        m.draw({0, 0});

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
