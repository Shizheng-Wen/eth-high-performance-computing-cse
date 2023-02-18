#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <omp.h>
double F(double x, double y) // Integrand
{
  	if (x * x + y * y < 1.) return 4.; // inside unit circle 
  	return 0.;
}

double C0(size_t n) // Method 0: serial
{
	double sum = 0.;

	std::default_random_engine g(0);  // random generator with seed 0

	for (size_t i = 0; i < n; ++i)
	{
		std::uniform_real_distribution<double> u; // uniform distribution in [0, 1]
		double x = u(g);
		double y = u(g);
		sum += F(x, y);
	}
	return sum / n;
}

double C1(size_t n) // Method 1: openmp, no arrays 
{
	double sum = 0.;

	#pragma omp parallel reduction(+:sum)
	{
		int t = omp_get_thread_num(); 
		std::default_random_engine g(t);
		#pragma omp for
		for (size_t i = 0; i < n; ++i)
		{
			std::uniform_real_distribution<double> u;
			double x = u(g);
			double y = u(g);
			sum += F(x, y);
		}
	}
	return sum / n;
}


// Method 2, only `omp parallel for reduction`, arrays without padding
double C2(size_t n)
{
	int mt = omp_get_max_threads();

	auto* gg = new std::default_random_engine[mt]; // local generators

	for (int t = 0; t < mt; ++t) 
		gg[t].seed(t + 1);

	double sum = 0.;

	#pragma omp parallel for reduction(+:sum)
	for (size_t i = 0; i < n; ++i)
	{
		int t = omp_get_thread_num(); 
		std::uniform_real_distribution<double> u;
		double x = u(gg[t]);
		double y = u(gg[t]);
		sum += F(x, y);
	}

	delete [] gg;

	return sum / n;
}

// Method 3, only `omp parallel for reduction`, arrays with padding
double C3(size_t n)
{
	struct GP // generator with padding
	{ 
		std::default_random_engine g;
		char p[64];   // padding to avoid false sharing
	};

	int mt = omp_get_max_threads();

	auto* gg = new GP[mt]; // local generators

	for (int t = 0; t < mt; ++t)
		gg[t].g.seed(t + 1);

	double sum = 0.;

	#pragma omp parallel for reduction(+:sum)
	for (size_t i = 0; i < n; ++i)
	{
		int t = omp_get_thread_num();
		std::uniform_real_distribution<double> u;
		double x = u(gg[t].g);
		double y = u(gg[t].g);
		sum += F(x, y);
	}
   
	delete [] gg;

	return sum / n;
}


int main(int argc, char *argv[])
{
	size_t ndef = 1e8;// default number of samples

	if (argc < 2 || argc > 3 || std::string(argv[1]) == "-h")
	{
		fprintf(stderr, "usage: %s METHOD [N=%ld]\n", argv[0], ndef);
		fprintf(stderr, "Monte-Carlo integration with N samples.\n");
		fprintf(stderr, "METHOD:\n");
		fprintf(stderr, "0: serial\n");
		fprintf(stderr, "1: openmp, no arrays\n");
		fprintf(stderr, "2: `omp parallel for reduction`, arrays without padding\n");
		fprintf(stderr, "3: `omp parallel for reduction`, arrays with padding\n");
		return 1;
	} 

	size_t m = atoi(argv[1]); //method
	size_t n = (argc > 2 ? atoi(argv[2]) : ndef); // number of samples
	double ref = 3.14159265358979323846; // reference solution

	double t0 = omp_get_wtime();
	double res = 0.;
	if      (m==0) res = C0(n);
	else if (m==1) res = C1(n); 
	else if (m==2) res = C2(n); 
	else if (m==3) res = C3(n); 
	else
	{
		printf("Unknown method '%ld'\n", m);
		abort();
	}
	double t1 = omp_get_wtime();

	printf("res:  %.20f\nref:  %.20f\nerror: %.20e\ntime: %.20f\n", res, ref, res - ref, t1 - t0);

	return 0;
}
