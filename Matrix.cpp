#include "Matrix.h"

Matrix::Matrix(int width, int height, float value){

    this->width = width;
    this->height = height;

    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            this->mat[i][j] = value;
        }
    }
}

Matrix Matrix::identity(int width, int height){
    Matrix m = Matrix(width, height, 1.f);

    return m;
}

Matrix Matrix::operator+(Matrix m){

    if(m.width == this->width && m.height == this->height){

        Matrix r = Matrix(width, height, 0.f);

        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                r.mat[i][j] = this->mat[i][j] + m.mat[i][j];
            }
        }

        return r;

    }else{
        throw std::length_error("Non-matching matrices");
    }
}

Matrix Matrix::operator-(Matrix m){

    if(m.width == this->width && m.height == this->height){

        Matrix r = Matrix(width, height, 0.f);

        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                r.mat[i][j] = this->mat[i][j] - m.mat[i][j];
            }
        }

        return r;

    }else{
        throw std::length_error("Non-matching matrices");
    }
}

Matrix Matrix::operator*(Matrix m){

    if(m.height == this->width && m.width == this->height){

        Matrix r = Matrix(height, width, 0.f);

        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                for(int k = 0; k < height; k++){

                    r.mat[i][j] += this->mat[i][k] * m.mat[k][j];
                }
            }
        }

        return r;

    }else{
        throw std::length_error("Non-matching matrices");
    }
}

Matrix Matrix::operator*(float f){

    Matrix r = Matrix(width, height, 0.f);

    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            r.mat[i][j] *= f;
        }
    }

    return r;
}
