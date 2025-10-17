#include <iostream>
#include <iomanip>   
#include <stdexcept>
#include "MatrizDinamica.h"

int main() {
    try {
        std::cout << "--- Sistema de Transformaciones Lineales (versión taquera) ---\n\n";

        // A: INT 2x3, luego lo redimensionamos a 3x3 pa' que haga match.
        MatrizDinamica<int> A(2, 3);
        // Funciona porque asignamos celda por celda como si fuera arreglo normal.
        A(0,0)=1; A(0,1)=2; A(0,2)=3;
        A(1,0)=4; A(1,1)=5; A(1,2)=6;

        std::cout << "A (2x3) cargada a patin. Redimension a 3x3 (los nuevos a 0):\n";
        A.redimensionar(3,3);

        //  FLOAT 3x2, como en el ejemplo
        MatrizDinamica<float> B(3,2);
        B(0,0)=1.5f; B(0,1)=0.5f;
        B(1,0)=2.0f; B(1,1)=1.0f;
        B(2,0)=1.0f; B(2,1)=2.5f;

        // C = A x B (mezcla tipos) sube a float por common_type
        auto C = multiplicar(A, B);

        // Impresión pedestre for anidados cero helpers
        auto printInt = [](const MatrizDinamica<int>& M){
            for (int i=0;i<M.numFilas();++i){ std::cout<<"|";
                for (int j=0;j<M.numColumnas();++j){ std::cout<<" "<<M(i,j)<<" "; }
                std::cout<<"|\n";
            }
        };
        auto printFloat1 = [](const MatrizDinamica<float>& M){
            std::cout.setf(std::ios::fixed); std::cout<<std::setprecision(1);
            for (int i=0;i<M.numFilas();++i){ std::cout<<"|";
                for (int j=0;j<M.numColumnas();++j){ std::cout<<" "<<M(i,j)<<" "; }
                std::cout<<"|\n";
            }
            std::cout.unsetf(std::ios::fixed);
        };

        std::cout << "\nA (3x3, int):\n";    printInt(A);
        std::cout << "\nB (3x2, float):\n";  printFloat1(B);
        std::cout << "\nC = A x B (3x2, float):\n"; printFloat1(C);

        // Prueba de que no se cae si cambias tamaño
        std::cout << "\nRedimensionando B a 2x2 nomas por molestar...\n";
        B.redimensionar(2,2); // los recortes se pierden
        printFloat1(B);

        std::cout << "\nTodo chill. Memoria liberada al final sin lloriquear.\n";
        return 0;
    } catch (const std::bad_alloc& e) {
        std::cerr << "Se acabo la RAM: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Explosion controlada: " << e.what() << "\n";
        return 1;
    }
}
