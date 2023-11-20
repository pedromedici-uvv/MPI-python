// You have access to the following global variables :
int world_rank, world_size;
MPI_Comm custom_comm1, custom_comm2, custom_comm3, tmp;

// The tmp communicator is provided if you need to point somewhere, where you don't
// care about the outputted communicator. For instance, when using MPI_UNDEFINED as color.

// world_rank : rank of the process on MPI_COMM_WORLD
// world_size : size of MPI_COMM_WORLD
// These two variables are already initialised when splitting() is called

void splitting()
{
    int color;
    MPI_Comm *new_comm;

    // 1- First splitting here.
    // With only one call to MPI_Comm_split you should be able to split processes 0-3 in custom_comm1
    // and processes 4-6 in custom_comm2

    if (world_rank >= 0 && world_rank <= 3)
    {
        color = 0; // color for processes 0-3
    }
    else if (world_rank >= 4 && world_rank <= 6)
    {
        color = 1; // color for processes 4-6
    }
    else
    {
        color = MPI_UNDEFINED; // color for other processes
    }

    MPI_Comm_split(MPI_COMM_WORLD, color, world_rank, &custom_comm1);
    MPI_Comm_split(MPI_COMM_WORLD, color, world_rank, &custom_comm2);

    // 2- Second splitting here
    // Now put processes 0 and 4 in custom_comm3

    if (world_rank == 0 || world_rank == 4)
    {
        color = 0; // color for processes 0 and 4
    }
    else
    {
        color = MPI_UNDEFINED; // color for other processes
    }

    MPI_Comm_split(MPI_COMM_WORLD, color, world_rank, &custom_comm3);
}
