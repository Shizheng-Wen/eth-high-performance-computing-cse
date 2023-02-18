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
	size_t n_local_size = n/procs;
	size_t n_start = 0;
	size_t n_end   = n_local_size;

	for (size_t i = n_start; i < n_end; ++i)
	{
		double x,y;
		x = -5 + 27 * u(g);
		y = -10 + 20 * u(g);
		if(x*x + y*y <= R1 * R1  && (x-x2)*(x-x2) + y*y <= R2*R2) pts_inside++;     // TODO_a: implement the MC integration part here!

	}

	return  pts_inside;
}




int main(int argc, char *argv[])
{
	// TODO_b: Start-up the MPI environment and determine this process' rank ID as
    // well as the total number of processes (=ranks) involved in the
    // communicator
    MPI_Init(&argc,&argv);

    int rank, procs;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&procs);

	const double R1 = 5.0;		// Radius of first circle
	const double R2 = 10.0;		// Radius of second circle
	const double x1 = 0.0;		// x1 coordinate of first circle center
	const double x2 = 12.0;		// x2 coordinate of second circle center
	const double y1 = 0.0;
	const double y2 = 0.0;

	// TODO_a: calculate the rectangle area for which you uniformly sample x & y
	double x_min = std::min(x1-R1, x2-R2); 
	double x_max = std::max(x1+R1, x2+R2);

	double y_min = std::min(y1-R1,y2-R2);
	double y_max = std::max(y1+R1,y2+R2);


	const double area_rectangle = (x_max-x_min)*(y_max-y_min);

	size_t n = 1e9 + 1;// default number of MC samples

	double ref = 17.0097776; // reference solution

	double t0 = MPI_Wtime(); 

	unsigned local_sum = overlapMC(x2, R1, R2, n, rank, procs);

	unsigned global_sum = (procs == -1) ? local_sum : 0;

	// TODO_b: Sum up and average all the local_sums to the master ranks global_sum using MPI_Reduce
	MPI_Reduce(&local_sum,&global_sum,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

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

	// TODO_b: Shutdown the MPI environment
	MPI_Finalize();

	return 0;
}
