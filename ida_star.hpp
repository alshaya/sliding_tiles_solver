#ifndef IDA_STAR_HPP
#define IDA_STAR_HPP

#include <string>
#include <stack>
#include <tuple>
#include <limits>
#include <algorithm>
#include "board.hpp"
#include "utility.hpp"

// <goal_found, goal_path, new_bound or infinity>
std::tuple<bool, std::string, int> ida_star_impl(const board_t& board, int bound) {
    std::stack<node_t> fringe;
    node_t root;
    int new_bound = std::numeric_limits<int>::max();

    // prepare root node and add it to fringe
    root.board = board;
    root.g = 0;
    root.h = distance(root.board, goal16);
    root.f = root.g + root.h;
    fringe.push(root);

    while(!fringe.empty()) {
        bool is_goal = is_goal_board(fringe.top().board, goal16);
        if(is_goal) {
            return std::make_tuple(true, fringe.top().path, bound);
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
            node.h = distance(node.board, goal16);
            node.f = node.g + node.h;

            if(node.f > bound) { // prune if cost is greater than bound
                new_bound = std::min(node.f, new_bound);
            }
            else {
                fringe.push(node);
            }
        }
    }
    return std::make_tuple(false, "", new_bound);
}

std::string ida_star(const board_t& board) {
    int bound = distance(board, goal16);

    while(true) {
        auto [goal_found, path, new_bound] = ida_star_impl(board, bound);
        if(goal_found) {
            return path;
        }
        else if(new_bound == std::numeric_limits<int>::max()) {
            break;
        }
        else {
            bound = new_bound;
        }
    }
    return "No Path is found";
}

#endif