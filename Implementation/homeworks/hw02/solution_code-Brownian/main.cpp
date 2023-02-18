#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <fstream>
#include <iostream>
#include <cmath>
// TODO 1: add OpenMP headers
#include <omp.h>

#ifndef SEED
#define SEED 19
#endif

// evolution time
double tmax = 0.2;
// number of bins in histogram
size_t nb = 50;
// range of histogram
double xmin = -2;
double xmax = 2;

// hh: histogram, number of particles in each bin
// name: filename
void WriteHistogram(const std::vector<double>& hh, std::string name) {
  std::ofstream o(name);
  size_t nb = hh.size();
  for (size_t j = 0; j < nb; ++j) {
    double x = xmin + (xmax - xmin) * (j + 0.5) / nb;
    o << x << " " << hh[j] << "\n";
  }
}

// TODO 1: implement using OpenMP
// Returns current wall-clock time in seconds
double GetWtime() {
  return omp_get_wtime();
}

// TODO 3: parallelize with OpenMP
// Returns histogram of positions xx in range [xmin,xmax] with nb bins
std::vector<double> GetHistogram(const std::vector<double>& xx) {
  std::vector<double> hh(nb, 0);
  #pragma omp parallel
  {
    std::vector<double> lhh(nb, 0);
    #pragma omp for nowait
    for (size_t i = 0; i < xx.size(); ++i) {
      int j = (xx[i] - xmin) / (xmax - xmin) * nb;
      j = std::max(0, std::min(int(nb) - 1, j));
      lhh[j] += 1;
    }
    #pragma omp critical
    for (size_t j = 0; j < hh.size(); ++j) {
      hh[j] += lhh[j];
    }
  }
  return hh;
}

int main(int argc, char *argv[]) {
  if (argc < 2 || argc > 3 || std::string(argv[1]) == "-h") {
    fprintf(stderr, "usage: %s N M\n", argv[0]);
    fprintf(stderr, "Brownian motion of N paritcles in M steps in time");
    return 1;
  }

  // number of particles
  size_t N = atoi(argv[1]);
  // number of steps
  size_t M = atoi(argv[2]);
  // time step
  double dt = tmax / M;

  // Seed generator
  int seed = SEED;
  std::default_random_engine gen(seed);

  struct Gen {
    std::default_random_engine gen;
    char p[64];    // padding to avoid false sharing
  };

  // local generators
  std::vector<Gen> gg(omp_get_max_threads());
  for (size_t p = 0; p < gg.size(); ++p) {
    gg[p].gen.seed(gen());
  }

  // Initialize particles
  std::vector<double> xx(N);
  {
    std::uniform_real_distribution<double> dis(-0.5, 0.5);
    for (size_t i = 0; i < N; ++i) {
      xx[i] = dis(gg[0].gen);
    }
  }

  WriteHistogram(GetHistogram(xx), "hist_0.dat");

  double wt0, wt1;

  // Perform M steps of random walks and measure time
  // TODO 2: parallelize with OpenMP
  wt0 = GetWtime();
  {
    #pragma omp parallel
    {
      size_t p = omp_get_thread_num();
      auto& g = gg[p].gen;
      std::normal_distribution<double> dis(0., std::sqrt(dt));
      #pragma omp for nowait
      for (size_t i = 0; i < N; ++i) {
        for (size_t m = 0; m < M; ++m) {
          xx[i] += dis(g);
        }
      }
    }
  }
  wt1 = GetWtime();
  double wtime_walk = wt1 - wt0;

  // Compute histogram and measure time
  wt0 = GetWtime();
  double hsum = 0;
  std::vector<double> hh;
  for (size_t n = 0; n < 100; ++n) {
    hh = GetHistogram(xx);
    hsum += hh[0];
  }
  wt1 = GetWtime();
  double wtime_hist = wt1 - wt0;

  WriteHistogram(hh, "hist_1.dat");

  printf("walk: %.16f\n", wtime_walk);
  printf("hist: %.16f\n", wtime_hist);
}
