#include <CQSwatch.h>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>

CQSwatch::
CQSwatch() :
 QWidget(NULL)
{
  init();
}

CQSwatch::
CQSwatch(QWidget *widget) :
 QWidget(NULL)
{
  init();

  addWidget(widget);
  addSeparator();
}

CQSwatch::
CQSwatch(const char *title, QWidget *w) :
 QWidget(NULL)
{
  init();

  setTitle(title);

  addWidget(w);
  addSeparator();
}

CQSwatch::
CQSwatch(const char *title, QWidget *w1, QWidget *w2) :
 QWidget(NULL)
{
  init();

  setTitle(title);

  addWidget(w1);
  addWidget(w2);
  addSeparator();
}

CQSwatch::
CQSwatch(const char *title, QWidget *w1, QWidget *w2, QWidget *w3) :
 QWidget(NULL)
{
  init();

  setTitle(title);

  addWidget(w1);
  addWidget(w2);
  addWidget(w3);
  addSeparator();
}

CQSwatch::
CQSwatch(const char *title, QWidget *w1, QWidget *w2, QWidget *w3, QWidget *w4) :
 QWidget(NULL)
{
  init();

  setTitle(title);

  addWidget(w1);
  addWidget(w2);
  addWidget(w3);
  addWidget(w4);
  addSeparator();
}

void
CQSwatch::
init()
{
  pos_ = 0;

  has_title_ = false;

  grid_ = new QGridLayout(this);

  grid_->setMargin(0); grid_->setSpacing(1);
}

void
CQSwatch::
setTitle(const char *title)
{
  int rows = 1;
  int cols = 1;

  QString title1 = title;

  int len = title1.length();

  if (len > 3 && title1[len - 3] == '<' && title1[len - 2].isDigit() && title1[len - 1] == '>') {
    cols = title1[len - 2].toLatin1() - '0';

    title1 = title1.mid(0, len - 3);
  }

  QLabel *label = new QLabel("<small><b>" + title1 + "</b></small>");

  grid_->addWidget(label, 0, 0, rows, cols);

  has_title_ = true;
}

void
CQSwatch::
addWidget(QWidget *w)
{
  int row = (has_title_ ? 1 : 0);

  grid_->addWidget(w, row, pos_++);
}

void
CQSwatch::
addWidget(QWidget *w, int rows, int cols)
{
  int row = (has_title_ ? 1 : 0);

  grid_->addWidget(w, row, pos_++, rows, cols);
}

void
CQSwatch::
addSeparator()
{
  QFrame *frame = new QFrame;

  frame->setFrameShadow(QFrame::Raised);
  frame->setFrameShape (QFrame::VLine);

  grid_->addWidget(frame, 0, pos_++, 2, 1);
}
