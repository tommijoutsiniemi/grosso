//
// Grosso: Field.cc
//

// 2002/06/09 10:29:50 tommi

///////////////////////////////////////////////////////////////////////////

#ifndef MAKEDEPEND
#include <unistd.h>
#include <string.h>
#endif

#include "config.hh"
#include "Field.hh"

const int BUFSIZE = 1024;

using namespace std;

///////////////////////////////////////////////////////////////////////////

Field::Field(Dim width, Dim height, ColourNum fg, ColourNum bg) : nColours_(0)
{
  int i;
  char s[BUFSIZE];

  // curses and screen initialization
  initscr();
  cbreak();
  noecho();
  nonl();
  curs_set(0);
  if (has_colors()) {
    start_color();
  }
  fieldColourNum_ = NewColour(fg, bg);

  // get game, window and screen size
  getmaxyx(stdscr, screenHeight_, screenWidth_);
  if (width > 15) {
    width_ = MIN(screenWidth_, width);
  }
  else {
    width_ = screenWidth_;
  }
  if (height > 15) {
    height_ = MIN(screenHeight_, height);
  }
  else {
    height_ = screenHeight_;
  }

  //
  wIndent_ = 0;
  if (screenWidth_ > width_) {
    wIndent_ = (screenWidth_ - width_) / 2;
  }
  hIndent_ = 0;
  if (screenHeight_ > height_) {
    hIndent_ = (screenHeight_ - height_) / 2;
  }

#ifdef AVOID_MVWINCH
  aargh.reserve(width_*height_);
#endif // AVOID_MVWINCH

  // create and initialize game window
  window_ = newwin(height_, width_, hIndent_, wIndent_);
  wbkgdset(window_, COLOR_PAIR(1));
  nodelay(window_, TRUE);
  leaveok(window_, TRUE);
  wclear(window_);

  // set up dimension lookup tables for moving
  xdim_.push_back(width_-1);
  for (i = 0; i < width_; i++) {
    xdim_.push_back(i);
  }
  xdim_.push_back(0);
  ydim_.push_back(height_-1);
  for (i = 0; i < height_; i++) {
    ydim_.push_back(i);
  }
  ydim_.push_back(0);

  // count levels
  for (numLevels_ = 0;
       sprintf(s, "%s/%02d", LEVELDIR, numLevels_+1), access(s, R_OK) == 0;
       numLevels_++)
    ; // Empty
}

///////////////////////////////////////////////////////////////////////////

Field::~Field()
{
  // delete game window
  delwin(window_);
  // shut down curses
  endwin();
}

///////////////////////////////////////////////////////////////////////////

void
Field::Clear()
{
  wclear(window_);

#ifdef AVOID_MVWINCH
  for (int i = 0; i < width_*height_; i++) {
    aargh[i] = EMPTY;
  }
#endif // AVOID_MVWINCH

  touchwin(stdscr);
  wrefresh(stdscr);
}

///////////////////////////////////////////////////////////////////////////

void
Field::Puts(Dim x, Dim y, const string& s, ColourNum c)
{
  if (c == -1) {
    c = fieldColourNum_;
  }

  AttrOn(COLOR_PAIR(c));

#ifdef AVOID_MVWINCH
  for (unsigned i = 0; i < s.size(); i++) {
    if (s[i] == '\n') {
      y++;
      x = 0;
    }
    else{
      PutItem(x, y, s[i], c);
      x++;
    }
  }
#else
  mvwaddstr(window_, y, x, s.c_str());
#endif // AVOID_MVWINCH

  AttrOff(COLOR_PAIR(c));
}

///////////////////////////////////////////////////////////////////////////

int
Field::Display(const string& message, ColourNum c, Dim dx, Dim dy)
{
  int key;
  WINDOW *w;
  Dim x, y;
  char m[1024];
  char *line;
  short i;

  // show the message in a temporary window
  w = newwin(height_, width_, hIndent_, wIndent_);
  wbkgdset(w, COLOR_PAIR(1));
  wattron(w, COLOR_PAIR(c));
  wclear(w);
  x = (width_ - dx) / 2;
  y = (height_ - dy) / 2;
  strcpy(m, message.c_str());
  for (line = strtok(m, "\n"), i = 0;
       line != NULL;
       line = strtok(NULL, "\n"), ++i)
  {
    mvwaddstr(w, y+i, x, line);
  }
  wrefresh(w);
  while ((key = Key()) == ERR) {
    Sleep(100);
  }
  wattroff(w, COLOR_PAIR(c));
  delwin(w);

  touchwin(stdscr);
  wrefresh(stdscr);
  touchwin(window_);
  wrefresh(window_);

  return key;
}

///////////////////////////////////////////////////////////////////////////

void
Field::DrawLevel(FILE* f, Dim x, Dim y, Dim width, Dim height, ColourNum c)
{
  int i, j;
  char s[BUFSIZE];

  // draw the level into the specified position
  for (j = 0; j < height && fgets(s, sizeof(s), f); j++) {
    for (i = 0; i < width && s[i] != '\n'; i++) {
      if (s[i] != ' ') {
        PutItem(x + i, y + j, 'X', c);
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////

void
Field::Level(int level)
{
  char s[BUFSIZE];
  Dim x, y, w, h;
  FILE* lf;

  Clear();
  sprintf(s, "%s/%02d", LEVELDIR, (level - 1) % numLevels_ + 1);
  lf = fopen(s, "r");
  if (lf) {
    AttrOn(Field::REVERSE);
    // discard comment and get level size
    if (fgets(s, sizeof(s), lf) != NULL && fscanf(lf,"%d %d\n",&w,&h) > 0) {
      for (y = 0; y < Height(); y += h + 3) {
        for (x = 0; x < Width(); x += w + 3) {
          rewind(lf);
          // discard comment and dimensions lines
          if (fgets(s,sizeof(s),lf) != NULL && fgets(s,sizeof(s),lf) !=NULL) {
            DrawLevel(lf, x, y, MIN(Width()-x, w), MIN(Height()-y, h), fieldColourNum_);
          }
        }
      }
    }
    AttrOff(Field::REVERSE);
    fclose(lf);
  }
}

///////////////////////////////////////////////////////////////////////////

Field::ColourNum
Field::NewColour(ColourNum fg, ColourNum bg)
{
  init_pair(++nColours_, fg, bg);
  return nColours_;
}
