//
// Grosso: Worm.cc
//

// 2002/06/09 10:29:49 tommi

///////////////////////////////////////////////////////////////////////////

#include "config.hh"
#include "Field.hh"
#include "Worm.hh"

///////////////////////////////////////////////////////////////////////////

static const Field::Dim xdimd[] = {  0,  1,  0, -1};
static const Field::Dim ydimd[] = { -1,  0,  1,  0};
static const Field::Item tail[] = { 'A', '>', 'V', '<'};

///////////////////////////////////////////////////////////////////////////

Worm::Worm(Field* field, Field::Item head, Field::ColourNum fg, Field::ColourNum bg)
{
  field_ = field;
  head_ = head;
  lengthen_ = 10;
  dir_ = 2;
  headx_ = 0;
  heady_ = 0;
  tailx_ = 0;
  taily_ = 0;
  colourNum_ = field_->NewColour(fg, bg);
}

///////////////////////////////////////////////////////////////////////////

void
Worm::Reset(Field::Dim headx, Field::Dim heady, int dir, int length)
{
  lengthen_ = length;
  dir_ = Direction(dir);
  headx_ = headx;
  heady_ = heady;
  tailx_ = headx;
  taily_ = heady;

  field_->PutItem(headx_, heady_, tail[dir_], colourNum_);
}

///////////////////////////////////////////////////////////////////////////

Field::Item
Worm::Peek(int turn)
{
  int dir = Direction(dir_+turn);
  Field::Dim x = headx_;
  Field::Dim y = heady_;

  field_->Move(x, y, xdimd[dir], ydimd[dir]);
  return field_->GetItem(x, y);
}

///////////////////////////////////////////////////////////////////////////

Worm::MoveResult
Worm::MoveHead()
{
  Field::Item i;

  field_->PutItem(headx_, heady_, tail[dir_], colourNum_);
  field_->Move(headx_, heady_, xdimd[dir_], ydimd[dir_]);
  i = field_->GetItem(headx_, heady_);
  field_->PutItem(headx_, heady_, head_, colourNum_);

  switch (i) {
  case FOOD:
    Lengthen(10);
    return ATE;
  case Field::EMPTY:
    return MOVED;
  default:
    return CRASHED;
  }
}

///////////////////////////////////////////////////////////////////////////

bool
Worm::MoveTail()
{
  int dir;
  Field::Item i;
  bool result;

  if (lengthen_ > 0) {
    lengthen_--;
    result = true;
  }
  else {
    i = field_->GetItem(tailx_, taily_);
    result = (i != head_);
    for (dir = 0; dir < NUM_DIRECTIONS; dir++) {
      if (i == tail[dir]) {
        field_->PutItem(tailx_, taily_, Field::EMPTY, colourNum_);
        field_->Move(tailx_, taily_, xdimd[dir], ydimd[dir]);
        dir = NUM_DIRECTIONS;
      }
    }
  }

  return result;
}
