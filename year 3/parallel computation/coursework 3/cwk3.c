//
// Starting point for the OpenCL coursework for COMP/XJCO3221 Parallel Computation.
//
// Once compiled, execute with the size of the square grid as a command line argument, i.e.
//
// ./cwk3 16
//
// will generate a 16 by 16 grid. The C-code below will then display the initial grid,
// followed by the same grid again. You will need to implement OpenCL that applies the heat
// equation as per the instructions, so that the final grid is different.
//


//
// Includes.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// For this coursework, the helper file has 2 routines in addition to simpleOpenContext_GPU() and compileKernelFromFile():
// getCmdLineArg()  :  Parses grid size N from command line argument, or fails with error message.
// fillGrid()       :  Fills the grid with random values, except boundary values which are always zero.
// Do not alter these routines, as they will be replaced with different versions for assessment.
#include "helper_cwk.h"

void check_status( cl_int status, char* msg ) {
    if (status != CL_SUCCESS) {
        printf("%s (CODE: %d)\n", msg, status);
        exit(EXIT_FAILURE);
    }
}

//
// Main.
//
int main( int argc, char **argv )
{
 
    //
    // Parse command line argument and check it is valid. Handled by a routine in the helper file.
    //
    int N;
    getCmdLineArg( argc, argv, &N );

    //
    // Initialisation.
    //

    // Set up OpenCL using the routines provided in helper_cwk.h.
    cl_device_id device;
    cl_context context = simpleOpenContext_GPU(&device);

    // Open up a single command queue, with the profiling option off (third argument = 0).
    cl_int status;
    cl_command_queue queue = clCreateCommandQueue( context, device, 0, &status );

    // Allocate memory for the grid. For simplicity, this uses a one-dimensional array.
	float *hostGrid = (float*) malloc( N * N * sizeof(float) );

	// Fill the grid with some initial values, and display to stdout. fillGrid() is defined in the helper file.
    fillGrid( hostGrid, N );
    printf( "Original grid (only top-left shown if too large):\n" );
    displayGrid( hostGrid, N );

	//
	// Allocate memory for the grid(s) on the GPU and apply the heat equation as per the instructions.
	//
    // Input grid
    cl_mem deviceGrid = clCreateBuffer(
        context,
        CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        N*N*sizeof(float),
        hostGrid,
        &status
    );
    check_status(status, "Error occurred while creating input buffer");
    
    // Output grid
    cl_mem deviceOutputGrid = clCreateBuffer(
        context,
        CL_MEM_WRITE_ONLY,
        N*N*sizeof(float),
        NULL,
        &status
    );
    check_status(status, "Error occurred while creating output buffer");

    // Compile Kernel
    cl_kernel kernel = compileKernelFromFile(
        "cwk3.cl", 
        "apply_heat_equation",
        context, 
        device
    );

    // Pass arguments to kernel
    // Input grid
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &deviceGrid);
    check_status(status, "Error occurred while setting argument 0");

    // Output grid
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &deviceOutputGrid);
    check_status(status, "Error occurred while setting argument 1");

    // Output grid
    status = clSetKernelArg(kernel, 2, sizeof(int), &N);
    check_status(status, "Error occurred while setting argument 2");

    size_t globalSize [2] = { N, N };

    // Calc work group size. We want as big as possible but also needs to be less than N and also divisible by N
    size_t maxWorkItems;
    clGetDeviceInfo(
        device, 
        CL_DEVICE_MAX_WORK_GROUP_SIZE, 
        sizeof(size_t), 
        &maxWorkItems,
        NULL
    );

    size_t maxWorkGroupSize = (size_t) sqrt((double)maxWorkItems);
    size_t wgSize;
    // Find a work group size that is a divisor of N
    // This results in a minimum size of 1
    for (wgSize = maxWorkGroupSize; wgSize > 0; wgSize--) 
        if (N % wgSize == 0) break;

    size_t workGroupSize[2] = { wgSize, wgSize };

    // Enqueue the kernel with the sizes
    status = clEnqueueNDRangeKernel(
        queue, kernel, 2, 0,
        globalSize, workGroupSize,
        0, NULL, NULL
    );
    check_status(status, "Error occurred while enqueuing kernel");

    // Read the result to the hostGrid
    status = clEnqueueReadBuffer(
        queue, 
        deviceOutputGrid, 
        CL_TRUE, 0, 
        N*N*sizeof(float),
        hostGrid,
        0, NULL, NULL
    );
    check_status(status, "Error occurred while reading the result from the device");

    //
    // Display the final result. This assumes that the iterated grid was copied back to the hostGrid array.
    //
    printf( "Final grid (only top-left shown if too large):\n" );
    displayGrid( hostGrid, N );

    //
    // Release all resources.
    //
    clReleaseCommandQueue( queue   );
    clReleaseContext     ( context );
    clReleaseMemObject   ( deviceGrid );
    clReleaseMemObject   ( deviceOutputGrid );
    clReleaseKernel      ( kernel );

    free( hostGrid );

    return EXIT_SUCCESS;
}

