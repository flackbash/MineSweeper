// Copyright 2014, flackbash

#ifndef MINESWEEPER__MINESWEEPERGAME_H_  // NOLINT
#define MINESWEEPER__MINESWEEPERGAME_H_  // NOLINT

#include <ncurses.h>
#include "./MineSweeperStateBase.h"

// class to play the game and bring together the different components
class MineSweeperGame {
 public:
  // play the game
  void play(size_t numRows, size_t numCols, size_t numMines);

  // print appropriate output if game is over
  void gameOver();

  // process the users mouse input
  void processClick();

 private:
  MineSweeperState _mss;
  MEVENT _me;
};

#endif  // MINESWEEPER__MINESWEEPERGAME_H_  // NOLINT
