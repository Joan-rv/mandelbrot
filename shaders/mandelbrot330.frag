#version 330

#extension GL_ARB_gpu_shader_fp64: enable
#ifdef GL_ARB_gpu_shader_fp64
#define complex dvec2
#else
#define complex vec2
#endif

in vec2 fragTexCoord;

out vec4 finalColor;

uniform ivec2 resolution;
uniform float zoom;
uniform vec2 offset;

vec3 gradient(vec3 start, vec3 end, float point) {
    return start * (1 - point) + end * point;
}

complex cx_sqr(complex v) {
    return complex(
        v.x * v.x - v.y * v.y,
        2 * v.x * v.y
    );
}

vec3 mandelbrot(complex c) {
    const int max_iters = 200;
    complex z = vec2(0.0);
    for (int i = 0; i < max_iters; i++) {
        if (dot(z, z) > 2 * 2) {
            vec3 start = vec3(0x44 / 255.0, 0x01 / 255.0, 0x54 / 255.0);
            vec3 end = vec3(0xfd / 255.0, 0xe7 / 255.0, 0x25 / 255.0);
            return gradient(start, end, float(i) / max_iters);
        }
        z = cx_sqr(z) + c;
    }
    return vec3(0.0);
}

void main() {
    complex pos = gl_FragCoord.xy / resolution;
    // scale to be in mandelbrot coordinates
    pos.x = pos.x * (2.0 + 0.47) - 2.0;
    pos.y = pos.y * (1.12 + 1.12) - 1.12;
    pos /= zoom;
    pos += offset;
    finalColor = vec4(mandelbrot(pos), 1.0);
}
