#ifndef __Critical_Point_h__
#define __Critical_Point_h__

#include <complex>

typedef std::complex<short int> CriticalPointType;

static const CriticalPointType::value_type FullRank(3);

enum {FullRankTypeMinimum = +3, FullRankTypeSaddleR = +1, FullRankTypeSaddleB = -1, FullRankTypeMaximum = -3};

// signature of a Cage Critical Point (CCP, local minimum)
static const CriticalPointType cpTypeMinimum(FullRank, FullRankTypeMinimum);

// signature of a Ring Critical Point (RCP, saddle)
static const CriticalPointType cpTypeSaddleR(FullRank, FullRankTypeSaddleR);

// signature of a Bond Critical Point (BCP, reversed saddle point)
static const CriticalPointType cpTypeSaddleB(FullRank, FullRankTypeSaddleB);

// signature of a Atom Critical Point (ACP, local maximum)
static const CriticalPointType cpTypeMaximum(FullRank, FullRankTypeMaximum);

#endif // !__Critical_Point_h__
