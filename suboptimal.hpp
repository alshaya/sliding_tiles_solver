#ifndef SUBOPTIMAL_HPP
#define SUBOPTIMAL_HPP

#include <string>
#include <stack>
#include <tuple>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include "board.hpp"
#include "utility.hpp"
#include "a_star.hpp"

// This is a magic number that is multiplied by the heuristic function
// which somehow is fastest when is set to 3. I don't understand why!
// We are just trying to make the heuristic inadmissible, but the
// exact magic number was based on trial and error!
const int MAGIC_NUMBER = 3;

// <goal_found, goal_path, new_bound or infinity>
std::tuple<bool, std::string, int, board_t> suboptimal_ida_star_impl(const board_t& board, int bound, const board_t& goal) {
    std::stack<node_t> fringe;
    node_t root;
    int new_bound = std::numeric_limits<int>::max();

    // prepare root node and add it to fringe
    root.board = board;
    root.g = 0;
    root.h = distance(root.board, goal) * MAGIC_NUMBER;
    root.f = root.g + root.h;
    fringe.push(root);

    while(!fringe.empty()) {
        bool is_goal = is_goal_board(fringe.top().board, goal);
        if(is_goal) {
            return std::make_tuple(true, fringe.top().path, bound, fringe.top().board);
        }

        node_t current = fringe.top();
        board_t current_board = current.board;
        std::vector<move_t> moves = possible_moves(current_board);

        // remove the current node from the fringe
        fringe.pop();

        // add new board moves to the fringes
        for(move_t board_move : moves) {
            auto [board, move] = board_move;
            node_t node;
            node.board = board;
            node.path = current.path + move;
            node.g = current.g + 1;
            node.h = distance(node.board, goal) * MAGIC_NUMBER;
            node.f = node.g + node.h;

            if(node.f > bound) { // prune if cost is greater than bound
                new_bound = std::min(node.f, new_bound);
            }
            else {
                fringe.push(node);
            }
        }
    }
    return std::make_tuple(false, "", new_bound, board_t());
}

std::tuple<std::string, board_t> suboptimal_ida_star(const board_t& board, const board_t& goal) {
    int bound = distance(board, goal) * MAGIC_NUMBER;

    while(true) {
        auto [goal_found, path, new_bound, result_board] = suboptimal_ida_star_impl(board, bound, goal);
        if(goal_found) {
            return std::make_tuple(path, result_board);
        }
        else if(new_bound == std::numeric_limits<int>::max()) {
            break;
        }
        else {
            bound = new_bound;
        }
    }
    return std::make_tuple("No Path is found", board_t());
}

std::string suboptimal(const board_t& board) {
    board_t inner_board; // inner 3x3 board
    int width = board_width(board);
    // mapping of remaining tiles in the 5x5 board into the inner 3x3 board
    std::unordered_map<int, int> inner_board_map = {
        {13, 1},
        {14, 2},
        {15, 3},
        {18, 4},
        {19, 5},
        {20, 6},
        {23, 7},
        {24, 8},
        {25, 9},
    };
    // solve first row, first column, second row, second column and finally solve the inner 
    auto [first_row_path, row_board] = suboptimal_ida_star(board, suboptimal_row_goal25);
    auto [first_col_path, col_board] = suboptimal_ida_star(row_board, suboptimal_col_goal25);
    auto [second_row_path, row2_board] = suboptimal_ida_star(col_board, suboptimal_row2_goal25);
    auto [second_col_path, col2_board] = suboptimal_ida_star(row2_board, suboptimal_col2_goal25);

    for(int y = 2; y < width; y++) {
        for(int x = 2; x < width; x++) {
            int tile = col2_board[x + y * width]; // normalize tile to start at 1 for 3x3 board
            int normalized_tile = inner_board_map[tile];
            inner_board.push_back(normalized_tile);
        }
    }
    std::string inner_board_path = a_star(inner_board);
    
    return first_row_path + first_col_path + second_row_path + second_col_path + inner_board_path;
}

#endif