#version 330

in vec2 fragTexCoord;

out vec4 finalColor;

uniform ivec2 resolution;

vec2 cx_sqr(vec2 v) {
    return vec2(
        v.x * v.x - v.y * v.y,
        2 * v.x * v.y
    );
}

vec3 mandelbrot(vec2 c) {
    const int max_iters = 200;
    vec2 z = vec2(0.0);
    for (int i = 0; i < max_iters; i++) {
        if (dot(z, z) > 2 * 2) {
            return vec3(1.0);
        }
        z = cx_sqr(z) + c;
    }
    return vec3(0.0);
}

void main() {
    vec2 pos = gl_FragCoord.xy / resolution;
    // scale to be in mandelbrot coordinates
    pos.x = pos.x * (2.0 + 0.47) - 2.0;
    pos.y = pos.y * (1.12 + 1.12) - 1.12;
    finalColor = vec4(mandelbrot(pos), 1.0);
}
