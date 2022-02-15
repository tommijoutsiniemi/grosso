//
// Grosso: Fred.hh
//

// $Id: Fred.hh,v 1.1.1.1 2002/06/09 10:29:50 tommi Exp $

///////////////////////////////////////////////////////////////////////////

#ifndef FRED_HH_
#define FRED_HH_

#include "Worm.hh"

///////////////////////////////////////////////////////////////////////////

class Fred: public Worm {
public:
  Fred(Field* field, Field::Item head, Field::ColourNum fg, Field::ColourNum bg, int probab);
  Fred() { };

  void Reset(Field::Dim headx, Field::Dim heady, int dir, int length);

  MoveResult TryMove(int turn = FORWARD);

private:
  MoveResult TryTurn();

  int probab_;			// probability for turning, o/oo
  int strength_;		// how long able to be BLOCKED
  bool dead_;
};

#endif // FRED_HH_
