#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <array>

#ifdef PLATFORM_DESKTOP
#define GLSL_VERSION 330
#else // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

int main() {
    std::array<int, 2> size = {400, 400};
    std::array<float, 2> offset = {0.f, 0.f};
    float zoom = 1.0;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(size[0], size[1], "mandelbrot");
    SetTargetFPS(60);

    Shader mandelbrot_shader =
        LoadShader(NULL, TextFormat("shaders/mandelbrot%i.frag", GLSL_VERSION));
    auto set_mandelbrot_uniform =
        [&](const char *name, const void *value, ShaderUniformDataType type) {
            int loc = GetShaderLocation(mandelbrot_shader, name);
            SetShaderValue(mandelbrot_shader, loc, value, type);
        };

    while (!WindowShouldClose()) {
        size[0] = GetScreenWidth();
        size[1] = GetScreenHeight();
        zoom *= 1 + 0.05 * GetMouseWheelMove();
        zoom = std::max(0.01f, zoom);
        if (IsKeyDown(KEY_W))
            offset[1] += 0.1f / zoom;
        if (IsKeyDown(KEY_S))
            offset[1] -= 0.1f / zoom;
        if (IsKeyDown(KEY_D))
            offset[0] += 0.1f / zoom;
        if (IsKeyDown(KEY_A))
            offset[0] -= 0.1f / zoom;

        set_mandelbrot_uniform("resolution", &size, SHADER_UNIFORM_IVEC2);
        set_mandelbrot_uniform("zoom", &zoom, SHADER_UNIFORM_FLOAT);
        set_mandelbrot_uniform("offset", &offset, SHADER_UNIFORM_VEC2);

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
