#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>

template <size_t rows, size_t cols>
void drawToFile(std::ofstream& fout, std::tuple<short, short, short> (&colorData)[rows][cols]) {
    fout << "P3\n";
    fout << rows << " " << cols << "\n";
    fout << 255 << "\n"; 

    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            short red, green, blue;
            std::tie(red, green, blue) = colorData[i][j];
            fout << red << " " << green << " " << blue << (j == cols - 1 ? "\n" : " ");
        }
    }
}

int main() {
    std::ofstream fout("picmaker.ppm");
    std::tuple<short, short, short> red[256][256];

    for(int i = 0; i < 256; ++i) {
        for(int j = 0; j < 256; ++j) {
            red[i][j] = {255, 0, 0};
        }
    }

    drawToFile(fout, red);
    fout.close();
}

