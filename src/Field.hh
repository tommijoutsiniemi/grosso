//
// Grosso: Field.hh
//

// $Id: Field.hh,v 1.1.1.1 2002/06/09 10:29:50 tommi Exp $

///////////////////////////////////////////////////////////////////////////

#ifndef FIELD_HH_
#define FIELD_HH_

#ifndef MAKEDEPEND
#include <string>
#include <vector>
#include <curses.h>
#include <stdio.h>
#endif

///////////////////////////////////////////////////////////////////////////

class Field {
public:
  typedef chtype Item;
  typedef attr_t Attr;
  typedef int Dim;
  typedef short ColourNum;

  Field(Dim width, Dim height, ColourNum fg, ColourNum bg);
  ~Field();

  void Clear();
  void Refresh();
  void Flush();
  ColourNum NewColour(ColourNum fg, ColourNum bg);

  Item GetItem(Dim x, Dim y);
  void PutItem(Dim x, Dim y, Item i, ColourNum colourNum);

  void Puts(Dim x, Dim y, const std::string& s, ColourNum c = -1);
  int Display(const std::string& message, ColourNum c, Dim dx, Dim dy);

  void AttrOn(Attr attrs);
  void AttrOff(Attr attrs);
  static const Attr REVERSE = A_REVERSE;

  void Move(Dim& x, Dim& y, Dim dx, Dim dy);
  int Key();

  Dim Width();
  Dim Height();

  void Level(int level);
  int NumLevels();

  static const Item EMPTY = ' ';

private:
  void DrawLevel(FILE* f, Dim x, Dim y, Dim width, Dim height, ColourNum colourNum);

  Dim screenWidth_;
  Dim screenHeight_;
  Dim width_;
  Dim height_;
  Dim wIndent_;
  Dim hIndent_;

  WINDOW* window_;

  typedef std::vector<Dim> Dims;
  Dims xdim_;
  Dims ydim_;

  short nColours_;
  ColourNum fieldColourNum_;

  int numLevels_;

#ifdef AVOID_MVWINCH
  std::vector<Item> aargh;
#endif // AVOID_MVWINCH
};

///////////////////////////////////////////////////////////////////////////

inline void
Field::AttrOn(Attr attrs)
{
  wattron(window_, attrs);
}

///////////////////////////////////////////////////////////////////////////

inline void
Field::AttrOff(Attr attrs)
{
  wattroff(window_, attrs);
}

///////////////////////////////////////////////////////////////////////////

inline void
Field::Refresh()
{
  wrefresh(window_);
}

///////////////////////////////////////////////////////////////////////////

inline void
Field::Flush()
{
  flushinp();
}

///////////////////////////////////////////////////////////////////////////

inline Field::Item
Field::GetItem(Dim x, Dim y)
{
#ifdef AVOID_MVWINCH
  return aargh[x + width_*y] & A_CHARTEXT;
#else
  return mvwinch(window_, y, x) & A_CHARTEXT;
#endif // AVOID_MVWINCH
}

///////////////////////////////////////////////////////////////////////////

inline void
Field::PutItem(Dim x, Dim y, Item i, short colourNum)
{
  AttrOn(COLOR_PAIR(colourNum));
  mvwaddch(window_, y, x, i);
  AttrOff(COLOR_PAIR(colourNum));

#ifdef AVOID_MVWINCH
  aargh[x + width_*y] = i;
#endif // AVOID_MVWINCH
}

///////////////////////////////////////////////////////////////////////////

inline void
Field::Move(Dim& x, Dim& y, Dim dx, Dim dy)
{
  x = xdim_[x+dx+1];
  y = ydim_[y+dy+1];
}

///////////////////////////////////////////////////////////////////////////

inline int
Field::Key()
{
  return wgetch(window_);
}

///////////////////////////////////////////////////////////////////////////

inline Field::Dim
Field::Width()
{
  return width_;
}

///////////////////////////////////////////////////////////////////////////

inline Field::Dim
Field::Height()
{
  return height_;
}

///////////////////////////////////////////////////////////////////////////

inline int
Field::NumLevels()
{
  return numLevels_;
}

#endif // FIELD_HH_
