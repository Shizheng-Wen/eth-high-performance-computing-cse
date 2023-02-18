#include <iostream>
#include <random>
#include <omp.h>

//global variables

//cube
double x1 = 1.5;
double y1_ = 2.5;
double z1 = 0.5;
double cube_l = 3.0 * 0.5;

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
	bool x_dim = x1 - cube_l < x && x < x1 + cube_l;
	bool y_dim = y1_ - cube_l < y && y < y1_ + cube_l;
	bool z_dim = z1 - cube_l < z && z < z1 + cube_l;

	return x_dim && y_dim && z_dim;
}

bool inside_blue_circle(double x, double y, double z){
	return (x - x2)*(x - x2) + (y - y2)*(y - y2) + (z - z2)*(z - z2) <= blue_r*blue_r;
}

bool inside_green_circle(double x, double y, double z){
	return (x - x3)*(x - x3) + (y - y3)*(y - y3) + (z - z3)*(z - z3) <= green_r*green_r;
}

unsigned overlapMC(double x_min, double x_max, double y_min, double y_max, double z_min, double z_max, size_t n)
{
	unsigned pts_inside = 0;

	#pragma omp parallel reduction(+:pts_inside)
	{
		int t = omp_get_thread_num();
		std::default_random_engine g(42 + t);  // random generator with seed 42
		std::uniform_real_distribution<double> u_x(x_min, x_max);
		std::uniform_real_distribution<double> u_y(y_min, y_max);
		std::uniform_real_distribution<double> u_z(z_min, z_max);

		#pragma omp for
		for (size_t i = 0; i < n; ++i)
		{
			// TODO: implement the MC integration part here!
			double x = u_x(g);
			double y = u_y(g);
			double z = u_z(g);

			if(inside_cube(x,y,z) && inside_blue_circle(x,y,z) && inside_green_circle(x,y,z)){
				++pts_inside;
			}
		}
	}

	return  pts_inside;
}




int main(){
	double x_min = x3 - green_r < x2 - blue_r ?  x3 - green_r : x2 - blue_r;
	x_min = x_min < x1 - cube_l ? x_min : x1 - cube_l;
	double x_max = x3 + green_r > x2 + blue_r ?  x3 + green_r : x2 + blue_r;
	x_max = x_max > x1 + cube_l ? x_max : x1 + cube_l;

	double y_min = y3 - green_r < y2 - blue_r ?  y3 - green_r : y2 - blue_r;
	y_min = y_min < y1_ - cube_l ? y_min : y1_ - cube_l;
	double y_max = y3 + green_r > y2 + blue_r ?  y3 + green_r : y2 + blue_r;
	y_max = y_max > y1_ + cube_l ? y_max : y1_ + cube_l;

	double z_min = z3 - green_r < z2 - blue_r ?  z3 - green_r : z2 - blue_r;
	z_min = z_min < z1 - cube_l ? z_min : z1 - cube_l;
	double z_max = z3 + green_r > z2 + blue_r ?  z3 + green_r : z2 + blue_r;
	z_max = z_max > z1 + cube_l ? z_max : z1 + cube_l;

	std::cout << x_min << ", " << x_max << ", " << y_min << ", " << y_max << ", " << z_min << ", " << z_max << ", " <<"\n";

	// TODO: calculate the rectangle area for which you uniformly sample
	const double area_rectangle = (x_max - x_min)*(y_max - y_min)*(z_max - z_min);

	size_t n = 1e8 + 1;// default number of MC samples

	double ref = 0.9108; // reference solution

	double t0 = omp_get_wtime();

	unsigned sum = overlapMC(x_min, x_max, y_min, y_max, z_min, z_max, n);

	double area = sum / double(n) * area_rectangle;

	double t1 = omp_get_wtime();

	double error = std::abs(area - ref);
	if(error > 1e-2){
		printf("ERROR: you should get %.20f, but got: %.20f\n", ref, area);
	}
	printf("result:  %.20f\nref: %.20e\ntime: %.20f\n", area, ref, t1 - t0);

	return 0;
}
