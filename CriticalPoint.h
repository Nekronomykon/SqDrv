#ifndef __Critical_Point_h__
#define __Critical_Point_h__

#include <complex>

typedef std::complex<short int> CriticalPointType;

static const CriticalPointType::value_type FullRank(3);

static const CriticalPointType cpTypeMininum(3,+3); // signature of a Cage Critical Point (CCP, local minimum)
static const CriticalPointType cpTypeSaddleR(3,+1); // signature of a Ring Critical Point (RCP, saddle)
static const CriticalPointType cpTypeSaddleB(3,-1); // signature of a Bond Critical Point (BCP, saddle point)
static const CriticalPointType cpTypeMaximum(3,-3); // signature of a Atom Critical Point (ACP, local maximum)

#endif // !__Critical_Point_h__
