#include "helpers.hpp"
#include <math.h>
#include <random>

// Global simulation variables
constexpr double EPS = 1e-3;          // depth potential well
constexpr double SIGMA = 0.2;         // distance where potential is zero
constexpr double RCUT = 2.5 * SIGMA;  // cut-off radius
constexpr double RCUT2 = RCUT * RCUT; // square of cut-off radius

constexpr double SIGMA6 = std::pow(SIGMA, 6);   // some constant
constexpr double SIGMA12 = std::pow(SIGMA, 12); // some constant
constexpr double K = 24. * EPS;                 // some constant

constexpr double dt = 1e-3; // time step
constexpr double RHO = 1;   // particle density in arrangement [particles/m3]

void simulateParticles(std::vector<particle> &particles) {

  const size_t N = particles.size();

  for (size_t i = 0; i < N; i++) {
    // Pairwise force calculation
    for (size_t j = i + 1; j < N; j++) {
      // Calculate particle distance
      const double dx = particles[j].x - particles[i].x;
      const double dy = particles[j].y - particles[i].y;
      const double dz = particles[j].z - particles[i].z;

      // Calculate square distance
      const double dist2 = dx * dx + dy * dy + dz * dz;
      if (dist2 > RCUT2)
        continue;

      // Calculate squared inverse distance
      const double invDist2 = 1.0 / dist2;

      // Calculate powers of inverse distance
      const double invDist4 = invDist2 * invDist2;
      const double invDist8 = invDist4 * invDist4;
      const double invDist6 = invDist2 * invDist4;
      const double invDist12 = invDist4 * invDist8;

      // Calculate constant force term
      const double fij =
          -K * invDist2 * (2. * SIGMA12 * invDist12 - SIGMA6 * invDist6);

      // Calculate spatial force terms
      const double xforce = fij * dx;
      const double yforce = fij * dy;
      const double zforce = fij * dz;

      // Update force term on particle i
      particles[i].fx += xforce;
      particles[i].fy += yforce;
      particles[i].fz += zforce;

      // Update force term on particle j in negative direction
      particles[j].fx -= xforce;
      particles[j].fy -= yforce;
      particles[j].fz -= zforce;
    }
  }

  // Update position of all particles with Leap-frog scheme
  for (auto &p : particles) {
    const double invM = 1. / p.m;
    p.vx += invM * p.fx * dt;
    p.vy += invM * p.fy * dt;
    p.vz += invM * p.fz * dt;

    p.x += p.vx * dt;
    p.y += p.vy * dt;
    p.z += p.vz * dt;

    p.fx = 0.;
    p.fy = 0.;
    p.fz = 0.;
  }
}

void simulateParticlesOptimized(std::vector<particle> &particles,
                                const size_t blockSize) {

  // TODO: optimize data movements in this method

  const size_t N = particles.size();

  for (size_t i = 0; i < N; i++) {
    // Pairwise force calculation
    for (size_t j = i + 1; j < N; j++) {
      // Calculate particle distance
      const double dx = particles[j].x - particles[i].x;
      const double dy = particles[j].y - particles[i].y;
      const double dz = particles[j].z - particles[i].z;

      // Calculate square distance
      const double dist2 = dx * dx + dy * dy + dz * dz;
      if (dist2 > RCUT2)
        continue;

      // Calculate squared inverse distance
      const double invDist2 = 1.0 / dist2;

      // Calculate powers of inverse distance
      const double invDist4 = invDist2 * invDist2;
      const double invDist8 = invDist4 * invDist4;
      const double invDist6 = invDist2 * invDist4;
      const double invDist12 = invDist4 * invDist8;

      // Calculate constant force term
      const double fij =
          -K * invDist2 * (2. * SIGMA12 * invDist12 - SIGMA6 * invDist6);

      // Calculate spatial force terms
      const double xforce = fij * dx;
      const double yforce = fij * dy;
      const double zforce = fij * dz;

      // Update force term on particle i
      particles[i].fx += xforce;
      particles[i].fy += yforce;
      particles[i].fz += zforce;

      // Update force term on particle j in negative direction
      particles[j].fx -= xforce;
      particles[j].fy -= yforce;
      particles[j].fz -= zforce;
    }
  }

  // Update position of all particles with Leap-frog scheme
  for (auto &p : particles) {
    const double invM = 1. / p.m;
    p.vx += invM * p.fx * dt;
    p.vy += invM * p.fy * dt;
    p.vz += invM * p.fz * dt;

    p.x += p.vx * dt;
    p.y += p.vy * dt;
    p.z += p.vz * dt;

    p.fx = 0.;
    p.fy = 0.;
    p.fz = 0.;
  }
}

int main() {

  const bool output = false;
  const size_t numSimulationSteps = 100;

  std::vector<size_t> numParticles{1024, 2048, 4096, 8192, 16384, 32768};
  std::vector<size_t> blockSize{2, 4, 8, 16, 32, 64, 128, 256, 512};

  const size_t M = numParticles.size();
  const size_t Bs = blockSize.size();

  std::vector<double> times1(M);
  std::vector<std::vector<double>> times2(Bs, std::vector<double>(M));

  // Init random number generator
  std::random_device rd{};
  std::mt19937 gen{rd()};
  std::normal_distribution<> normalDist{0, 1.0};

  for (size_t m = 0; m < M; m++) {

    const size_t N = numParticles[m];

    printf("Working with num particles %zu\n", N);
    printf("---------------------------------------------\n");

    // keep particle density constant
    const double L = std::cbrt(N / RHO); // cube root for L calculation
    std::uniform_real_distribution<> uniDist{0., L};

    // Init memory
    std::vector<particle> particles(N);

    // Init particles
    for (auto &p : particles) {
      // Initialize particles in [0,L]^3 domain
      p.x = uniDist(gen);
      p.y = uniDist(gen);
      p.z = uniDist(gen);

      // Initialize velocities
      p.vx = normalDist(gen);
      p.vy = normalDist(gen);
      p.vz = normalDist(gen);

      // Initialize masses
      p.m = 10 + normalDist(gen);
      if (p.m < 0)
        p.m *= -1; // avoid negative masses

      // Initialize accelerations
      p.fx = 0.;
      p.fy = 0.;
      p.fz = 0.;
    }

    printf("Start particle simulation (non optimized, warm-up).\n");
    times1[m] = benchmark_AB(particles, 0, 0, numSimulationSteps, output);

    printf("---------------------------------------------\n");

    for (size_t b = 0; b < Bs; b++) {
      printf("Start particle simulation (optimized, block size=%zu).\n",
             blockSize[b]);
      times2[b][m] =
          benchmark_AB(particles, 1, blockSize[b], numSimulationSteps, false);
    }

    printf("Start particle simulation (non optimized).\n");
    times1[m] = benchmark_AB(particles, 0, 0, numSimulationSteps, false);

    printf("==================================================\n");
  }

  writeTimingsToFile(times1, times2, blockSize, numParticles);

  return 0;
}
