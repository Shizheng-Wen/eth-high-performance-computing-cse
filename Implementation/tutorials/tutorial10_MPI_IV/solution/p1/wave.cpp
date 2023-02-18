#include "wave.h"

/********************************************************************/
/* Subquestion a: change the following function and use a Cartesian */
/* topology to find coords[3], rank_plus[3] and rank_minus[3]       */
/********************************************************************/
void WaveEquation::FindCoordinates()
{
  int nums    [3] = {procs_per_dim,procs_per_dim,procs_per_dim};
  int periodic[3] = {true,true,true};
  //int periodic[3] = {false,false,false};

  // Make a new communicator to which topology information has been attached
  MPI_Cart_create(MPI_COMM_WORLD, 3, nums, periodic, true, &cart_comm);
    
  // Determine the rank of the calling process in the communicator
  MPI_Comm_rank(cart_comm,&rank); 
  
  // Return shifted source and destination ranks, given a shift direction and amount
  MPI_Cart_shift(cart_comm, 0, 1, &rank_plus[0], &rank_minus[0]);
  MPI_Cart_shift(cart_comm, 1, 1, &rank_plus[1], &rank_minus[1]);
  MPI_Cart_shift(cart_comm, 2, 1, &rank_plus[2], &rank_minus[2]);

  MPI_Cart_coords(cart_comm,rank,3,&coords[0]);
}

void WaveEquation::run(double t_end)
{
  t = 0;

  /********************************************************************/
  /* Subquestion b: you need to define 12 custom datatypes.           */
  /* For sending data, six datatypes (one per face) are required.     */
  /* For receiving data, six more datatypes are required.             */
  /* You should use MPI_Type_create_subarray for those datatypes.     */
  /********************************************************************/

  /* Subquestion b: Create and commit custom datatypes here */
  /************************************************************************************************/
  MPI_Datatype SEND_FACE_PLUS[3];
  MPI_Datatype SEND_FACE_MINUS[3];

  MPI_Datatype RECV_FACE_PLUS[3];
  MPI_Datatype RECV_FACE_MINUS[3];

  int ndims = 3;
  int order =  MPI_ORDER_C;
  int array_of_sizes [3] = {N+2,N+2,N+2};
  int array_of_subsizes [3];
  int array_of_starts [3];  
  for (int d0 = 0; d0 < 3; d0++)
  {
    int d1 = (d0+1)%3;
    int d2 = (d0+2)%3;

    array_of_subsizes[d0] = 1;
    array_of_subsizes[d1] = N;
    array_of_subsizes[d2] = N;

    array_of_starts[d0] = 1;
    array_of_starts[d1] = 1;
    array_of_starts[d2] = 1;
    MPI_Type_create_subarray(ndims,array_of_sizes,array_of_subsizes,array_of_starts,order,MPI_DOUBLE,&SEND_FACE_MINUS[d0]);
    
    array_of_starts[d0] = N;
    array_of_starts[d1] = 1;
    array_of_starts[d2] = 1;
    MPI_Type_create_subarray(ndims,array_of_sizes,array_of_subsizes,array_of_starts,order,MPI_DOUBLE,&SEND_FACE_PLUS[d0]);


    array_of_starts[d0] = 0;
    array_of_starts[d1] = 1;
    array_of_starts[d2] = 1;
    MPI_Type_create_subarray(ndims,array_of_sizes,array_of_subsizes,array_of_starts,order,MPI_DOUBLE,&RECV_FACE_MINUS[d0]);

    array_of_starts[d0] = N+1;
    array_of_starts[d1] = 1;
    array_of_starts[d2] = 1;
    MPI_Type_create_subarray(ndims,array_of_sizes,array_of_subsizes,array_of_starts,order,MPI_DOUBLE,&RECV_FACE_PLUS[d0]);

    // A datatype object has to be committed before it can be used in a communication
    MPI_Type_commit(&SEND_FACE_PLUS [d0]);
    MPI_Type_commit(&RECV_FACE_PLUS [d0]);
    MPI_Type_commit(&SEND_FACE_MINUS[d0]);
    MPI_Type_commit(&RECV_FACE_MINUS[d0]);
  }
  /************************************************************************************************/

 
  do
  {

    MPI_Request request[12];

    /* Subquestion b: Replace the sends and receives with ones that correspond
     * to custom datatypes*/
    /**********************************************************************************************/    
      for (int d=0; d<3; d++)
      {
        MPI_Irecv(u,1,RECV_FACE_MINUS[d],rank_minus[d],100*d  ,cart_comm,&request[2*d  ]);     
        MPI_Irecv(u,1,RECV_FACE_PLUS [d],rank_plus [d],100*d+1,cart_comm,&request[2*d+1]);
      }

      for (int d=0; d<3; d++)
      {
        MPI_Isend(u,1,SEND_FACE_PLUS [d],rank_plus [d],100*d  ,cart_comm,&request[6+2*d  ]);
        MPI_Isend(u,1,SEND_FACE_MINUS[d],rank_minus[d],100*d+1,cart_comm,&request[6+2*d+1]);
      }
    /**********************************************************************************************/

    // Wait for communication to finish
    MPI_Waitall(12, &request[0], MPI_STATUSES_IGNORE);


    for (int i0 = 1; i0 <= N; i0++)
      for (int i1 = 1; i1 <= N; i1++)
        for (int i2 = 1; i2 <= N; i2++)
          UpdateGridPoint(i0, i1, i2);

    double *temp = u_old;
    u_old = u;
    u = u_new;
    u_new = temp;
    t += dt;
  } while (t < t_end);

  double s = 0;
  double Checksum = 0;
  for (int k = 1; k <= N; k++)
    for (int j = 1; j <= N; j++)
      for (int i = 1; i <= N; i++)
      {
        int m = k + j * (N + 2) + i * (N + 2) * (N + 2);
        s += u[m] * u[m];
      }

  MPI_Reduce(&s, &Checksum, 1, MPI_DOUBLE, MPI_SUM, 0, cart_comm);
  if (rank == 0)
    std::cout << "Checksum = " << Checksum << "\n";

  /* Subquestion b: You should free the custom datatypes and the communicator
   * here. */
  for (int d=0; d<3; d++)
  {
    MPI_Type_free(&RECV_FACE_PLUS[d]) ;
    MPI_Type_free(&RECV_FACE_MINUS[d]);   
    MPI_Type_free(&SEND_FACE_PLUS[d]) ;
    MPI_Type_free(&SEND_FACE_MINUS[d]);
  }
  MPI_Comm_free(&cart_comm);
}
