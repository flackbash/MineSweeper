// Copyright 2014, flackbash

#include <stdio.h>
#include <ncurses.h>
#include "./MineSweeperGame.h"
#include "./MineSweeperStateBase.h"

// _____________________________________________________________________________
void MineSweeperGame::play(size_t numRows, size_t numCols, size_t numMines) {
  _mss.initialize(numRows, numCols, numMines);
  _mss.setMines();
  mousemask(ALL_MOUSE_EVENTS, NULL);
  while (true) {
    _mss.drawField();

    // check whether game is already over and print appropriate output if it is
    if (_mss.status() != MineSweeperState::GameStatus::ONGOING) {
      gameOver();

    // process user input
    } else {
      printf("\x1b[%d;%dH", _mss.yAl() - 3, _mss.xAl() + _mss.numCols());
      printf("\xF0\x9F\x98\x8A");
      if (getmouse(&_me) == OK) {
        processClick();
      }
    }
    int ch = getch();
    if (ch == 'q') break;
    if (ch == 'r') _mss.reset();
  }
}

// _____________________________________________________________________________
void MineSweeperGame::gameOver() {
  if (_mss.status() == MineSweeperState::GameStatus::WON) {
    printf("\x1b[%d;%dH", _mss.yAl() - 5, _mss.xAl() + _mss.numCols() - 3);
    printf("YOU WON!\n\r");
    printf("\x1b[%d;%dH", _mss.yAl() - 3, _mss.xAl() + _mss.numCols());
    printf("\xF0\x9F\x98\x83");
  }
  if (_mss.status() == MineSweeperState::GameStatus::LOST) {
    printf("\x1b[%d;%dH", _mss.yAl() - 5, _mss.xAl() + _mss.numCols() - 4);
    printf("YOU LOST!\n\r");
    printf("\x1b[%d;%dH", _mss.yAl() - 3, _mss.xAl() + _mss.numCols());
    printf("\xF0\x9F\x98\xA2");
  }
  printf("\x1b[%d;%dH", _mss.yAl() + _mss.numRows() * 2 + 2, _mss.xAl() +
  _mss.numCols() - 8);
  printf("Press 'Q' to quit.");
  printf("\x1b[%d;%dH", _mss.yAl() + _mss.numRows() * 2 + 3, _mss.xAl() +
  _mss.numCols() - 11);
  printf("Press 'R' to play again.");
  fflush(stdout);
}

// _____________________________________________________________________________
void MineSweeperGame::processClick() {
  // reveal cell on single click
  if (_me.bstate & BUTTON1_CLICKED) {
    // make sure the click is on a cell and not on a space in between
    if ((_me.y - _mss.yAl()) % 2 == 0 && (_me.x - _mss.xAl()) % 2 == 0) {
      _mss.revealCell((_me.y - _mss.yAl()) / 2, (_me.x - _mss.xAl()) / 2);
    }

  // mark cell on double click
  } else if (_me.bstate & BUTTON1_DOUBLE_CLICKED) {
    // make sure the click is on a cell and not on a space in between
    if ((_me.y - _mss.yAl()) % 2 == 0 && (_me.x - _mss.xAl()) % 2 == 0) {
      _mss.markCell((_me.y - _mss.yAl()) / 2, (_me.x - _mss.xAl()) / 2);
    }
  }
}
