#include "map.h"
#include <stdio.h>

int seed = 1845;

/**
   @var static int permutation[256]
   @brief Permutation table for remapping all integers on [0, 255]

   Takes an integer between 0 and 255 inclusive and assigns each to a different
   random output.

   Taken from the original permutation by Ken Perlin.
*/
static int permutation[256] = {
        151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225,
        140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148,
        247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32,
        57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175,
        74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,
        60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,
        65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169,
        200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,
        52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212,
        207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213,
        119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9,
        129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104,
        218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,
        81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157,
        184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
        222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180
};

/**
   @fn static int hash(const int x, const int y)
   @brief Hash function outputting a value on [0, 255] for an integer (x, y) coordinate

   Hash function that uses permutation table lookups to return a random number
   [0, 255] given an input on [0, 255].

   Relies on the seed value.

   @param x x-value of coordinate to be hashed
   @param y y-value fo coordinate to be hashed
*/
static int hash(const int x, const int y) {
        return permutation[(permutation[(x + seed) % 256] + y) % 256];
}

/**
   @fn static float vector_product(const int x0, const int y0, const float x, const float y)
   @brief Computes the dot product of a given vector with a random vector.

   Potential output vectors are the orthogonal vectors (1, 0),
   (0, 1), (-1, 0), (0, -1), (1, 1), (1, -1), (-1, 1), and (-1, -1)

   @param x0 Integer value of x-coordinate
   @param y0 Integer value of y-coordinate
   @param x x-value of given vector
   @param y y-value of given vector
*/
static float vector_product(const int x0, const int y0, const float x, const float y) {
        const int h = hash(x0, y0) % 8;

        switch (h) {
        case 0:
		return x;
	case 1:
		return y;
	case 2:
		return -x;
	case 3:
		return -y;
        case 4:
                return x + y;
        case 5:
                return x - y;
        case 6:
                return -x + y;
        case 7:
                return -x - y;
        default:
                return 0;
        }
}

/**
   @fn static float linear_interpolate(float x, float y, float s)
   @brief Linear interpolation of some fraction s between x and y.

   Can be interpreted as L(s) for s in [0, 1] where L(0) = x
   and L(1) = y, and L is a linear function

   @param x Leftmost value
   @param y Rightmost value
   @param s Fraction of the way between the bounds x and y
*/
static float linear_interpolate(float x, float y, float s) {
        return x + s * (y - x);
}

/**
   @fn static float interpolate(float x, float y, float s)
   @brief Quintic interpolation some fraction s between x and y.

   Unlike the linear function, this function flattens out at 0 and 1,
   which allows the output of the interpolation to be twice
   differentiable rather than merely continuous.

   @param x Leftmost value
   @param y Rightmost value
   @param s Fraction of the way between the bounds x and y
*/
static float interpolate(float x, float y, float s) {
        return linear_interpolate(x, y, s * s * s * (s * (6 * s - 15) + 10));
}

/**
   @fn static float perlin2d(const float x, const float y)
   @brief Generate a noise value between 0 and 1 for some real coordinates

   Uses random vectors to compute a random noise value for some real
   coordinates (x, y) in a plane.

   Implements Perlin's "Improving Noise" paper.

   @param x Real-valued x-coordinate of point
   @param y Real-valued y-coordinate of point
*/
static float perlin2d(const float x, const float y) {
        const int x0 = (int)x;
        const int y0 = (int)y;

        const int x1 = x0 + 1;
        const int y1 = y0 + 1;

        const float xf = x - x0;
        const float yf = y - y0;

        const float d00 = vector_product(x0, y0, xf, yf);
        const float d10 = vector_product(x1, y0, xf - 1, yf);
        const float d01 = vector_product(x0, y1, xf, yf - 1);
        const float d11 = vector_product(x1, y1, xf - 1, yf - 1);

        const float s = interpolate(d00, d10, xf);
        const float t = interpolate(d01, d11, xf);

        return interpolate(s, t, yf);
}

float fractal_perlin2d(float x, float y, float freq, int depth) {
        float v = 0;
        float amp = 1;
        int i;

        for (i = 0; i < depth; i++) {
                v += perlin2d(x * freq, y * freq) * amp;

                amp /= 2;
                freq *= 2;
        }

        return v;
}
