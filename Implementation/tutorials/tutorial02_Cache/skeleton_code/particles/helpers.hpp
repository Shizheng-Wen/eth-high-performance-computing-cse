#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

// The particle object
struct particle {
  double x;
  double y;
  double z;
  double vx;
  double vy;
  double vz;
  double fx;
  double fy;
  double fz;
  double m;
};

// Simulation methods implemented in particles.cpp
void simulateParticles(std::vector<particle> &particles);
void simulateParticlesOptimized(std::vector<particle> &particles,
                                const size_t blockSize);

// Helper function to write particle state to txt file
void writeState(const std::vector<particle> &particles,
                const size_t iteration) {
  FILE *fp = nullptr;

  char fileName[30];
  snprintf(fileName, 30, "./states/state_%06zu.txt", iteration);
  fp = fopen(fileName, "w");

  // write location of particles
  for (auto &p : particles) {
    fprintf(fp, "%lf %lf %lf\n", p.x, p.y, p.z);
  }
  fclose(fp);
}

// Helper function to benchmark simulation methods
double benchmark_AB(std::vector<particle> particles, const size_t mode,
                    const size_t blockSize, const size_t Ns,
                    const bool output) {

  size_t numParticles = particles.size();
  double times = 0;

  // Check mode
  if (mode > 1) {
    printf("Error: mode must either be '0' or '1' (is %zu).\n", mode);
    exit(1);
  }

  // Check that blocksize is a divisor of numParticles
  if (mode > 1 && (numParticles % blockSize) != 0) {
    printf("Error: the size of the matrix (%zu) should be divided by the "
           "blockSize variable (%zu).\n",
           numParticles, blockSize);
    exit(1);
  }

  for (size_t i = 0; i < Ns; i++) {
    auto t1 = std::chrono::system_clock::now();

    if (mode == 0) {
      simulateParticles(particles);
    } else {
      simulateParticlesOptimized(particles, blockSize);
    }

    auto t2 = std::chrono::system_clock::now();
    times += std::chrono::duration<double>(t2 - t1).count();

    // Write particle states if required
    if (output == true) {
      writeState(particles, i);
    }
  }
  printf("Done in total %9.4fs  --  average %9.4fs\n", times, times / Ns);

  return times / Ns;
}

void writeTimingsToFile(const std::vector<double> &times1,
                        const std::vector<std::vector<double>> &times2,
                        const std::vector<size_t> &blockSize,
                        const std::vector<size_t> &numParticles) {

  const size_t M = numParticles.size();
  const size_t Bs = blockSize.size();

  FILE *fp = nullptr;
  fp = fopen("particles.txt", "w");
  // write header to the file
  std::string header = " N   unopt ";
  for (size_t b = 0; b < Bs; b++)
    header = header + "  bs_" + std::to_string(blockSize[b]);
  header = header + "\n";
  fprintf(fp, "%s", header.c_str());

  for (size_t m = 0; m < M; m++) {
    fprintf(fp, "%zu %lf", numParticles[m], times1[m]);
    for (size_t b = 0; b < Bs; b++)
      fprintf(fp, " %lf ", times2[b][m]);
    fprintf(fp, "\n");
  }
  fclose(fp);
}
