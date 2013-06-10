#ifndef CQWINDOW_H
#define CQWINDOW_H

#include <QWidget>

#include <CWindow.h>

class CQWindowFactory : public CWindowFactory {
 public:
  CWindow *createWindow(int x, int y, uint width, uint height);

  CWindow *createWindow(CWindow *parent, int x, int y, uint width, uint height);
};

class CQWindow : public QWidget, public CWindow {
  Q_OBJECT

 private:
  bool pressed_;

 public:
  static void setFactory();

  CQWindow(CQWindow *parent=NULL);
  CQWindow(QWidget *parent);

 ~CQWindow();

  // get current position and size
  void getPosition(int *x, int *y) const;
  void getSize(uint *w, uint *h) const;

  void getScreenSize(uint *w, uint *h) const;

  // destrow window
  void destroy();

  // move window
  void move(int x, int y);

  // resize window
  void resize(uint width, uint height);

  // map/unmap window
  void map  ();
  void unmap();

  // get if window is mapped
  bool isMapped();

  // iconize/deiconize window
  void iconize  ();
  void deiconize();

  // maximize/demaximize window
  void maximize  ();
  void demaximize();

  // lower/raise window
  void lower();
  void raise();

  // set window/icon title
  void setWindowTitle(const std::string &title);
  void setIconTitle(const std::string &);

  // get window/icon title
  void getWindowTitle(std::string &title) const;
  void getIconTitle(std::string &title) const;

  // signal expose event
  void expose();

  // set event adapter
  void setEventAdapter(CEventAdapter *eventAdpater);

  // property
  bool setProperty(const std::string &name, const std::string &value);

  // selection
  bool setSelectText(const std::string &text);

  // draw window contents
  void redraw();

  void beep();

  //------

  // hints

  void setMinSize(int width, int height);
  void setBaseSize(int width, int height);
  void setResizeInc(int width, int height);

  //------

  virtual void mousePressEvent(QMouseEvent *);
  virtual void mouseReleaseEvent(QMouseEvent *);
  virtual void mouseDoubleClickEvent(QMouseEvent *);
  virtual void mouseMoveEvent(QMouseEvent *);

  virtual void keyPressEvent(QKeyEvent *);
  virtual bool keyPressEvent(const CKeyEvent &);
  virtual void keyReleaseEvent(QKeyEvent *);
  virtual bool keyReleaseEvent(const CKeyEvent &);

  virtual void paintEvent(QPaintEvent *);

  virtual void resizeEvent(QResizeEvent *);
  virtual bool resizeEvent();

  virtual void hideEvent(QHideEvent *);
  virtual bool hideEvent();
  virtual void showEvent(QShowEvent *);
  virtual bool showEvent();

  virtual void enterEvent(QEvent *);
  virtual bool enterEvent();
  virtual void leaveEvent(QEvent *);
  virtual bool leaveEvent();
};

#endif
