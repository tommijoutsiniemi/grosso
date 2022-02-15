//
// Grosso: ScoreBoard.hh
//

// $Id: ScoreBoard.hh,v 1.1.1.1 2002/06/09 10:29:50 tommi Exp $

///////////////////////////////////////////////////////////////////////////

#ifndef SCOREBOARD_HH_
#define SCOREBOARD_HH_

#ifndef MAKEDEPEND
#include <list>
#include <string>
#include <stdio.h>
#endif

#include "config.hh"

///////////////////////////////////////////////////////////////////////////

class ScoreBoard {
public:
  ScoreBoard(const std::string& scoreFile, int numScores = 20);
  ~ScoreBoard();

  int Add(int score, const std::string& name);
  void Show(FILE* f, int rank, int score);

private:
  typedef std::string ScoreEntry;
  typedef std::list<ScoreEntry> Scores;
  Scores scores_;

  int numScores_;
  std::string scoreFile_;
};

#endif // SCOREBOARD_HH_
