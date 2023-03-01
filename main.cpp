/* 
    Sliding Tiles Puzzle Solver
    ---------------------------
    8-tile : Optimal solution using A-Star (A*) algorithm
    15-tile: Optimal solution using Iterative Deepening A-Star (IDA*) algorithm
    24-tile: Suboptimal solution by solving first row, then first column and reducing the problem into 15-tiles problem

    Usage
    -----
    $ program_name tile1,tile2,..,tileN # '9' represents the empty tile
    $ main 5,8,1,2,4,3,7,6,9
*/

#include <iostream>
#include <cstdlib>
#include "board.hpp"
#include "a_star.hpp"
#include "ida_star.hpp"
#include "suboptimal.hpp"

void print_help() {
    std::cout << "Invalid arguments, example below:" << std::endl;
    std::cout << "$ solver 1,2,3,4,5,6,7,8,9" << std::endl;
}

board_t parse_validate_board(int argc, char* argv[]) {
    if(argc != 2) {
        print_help();
        std::exit(EXIT_FAILURE);
    }
    // Parse board from program arguments
    board_t board = parse_board(argv[1]);
    if(!is_valid_board_size(board) || !is_valid_board(board)) {
        print_help();
        std::exit(EXIT_FAILURE);
    }
    return board;
}

std::string select_algorithm_solve(const board_t& board) {
    std::string solution_steps;

    if(board.size() == 9) { // 3x3 board
        solution_steps = a_star(board);
    }
    else if(board.size() == 16) { // 4x4 board
        solution_steps = ida_star(board);
    }
    else if(board.size() == 25) { // 5x5 board
        solution_steps = suboptimal(board);
    }
    return solution_steps;
}

std::string format_solution(const std::string& raw) {
    std::string formatted;
    bool first_step = true;
    for(char step : raw) {
        if(first_step) {
            first_step = false;
            formatted += step;
        }
        else {
            formatted += ',';
            formatted += step;
        }
    }
    return formatted;
}

int main(int argc, char* argv[]) {
    board_t board = parse_validate_board(argc, argv);
    std::string solution_steps = select_algorithm_solve(board);
    std::cout << "Solution Steps: " << format_solution(solution_steps) << std::endl;
    return EXIT_SUCCESS;
}