//
// Grosso: main.cc
//

// 2002/06/09 10:29:50 tommi

///////////////////////////////////////////////////////////////////////////

#ifndef MAKEDEPEND
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#endif

#include "config.hh"
#include "ScoreBoard.hh"
#include "Game.hh"
#include "version.hh"

///////////////////////////////////////////////////////////////////////////

static void
usage() {
  fprintf(stderr,
	  "Usage: grosso [-v] [-f<num #'s>] [-s<sleeptime ms>] "
	  "[-h<height>] [-w<width>]\n");
}

///////////////////////////////////////////////////////////////////////////

int
main(int argc, char* argv[])
{
  int i;
  int numfreds = 3;
  int sleeptime = 120;
  bool version = false;
  int width = 0;
  int height = 0;
  
  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
      case 'f':
	numfreds = atoi(argv[i]+2);
	break;
      case 'h':
	height = atoi(argv[i]+2);
	break;
      case 's':
	sleeptime = atoi(argv[i]+2);
	break;
      case 'v':
	version = true;
	break;
      case 'w':
	width = atoi(argv[i]+2);
	break;
      default:
	usage();
	return 1;
      }
    }
    else {
      // all paramters without the leading dash are erroneous
      usage();
      return 1;
    }
  }
  
  if (version) {
    fprintf(stderr, "This is Grosso version " VERSION "\n");
  }
  else {
    srand(time(0));
    int score = Game::Play(width, height, numfreds, sleeptime);
    ScoreBoard sb(SCOREFILE);
    int rank = sb.Add(score, getpwuid(getuid())->pw_name);
    sb.Show(stdout, rank, score);
  }
  
  return 0;
}
