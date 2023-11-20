// Comunicações ponto a ponto, exercício 1

#include <iostream>
#include <mpi.h>
#include <cstdlib>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Read the local value of the process
  // local_value will hold a specific int for process 0, and another for process 1
  int local_value;
  local_value = atoi(argv[1]);

  int other_value;

  if (rank == 0) {
    // Processo #0 envia o valor para o Processo #1
    MPI_Send(&local_value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

    // Processo #0 recebe o valor do Processo #1 em other_value
    MPI_Recv(&other_value, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Imprime a soma dos dois valores em stdout
    std::cout << "Process #0: Soma = " << local_value + other_value << std::endl;
  } else {
    // Processo #1 recebe o valor do Processo #0 em other_value
    MPI_Recv(&other_value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Processo #1 envia o valor para o Processo #0
    MPI_Send(&local_value, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);

    // Imprime o produto dos dois valores em stdout
    std::cout << "Process #1: Produto = " << local_value * other_value << std::endl;
  }

  MPI_Finalize();

  return 0;
}
