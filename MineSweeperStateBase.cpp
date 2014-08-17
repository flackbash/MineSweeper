// Copyright 2014, flackbash

#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include "./MineSweeperStateBase.h"

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[37m"
#define RESET   "\x1b[0m"

// _____________________________________________________________________________
MineSweeperState::MineSweeperState() {
  // initialize screen
  initscr();
  nodelay(stdscr, true);
  noecho();
  cbreak();
  curs_set(false);
  keypad(stdscr, true);
}

// _____________________________________________________________________________
MineSweeperState::~MineSweeperState() {
  for (int i = 0; i < _numRows; i++) { delete[] _mineField[i]; }
  delete[] _mineField;
  endwin();
}

// _____________________________________________________________________________
void MineSweeperState::initialize(size_t numRows, size_t numCols,
size_t numMines) {
  _numRows = numRows;
  _numCols = numCols;
  _numMines = numMines;
  _numRevealed = 0;
  _numMarked = 0;
  _status = ONGOING;

  // initialize two-dimensional _mineField with UNREVEALED
  _mineField = new CellInfo* [_numRows];
  for (int h = 0; h < _numRows; h++) {
    _mineField[h] = new CellInfo[_numCols];
    for (int k = 0; k < _numCols; k++) {
      _mineField[h][k] = UNREVEALED;
    }
  }

  // set alignment
  setAlignment();
}

// _____________________________________________________________________________
void MineSweeperState::setAlignment() {
  // get window size and adjust alignment
  int winWidth, winHeight;
  getmaxyx(stdscr, winHeight, winWidth);
  _xAl = (winWidth / 2) - (_numCols);
  _yAl = 8;

  // check whether the window is big enough to display the field
  // end program if it's not
  if (winWidth <= _numCols * 2 || winHeight <= _numRows * 2 + 10) {
    printf("\x1b[%d;%dH", _yAl - 7, _xAl + _numCols - 16);
    printf("Please increase your windows size.");
    fflush(stdout);
    usleep(2 * 1000 * 1000);
    endwin();
    exit(1);
  }
}

// _____________________________________________________________________________
void MineSweeperState::drawField() {
  for (int i = 0; i < _numRows; i++) {
    for (int j = 0; j < _numCols; j++) {
      printf("%s\x1b[%d;%dH", RESET, i * 2 + _yAl + 1, j * 2 + _xAl + 1);
      CellInfo cellInfo = getCellInfo(i, j);

      // draw '? if cell is unrevealed and not a mine
      if (cellInfo == UNREVEALED) printf("?");

      // draw '?' if cell is unrevealed (mine) and 'x' when the game is lost
      else if (cellInfo == UNREVEALED_MINE) {
        if (_status == LOST) {
          printf("%sx", RED);
        } else {
          printf("?");
        }

      // draw 'X' if cell is marked (wrongly) and '-' when the game is lost
      } else if (cellInfo == MARKED_WRONG) {
        if (_status == LOST) {
          printf("%s-", RED);
        } else {
          printf("%sX", RED);
        }

      // draw 'X' if cell is marked (correctly)
      } else if (cellInfo == MARKED_CORRECT) {
        printf("%sX", RED);

      // draw '*' if cell is a revealed mine
      } else if (cellInfo == REVEALED_MINE) {
        printf("%s*", RED);

      // draw number of surrounding mines in diff. colors if cell is revealed
      } else if (cellInfo >= 0) {
        switch (cellInfo) {
          case(REVEALED_ZERO):
            printf("%s", WHITE);
            break;
          case(REVEALED_ONE):
            printf("%s", CYAN);
            break;
          case(REVEALED_TWO):
            printf("%s", GREEN);
            break;
          case(REVEALED_THREE):
            printf("%s", MAGENTA);
            break;
          case(REVEALED_FOUR):
            printf("%s", BLUE);
            break;
          case(REVEALED_FIVE):
            printf("%s", YELLOW);
            break;
          case(REVEALED_SIX):
            printf("%s", CYAN);
            break;
          case(REVEALED_SEVEN):
            printf("%s", GREEN);
            break;
          case(REVEALED_EIGHT):
            printf("%s", RED);
            break;
        }
        printf("%d", cellInfo);
      }
    }
  }
  printf("%s", RESET);
}

// _____________________________________________________________________________
void MineSweeperState::setMines() {
  int fieldIndex;
  int mineCount = 0;
  int row;
  int col;
  unsigned int seed = (unsigned int)(time(NULL));
  while (mineCount < _numMines) {
    fieldIndex = rand_r(&seed) % (_numRows * _numCols);
    row = fieldIndex / _numCols;
    col = fieldIndex % _numCols;
    if (_mineField[row][col] == UNREVEALED) {
      _mineField[row][col] = UNREVEALED_MINE;
      mineCount++;
    }
  }
}

// _____________________________________________________________________________
MineSweeperState::CellInfo MineSweeperState::getCellInfo(size_t row,
size_t col) const {
  // make sure the given index is within range
  if (row >= 0 && row < _numRows && col >= 0 && col < _numCols) {
    return _mineField[row][col];
  }
}

// _____________________________________________________________________________
void MineSweeperState::revealCell(size_t row, size_t col) {
  int mineCount = 0;

  // the game is lost if an unrevealed mine is revealed
  if (getCellInfo(row, col) == UNREVEALED_MINE) {
    _status = LOST;
    _mineField[row][col] = REVEALED_MINE;
  }

  // get number of mines in surrounding cells
  // do if the field is unrevealed and not a mine or marked as one
  if (getCellInfo(row, col) == UNREVEALED) {
    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        if (getCellInfo(row + i, col + j) <= MARKED_CORRECT) mineCount++;
      }
    }
    _mineField[row][col] = CellInfo(mineCount);
    _numRevealed++;

    // reveal cells recursively if there are no mines in surrounding cells
    if (mineCount == 0) {
      for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
          revealCell(row + i, col + j);
        }
      }
    }

    // check whether the game is won already
    if (_numMarked + _numRevealed == _numCols * _numRows) _status = WON;
  }
}

// _____________________________________________________________________________
void MineSweeperState::markCell(size_t row, size_t col) {
  // set CellInfo to marked (correctly/ẃrongly) if cell was unrevealed before
  if (getCellInfo(row, col) == UNREVEALED_MINE) {
    _mineField[row][col] = MARKED_CORRECT;
    _numMarked++;
  } else if (getCellInfo(row, col) == UNREVEALED) {
    _mineField[row][col] = MARKED_WRONG;

  // set CellInfo to unrevealed (no mine/mine) if cell was marked before
  } else if (getCellInfo(row, col) == MARKED_CORRECT) {
    _mineField[row][col] = UNREVEALED_MINE;
    _numMarked--;
  } else if (getCellInfo(row, col) == MARKED_WRONG) {
    _mineField[row][col] = UNREVEALED;
  }

  // check whether the game is won already
  if (_numMarked + _numRevealed == _numCols * _numRows) _status = WON;
}

// _____________________________________________________________________________
void MineSweeperState::reset() {
  for (int i = 0; i < _numRows; i++) { delete[] _mineField[i]; }
  delete[] _mineField;
  clear();
  initialize(_numRows, _numCols, _numMines);
  setMines();
}
// _____________________________________________________________________________
MineSweeperState::GameStatus MineSweeperState::status() const {
  return _status;
}

// _____________________________________________________________________________
size_t MineSweeperState::numRows() const {
  return _numRows;
}

// _____________________________________________________________________________
size_t MineSweeperState::numCols() const {
  return _numCols;
}

// _____________________________________________________________________________
size_t MineSweeperState::numMines() const {
  return _numMines;
}

// _____________________________________________________________________________
size_t MineSweeperState::numRevealed() const {
  return _numRevealed;
}

// _____________________________________________________________________________
size_t MineSweeperState::numMarked() const {
  return _numMarked;
}

// _____________________________________________________________________________
size_t MineSweeperState::xAl() const {
  return _xAl;
}

// _____________________________________________________________________________
size_t MineSweeperState::yAl() const {
  return _yAl;
}
