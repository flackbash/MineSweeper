// Copyright 2014, flackbash

#include <gtest/gtest.h>
#include "./MineSweeperStateBase.h"

// _____________________________________________________________________________
TEST(MineSweeperStateTest, getCellInfo) {
  MineSweeperState mss;
  mss.initialize(5, 5, 5);
  ASSERT_EQ(MineSweeperState::CellInfo::UNREVEALED, mss.getCellInfo(2, 4));
}

// _____________________________________________________________________________
TEST(MineSweeperStateTest, revealCell) {
  MineSweeperState mss;
  mss.initialize(3, 3, 3);
  mss._mineField[1][1] = MineSweeperState::CellInfo::UNREVEALED_MINE;
  mss._mineField[0][1] = MineSweeperState::CellInfo::UNREVEALED_MINE;
  mss.revealCell(0, 0);
  ASSERT_EQ(MineSweeperState::CellInfo::REVEALED_TWO, mss.getCellInfo(0, 0));
  ASSERT_EQ(1, mss.numRevealed());
  mss.revealCell(0, 1);
  ASSERT_EQ(MineSweeperState::CellInfo::REVEALED_MINE, mss.getCellInfo(0, 1));
  ASSERT_EQ(MineSweeperState::GameStatus::LOST, mss.status());
}

// _____________________________________________________________________________
TEST(MineSweeperStateTest, markCell) {
  MineSweeperState mss;
  mss.initialize(3, 3, 3);
  mss._mineField[1][1] = MineSweeperState::CellInfo::UNREVEALED_MINE;
  mss._mineField[0][1] = MineSweeperState::CellInfo::UNREVEALED_MINE;
  mss.markCell(1, 1);
  ASSERT_EQ(1, mss.numMarked());
  ASSERT_EQ(MineSweeperState::CellInfo::MARKED_CORRECT, mss.getCellInfo(1, 1));
  mss.markCell(0, 0);
  ASSERT_EQ(1, mss.numMarked());
  ASSERT_EQ(MineSweeperState::CellInfo::MARKED_WRONG, mss.getCellInfo(0, 0));
  mss.markCell(0, 0);
  ASSERT_EQ(1, mss.numMarked());
  ASSERT_EQ(MineSweeperState::CellInfo::UNREVEALED, mss.getCellInfo(0, 0));
}

// _____________________________________________________________________________
TEST(MineSweeperStateTest, initialize) {
  MineSweeperState mss;
  mss.initialize(10, 10, 20);

  ASSERT_EQ(20, mss._numMines);
  ASSERT_EQ(10, mss._numRows);
  ASSERT_EQ(10, mss._numCols);
  ASSERT_EQ(0, mss._numRevealed);
  ASSERT_EQ(0, mss._numMarked);
  ASSERT_EQ(MineSweeperState::GameStatus::ONGOING, mss._status);
  for (int h = 0; h < mss._numRows; h++) {
    for (int k = 0; k < mss._numCols; k++) {
      ASSERT_EQ(MineSweeperState::CellInfo::UNREVEALED, mss._mineField[h][k]);
    }
  }
}

// _____________________________________________________________________________
TEST(MineSweeperStateTest, setMines) {
  MineSweeperState mss;
  int mineCounter = 0;
  mss.initialize(10, 10, 20);
  mss.setMines();
  for (int i = 0; i < mss.numRows(); i++) {
    for (int j = 0; j < mss.numCols(); j++) {
      if (mss.getCellInfo(i, j) == MineSweeperState::CellInfo::UNREVEALED_MINE)
        mineCounter++;
    }
  }
  ASSERT_EQ(20, mineCounter);
}

// _____________________________________________________________________________
TEST(MineSweeperStateTest, reset) {
  MineSweeperState mss;
  mss.initialize(11, 12, 40);
  mss._status = MineSweeperState::GameStatus::LOST;
  mss._numRevealed = 10;
  mss._numMarked = 10;
  mss.reset();
  ASSERT_EQ(MineSweeperState::GameStatus::ONGOING, mss.status());
  ASSERT_EQ(0, mss._numRevealed);
  ASSERT_EQ(0, mss._numMarked);
}
