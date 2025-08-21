#version 100
precision highp float;

#define complex vec2

uniform ivec2 resolution;
uniform float zoom;
uniform vec2 offset;

vec3 gradient(vec3 start, vec3 end, float point) {
    return start * (1.0 - point) + end * point;
}

complex cx_sqr(complex v) {
    return complex(
        v.x * v.x - v.y * v.y,
        2.0 * v.x * v.y
    );
}

vec3 mandelbrot(complex c) {
    const int max_iters = 200;
    complex z = vec2(0.0);
    for (int i = 0; i < max_iters; i++) {
        if (dot(z, z) > 2.0 * 2.0) {
            vec3 start = vec3(0.2666666, 0.00392156, 0.32941176);
            vec3 end = vec3(0.992156, 0.90588235, 0.14509803);
            return gradient(start, end, float(i) / float(max_iters));
        }
        z = cx_sqr(z) + c;
    }
    return vec3(0.0);
}

void main() {
    complex pos = gl_FragCoord.xy / float(resolution);
    // scale to be in mandelbrot coordinates
    pos.x = pos.x * (2.0 + 0.47) - 2.0;
    pos.y = pos.y * (1.12 + 1.12) - 1.12;
    pos /= zoom;
    pos += offset;
    gl_FragColor = vec4(mandelbrot(pos), 1.0);
}
