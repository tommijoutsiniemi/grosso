//
// Grosso: Game.cc
//

// 2004/08/06 12:25:25 tommi

///////////////////////////////////////////////////////////////////////////

#ifndef MAKEDEPEND
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#endif

#include "config.hh"
#include "Game.hh"

using namespace std;

///////////////////////////////////////////////////////////////////////////

Game::Game(int width, int height, int numFreds, int sleepTime)
{
  // make sure installed correctly
  if (access(LEVELDIR, R_OK) != 0) {
    //cerr << "Can't access Grosso level directory \"" LEVELDIR "\"" << endl;
    fprintf(stderr, "Can't access Grosso level directory \"" LEVELDIR "\"\n");
    sleepTime_ = -1;
    return;
  }

  // initialize field and grosso
  field_ = new Field(width, height, COLOR_BLUE, COLOR_WHITE);
  foodColourNum_ = field_->NewColour(COLOR_CYAN, COLOR_WHITE);
  textColourNum_ = field_->NewColour(COLOR_BLACK, COLOR_WHITE);
  grosso_ = new Grosso(field_, 'O', COLOR_BLACK, COLOR_WHITE);

  // create freds
  for (int i = 0; i < numFreds; i++) {
    Fred f(field_, '#', COLOR_RED, COLOR_WHITE, 3);
    freds_.push_back(f);
  }

  sleepTime_ = sleepTime;
  foodTimer_ = -999;
}

///////////////////////////////////////////////////////////////////////////

Game::~Game()
{
  delete grosso_;
  delete field_;
}

///////////////////////////////////////////////////////////////////////////

int
Game::Pause(const string& shape)
{
  int i = 0;
  int key;

  field_->Flush();
  field_->Refresh();
  do {
    // cycle grosso's head shape
    if (shape.size() > 1) {
      grosso_->ShowHead(shape[i % shape.size()]);
      field_->Refresh();
    }

    key = field_->Key();
    if (key == '?' || key == 'h') {
      Help();
      key = ERR;
    }
    else if (key == 'q') {
      key = ConfirmQuit();
      if (key == 'y') {
	EndGame();
      }
      else {
	key = ERR;
      }
    }
    else {
      Sleep(sleepTime_ / ((shape.size() > 1) ? shape.size() : 1));
    }
    i++;
  } while (key == ERR || (shape.size() > 1 && i < 42/2));

  return key;
}

///////////////////////////////////////////////////////////////////////////

int
Game::Play(int width, int height, int numFreds, int sleepTime)
{
  int score;
  Game game(width, height, numFreds, sleepTime);

  if (game.sleepTime_ != -1) {
    score = game.Play();
  }
  else {
    score = -1;
  }

  return score;
}

///////////////////////////////////////////////////////////////////////////

int
Game::Play()
{
  score_ = 0;
  int level = 1;
  for(nGrossoes_ = 3; nGrossoes_ > 0; /* Empty */) {
    if (PlayLevel(level)) {
      level++;
    }
    else {
      nGrossoes_--;
    }
  }

  return score_;
}

///////////////////////////////////////////////////////////////////////////

bool
Game::PlayLevel(int level)
{
  Freds::iterator fi;
  Field::Dim x, y;

  foodOnField_ = false;
  numFoods_ = 10;
  field_->Level(level);
  grosso_->Reset(0, 0, 2, 10);
  InitFrame();
  for (fi = freds_.begin(); fi != freds_.end(); fi++) {
    do {
      x = rand()%field_->Width();
      y = rand()%field_->Height();
    } while ((x < 5 && y < 5) || field_->GetItem(x, y) != Field::EMPTY);
    fi->Reset(x, y, rand(), 10);
  }
  field_->Refresh();

  running_ = true;
  Pause();
  while (running_) {
    Sleep(sleepTime_);
    FoodOnField();
    MoveFreds();
    if (MoveGrosso(ProcessKeyPress()) == Grosso::ATE) {
      numFoods_--;
      if (numFoods_ == 0) {
        InitFrame();
        running_ = false;
        Pause("Oo.o");
      }
    }
    InitFrame();
  }

  return numFoods_ == 0;
}

///////////////////////////////////////////////////////////////////////////

void
Game::InitFrame()
{
  char s[16];

  if (foodTimer_ > 0) {
    foodTimer_--;
  }

  sprintf(s, "F%02d-%06d-G%02d", numFoods_, score_, nGrossoes_);
  field_->Puts((field_->Width()-strlen(s))/2, 0, s);

  field_->Refresh();
}

///////////////////////////////////////////////////////////////////////////

void
Game::FoodOnField()
{
  Field::Dim x, y;

  // if not already dropped, try to drop food into the field
  if (!foodOnField_) {
    if (rand()%100 < 10) {
      x = rand()%field_->Width();
      y = rand()%field_->Height();
      if (field_->GetItem(x, y) == Field::EMPTY) {
        field_->PutItem(x, y, Worm::FOOD, foodColourNum_);
        foodOnField_ = true;
        foodTimer_ = field_->Width() + field_->Height();
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////

int
Game::ProcessKeyPress()
{
  int key;
  int turn = Grosso::FORWARD;

  switch(key = field_->Key()) {
  case 'q':
  case ERR:
    break;
  case 'v':
  case '4':
    turn = Grosso::LEFT;
    break;
  case 'n':
  case '6':
    turn = Grosso::RIGHT;
    break;
  case '?':
  case 'h':
    Help();
    key = Pause();
    break;
  default:
    key = Pause();
    break;
  }
  if (key == 'q') {
    if (ConfirmQuit() == 'y') {
      EndGame();
    }
    else {
      Pause();
    }
  }

  return turn;
}

///////////////////////////////////////////////////////////////////////////

Grosso::MoveResult
Game::MoveGrosso(int turn)
{
  Grosso::MoveResult result;

  switch (result = grosso_->TryMove(turn)) {
  case Worm::ATE:
    score_ += 10 + foodTimer_;
    foodOnField_ = false;
    break;
  case Worm::CRASHED:
    running_ = false;
    Pause("\\|/-");
  default:
    break;
  }

  return result;
}

///////////////////////////////////////////////////////////////////////////

void
Game::MoveFreds()
{
  Freds::iterator fi;

  for (fi = freds_.begin(); fi != freds_.end(); fi++) {
    switch (fi->TryMove()) {
    case Worm::ATE:
      score_ += 10 + foodTimer_;
      foodOnField_ = false;
      break;
    case Worm::GOT_BLOCKED:
      score_++;
      break;
    case Worm::DIED:
      score_ += 100;;
      break;
    default:
      break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////

void
Game::EndGame()
{
  running_ = false;
  nGrossoes_ = 0;
}

///////////////////////////////////////////////////////////////////////////

int
Game::ConfirmQuit()
{
  return field_->Display
    ("Are you sure you want to quit Grosso?"
     " \n \n"
     "            Press y or n.",
     textColourNum_, 37, 3);
}

///////////////////////////////////////////////////////////////////////////

void
Game::Help()
{
  field_->Display
    ("    *** This is Grosso, a simple character terminal game ***\n"
     " \n"
     "  You control the O-headed worm. Don't crash into anything,\n"
     "  eat all *'s, and you are on the next level! Be careful with and\n"
     "  exploit the wraparound. See the man page for more info.\n"
     " \n"
     "  Keys: v = turn left\n"
     "        n = turn right\n"
     "    space = pause\n"
     "        h = this help\n"
     "        q = quit\n"
     " \n"
     "  This program is PostcardWare: As you have come as far as reading\n"
     "  this message, you might as well send a picture postcard to: *B->\n"
     " \n"
     "    Tommi Joutsiniemi\n"
     "    Lukinkatu 2d\n"
     "    33580 Tampere\n"
     "    Finland\n"
     " \n"
     "  Press any key to resume game.\n",
     textColourNum_, 66, 21);
}
