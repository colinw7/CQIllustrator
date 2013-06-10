#ifndef CQRADIO_BUTTONS_H
#define CQRADIO_BUTTONS_H

#include <vector>
#include <QWidget>

class QRadioButton;
class QLayout;

class CQRadioButtons : public QWidget {
  Q_OBJECT

 public:
  CQRadioButtons(QWidget *parent=NULL);

  virtual ~CQRadioButtons();

  void addButton(const QString &name);

  int     getCurrentValue() const;
  QString getCurrentName () const;

 protected:
  void addButtons(const std::vector<QString> &names);

  virtual QLayout *createLayout(QWidget *parent);

  virtual void addWidget(QWidget *widget);

 protected slots:
  void valueChangedI(bool value);

 signals:
  void valueChanged(int id);
  void valueChanged(const QString &d);

 protected:
  bool                        hasLayout_;
  std::vector<QRadioButton *> buttons_;
};

class CQHRadioButtons : public CQRadioButtons {
 public:
  CQHRadioButtons(QWidget *parent, const char *name, ...);
  CQHRadioButtons(QWidget *parent=NULL);

 protected:
  QLayout *createLayout(QWidget *parent);

  void addWidget(QWidget *widget);
};

class CQVRadioButtons : public CQRadioButtons {
 public:
  CQVRadioButtons(QWidget *parent, const char *name, ...);
  CQVRadioButtons(QWidget *parent=NULL);

 protected:
  QLayout *createLayout(QWidget *parent);

  void addWidget(QWidget *widget);
};

class CQToolbarRadioButtons : public CQRadioButtons {
 public:
  CQToolbarRadioButtons(QWidget *parent, const char *title, const char *name, ...);
  CQToolbarRadioButtons(QWidget *parent, const char *title);

 protected:
  QLayout *createLayout(QWidget *parent);

  void addWidget(QWidget *widget);

 protected:
  QString title_;
};

#endif
