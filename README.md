[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/TICbghRr)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-2972f46106e565e64193e422d61a12cf1da4916b45550586e14ef0a7c637dd04.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=21061788)
# Actividad Práctica: Sistema Genérico de Análisis de Transformaciones Lineales

## Definición del Problema a Resolver

Una empresa de simulaciones científicas requiere un módulo fundamental para su software: una herramienta capaz de realizar operaciones básicas con matrices dinámicas **2D** cuyo tipo de datos (entero, flotante, doble) no está predefinido, sino que debe ser flexible para manejar diferentes precisiones numéricas.

El sistema debe permitir al usuario crear, redimensionar y operar con matrices de cualquier tipo numérico. Esto obliga a implementar la matriz dinámica **2D** y las funciones de operación utilizando plantillas (Templates) de **C++**.

El objetivo es crear una clase genérica `MatrizDinamica<T>` que gestione:

1. La asignación de memoria para una matriz $M \times N$ de cualquier tipo `T`.
2. La redimensión de la matriz en tiempo de ejecución.
3. La multiplicación matricial $C = A \times B$ para cualquier tipo numérico `T`.

Este problema sirve como un caso de estudio sobre cómo los templates permiten escribir código eficiente y reutilizable, mientras se mantiene el control total sobre la memoria.

---

## Definición y Detalles del Proceso a Desarrollar

### Estructuras Dinámicas Requeridas

La solución debe centrarse en una clase genérica que encapsule la gestión de la matriz:

```cpp
template <typename T>
class MatrizDinamica {
private:
    T **datos;
    int filas;
    int columnas;

public:
    // Constructor, Destructor, Métodos de gestión y operaciones
};
```

### Proceso de Solución Detallado

1. **Instanciación Genérica:** El programa principal (**main**) debe instanciar dos objetos de la clase `MatrizDinamica`: uno para tipo `int` y otro para tipo `float` (o `double`), demostrando la genericidad.

2. **Asignación de Memoria (Constructor):** El constructor debe recibir las dimensiones (**M×N**) y utilizar punteros dobles (`T**`) para asignar la memoria de la matriz **2D**.
    *   Asignar un vector de punteros de **M** filas (`new T*[M]`).
    *   Para cada fila, asignar un vector de datos de **N** columnas (`new T[N]`).
3.  **Liberación de Memoria (Destructor):** El destructor debe implementar la liberación de memoria en el orden inverso a la asignación (liberar las columnas y luego las filas) utilizando `delete[]` para prevenir fugas de memoria.
4. **Redimensión Dinámica:** Implementar el método `redimensionar(nuevaF, nuevaC)`. Este método debe:

    *   Crear una nueva matriz temporal con las nuevas dimensiones.
    *   Copiar los datos de la matriz antigua a la nueva, conservando solo los elementos dentro del rango de las dimensiones menores.
    *   Liberar la memoria de la matriz antigua.
    *   Asignar la matriz temporal como la nueva matriz de la clase.
5. **Multiplicación Matricial Genérica:** Implementar el método estático o de clase `multiplicar(const MatrizDinamica<T>& A, const MatrizDinamica<T>& B)`. Esta función debe:
    * Validar las dimensiones para la multiplicación ($N_A = N_B$​).
    *   Crear la matriz resultante **C** de dimensión $M_A \times N_B$.
    *   Implementar la fórmula de multiplicación:
        $$
        C_{ij} = \sum_{k=1}^{N_A} A_{ik} \cdot B_{kj}
        $$
    *   Devolver la matriz resultante **C** (gestionando correctamente la devolución de un objeto dinámico).

---

## Requerimientos Funcionales y No Funcionales

### Requisitos Funcionales

*   **Clase Genérica:** Toda la lógica de la matriz (almacenamiento, redimensión, operaciones) debe residir dentro de la clase `MatrizDinamica<T>`.
*   **Constructor y Destructor:** Deben gestionar la asignación y liberación de memoria de forma manual y robusta.
*   **Redimensión:** El método debe manejar casos donde la nueva dimensión sea mayor o menor que la original.
*   **Operación Central:** Implementar la Multiplicación Matricial Genérica ($A \times B$).
*   **Prueba de Genericidad:** El programa principal (**main**) debe demostrar el uso de la misma clase `MatrizDinamica` para al menos dos tipos diferentes (ej., `int` y `float`).

### Requisitos No Funcionales

*   **Uso de Templates:** El código debe ser completamente genérico en términos del tipo de datos almacenado en la matriz.
*   **Validación:** Se deben validar las dimensiones de las matrices antes de cualquier operación de multiplicación o redimensión.

---

## Ejemplo de Entradas y Salidas del Problema en Consola

```
--- Sistema de Análisis de Transformaciones Lineales ---

>> Prueba de Matriz ENTERA (Matriz A) <<
Creando Matriz A (Tipo INT) de 2x3...

Ingresar valores para A (2x3):
A[0,0]: 1
A[0,1]: 2
A[0,2]: 3
A[1,0]: 4
A[1,1]: 5
A[1,2]: 6

>> Redimensionando Matriz A <<
Redimensionando A a 3x3. Datos conservados:
| 1 | 2 | 3 |
| 4 | 5 | 6 |
| 0 | 0 | 0 | (Nuevos valores inicializados a 0)

>> Prueba de Multiplicación (Tipo FLOAT) <<
Creando Matriz B (Tipo FLOAT) de 3x2...
B[0,0]: 1.5, B[0,1]: 0.5
B[1,0]: 2.0, B[1,1]: 1.0
B[2,0]: 1.0, B[2,1]: 2.5

Matriz C = A(3x3) x B(3x2) ...

Matriz C (Resultado 3x2, Tipo FLOAT):
| 8.5 | 9.0 |
| 21.0 | 18.5 |
| 0.0 | 0.0 |

Liberando memoria de todas las matrices...
```

---

## Temas de Investigación Adicional Necesarios

Para llevar a cabo esta actividad con éxito, el alumno deberá dominar, además de los temas vistos en clase, los siguientes conceptos clave:

*   **Regla de los Tres (o Cinco) de C++:** Entender la necesidad de implementar manualmente el Constructor de Copia, el Operador de Asignación y el Destructor para evitar la copia superficial (shallow copy) y las fugas de memoria en clases que manejan punteros.
*   **Excepciones en C++:** Implementación básica de `try-catch` para manejar errores críticos, como la fallida asignación de memoria (`std::bad_alloc`) o la multiplicación de matrices con dimensiones incompatibles.
*   **Inicialización de Memoria en C:** Técnicas para garantizar que las nuevas celdas creadas durante la redimensión se inicialicen correctamente (ej., a cero) para evitar valores basura.

