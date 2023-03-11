# sliding_tiles_solver
This is a quick and dirty solution for solving 3x3, 4x4 and 5x5 sliding tile puzzle in C++.

* 3x3 board: is solved optimally using A* algorithm

* 4x4 board: is solved optimally using Iterative Deepinging A* algorithm

* 5x5 board: is solved sub-optimally using by solving the top row and left column and then applying the same method to the 4x4 until the 3x3 board is solved using A* algorithm
