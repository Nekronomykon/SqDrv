#include "ResetCursor.h"

#include <QApplication>

ResetCursor::ResetCursor(const QCursor& crs)
{ QApplication::setOverrideCursor(crs); }

ResetCursor::~ResetCursor()
{ QApplication::restoreOverrideCursor(); }
