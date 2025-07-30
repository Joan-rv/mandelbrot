#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <array>

int main() {
    std::array<int, 2> size = {400, 400};

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(size[0], size[1], "mandelbrot");
    SetTargetFPS(60);

    Shader mandelbrot_shader = LoadShader(NULL, "shaders/mandelbrot.frag");
    float zoom = 1.0;

    while (!WindowShouldClose()) {
        size[0] = GetScreenWidth();
        size[1] = GetScreenHeight();
        zoom *= 1 + 0.05 * GetMouseWheelMove();
        zoom = std::clamp(zoom, 0.01f, 10.0f);
        SetShaderValue(mandelbrot_shader,
                       GetShaderLocation(mandelbrot_shader, "resolution"),
                       &size, SHADER_UNIFORM_IVEC2);
        SetShaderValue(mandelbrot_shader,
                       GetShaderLocation(mandelbrot_shader, "zoom"), &zoom,
                       SHADER_UNIFORM_FLOAT);

        BeginDrawing();

        ClearBackground(WHITE);

        {
            BeginShaderMode(mandelbrot_shader);
            DrawRectangle(0, 0, size[0], size[1], WHITE);
            EndShaderMode();
        }

        EndDrawing();
    }

    UnloadShader(mandelbrot_shader);
    CloseWindow();
    return 0;
}
