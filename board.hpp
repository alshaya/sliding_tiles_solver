#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <cmath>
#include <sstream>
#include <string>
#include <iomanip>
#include <iostream>

// Define board type
using board_t = std::vector<int>;

// only support three variants of the board (9, 16 and 25 tiles)
bool is_valid_board_size(const board_t& board) {
   return board.size() == 9 || board.size() == 16 || board.size() == 25;
}

// Check the tiles and empty tile
// Empty tile is represented as (largest tile + 1) for example in 3x3 board it is 9
bool is_valid_board(board_t board) {
    bool is_valid = true;
    std::sort(board.begin(), board.end());
    for(int i = 0; i < board.size(); i++) {
        if(board[i] != i+1) {
            is_valid = false;
            break;
        }
    }
    return is_valid ;
}

// Assuming the board is a square, return the width of the board
int board_width(const board_t& board) {
    double board_width = std::sqrt(board.size());
    return static_cast<int>(board_width);
}

board_t parse_board(const std::string& tiles) {
    board_t board;
    std::istringstream parser(tiles);
    std::string tile;
    while(std::getline(parser, tile, ',')) {
        board.push_back(std::stoi(tile));
    }
    return board;
}

void print_board(const board_t& board) {
    const int rows = board_width(board);
    const int empty_tile = board.size();
    for(int i = 0; i < board.size(); i++) {
        if(i > 0 && i % rows == 0) {
            std::cout << std::endl;
        }
        if(board[i] == empty_tile) {
            std::cout << std::setfill(' ') << std::setw(3) << "X";
        }
        else {
            std::cout << std::setfill(' ') << std::setw(3) << board[i];
        }
    }
    std::cout << std::endl;
}

#endif