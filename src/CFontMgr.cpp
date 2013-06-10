#include "CFontMgr.h"

void
CFontMgr::
setPrototype(CFontPtr ptr)
{
  prototype_ = ptr;

  font_list_.clear();
}

CFontPtr
CFontMgr::
lookupFont(const std::string &family, CFontStyle style, double size,
           double angle, double char_angle, uint x_res, uint y_res)
{
  FontList::iterator pfont1 = font_list_.begin();
  FontList::iterator pfont2 = font_list_.end();

  for ( ; pfont1 != pfont2; ++pfont1)
    if ((*pfont1)->getFamily    () == family           &&
        (*pfont1)->getStyle     () == style            &&
        (*pfont1)->getISize     () == uint(size)       &&
        (*pfont1)->getIAngle    () == uint(angle)      &&
        (*pfont1)->getICharAngle() == uint(char_angle) &&
        (*pfont1)->getXRes      () == x_res            &&
        (*pfont1)->getYRes      () == y_res)
      return *pfont1;

  CFontPtr ptr;

  if (prototype_.isValid())
    ptr = prototype_->dup(family, style, size, angle, char_angle, x_res, y_res);
  else {
    std::cerr << "Warning: no font prototype" << std::endl;

    CFont *font =
      new CFont(family, style, size, angle, char_angle, x_res, y_res);

    ptr = CFontPtr(font);
  }

  addFont(ptr);

  return ptr;
}

CFontPtr
CFontMgr::
lookupFont(const std::string &full_name)
{
  FontList::iterator pfont1 = font_list_.begin();
  FontList::iterator pfont2 = font_list_.end();

  for ( ; pfont1 != pfont2; ++pfont1)
    if ((*pfont1)->getXFontName() == full_name)
      return *pfont1;

  CFontPtr ptr;

  if (prototype_.isValid()) {
    ptr = prototype_->dup(full_name);

    addFont(ptr);
  }

  return ptr;
}

bool
CFontMgr::
addFont(CFontPtr font)
{
  if (debug_)
    std::cerr << "Add Font: " << *font << std::endl;

  font_list_.push_back(font);

  return true;
}

bool
CFontMgr::
deleteFont(CFontPtr font)
{
  if (debug_)
    std::cerr << "Remove Font: " << *font << std::endl;

  font_list_.remove(font);

  return true;
}
