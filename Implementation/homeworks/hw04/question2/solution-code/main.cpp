#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <fstream>
#include <string>
#include <mpi.h>


unsigned overlapMC(const double x2, const double R1, const double R2, size_t n, int rank, int procs)
{
	unsigned pts_inside = 0;

	std::default_random_engine g(42 + rank);  // random generator with seed 42
	std::uniform_real_distribution<double> u; // uniform distribution in [0, 1]

	// TODO_b: split the amount of work as equally as possible for each process.
	size_t n_local_size = double(n)/procs;
	size_t n_start = rank*n_local_size;
	size_t n_end   = rank == (procs - 1) ? n : n_start + n_local_size;

	for (size_t i = n_start; i < n_end; ++i)
	{
		double xi = (x2 + R2 + R1)*u(g) - R1;
		double yi = 2.0 * R2 * u(g) - R2;
		
		if((xi*xi + yi*yi <= R1*R1) && ((xi - x2)*(xi - x2) + yi*yi <= R2*R2)){
			++pts_inside;
		}
	}

	return  pts_inside;
}




int main(int argc, char *argv[])
{
	// TODO_b: Start-up the MPI environment and determine this process' rank ID as
    // well as the total number of processes (=ranks) involved in the
    // communicator

    int rank, procs = -1;

    // *** start MPI part ***
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    // *** end MPI part ***

	const double R1 = 5.0;
	const double R2 = 10.0;
	const double x2 = 12.0;

	const double area_rectangle = (R1 + x2 + R2) * 2.0 * R2;

	size_t n = 1e9 + 1;// default number of MC samples

	double ref = 17.0097776; // reference solution

	double t0 = MPI_Wtime(); 

	unsigned local_sum = overlapMC(x2, R1, R2, n, rank, procs);

	unsigned global_sum;

	// TODO_b: Sum up and average all the sums to the master rank
	MPI_Reduce(&local_sum, &global_sum, 1, MPI_UNSIGNED, MPI_SUM, 0, MPI_COMM_WORLD);

	double area = global_sum / double(n) * area_rectangle;

	double t1 = MPI_Wtime();

	if(rank == 0){
		double error = std::abs(area - ref);
		if(error > 1e-2){
			printf("ERROR: you should get pi, but got: %.20f\n", area);
		}
		else{
			printf("result:  %.20f\nref: %.20e\ntime: %.20f\n", area, ref, t1 - t0);

			std::string file_name = "out/";
			file_name += std::to_string(procs);
			file_name += ".txt";
			//output time in a file
			std::ofstream myfile;
			myfile.open (file_name);
			myfile << procs << " " << t1 - t0 << "\n";
			myfile.close();
		}
	}

	// TODO: Shutdown the MPI environment
    // *** start MPI part ***
    MPI_Finalize();
    // *** end MPI part ***

	return 0;
}
