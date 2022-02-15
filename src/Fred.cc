//
// Grosso: Fred.cc
//

// 2002/06/09 10:29:50 tommi

///////////////////////////////////////////////////////////////////////////

#ifndef MAKEDEPEND
#include <stdlib.h>
#endif

#include "config.hh"
#include "Fred.hh"

///////////////////////////////////////////////////////////////////////////

Fred::Fred(Field* field, Field::Item head, Field::ColourNum fg, Field::ColourNum bg, int probab) :
  Worm(field, head, fg, bg), probab_(probab)
{
}

///////////////////////////////////////////////////////////////////////////

void
Fred::Reset(Field::Dim headx, Field::Dim heady, int dir, int length)
{
  Worm::Reset(headx, heady, dir, length);
  strength_ = 100;
  dead_ = false;
}

///////////////////////////////////////////////////////////////////////////

Fred::MoveResult
Fred::TryTurn()
{
  // try a random turn
  int turn = rand()&1 ? LEFT : RIGHT;
  if (IsGoable(Peek(turn))) {
    Turn(turn);
  }
  else if (IsGoable(Peek(-turn))) {
    Turn(-turn);
  }
  else {
    // can't turn, give up
    return GOT_BLOCKED;
  }

  // turned if here, so lengthen and actually take the turn
  Lengthen(10);
  return Move();
}

///////////////////////////////////////////////////////////////////////////

Fred::MoveResult
Fred::TryMove(int)
{
  MoveResult result;

  if (dead_) {
    return IS_DEAD;
  }

  result = GOT_BLOCKED;
  // try to move, sometimes try to turn first
  if ((rand()%1000) < probab_) {
    // first try turning, if still blocked try forward
    result = TryTurn();
    if (result == GOT_BLOCKED && IsGoable(Peek(FORWARD))) {
      result = Move();
    }
  }
  else {
    // first try forward, if still blocked try turning
    if (IsGoable(Peek(FORWARD))) {
      result = Move();
    }
    if (result == GOT_BLOCKED) {
      result = TryTurn();
    }
  }

  if (result == GOT_BLOCKED) {
    if (!MoveTail()) {
      // couldn't move the head nor the tail
      strength_--;
      if (strength_ == 0) {
        dead_ = true;
        result = DIED;
        ShowHead(Field::EMPTY);
      }
    }
    else {
      // got blocked, but shrinked successfully, so "moved"
      result = MOVED;
    }
  }

  return result;
}
