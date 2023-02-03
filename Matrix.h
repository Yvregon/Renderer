#include <math.h>
#include <vector>
#include <stdexcept>
#include "figures.h"

class Matrix{

    private:
        
    public:
        std::vector<std::vector<float>> mat;
        int width;
        int height;

        Matrix(int width, int height, float value);

        Matrix identity(int width, int height);

        Matrix operator+(Matrix m);

        Matrix operator-(Matrix m);

        Matrix operator*(Matrix m);

        Matrix operator*(float f);

};
