#include "ModelFiles.h"

#include "FormatFile.h"

#include "FrameStructure.h"

ModelFiles::ModelFiles() : QFileSystemModel()
{
  // this->setNameFilterDisables(true); // --> inappropriate files are visible but not active
  this->setNameFilterDisables(false);   // --> inappropriate files are not visible
  this->setNameFilters(FrameStructure::listExtensionsFor(IsFormatToLoad()));
}
