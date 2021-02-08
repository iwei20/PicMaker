#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>
#include <cmath>

/**
 * Restricts color value to range [0, 255]
 */
short bound(short colorValue) {
    return std::max(std::min(colorValue, (short)255), (short)0);
}

/**
 * Writes RGB color data in PPM format to a file.
 */
template <size_t rows, size_t cols>
void write(std::tuple<short, short, short> (&colorData)[rows][cols], std::ostream& out) {
    out << "P3\n";
    out << rows << " " << cols << "\n";
    out << 255 << "\n"; 

    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            short red, green, blue;
            std::tie(red, green, blue) = colorData[i][j];
            out << red << " " << green << " " << blue << (j == cols - 1 ? "\n" : " ");
        }
    }
}

/**
 * Fills color data with black
 */
template<size_t rows, size_t cols>
void clear(std::tuple<short, short, short> (&colorData)[rows][cols]) {
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            colorData[i][j] = {0, 0, 0};
        }
    }
}

/**
 * Fills color data array with random RGB values, with each value in the triple being bounded by min and max inclusive.
 * Optional modifiers: 
 * Weights that scale the color triples correspondingly
 * Bounds for what part of the image should be filled
 * Minimum and maximum to bound the color values
 */
template <size_t rows, size_t cols>
void fillNoise(std::tuple<short, short, short> (&colorData)[rows][cols], std::tuple<float, float, float> weights = {1, 1, 1}, std::tuple<int, int, int, int> bounds = {0, 0, cols, rows}, int min = 0, int max = 255) {
    int left, top, right, bot;
    std::tie(left, top, right, bot) = bounds;
    for(int i = top; i < bot; ++i) {
        for(int j = left; j < right; ++j) {
            float rweight, gweight, bweight;
            std::tie(rweight, gweight, bweight) = weights;
            colorData[i][j] = {
                bound(rweight * ((rand() % (std::min(max, 255) - std::max(min, 0) + 1) + std::max(min, 0)))), 
                bound(gweight * ((rand() % (std::min(max, 255) - std::max(min, 0) + 1) + std::max(min, 0)))), 
                bound(bweight * ((rand() % (std::min(max, 255) - std::max(min, 0) + 1) + std::max(min, 0))))
            };
        }
    }
}

/**
 * Main image generation part: filling the array with noise but weighting it
 * higher the x, the more green
 * higher the y, the more red
 * higher the distance from the bottom right corner, the more blue
 * also caps the maximum distance based on distance from the center so i can have "shadows" on the edges
 */
template <size_t rows, size_t cols>
void wheelShadowNoise(std::tuple<short, short, short> (&colorData)[rows][cols], int squareSize) {
    float scale = squareSize / 2048.0; 
    int MAX_DIST = sqrt(rows * rows / 4 + cols * cols / 4);
    // 2048 seems to be a magic number of some sort.  256, 512, 1024 all produce magenta-cyan-white gradients with a yellow spot in the middle
    for(int i = 0; i < rows; i += squareSize) {
        for(int j = 0; j < cols; j += squareSize) {
            int xDist = std::abs(j - (int)cols / 2);
            int yDist = std::abs(i - (int)rows / 2);
            int centDist = sqrt(xDist * xDist + yDist * yDist);
            fillNoise(
                colorData, 
                {i * scale, j * scale, sqrt((rows - i) * (rows - i) + (cols - j) * (cols - j)) * scale}, 
                {j, i, j + squareSize, i + squareSize},
                0,
                (float)(MAX_DIST - centDist) / MAX_DIST * 255.0 
            );
        }   
    }
}

int main() { 
    std::ofstream fout("picmaker.ppm");
    std::tuple<short, short, short> image[512][512];

    clear(image);
    wheelShadowNoise(image, 8);
    write(image, fout);
    fout.close();
}

