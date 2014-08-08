// Copyright 2014, flackbash

#include <iostream>
#include "./MineSweeperStateBase.h"
#include "./MineSweeperGame.h"

int main(int argc, char** argv) {
  int numRows, numCols, numMines;
  std::cout << "Number of rows: ";
  std::cin >> numRows;
  if (numRows < 0 || numRows > 50) {
    std::cout << "Invalid number of rows.\n";
    exit(1);
  }
  std::cout << "Number of columns: ";
  std::cin >> numCols;
  if (numCols < 0 || numCols > 150) {
    std::cout << "Invalid number of columns.\n";
    exit(1);
  }
  std::cout << "Number of mines: ";
  std::cin >> numMines;
  if (numMines > numRows * numCols) {
    std::cout << "Enter a smaller number of mines.\n";
    exit(1);
  }
  MineSweeperGame msg;
  msg.play(numRows, numCols, numMines);
}
