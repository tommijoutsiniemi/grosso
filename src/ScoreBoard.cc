//
// Grosso: ScoreBoard.cc
//

// 2002/06/09 10:29:50 tommi

///////////////////////////////////////////////////////////////////////////

#ifndef MAKEDEPEND
//#include <iomanip.h>
#include <time.h>
#endif

#include "config.hh"
#include "ScoreBoard.hh"

using namespace std;

///////////////////////////////////////////////////////////////////////////

ScoreBoard::ScoreBoard(const string& scoreFile, int numScores) :
  numScores_(numScores)
{
  scoreFile_ = scoreFile;
  FILE *sf = fopen(scoreFile_.c_str(), "r");

  if (sf == NULL) {
    fprintf(stderr, "Couldn't open high score file in read mode.\n");
    perror(scoreFile_.c_str());
  }
  else {
    for (int i = 0; i < numScores_ && !feof(sf); i++) {
      char s[1024];
      if (fgets(s, 1024, sf)) {
        scores_.push_back(s);
      }
    }

    fclose(sf);
  }
}

///////////////////////////////////////////////////////////////////////////

ScoreBoard::~ScoreBoard()
{
  FILE *sf = fopen(scoreFile_.c_str(), "w");

  if (sf == NULL) {
    fprintf(stderr, "Couldn't open high score file in write mode.\n");
    perror(scoreFile_.c_str());
  }
  else {
    while(!scores_.empty()) {
      fprintf(sf, "%s", scores_.front().c_str());
      scores_.pop_front();
    }

    fclose(sf);
  }
}

///////////////////////////////////////////////////////////////////////////

int
ScoreBoard::Add(int score, const string& name)
{
  char s[256];
  time_t t;

  t = time(0);
  sprintf(s, "%6d  %10.10s %4.4s  %s\n",
	  score, ctime(&t), ctime(&t)+20, name.c_str());

  const ScoreEntry entry = s;
  Scores::iterator si;
  int i;
  for (i = 1, si = scores_.begin();
       si != scores_.end() && entry < *si;
       si++, i++)
    ;

  if (i <= numScores_) {
    scores_.insert(si, entry);
  }

  return i;
}

///////////////////////////////////////////////////////////////////////////

void
ScoreBoard::Show(FILE* f, int rank, int score)
{
  int i;
  Scores::const_iterator si;

  for (i = 1, si = scores_.begin(); si != scores_.end(); si++, i++) {
    fprintf(f, "%s %2d. %s", rank==i ? "O ":"  ", i, si->c_str());
  }

  if (rank >= i) {
    fprintf(f, "  You scored %d\n", score);
  }
}
