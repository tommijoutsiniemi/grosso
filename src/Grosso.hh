//
// Grosso: Grosso.hh
//

// $Id: Grosso.hh,v 1.1.1.1 2002/06/09 10:29:50 tommi Exp $

///////////////////////////////////////////////////////////////////////////

#ifndef GROSSO_HH_
#define GROSSO_HH_

#include "Worm.hh"

///////////////////////////////////////////////////////////////////////////

class Grosso: public Worm {
public:
  Grosso(Field* field, Field::Item head, Field::ColourNum fg, Field::ColourNum bg);

  MoveResult TryMove(int turn);
};

#endif // GROSSO_HH_
