#ifndef __Cursor_Helper_h__
#define __Cursor_Helper_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

/*=========================================================================

  Program:   Visualization Toolkit Local Fork Copy
  Module:    ResetCursor.h
  Copyright (c) ScrewDriver te Blackheadborough
  All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <QCursor>

class ResetCursor
{
public:
  explicit ResetCursor(const QCursor &);
  explicit ResetCursor(Qt::CursorShape shape = Qt::WaitCursor);
  ~ResetCursor();

private:
  ResetCursor(const ResetCursor &) = delete;
};

#endif // !__Cursor_Helper_h__
