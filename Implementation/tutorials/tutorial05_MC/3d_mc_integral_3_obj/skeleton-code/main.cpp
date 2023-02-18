//#include <stdio.h>
//#include <stdlib.h>
#include <random>
#include <omp.h>

//global variables

//cube
double x1 = 1.5;
double y1_ = 2.5;
double z1 = 0.5;
double cube_l = 3.0*0.5;

//blue circle
double x2 = 4.0;
double y2 = 2.0;
double z2 = -0.5;
double blue_r = 2.0;

//green circle
double x3 = 3.0;
double y3 = 1.0;
double z3 = 0.0;
double green_r = 1.0;

bool inside_cube(double x, double y, double z){
	//TODO: return true if x,y,z are inside the cube
}

bool inside_blue_circle(double x, double y, double z){
	//TODO: return true if x,y,z are inside the blue circle
}

bool inside_green_circle(double x, double y, double z){
	//TODO: return true if x,y,z are inside the blue circle
}

unsigned overlapMC(double x_min, double x_max, double y_min, double y_max, double z_min, double z_max, size_t n)
{
	unsigned pts_inside = 0;

	int t = omp_get_thread_num();
	std::default_random_engine g(42 + t);  // random generator with seed 42
	
	//TODO: generate random uniform random sample generators here

	for (size_t i = 0; i < n; ++i)
	{
		// TODO: implement the MC integration part here!
		
	}

	return  pts_inside;
}




int main(){

	//TODO: compute the min and max distance of all three coordinates
	// these will be used to determine the sample area
	double x_min;
	double x_max;

	double y_min;
	double y_max;

	double z_min;
	double z_max;

	// TODO: compute the cuboid area for which you uniformly sample
	const double area_cuboid;

	size_t n = 1e8 + 1;// default number of MC samples

	double ref = 0.9108; // reference solution

	double t0 = omp_get_wtime();

	unsigned sum = overlapMC(x_min, x_max, y_min, y_max, z_min, z_max, n);

	//TODO: compute the integrated area
	double area;

	double t1 = omp_get_wtime();

	double error = std::abs(area - ref);
	if(error > 1e-2){
		printf("ERROR: you should get %.20f, but got: %.20f\n", ref, area);
	}
	printf("result:  %.20f\nref: %.20e\ntime: %.20f\n", area, ref, t1 - t0);

	return 0;
}
