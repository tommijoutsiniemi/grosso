//
// Grosso: config.hh
//

// $Id: config.hh,v 1.1.1.1 2002/06/09 10:29:49 tommi Exp $

///////////////////////////////////////////////////////////////////////////

#ifndef CONFIG_HH_
#define CONFIG_HH_

// How to sleep
#define Sleep(ms) napms(ms)

// Where to store high scores
#define SCOREFILE SCOREDIR"/grosso.scores"

// Minimum and maximum, these only for ints
inline int MIN(int a, int b) { return a < b ? a : b; }
inline int MAX(int a, int b) { return a > b ? a : b; }

#endif // CONFIG_HH_
