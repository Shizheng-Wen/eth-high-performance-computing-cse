#include <mpi.h>
#include <stdio.h>
#include <cmath>


struct particle
{
  int id;
  double x[3];
  bool state;
  double gamma;
};



int main(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);

  int rank,size;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  
  particle p;

  MPI_Datatype MPI_PARTICLE;
  int count = 4;
  int array_of_blocklengths [4] = {1,3,1,1};
  MPI_Aint array_of_displacements [4];
  MPI_Datatype array_of_types [4] = {MPI_INT,MPI_DOUBLE,MPI_C_BOOL,MPI_DOUBLE};

  
  MPI_Aint base;
  MPI_Aint id_address;
  MPI_Aint x_address;
  MPI_Aint state_address;
  MPI_Aint gamma_address;
  
  MPI_Get_address(&p,&base);
  MPI_Get_address(&p.id,&id_address);
  MPI_Get_address(&p.x,&x_address);
  MPI_Get_address(&p.state,&state_address);
  MPI_Get_address(&p.gamma,&gamma_address);
  
  array_of_displacements[0] = id_address - base;
  array_of_displacements[1] = x_address - base;
  array_of_displacements[2] = state_address - base;
  array_of_displacements[3] = gamma_address - base;


  MPI_Type_create_struct(count,array_of_blocklengths,array_of_displacements,array_of_types,&MPI_PARTICLE);
  MPI_Type_commit(&MPI_PARTICLE);



  if (rank == 0)
  {
    p.id = 1;
    p.x[0] = 3.14159265359;
    p.x[1] = 2.71828182846;
    p.x[2] = ( 1.0 + sqrt(5.0) ) / 2.0;
    p.state = false;
    p.gamma = 0.57721566490;
  
    MPI_Send(&p,1,MPI_PARTICLE,1,100,MPI_COMM_WORLD);
  }
  else if (rank == 1)
  {
    MPI_Recv(&p,1,MPI_PARTICLE,0,100,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
   
    printf("%d \n", p.id );
    printf("%10.8f \n", p.x[0]  );
    printf("%10.8f \n", p.x[1]  );
    printf("%10.8f \n", p.x[2]  );
    printf("%d \n"    , p.state );
    printf("%10.8f \n", p.gamma );
  }
  else
  {
    printf("Run with exactly two ranks!\n");
    int err = 1;
    MPI_Abort(MPI_COMM_WORLD,err);
  }
  MPI_Type_free(&MPI_PARTICLE);

  MPI_Finalize();
  return 0;
}
