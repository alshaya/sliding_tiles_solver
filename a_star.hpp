#ifndef A_STAR_HPP
#define A_STAR_HPP

#include <string>
#include <queue>
#include "board.hpp"
#include "utility.hpp"

std::string a_star(const board_t& board) {
    auto comp = [](const node_t& left, const node_t& right) { return left.f > right.f; };
    std::priority_queue<node_t, std::vector<node_t>, decltype(comp)> fringe;
    node_t root;

    // prepare root node and add it to fringe
    root.board = board;
    root.g = 0;
    root.h = distance(root.board, goal9);
    root.f = root.g + root.h;
    fringe.push(root);

    while(!fringe.empty()) {
        bool is_goal = is_goal_board(fringe.top().board, goal9);
        if(is_goal) {
            return fringe.top().path;
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
            node.h = distance(node.board, goal9);
            node.f = node.g + node.h;
            fringe.push(node);
        }
    }
    return "No Path is found";
}

#endif