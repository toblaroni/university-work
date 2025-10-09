# parallel-serial-converter
Parallel to serial converter build in the Nand2Tetris hardware description language. 
The circuit stores an 8-bit binary value from the 8-bit input bus (indata) when the load input is asserted (has value of 1).
When the enable input is asserted then the circuit outputs each of the bits stored over the next 8 clock cycles sequentially.
These bits are outputted via sout. When the circuit completes transmission of the 8 bits, the complete output should be asserted for 1 clock cycle.

This was the last question of coursework 2 of this module. It was one of the hardest challenges I overcame during my first year. It was not required that we completed this question of the coursework but I was determined and eventually managed to get my solution working. It can be run using the [NAND2TETRIS Hardware simulator](https://www.nand2tetris.org/software).
