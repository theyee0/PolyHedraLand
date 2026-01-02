#include "render.h"
#include <assert.h>
#include <stdio.h>

float terrain_freq = 0.073;
int terrain_depth = 1;
int chunk_size = 200;
float peak_height = 10;
double triangle_base = 1;

float roundto(float x, int v) {
        int xi = floor(x);

        if (xi % v > v / 2) {
                return xi - (xi % v) + v;
        } else {
                return xi - (xi % v);
        }
}
/*
void render_chunk_equillateral(const float x, const float y) {
        int i, j;
        Vector3 v1, v2, v3;

        const int half_chunk = chunk_size / 2 / triangle_base;
        const double triangle_height = triangle_base * sqrt(3.0) / 2.0;
        const double half_triangle_base = triangle_base / 2.0;

        for (i = -half_chunk; i < half_chunk; i++) {
                for (j = -half_chunk; j < half_chunk; j++) {
                        v1.x = roundto(x, triangle_base) + i * triangle_base;
                        v1.z = roundto(y, triangle_base) + j * triangle_height;
                        v1.y = peak_height * tanh(fractal_perlin2d(v1.x, v1.z, terrain_freq, terrain_depth));

                        v2.x = roundto(x, triangle_base) + (i + 1) * triangle_base;
                        v2.z = roundto(y, triangle_base) + j * triangle_height;
                        v2.y = peak_height * tanh(fractal_perlin2d(v2.x, v2.z, terrain_freq, terrain_depth));

                        v3.x = roundto(x, triangle_base) + i * triangle_base + half_triangle_base;
                        v3.z = roundto(y, triangle_base) + (j + 1) * triangle_height;
                        v3.y = peak_height * tanh(fractal_perlin2d(v3.x, v3.z, terrain_freq, terrain_depth));

                        DrawTriangle3D(v3, v2, v1, GREEN);
                }
        }
}
*/

void render_chunk(const float x, const float y) {
        const int x0 = roundto(x, triangle_base);
        const int y0 = roundto(y, triangle_base);
        
        int i, j;
        Vector3 v1, v2, v3, v4;

        const int half_chunk = chunk_size / 2 / triangle_base;

        for (i = -half_chunk; i < half_chunk; i++) {
                for (j = -half_chunk; j < half_chunk; j++) {
                        v1.x = x0 + i * triangle_base;
                        v1.z = y0 + j * triangle_base;
                        v1.y = fractal_perlin2d(v1.x, v1.z, terrain_freq, terrain_depth) * peak_height;

                        v2.x = x0 + (i + 1) * triangle_base;
                        v2.z = y0 + j * triangle_base;
                        v2.y = fractal_perlin2d(v2.x, v2.z, terrain_freq, terrain_depth) * peak_height;

                        v3.x = x0 + i * triangle_base;
                        v3.z = y0 + (j + 1) * triangle_base;
                        v3.y = fractal_perlin2d(v3.x, v3.z, terrain_freq, terrain_depth) * peak_height;

                        v4.x = x0 + (i + 1) * triangle_base;
                        v4.z = y0 + (j + 1) * triangle_base;
                        v4.y = fractal_perlin2d(v4.x, v4.z, terrain_freq, terrain_depth) * peak_height;

                        DrawTriangle3D(v3, v2, v1, GREEN);
                        DrawTriangle3D(v4, v2, v3, RED);
                }
        }
}
