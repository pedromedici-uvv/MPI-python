// Dispersão e coleta - exercício

#include <iostream>
#include <mpi.h>
#include <cmath>
#include <cstdlib>

// Global variables
int rank, size;

// Number of points on one side.
constexpr int p_count = 512;

// Other global variables.
int cutoff;
double min_x, max_x, min_y, max_y, dx, dy;

// Function to compute the modulus of a complex number
double modulus(double x, double y) {
  return sqrt(x * x + y * y);
}

// Function to multiply a complex number by itself
void self_mul(double &x, double &y) {
  double ox = x * x - y * y;
  double oy = x * y + y * x;
  x = ox;
  y = oy;
}

// Function to compute the Mandelbrot set for a subset of points
void compute_mandelbrot(double *points, int start, int end, int mset[]) {
  for (int i = start; i < end; ++i) {
    double px, py;
    px = points[i * 2];
    py = points[i * 2 + 1];

    int iteration = 0;
    double zx = 0;
    double zy = 0;

    while (iteration < cutoff) {
      self_mul(zx, zy);
      zx += px;
      zy += py;
      double mod = modulus(zx, zy);

      if (mod > 2.0f)
        break;

      iteration++;
    }

    if (iteration == cutoff)
      mset[i - start] = -1;
    else
      mset[i - start] = iteration;
  }
}

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  // Reading parameters from the command line
  min_x = std::stod(argv[1]);
  max_x = std::stod(argv[2]);
  min_y = std::stod(argv[3]);
  max_y = std::stod(argv[4]);
  dx = (max_x - min_x) / p_count;
  dy = (max_y - min_y) / p_count;
  cutoff = std::stoi(argv[5]);

  // Getting rank and size
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Initialize points
  MPI_Barrier(MPI_COMM_WORLD);
  double *points = nullptr;

  if (rank == 0) {
    points = new double[p_count * p_count * 2];
    for (int yp = 0; yp < p_count; ++yp) {
      double py = min_y + dy * yp;
      for (int xp = 0; xp < p_count; ++xp) {
        double px = min_x + dx * xp;

        int lid = yp * p_count * 2 + xp * 2;
        points[lid] = px;
        points[lid + 1] = py;
      }
    }
  }

  // Broadcasting the points to all processes
  MPI_Bcast(points, p_count * p_count * 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Calculate the range of points to be computed by each process
  int points_per_process = p_count * p_count / size;
  int start = rank * points_per_process;
  int end = (rank + 1) * points_per_process;
  if (rank == size - 1) {
    // Adjust the end for the last process to include remaining points
    end = p_count * p_count;
  }

  // Compute Mandelbrot set for the assigned points
  int mset[points_per_process];
  compute_mandelbrot(points, start, end, mset);

  // Gather the results on process 0
  int *gathered_mset = nullptr;
  if (rank == 0) {
    gathered_mset = new int[p_count * p_count];
  }

  MPI_Gather(mset, points_per_process, MPI_INT, gathered_mset, points_per_process, MPI_INT, 0, MPI_COMM_WORLD);

  // Printing the results
  if (rank == 0) {
    for (int yp = 0; yp < p_count; ++yp) {
      for (int xp = 0; xp < p_count; ++xp)
        std::cout << gathered_mset[yp * p_count + xp] << " ";
      std::cout << std::endl;
    }
  }

  // Cleanup
  delete[] points;
  delete[] gathered_mset;

  MPI_Finalize();
  return 0;
}
