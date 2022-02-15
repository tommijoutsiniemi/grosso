//
// Grosso: Game.hh
//

// $Id: Game.hh,v 1.1.1.1 2002/06/09 10:29:50 tommi Exp $

///////////////////////////////////////////////////////////////////////////

#ifndef GAME_HH_
#define GAME_HH_

#ifndef MAKEDEPEND
#include <list>
#include <string>
#endif

#include "Fred.hh"
#include "Field.hh"
#include "Grosso.hh"

///////////////////////////////////////////////////////////////////////////

class Game {
public:
  Game(int width, int height, int numfreds, int sleeptime);
  ~Game();

  static int Play(int width, int height, int numFreds, int sleeptime);

private:
  int Play();
  int Pause(const std::string& shape = "");

  bool PlayLevel(int level);
  void InitFrame();
  void FoodOnField();
  int ProcessKeyPress();
  void MoveFreds();
  Grosso::MoveResult MoveGrosso(int turn);

  void Help();
  int ConfirmQuit();
  void EndGame();

  Field* field_;
  Field::ColourNum foodColourNum_;
  Field::ColourNum textColourNum_;
  Grosso* grosso_;
  typedef std::list<Fred> Freds;
  Freds freds_;

  int nGrossoes_;
  int score_;
  int sleepTime_;

  bool running_;
  bool foodOnField_;
  int foodTimer_;
  int numFoods_;
};

#endif // GAME_HH_
