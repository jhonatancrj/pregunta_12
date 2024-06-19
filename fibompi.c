#include <stdio.h>
#include <mpi.h>

int fibonacci(int n) {
    if (n <= 1)
        return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main(int argc, char *argv[]) {
    int rank, size;
    int termino, resultado;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) { // Proceso maestro
        printf("Ingrese el número de términos de la secuencia de Fibonacci: ");
        scanf("%d", &termino);
        for (int i = 1; i < size; i++) {
            MPI_Send(&termino, 1, MPI_INT, i, 0, MPI_COMM_WORLD); // Envía el término a calcular a cada esclavo
        }
        resultado = fibonacci(termino);
        printf("El resultado del término %d es %d\n", termino, resultado);
        for (int i = 1; i < size; i++) {
            MPI_Recv(&resultado, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Recibe los resultados de los esclavos
            printf("El resultado del término %d es %d\n", termino, resultado);
        }
    } else { // Procesos esclavos
        MPI_Recv(&termino, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Recibe el término a calcular del maestro
        resultado = fibonacci(termino);
        MPI_Send(&resultado, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // Envía el resultado al maestro
    }

    MPI_Finalize();
    return 0;
}
