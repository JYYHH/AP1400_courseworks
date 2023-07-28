#include "hw1.h"
#include <ctime>
#include <iomanip>
#include <random>
#include <iostream>
#include <functional> 

namespace algebra{
    int op_time = 0;
    bool if_neglect_upper_triangular = 0;
    bool is_zero(double x){
        return fabs(x) < 1e-6;
    }

    Matrix Constant(std::size_t n, std::size_t m, double cnst = 0.0){
        Matrix ret;
        for(int i=0;i<n;i++){
            Vector tmp;
            for(int j=0;j<m;j++) tmp.pb(cnst);
            ret.pb(tmp);
        }
        return ret;
    }

    Matrix zeros(std::size_t n, std::size_t m){
        return Constant(n, m, 0.0);
    }

    Matrix ones(std::size_t n, std::size_t m){
        return Constant(n, m, 1.0);
    }

    Matrix identitys(std::size_t n){
        Matrix ret = zeros(n, n);
        for(int i=0;i<n;i++) ret[i][i] += 1.0;
        return ret;
    }

    Matrix random(std::size_t n, std::size_t m, double min, double max){
        if (min > max)
            throw std::logic_error( "random: min is larger than max" );

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(min, max);
        Matrix ret;
        for(int i=0;i<n;i++){
            Vector tmp;
            for(int j=0;j<m;j++) tmp.pb(dist(mt));
            ret.pb(tmp);
        }
        return ret;
    }

    void show(const Matrix& matrix){
        int sz1 = matrix.size(), sz2 = sz1 == 0 ? 0 : matrix[0].size();
        for(int i=0;i<sz1;i++){
            for(int j=0;j<sz2;j++) printf("%.2lf ", matrix[i][j]);
            std::cout << std::endl;
        }
    }

    Matrix multiply(const Matrix& matrix, double c){
        int sz1 = matrix.size(), sz2 = sz1 == 0 ? 0 : matrix[0].size();
        Matrix ret(matrix);
        for(int i=0;i<sz1;i++){
            for(int j=0;j<sz2;j++)
                ret[i][j] = ret[i][j] * c;
        }
        return ret;
    }

    Matrix multiply(const Matrix& matrix1, const Matrix& matrix2){
        int sz11 = matrix1.size(), sz12 = sz11 == 0 ? 0 : matrix1[0].size();
        int sz21 = matrix2.size(), sz22 = sz21 == 0 ? 0 : matrix2[0].size();

        if (sz12 != sz21) 
            throw std::logic_error( "multiply: number of col of mat1 not equals to number of row of mat2" );
        
        Matrix ret = zeros(sz11, sz22);
        for(int k=0;k<sz12;k++)
            for(int i=0;i<sz11;i++)
                for(int j=0;j<sz22;j++)
                    ret[i][j] += matrix1[i][k] * matrix2[k][j];
        return ret;
    }

    Matrix sum(const Matrix& matrix, double c){
        int sz1 = matrix.size(), sz2 = sz1 == 0 ? 0 : matrix[0].size();
        Matrix ret(matrix);
        for(int i=0;i<sz1;i++){
            for(int j=0;j<sz2;j++)
                ret[i][j] = ret[i][j] + c;
        }
        return ret;
    }

    Matrix sum(const Matrix& matrix1, const Matrix& matrix2){
        int sz11 = matrix1.size(), sz12 = sz11 == 0 ? 0 : matrix1[0].size();
        int sz21 = matrix2.size(), sz22 = sz21 == 0 ? 0 : matrix2[0].size();
        if (sz11 != sz21 || sz12 != sz22) 
            throw std::logic_error( "sum: two matrices are in diff shape" );
        
        Matrix ret(matrix1);
        for(int i=0;i<sz11;i++){
            for(int j=0;j<sz12;j++)
                ret[i][j] = ret[i][j] + matrix2[i][j];
        }
        return ret;
    }

    Matrix transpose(const Matrix& matrix){
        int n = matrix.size(), m = n == 0 ? 0 : matrix[0].size();
        Matrix ret;
        for(int i=0;i<m;i++){
            Vector tmp;
            for(int j=0;j<n;j++) tmp.pb(matrix[j][i]);
            ret.pb(tmp);
        }
        return ret;
    }

    Matrix minor(const Matrix& matrix, std::size_t n, std::size_t m){
        int sz1 = matrix.size(), sz2 = sz1 == 0 ? 0 : matrix[0].size();
        if (n < 0 || n >= sz1 || m < 0 || m >= sz2)
            throw std::logic_error( "minor: out of range" );
        Matrix ret;
        for(int i=0;i<sz1;i++) 
            if (i != n){
                Vector tmp;
                for(int j=0;j<sz2;j++)
                    if (j != m)
                        tmp.pb(matrix[i][j]);
                ret.pb(tmp);
            }
        return ret;
    }

    Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis=0){
        int sz11 = matrix1.size(), sz12 = sz11 == 0 ? 0 : matrix1[0].size();
        int sz21 = matrix2.size(), sz22 = sz21 == 0 ? 0 : matrix2[0].size();
        Matrix ret;

        if (axis == 0){
            if (sz12 != sz22) 
                throw std::logic_error( "concatenate: two matrices are in diff shape > column" );
            for(int i=0;i<sz11;i++){
                Vector tmp;
                for(int j=0;j<sz12;j++) tmp.pb(matrix1[i][j]);
                ret.pb(tmp);
            }
            for(int i=0;i<sz21;i++){
                Vector tmp;
                for(int j=0;j<sz22;j++) tmp.pb(matrix2[i][j]);
                ret.pb(tmp);
            }
        }
        else{
            if (sz11 != sz21) 
                throw std::logic_error( "concatenate: two matrices are in diff shape > row" );
            for(int i=0;i<sz11;i++){
                Vector tmp;
                for(int j=0;j<sz12;j++) tmp.pb(matrix1[i][j]);
                for(int j=0;j<sz22;j++) tmp.pb(matrix2[i][j]);
                ret.pb(tmp);
            }
        }

        return ret;
    }

    Matrix ero_swap(const Matrix& matrix, std::size_t r1, std::size_t r2){
        int sz1 = matrix.size();
        if (r1 < 0 || r1 >= sz1 || r2 < 0 || r2 >= sz1)
            throw std::logic_error( "ero_swap: out of range" );

        Matrix ret(matrix);
        ret[r1].swap(ret[r2]);
        return ret;
    }

    // protected
    void Ero_swap(Matrix& matrix, std::size_t r1, std::size_t r2){ // O(N)
        matrix[r1].swap(matrix[r2]);
    }

    Matrix ero_multiply(const Matrix& matrix, std::size_t r, double c){
        int sz1 = matrix.size();
        if (r < 0 || r >= sz1)
            throw std::logic_error( "ero_multiply: out of range" );

        Matrix ret(matrix);
        int m = sz1 == 0 ? 0 : matrix[0].size();

        for(int j=0;j<m;j++)
            ret[r][j] *= c;
        
        return ret;
    }

    // protected
    void Ero_multiply(Matrix& matrix, std::size_t r, double c){ // O(N)
        int m = matrix.size() == 0 ? 0 : matrix[0].size();
        for(int j=0;j<m;j++)
            matrix[r][j] *= c;
    }

    Matrix ero_sum(const Matrix& matrix, std::size_t r1, double c, std::size_t r2){
        int sz1 = matrix.size();
        if (r1 < 0 || r1 >= sz1 || r2 < 0 || r2 >= sz1)
            throw std::logic_error( "ero_sum: out of range" );

        Matrix ret(matrix);
        int m = sz1 == 0 ? 0 : matrix[0].size();

        for(int j=0;j<m;j++)
            ret[r2][j] += ret[r1][j] * c;
        
        return ret;
    }

    // protected
    void Ero_sum(Matrix& matrix, std::size_t r1, double c, std::size_t r2){ // O(N)
        int m = matrix.size() == 0 ? 0 : matrix[0].size();
        for(int j=0;j<m;j++)
            matrix[r2][j] += matrix[r1][j] * c;
    }

    Matrix upper_triangular(const Matrix& matrix){
        // only used when n == m
        Matrix ret(matrix);
        int n = matrix.size(), m = n == 0 ? 0 : matrix[0].size();
        int i=0, j=0;
        op_time = 0;

        if (n != m && if_neglect_upper_triangular == 0)
            throw std::logic_error( "upper_triangular: row not equals to col" );

        while (i<n && j<m){
            if (is_zero(ret[i][j])){
                for(int k=i+1;k<n;k++) 
                    if (!is_zero(ret[k][j])){ 
                        Ero_swap(ret, i, k);
                        op_time ++ ;
                        break;
                    }
            }

            if (is_zero(ret[i][j])){
                j++;
                continue;
            }

            for(int k=i+1;k<n;k++)
                if (!is_zero(ret[k][j])){
                    double C = ret[k][j] / ret[i][j];
                    Ero_sum(ret, i, -C, k);
                }

            i++, j++;
        }
        // printf("%d\n", op_time);
        return ret;
    }

    double determinant(const Matrix& matrix){
        int n = matrix.size(), m = n == 0 ? 0 : matrix[0].size();
        if (n != m)
            throw std::logic_error( "determinant: row not equals to col" );
        if (n == 0)
            return 1.0;
        
        double ret = 1.0;
        if_neglect_upper_triangular = 0;
        Matrix mid = upper_triangular(matrix);
        for(int i=0;i<n;i++) ret *= mid[i][i];
        if (op_time & 1)
            ret = -ret;
        
        return ret;
    }

    Matrix inverse(const Matrix& matrix){
        int n = matrix.size(), m = n == 0 ? 0 : matrix[0].size();
        if (n != m)
            throw std::logic_error( "inverse: row not equals to col" );
        if (n == 0)
            return matrix;
        double det = determinant(matrix);
        if (is_zero(det))
            throw std::logic_error( "inverse: det = 0" );

        // // method 1: using Minor (O(n^5))
        // Matrix ret;
        
        // for(int i=0;i<n;i++){
        //     Vector tmp;
        //     for(int j=0;j<m;j++)
        //         if (i+j&1) tmp.pb(-determinant(minor(matrix, i, j)));
        //         else tmp.pb(determinant(minor(matrix, i, j)));
        //     ret.pb(tmp);
        // }

        // return transpose(multiply(ret, 1.0/det));

        // method 2: faster method using concatenate (O(n^3))
        show(matrix);
        Matrix ret, mid = concatenate(matrix, identitys(n), 1);
        if_neglect_upper_triangular = 1;
        mid = upper_triangular(mid); // get an upper_tri for the left side, and middle state of the right side
        if_neglect_upper_triangular = 0; // recover
        // make the left side from upper_tri into Identity
        show(mid);
        for(int i=n-1;i>=0;i--){
            for(int j=i+1;j<m;j++) if(!is_zero(mid[i][j]))
                Ero_sum(mid, j, -mid[i][j], i);
            Ero_multiply(mid, i, 1.0/mid[i][i]);
        }
        // put the right side matrix as the inverse
        show(mid);
        for(int i=0;i<n;i++){
            Vector tmp;
            for(int j=0;j<m;j++) tmp.pb(mid[i][j+m]);
            ret.pb(tmp);
        }
        return ret;
    }
}