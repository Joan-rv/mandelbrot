#include <raylib.h>
#include <raymath.h>

#include <array>

int main() {
    std::array<int, 2> size = {400, 400};

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(size[0], size[1], "mandelbrot");
    SetTargetFPS(60);

    Shader mandelbrot_shader = LoadShader(NULL, "shaders/mandelbrot.frag");

    while (!WindowShouldClose()) {
        size[0] = GetScreenWidth();
        size[1] = GetScreenHeight();
        SetShaderValue(mandelbrot_shader,
                       GetShaderLocation(mandelbrot_shader, "resolution"),
                       &size, SHADER_UNIFORM_IVEC2);

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
