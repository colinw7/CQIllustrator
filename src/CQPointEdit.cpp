#include <CQPointEdit.h>
#include <QDoubleValidator>

#include <CStrUtil.h>
#include <CStrParse.h>

class CQPointEditValidator : public QValidator {
 public:
  CQPointEditValidator(CQPointEdit *edit) :
   QValidator(edit), edit_(edit) {
  }

  State validate(QString &input, int &) const {
    CPoint2D point;

    return edit_->stringToPoint(input.toStdString(), point);
  }

 private:
  CQPointEdit *edit_;
};

//------

CQPointEdit::
CQPointEdit(QWidget *parent, const CPoint2D &value) :
 QLineEdit(parent)
{
  init("edit", value);
}

CQPointEdit::
CQPointEdit(const CPoint2D &value)
{
  init("edit", value);
}

void
CQPointEdit::
init(const QString &name, const CPoint2D &value)
{
  setObjectName(name);

  CQPointEditValidator *validator = new CQPointEditValidator(this);

  setValidator(validator);

  connect(this, SIGNAL(editingFinished()), this, SLOT(editingFinishedI()));

  connect(this, SIGNAL(returnPressed()), this, SLOT(returnPressedI()));

  setValue(value);
}

void
CQPointEdit::
setValue(const CPoint2D &point)
{
  point_ = point;

  pointToWidget();
}

const CPoint2D &
CQPointEdit::
getValue() const
{
  return point_;
}

void
CQPointEdit::
editingFinishedI()
{
  if (widgetToPoint())
    emit valueChanged();
}

void
CQPointEdit::
returnPressedI()
{
  if (widgetToPoint()) {
    emit valueChanged();

    emit valueAccepted();
  }
}

void
CQPointEdit::
pointToWidget()
{
  setText(pointToString(point_).c_str());
}

std::string
CQPointEdit::
pointToString(const CPoint2D &point)
{
  return CStrUtil::strprintf("%g %g", point.getX(), point.getY());
}

bool
CQPointEdit::
widgetToPoint()
{
  CPoint2D point;

  QValidator::State state = stringToPoint(text().toStdString(), point);

  if (state != QValidator::Acceptable)
    return false;

  if (point == point_)
    return false;

  point_ = point;

  return true;
}

QValidator::State
CQPointEdit::
stringToPoint(const std::string &text, CPoint2D &point)
{
  static const char lchars[] = "({[<";
  static const char rchars[] = ")]>";
  static const char mchars[] = ",:;";

  double x, y;

  CStrParse parse(text);

  parse.skipSpace();

  if (parse.eof()) return QValidator::Intermediate;

  char c = parse.getCharAt();

  if (strchr(lchars, c) != NULL) {
    parse.skipChar();

    parse.skipSpace();

    if (parse.eof()) return QValidator::Intermediate;
  }

  std::string str;

  if (! parse.readReal(&x)) {
    char c = parse.getCharAt();

    if (c == '-' || c == '+') {
      parse.skipChar();

      c = parse.getCharAt();
    }

    if (parse.eof()) return QValidator::Intermediate;

    if (! isdigit(c))
      return QValidator::Invalid;

    if (! parse.readReal(&x))
      return QValidator::Intermediate;
  }

  parse.skipSpace();

  if (parse.eof()) return QValidator::Intermediate;

  c = parse.getCharAt();

  if (strchr(mchars, c) != NULL) {
    parse.skipChar();

    parse.skipSpace();

    if (parse.eof()) return QValidator::Intermediate;
  }

  if (! parse.readReal(&y)) {
    char c = parse.getCharAt();

    if (c == '-' || c == '+') {
      parse.skipChar();

      c = parse.getCharAt();
    }

    if (parse.eof()) return QValidator::Intermediate;

    if (! isdigit(c))
      return QValidator::Invalid;

    if (! parse.readReal(&y))
      return QValidator::Intermediate;
  }

  parse.skipSpace();

  if (! parse.eof()) {
    c = parse.getCharAt();

    if (strchr(rchars, c) != NULL) {
      parse.skipChar();

      parse.skipSpace();
    }
  }

  if (! parse.eof()) return QValidator::Invalid;

  point = CPoint2D(x, y);

  return QValidator::Acceptable;
}
