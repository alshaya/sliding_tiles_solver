#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <tuple>
#include <algorithm>
#include <cmath>
#include "board.hpp"

// graph node
struct node_t {
    board_t board; // current configuration of the board
    std::string path; // path to node from start configuration of the board
    int f; // evaluation function f = g + h
    int g; // actual cost up to this board
    int h; // heuristic cost from this board to goal
};

// possible moves <board, move> where move can be: 'L', 'U', 'R', 'D'
using move_t = std::tuple<board_t, char>;

// optimal goals 3x3 and 4x4
board_t goal9 { 1, 2, 3,
                4, 5, 6,
                7, 8, 9};

board_t goal16 {1,  2,  3,  4,
                5,  6,  7,  8,
                9,  10, 11, 12,
                13, 14, 15, 16};

// suboptimal goals for 5x5
board_t suboptimal_row_goal25 { 1,  2,  3,  4,  5,
                                0,  0,  0,  0,  0,
                                0,  0,  0,  0,  0,
                                0,  0,  0,  0,  0,
                                0,  0,  0,  0,  0};
board_t suboptimal_col_goal25 { 1,  2,  3,  4,  5,
                                6,  0,  0,  0,  0,
                                11, 0,  0,  0,  0,
                                16, 0,  0,  0,  0,
                                21, 0,  0,  0,  0};

board_t suboptimal_row2_goal25 { 1,  2,  3,  4,  5,
                                 6,  7,  8,  9,  10,
                                 11, 0,  0,  0,  0,
                                 16, 0,  0,  0,  0,
                                 21, 0,  0,  0,  0};

board_t suboptimal_col2_goal25 { 1,  2,  3,  4,  5,
                                 6,  7,  8,  9,  10,
                                 11, 12, 0,  0,  0,
                                 16, 17, 0,  0,  0,
                                 21, 22, 0,  0,  0};

// check if the board is at goal state
// zero is used to ignore tiles in suboptimal goals where only specific rows or columns are checked
bool is_goal_board(const board_t& board, const board_t& goal) {
    for(int i = 0; i < board.size(); i++) {
        if(goal[i] == 0) {
            continue;
        }
        if(goal[i] != board[i]) {
            return false;
        }
    }
    return true;
}

// manhattan distance function
// zero is used to ignore tiles in suboptimal goals where only specific rows or columns are calculated
int distance(const board_t& board, const board_t& goal) {
    int width = board_width(board);
    int total = 0;
    for(int i = 0; i < board.size(); i++) {
        int x = i % width, y = i / width;
        int normalized_tile = board[i] - 1; // start tile indexing at zero
        if(goal[normalized_tile] == 0) {
            continue;
        }
        int tile_x = normalized_tile % width, tile_y = normalized_tile / width;
        int distance = std::abs(tile_x - x) + std::abs(tile_y - y); 
        total += distance;
    }
    return total;
}

// Left, Up, Right, Down
std::vector<move_t> possible_moves(const board_t& board) {
    std::vector<move_t> moves;
    int width = board_width(board);
    int empty_tile = board.size();
    board_t left = board, up = board, right = board, down = board;
    int empty_index;
    // find index of empty tile
    for(int i = 0; i < board.size(); i++ ) {
        if(board[i] == empty_tile) {
            empty_index = i;
            break;
        }
    }
    // calculate coordinates of empty tile and create children boards
    int x = empty_index % width, y = empty_index / width;
    if(x > 0) { // left
        int new_x = x - 1, new_y = y;
        int new_empty_index = new_x + new_y * width;
        std::swap(left[empty_index], left[new_empty_index]);
        moves.push_back(std::make_tuple(left, 'L'));
    }
    if(y > 0) { // up
        int new_x = x, new_y = y - 1;
        int new_empty_index = new_x + new_y * width;
        std::swap(up[empty_index], up[new_empty_index]);
        moves.push_back(std::make_tuple(up, 'U'));
    };
    if(x < width - 1) { // right
        int new_x = x + 1, new_y = y;
        int new_empty_index = new_x + new_y * width;
        std::swap(right[empty_index], right[new_empty_index]);
        moves.push_back(std::make_tuple(right, 'R'));
    }
    if(y < width - 1) { // down
        int new_x = x, new_y = y + 1;
        int new_empty_index = new_x + new_y * width;
        std::swap(down[empty_index], down[new_empty_index]);
        moves.push_back(std::make_tuple(down, 'D'));
    }
    return moves;
}

#endif
