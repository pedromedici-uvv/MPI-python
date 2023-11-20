// OLÁ MUNDO

#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
    // Inicialização do MPI
    MPI_Init(&argc, &argv);

    // Obtém o número total de processos e o rank do processo atual
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Imprime a mensagem Hello World com o rank do processo
    std::cout << "Hello world, from process #" << rank << std::endl;

    // Finaliza o MPI
    MPI_Finalize();

    return 0;
}