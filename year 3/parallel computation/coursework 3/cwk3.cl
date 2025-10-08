// Kernel for the heat equation.

__kernel
void apply_heat_equation(__global float* input, __global float* output, int N) {
    int i = get_global_id(0);
    int j = get_global_id(1);

    // If work item is on the edge, result is 0
    if (i == 0 || i == N-1 ||
        j == 0 || j == N-1 ) {
        output[i*N+j] = 0.0f;
    } else {
        float up    = input[(i-1)*N + j];
        float down  = input[(i+1)*N + j];
        float left  = input[i*N + (j-1)];
        float right = input[i*N + (j+1)];
        output[i*N+j] = (up+down+left+right) / 4.0f;
    }
}
