// Copyright 2014, flackbash

#ifndef MINESWEEPER__MINESWEEPERSTATEBASE_H_  // NOLINT
#define MINESWEEPER__MINESWEEPERSTATEBASE_H_  // NOLINT

#include <gtest/gtest.h>

class MineSweeperStateBase;
extern MineSweeperStateBase* Mine;

// Class representing a state of the minesweeper game
class MineSweeperStateBase {
 public:
  // Virtual destructor
  virtual ~MineSweeperStateBase() {}

  // Get cell information
  enum CellInfo {
    REVEALED_MINE = -5, UNREVEALED_MINE = -4, MARKED_CORRECT = -3,
    MARKED_WRONG = -2, UNREVEALED = -1, REVEALED_ZERO = 0, REVEALED_ONE = 1,
    REVEALED_TWO =2, REVEALED_THREE = 3, REVEALED_FOUR = 4, REVEALED_FIVE = 5,
    REVEALED_SIX = 6, REVEALED_SEVEN = 7, REVEALED_EIGHT = 8
  };
  virtual CellInfo getCellInfo(size_t rowIndex, size_t colIndex) const = 0;

  // Initialize
  virtual void initialize(size_t numRows, size_t numCols, size_t numMines) = 0;

  // Status of the game
  enum GameStatus { ONGOING, LOST, WON };
  virtual GameStatus status() const = 0;

  // Getters
  virtual size_t numRows() const = 0;
  virtual size_t numCols() const = 0;
  virtual size_t numMines() const = 0;
  virtual size_t numRevealed() const = 0;
  virtual size_t numMarked() const = 0;
};

class MineSweeperState : public MineSweeperStateBase {
 public:
  // constructor
  MineSweeperState();

  // destructor
  ~MineSweeperState();

  // initialize the field with a given number of rows, columns and mines
  void initialize(size_t numRows, size_t numCols, size_t numMines);
  FRIEND_TEST(MineSweeperStateTest, initialize);

  // set Alignment in a way that the field appears centered in the window
  void setAlignment();

  // draw field
  void drawField();

  // randomly initialize mines on the field
  void setMines();

  // get information about the current state of a cell
  CellInfo getCellInfo(size_t row, size_t col) const;

  // reveal cell information
  void revealCell(size_t row, size_t col);
  FRIEND_TEST(MineSweeperStateTest, revealCell);

  // mark cell as mine
  void markCell(size_t row, size_t col);
  FRIEND_TEST(MineSweeperStateTest, markCell);

  // reset values of the game to play again
  void reset();
  FRIEND_TEST(MineSweeperStateTest, reset);

  // getters
  GameStatus status() const;
  size_t numRows() const;
  size_t numCols() const;
  size_t numMines() const;
  size_t numRevealed() const;
  size_t numMarked() const;
  size_t xAl() const;
  size_t yAl() const;

 private:
  // number of rows, columns, mines, revealed and CORRECTLY marked fields
  size_t _numRows;
  size_t _numCols;
  size_t _numMines;
  size_t _numRevealed;
  size_t _numMarked;

  // field containing information about each cell in the field
  CellInfo** _mineField;

  // variable holding the current status of the game
  GameStatus _status;

  // x and y alignment of the field
  size_t _xAl;
  size_t _yAl;
};

#endif  // MINESWEEPER__MINESWEEPERSTATEBASE_H_  // NOLINT
