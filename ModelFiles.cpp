#include "ModelFiles.h"

#include "FormatFile.h"

#include "FrameStructure.h"

ModelFiles::ModelFiles() : QFileSystemModel()
{
  this->setNameFilterDisables(true); // false? --> inappropriate files are visible but not active
  // this->setNameFilterDisables(false); // true? --> inappropriate files are not visible 
  this->setNameFilters(FrameStructure::listExtensionsFor(IsFormatToLoad()));
}