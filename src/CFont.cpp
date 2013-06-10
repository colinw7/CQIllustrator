#include <CFont.h>
#include <CFontMgr.h>

#include <CStrUtil.h>
#include <CMathGen.h>
#include <cmath>

CFontFamily::
CFontFamily(const std::string &name) :
 name_(name)
{
}

CFontFamily::
~CFontFamily()
{
}

CFontFamily::
CFontFamily(const CFontFamily &family) :
 name_(family.name_), font_defs_(family.font_defs_)
{
}

const CFontFamily &
CFontFamily::
operator=(const CFontFamily &family)
{
  name_      = family.name_;
  font_defs_ = family.font_defs_;

  return *this;
}

CFontFamily &
CFontFamily::
lookup(const std::string &name)
{
  static FamilyMap families_;

  FamilyMap::iterator p1 = families_.find(name);
  FamilyMap::iterator p2 = families_.end ();

  if (p1 != p2)
    return *(p1->second);

  CFontFamily *family = new CFontFamily(name);

  families_[name] = family;

  return *family;
}

CFontDef &
CFontFamily::
lookupFontDef(CFontStyle style, uint size, uint angle)
{
  CFontDefKey font_def(style, size, angle);

  FontDefMap::iterator p1 = font_defs_.find(font_def);
  FontDefMap::iterator p2 = font_defs_.end ();

  if (p1 != p2)
    return *(p1->second);

  CFontDef *font = new CFontDef(style, size, angle);

  font_defs_[font_def] = font;

  return *font;
}

//-------------------

CFont::
CFont(const std::string &family, CFontStyle style, double size, double angle, double char_angle,
      int x_res, int y_res) :
 family_(family), font_def_(style, size, angle, x_res, y_res), char_angle_(char_angle)
{
  id_ = getNextId();
}

CFont::
CFont(const std::string &full_name) :
 x_font_name_(full_name)
{
  uint isize;

  decodeXFontName(full_name, family_, font_def_.style, isize, font_def_.x_res, font_def_.y_res);

  font_def_.size  = isize;
  font_def_.angle = 0;

  id_ = getNextId();
}

CFont::
CFont(const CFont &font) :
 family_(font.family_), font_def_(font.font_def_), char_angle_(font.char_angle_),
 x_font_name_(font.x_font_name_)
{
  id_ = getNextId();
}

CFont &
CFont::
operator=(const CFont &font)
{
  family_      = font.family_;
  font_def_    = font.font_def_;
  char_angle_  = font.char_angle_;
  x_font_name_ = font.x_font_name_;
  id_          = font.id_;

  return *this;
}

CFontPtr
CFont::
dup() const
{
  return dup(getFamily(), getStyle(), getSize(), getAngle(), getCharAngle(), getXRes(), getYRes());
}

CFontPtr
CFont::
dup(const std::string &full_name) const
{
  CFontStyle  style;
  std::string family;
  uint        size, x_res, y_res;

  decodeXFontName(full_name, family, style, size, x_res, y_res);

  return dup(family, style, size, 0, 0, x_res, y_res);
}

CFontPtr
CFont::
dup(const std::string &family, CFontStyle style, double size, double angle, double char_angle,
    int x_res, int y_res) const
{
  CFont *font = new CFont(family, style, size, angle, char_angle, x_res, y_res);

  return CFontPtr(font);
}

CFontPtr
CFont::
rotated(double dangle) const
{
  double angle = getAngle() + dangle;

  while (angle <  0  ) angle += 360;
  while (angle >= 360) angle -= 360;

  return dup(getFamily(), getStyle(), getSize(), angle, getCharAngle(), getXRes(), getYRes());
}

void
CFont::
getStringBBox(const std::string &str, double x[4], double y[4]) const
{
  double w = getStringWidth(str);
  double a = getCharAscent ();
  double d = getCharDescent();

  double angle = getAngle();

  double c = cos(DEG_TO_RAD(angle));
  double s = sin(DEG_TO_RAD(angle));

  x[0] =     + d*s; y[0] =     - d*c;
  x[1] = w*c + d*s; y[1] = w*s - d*c;
  x[2] = w*c - a*s; y[2] = w*s + a*c;
  x[3] =     - a*s; y[3] =     + a*c;
}

void
CFont::
getIStringBBox(const std::string &str, int x[4], int y[4]) const
{
  double rx[4], ry[4];

  getStringBBox(str, rx, ry);

  for (uint i = 0; i < 4; ++i) {
    x[i] = CMathGen::Round(rx[i]);
    y[i] = CMathGen::Round(ry[i]);
  }
}

std::string
CFont::
getXFontName() const
{
  if (x_font_name_.size() == 0) {
    CFont *th = const_cast<CFont *>(this);

    th->x_font_name_ = buildXFontName();
  }

  return x_font_name_;
}

bool
CFont::
decodeXFontName(const std::string &x_font_name, std::string &family, CFontStyle &style,
                uint &size, uint &x_res, uint &y_res)
{
  std::vector<std::string> fields;

  CStrUtil::addFields(x_font_name, fields, "-");

  uint num_fields = fields.size();

  if (num_fields != 15)
    return false;

  family = fields[2];

  std::string weight = fields[3];
  std::string slant  = fields[4];

  if (! CStrUtil::toInteger(fields[7], &size))
    size = 12;

  if (! CStrUtil::toInteger(fields[9], &x_res))
    x_res = 100;

  if (! CStrUtil::toInteger(fields[10], &y_res))
    y_res = 100;

  std::string reg = fields[13];

  if (CStrUtil::casecmp(reg, "*") != 0 &&
      CStrUtil::casecmp(reg, "iso8859") != 0)
    return false;

  style = CFONT_STYLE_NORMAL;

  if (weight == "" || slant == "")
    return false;

  if      (CStrUtil::casecmp(weight, "medium" ) == 0 ||
           CStrUtil::casecmp(weight, "regular") == 0 ||
           CStrUtil::casecmp(weight, "book"   ) == 0 ||
           CStrUtil::casecmp(weight, "normal" ) == 0 ||
           CStrUtil::casecmp(weight, "roman"  ) == 0 ||
           CStrUtil::casecmp(weight, "light"  ) == 0)
    ;
  else if (CStrUtil::casecmp(weight, "bold"    ) == 0 ||
           CStrUtil::casecmp(weight, "black"   ) == 0 ||
           CStrUtil::casecmp(weight, "demi"    ) == 0 ||
           CStrUtil::casecmp(weight, "demibold") == 0)
    style = CFONT_STYLE_BOLD;
  else {
    std::cerr << "Unmatched weight " << CStrUtil::single_quote(weight) <<
                 " for " << CStrUtil::single_quote(x_font_name) << std::endl;
    return false;
  }

  if      (CStrUtil::casecmp(slant, "r") == 0)
    ;
  else if (CStrUtil::casecmp(slant, "i") == 0 ||
           CStrUtil::casecmp(slant, "o") == 0) {
    if (style == CFONT_STYLE_BOLD)
      style = CFONT_STYLE_BOLD_ITALIC;
    else
      style = CFONT_STYLE_ITALIC;
  }
  else  {
    std::cerr << "Unmatched slant " << CStrUtil::single_quote(slant) <<
                 " for " << CStrUtil::single_quote(x_font_name) << std::endl;
    return false;
  }

  return true;
}

std::string
CFont::
buildXFontName() const
{
  return encodeXFontName(family_, getStyle(), getISize());
}

std::string
CFont::
encodeXFontName(const std::string &family, CFontStyle style, uint size)
{
  std::string style_string;
  std::string slant_string;

  if (style & CFONT_STYLE_BOLD)
    style_string = "bold";
  else
    style_string = "medium";

  // TODO: config or registration of slant name
  if (style & CFONT_STYLE_ITALIC) {
    if      (CStrUtil::casecmp(family, "helvetica") == 0)
      slant_string = "o";
    else if (CStrUtil::casecmp(family, "courier") == 0)
      slant_string = "o";
    else
      slant_string = "i";
  }
  else
    slant_string = "r";

  std::string font_name =
    "-*-" + family + "-" + style_string + "-" + slant_string +
    "-normal-*-" + CStrUtil::toString(size) + "-*-*-*-*-*-*-*";

  return font_name;
}

std::string
CFont::
encodeFontName()
{
  return encodeFontName(family_, getStyle(), getISize());
}

std::string
CFont::
encodeFontName(const std::string &family, CFontStyle style, uint size)
{
  return family + "-" + CFont::fontStyleToString(style) + "-" + CStrUtil::toString(size);
}

bool
CFont::
decodeFontName(const std::string &name, std::string &family, CFontStyle &style, uint &size)
{
  family = "courier";
  style  = CFONT_STYLE_NORMAL;
  size   = 12;

  std::vector<std::string> words;

  CStrUtil::addFields(name, words, "-");

  uint num_words = words.size();

  if (num_words != 3)
    return false;

  family = words[0];
  style  = CFont::stringToFontStyle(words[1]);
  size   = CStrUtil::toInteger(words[2]);

  if (size <= 3) {
    size = 10;

    return false;
  }

  return true;
}

CFontStyle
CFont::
stringToFontStyle(const std::string &name)
{
  if      (CStrUtil::casecmp(name, "normal") == 0)
    return CFONT_STYLE_NORMAL;
  else if (CStrUtil::casecmp(name, "bold") == 0)
    return CFONT_STYLE_BOLD;
  else if (CStrUtil::casecmp(name, "italic") == 0)
    return CFONT_STYLE_ITALIC;
  else if (CStrUtil::casecmp(name, "bolditalic" ) == 0 ||
           CStrUtil::casecmp(name, "bold-italic") == 0)
    return CFONT_STYLE_BOLD_ITALIC;
  else {
    std::cerr << "Invalid Font Style : " << name << std::endl;
    return CFONT_STYLE_NORMAL;
  }
}

std::string
CFont::
fontStyleToString(CFontStyle style)
{
  if      (style == CFONT_STYLE_NORMAL)
    return "normal";
  else if (style == CFONT_STYLE_BOLD)
    return "bold";
  else if (style == CFONT_STYLE_ITALIC)
    return "italic";
  else if (style == CFONT_STYLE_BOLD_ITALIC)
    return "bold-italic";
  else
    return "normal";
}

//---------

CFontSet::
CFontSet(const std::string &family, uint size)
{
  setup(family, size);
}

void
CFontSet::
setup(const std::string &family, uint size)
{
  family_ = family;
  size_   = size;

  normal_ = CFontMgrInst->lookupFont(family_, CFONT_STYLE_NORMAL     , size_);
  bold_   = CFontMgrInst->lookupFont(family_, CFONT_STYLE_BOLD       , size_);
  italic_ = CFontMgrInst->lookupFont(family_, CFONT_STYLE_ITALIC     , size_);
  boldi_  = CFontMgrInst->lookupFont(family_, CFONT_STYLE_BOLD_ITALIC, size_);
}

CFontPtr
CFontSet::
getFont(CFontStyle style) const
{
  if      (style == CFONT_STYLE_NORMAL)
    return normal_;
  else if (style == CFONT_STYLE_BOLD)
    return bold_;
  else if (style == CFONT_STYLE_ITALIC)
    return italic_;
  else if (style == CFONT_STYLE_BOLD_ITALIC)
    return boldi_;
  else
    return normal_;
}
