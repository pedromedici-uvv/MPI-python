// Transmissão - exercício 1

#include <iostream>
#include <iomanip>
#include <mpi.h>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Input reading for process 0
  int n_elements;
  double *buffer;
  if (rank == 0) {
    std::cin >> n_elements;
    buffer = new double[n_elements];
    
    for (int i=0; i < n_elements; ++i)
      std::cin >> buffer[i];
  }

  // 1- Broadcast the value n_elements to all processes
  MPI_Bcast(&n_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // Here we create the buffers for the non-root processes now that
  // we have n_elements
  if (rank != 0)
    buffer = new double[n_elements];

  // 2- Broadcast the buffer to every process
  //    NOTE : The type here should be MPI_DOUBLE not MPI_FLOAT !
  MPI_Bcast(buffer, n_elements, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  

  // 3- Compute the sum depending on the rank
  double sum = 0.0;
  for (int i = 0; i < n_elements; ++i) {
    // Distribute the work among processes
    if (i % size == rank) {
      sum += buffer[i];
    }
  }

  // Use MPI_Reduce to sum up all the partial sums
  MPI_Reduce(&sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  
  // Printing the result and terminating the program
  // Precision is set high for the validation process, please do not modify this.
  if (rank == 0)
    std::cout << std::setprecision(16) << sum << std::endl;
  
  MPI_Finalize();
  if (rank == 0)
    delete [] buffer;

  return 0;
}
