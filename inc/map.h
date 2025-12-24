#ifndef __MAP_H
#define __MAP_H

/**
   @var int seed
   @brief Seed for random perlin noise
*/
extern int seed;

/**
   @fn float fractal_perlin2d(float x, float y, float freq, int depth);
   @brief Compute fractal perlin noise at given (x, y) coordinate

   This function uses an internal pseudorandom algorithm for
   computing a single point on a perlin noise map. A given
   (x, y) pair will always yield the same results, so computations
   do not necessarily need to be stored between calls.

   @param x x-value of the point at which the noise is calculated
   @param y y-value of the point at which the noise is calculated
   @param freq Frequency of generated noise map
   @param depth Number of octaves of noise combined
*/
float fractal_perlin2d(float x, float y, float freq, int depth);

#endif
