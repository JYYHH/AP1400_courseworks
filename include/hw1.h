#ifndef AP_HW1_H
#define AP_HW1_H


#endif //AP_HW1_H

#include <vector>
using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;
#define pb push_back

namespace algebra{
    Matrix zeros(std::size_t n, std::size_t m);
    Matrix ones(std::size_t n, std::size_t m);
    Matrix identitys(std::size_t n);
    Matrix random(std::size_t n, std::size_t m, double min, double max);
    void show(const Matrix& matrix);
    Matrix multiply(const Matrix& matrix, double c);
    Matrix multiply(const Matrix& matrix1, const Matrix& matrix2);
    Matrix sum(const Matrix& matrix, double c);
    Matrix sum(const Matrix& matrix1, const Matrix& matrix2);
    Matrix transpose(const Matrix& matrix);
    Matrix minor(const Matrix& matrix, std::size_t n, std::size_t m);
    double determinant(const Matrix& matrix);
    Matrix inverse(const Matrix& matrix);
    Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis);
    Matrix ero_swap(const Matrix& matrix, std::size_t r1, std::size_t r2);
    // protected
    void Ero_swap(Matrix& matrix, std::size_t r1, std::size_t r2); // O(N)
    Matrix ero_multiply(const Matrix& matrix, std::size_t r, double c);
    // protected
    void Ero_multiply(Matrix& matrix, std::size_t r, double c); // O(N)
    Matrix ero_sum(const Matrix& matrix, std::size_t r1, double c, std::size_t r2);
    // protected
    void Ero_sum(Matrix& matrix, std::size_t r1, double c, std::size_t r2); // O(N)
    Matrix upper_triangular(const Matrix& matrix);
}