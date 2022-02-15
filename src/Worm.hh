//
// Grosso: Worm.hh
//

// $Id: Worm.hh,v 1.1.1.1 2002/06/09 10:29:50 tommi Exp $

///////////////////////////////////////////////////////////////////////////

#ifndef WORM_HH_
#define WORM_HH_

#include "Field.hh"

///////////////////////////////////////////////////////////////////////////

class Worm {
public:
  enum MoveResult {
    IS_DEAD,
    DIED,
    ATE,
    MOVED,
    GOT_BLOCKED,
    CRASHED
  };

  enum {
    FORWARD = 0,
    LEFT = -1,
    RIGHT = 1
  };

  static const Field::Item FOOD = '*';

  Worm(Field* field, Field::Item head, Field::ColourNum foreground, Field::ColourNum background);
  Worm() { };
  virtual ~Worm() {};

  virtual void Reset(Field::Dim headx, Field::Dim heady, int dir, int length);
  virtual MoveResult TryMove(int turn) = 0;
  void ShowHead(Field::Item head);

protected:
  Field::Item Peek(int turn);
  MoveResult Move();
  MoveResult MoveHead();
  bool MoveTail();
  static int Direction(int dir);
  void Turn(int turn);
  void Lengthen(int length);
  bool IsGoable(Field::Item item);
  Field::ColourNum colourNum_;

private:
  int dir_;
  int lengthen_;

  Field* field_;
  Field::Item head_;
  Field::Dim headx_;
  Field::Dim heady_;
  Field::Dim tailx_;
  Field::Dim taily_;

  static const int NUM_DIRECTIONS = 4;
};

///////////////////////////////////////////////////////////////////////////

inline Worm::MoveResult
Worm::Move()
{
  MoveResult result = MoveHead();
  MoveTail();
  return result;
}

///////////////////////////////////////////////////////////////////////////

inline void
Worm::ShowHead(Field::Item head)
{
  field_->PutItem(headx_, heady_, head, colourNum_);
}

///////////////////////////////////////////////////////////////////////////

inline int
Worm::Direction(int dir)
{
  return dir&(NUM_DIRECTIONS-1);
}

///////////////////////////////////////////////////////////////////////////

inline void
Worm::Turn(int turn)
{
  dir_ = Direction(dir_ + turn);
}

///////////////////////////////////////////////////////////////////////////

inline void
Worm::Lengthen(int length)
{
  lengthen_ += length;
}

///////////////////////////////////////////////////////////////////////////

inline bool
Worm::IsGoable(Field::Item item)
{
  return item == FOOD || item == Field::EMPTY;
}

#endif // WORM_HH_
