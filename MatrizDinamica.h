#ifndef MATRIZ_DINAMICA_H
#define MATRIZ_DINAMICA_H

#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <new> 

template <typename T>
class MatrizDinamica {
private:
    T** datos;
    int filas;
    int columnas;

public:
    // Constructor vacío no hay memoria, no hay
    MatrizDinamica() : datos(nullptr), filas(0), columnas(0) {}

    // Constructor con tamaño: se apartan filas y columnas 
    // Funciona porque creamos un arreglo de punteros (filas) y cada fila es un arreglo T[col].
    MatrizDinamica(int f, int c) : datos(nullptr), filas(0), columnas(0) {
        if (f < 0 || c < 0) throw std::invalid_argument("dims negativas");
        if (f == 0 || c == 0) { datos = nullptr; filas = f; columnas = c; return; }
        datos = new T*[f]; // si truena, pues excepción
        try {
            for (int i = 0; i < f; ++i) {
                datos[i] = new T[c]{}; // {} = T{} = ceros pa' numéricos; no dejamos basura
            }
        } catch (...) {
            for (int i = 0; i < f; ++i) if (datos[i]) delete[] datos[i];
            delete[] datos; datos = nullptr;
            throw;
        }
        filas = f; columnas = c;
    }

    // Ctor copia: copia profunda, no prestamos los calcetines.
    // Funciona porque reservamos NUEVA memoria y clonamos celda por celda.
    MatrizDinamica(const MatrizDinamica& o) : datos(nullptr), filas(0), columnas(0) {
        if (o.filas == 0 || o.columnas == 0) { return; }
        datos = new T*[o.filas];
        try {
            for (int i = 0; i < o.filas; ++i) datos[i] = new T[o.columnas]{};
        } catch (...) {
            for (int i = 0; i < o.filas; ++i) if (datos[i]) delete[] datos[i];
            delete[] datos; datos = nullptr; throw;
        }
        filas = o.filas; columnas = o.columnas;
        for (int i = 0; i < filas; ++i)
            for (int j = 0; j < columnas; ++j)
                datos[i][j] = o.datos[i][j];
    }

    // Ctor movimiento
    MatrizDinamica(MatrizDinamica&& o) noexcept : datos(o.datos), filas(o.filas), columnas(o.columnas) {
        o.datos = nullptr; o.filas = 0; o.columnas = 0;
    }

    // Asignación por copia
    MatrizDinamica& operator=(const MatrizDinamica& o) {
        if (this == &o) return *this;
        if (filas != o.filas || columnas != o.columnas) {
            if (datos) { for (int i = 0; i < filas; ++i) delete[] datos[i]; delete[] datos; datos = nullptr; }
            if (o.filas == 0 || o.columnas == 0) { filas = 0; columnas = 0; return *this; }
            datos = new T*[o.filas];
            try {
                for (int i = 0; i < o.filas; ++i) datos[i] = new T[o.columnas]{};
            } catch (...) {
                for (int i = 0; i < o.filas; ++i) if (datos[i]) delete[] datos[i];
                delete[] datos; datos = nullptr; throw;
            }
            filas = o.filas; columnas = o.columnas;
        }
        // Copiado a la mala: for anidado y ya.
        for (int i = 0; i < filas; ++i)
            for (int j = 0; j < columnas; ++j)
                datos[i][j] = o.datos[i][j];
        return *this;
    }

    // Asignación por movimiento
    MatrizDinamica& operator=(MatrizDinamica&& o) noexcept {
        if (this == &o) return *this;
        if (datos) { for (int i = 0; i < filas; ++i) delete[] datos[i]; delete[] datos; }
        datos = o.datos; filas = o.filas; columnas = o.columnas;
        o.datos = nullptr; o.filas = 0; o.columnas = 0;
        return *this;
    }

    // Destructor cada new[] tiene su delete[], 
    ~MatrizDinamica() {
        if (datos) {
            for (int i = 0; i < filas; ++i) delete[] datos[i];
            delete[] datos;
        }
        datos = nullptr; filas = 0; columnas = 0;
    }

    // getters 
    int numFilas() const { return filas; }
    int numColumnas() const { return columnas; }

    // Acceso sin chequeo rapidito
    T& operator()(int i, int j) { return datos[i][j]; }
    const T& operator()(int i, int j) const { return datos[i][j]; }

    // Acceso con chequeo
    T& at(int i, int j) {
        if (i < 0 || i >= filas || j < 0 || j >= columnas) throw std::out_of_range("at fuera de rango");
        return datos[i][j];
    }
    const T& at(int i, int j) const {
        if (i < 0 || i >= filas || j < 0 || j >= columnas) throw std::out_of_range("at fuera de rango");
        return datos[i][j];
    }

    
    // Funciona porque los nuevos huecos salen como T{} y no quedan “basura”.
    void redimensionar(int nf, int nc) {
        if (nf < 0 || nc < 0) throw std::invalid_argument("dims negativas redimensionar");
        if (nf == filas && nc == columnas) return;
        if (nf == 0 || nc == 0) {
            if (datos) { for (int i = 0; i < filas; ++i) delete[] datos[i]; delete[] datos; }
            datos = nullptr; filas = nf; columnas = nc; return;
        }
        T** nuevo = new T*[nf];
        try {
            for (int i = 0; i < nf; ++i) nuevo[i] = new T[nc]{};
        } catch (...) {
            for (int i = 0; i < nf; ++i) if (nuevo[i]) delete[] nuevo[i];
            delete[] nuevo; throw;
        }
        int minF = (nf < filas ? nf : filas);
        int minC = (nc < columnas ? nc : columnas);
        for (int i = 0; i < minF; ++i)
            for (int j = 0; j < minC; ++j)
                nuevo[i][j] = datos[i][j];
        if (datos) { for (int i = 0; i < filas; ++i) delete[] datos[i]; delete[] datos; }
        datos = nuevo; filas = nf; columnas = nc;
    }

    // Multiplicación mismo tipo triple 
    static MatrizDinamica multiplicar(const MatrizDinamica& A, const MatrizDinamica& B) {
        if (A.columnas != B.filas) throw std::invalid_argument("dims incompatibles");
        MatrizDinamica C(A.filas, B.columnas);
        for (int i = 0; i < A.filas; ++i) {
            for (int j = 0; j < B.columnas; ++j) {
                T s = T{};
                for (int k = 0; k < A.columnas; ++k) s += A.datos[i][k] * B.datos[k][j];
                C.datos[i][j] = s;
            }
        }
        return C; 
    }
};

// Mezcla de tipos subimos al tipo común 
template <typename A, typename B>
MatrizDinamica<typename std::common_type<A,B>::type>
multiplicar(const MatrizDinamica<A>& X, const MatrizDinamica<B>& Y) {
    if (X.numColumnas() != Y.numFilas()) throw std::invalid_argument("dims incompatibles mix");
    using R = typename std::common_type<A,B>::type;
    MatrizDinamica<R> Z(X.numFilas(), Y.numColumnas());
    for (int i = 0; i < X.numFilas(); ++i) {
        for (int j = 0; j < Y.numColumnas(); ++j) {
            R s = R{};
            for (int k = 0; k < X.numColumnas(); ++k) s += static_cast<R>(X(i,k)) * static_cast<R>(Y(k,j));
            Z(i,j) = s;
        }
    }
    return Z;
}

#endif 
