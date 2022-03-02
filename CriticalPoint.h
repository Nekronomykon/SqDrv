#ifndef __Critical_Point_h__
#define __Critical_Point_h__

#include <complex>

typedef std::complex<short int> CriticalPointType;

static const CriticalPointType cptypeMininum(3,+3); // signature of a Cage Critical Point (CCP, local minimum)
static const CriticalPointType cptypeSaddleR(3,+1); // signature of a Ring Critical Point (RCP)
static const CriticalPointType cptypeSaddleP(3,-1); // signature of a Bond Critical Point (BCP)
static const CriticalPointType cptypeMaximum(3,-3); // signature of a Atom Critical Point (ACP, local maximum)

#endif // !__Critical_Point_h__
