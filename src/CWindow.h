#ifndef CWINDOW_H
#define CWINDOW_H

// CWindow:
//  Class to abstract the behavior of a window (usually a toplevel window).
//  Implementations in X (CXLib/CXWindow), Qt (CQWindow) and GL (CGLApp/CGLWindow)
//
//  Used in CTerm code to create the window to display the terminal 'text' window
//  and handle the terminal window manipulation requests (move, resize, set title, ...)

#include <CEvent.h>

enum CWindowType {
  CWINDOW_TYPE_NORMAL,
  CWINDOW_TYPE_OVERRIDE
};

class CWindow;

#define CWindowMgrInst CWindowMgr::getInstance()

class CWindowFactory {
 public:
  CWindowFactory() { }

  virtual ~CWindowFactory() { }

  virtual CWindow *createWindow(int x, int y, uint width, uint height) = 0;
  virtual CWindow *createWindow(CWindow *parent, int x, int y, uint width, uint height) = 0;
};

class CWindowMgr {
 private:
  CWindowFactory *factory_;

 protected:
  CWindowMgr();
 ~CWindowMgr();

 public:
  static CWindowMgr *getInstance();

  void setFactory(CWindowFactory *factory) { factory_ = factory; }

  CWindow *createWindow(int x, int y, uint width, uint height);
  CWindow *createWindow(CWindow *parent, int x, int y, uint width, uint height);

 private:
  CWindowMgr(const CWindowMgr &rhs);
  CWindowMgr &operator=(const CWindowMgr &rhs);
};

class CWindow {
 protected:
  CWindow *parent_;

 public:
  CWindow(CWindow *parent=NULL);

  virtual ~CWindow();

  // get current position and size
  virtual void getPosition(int *x, int *y) const = 0;
  virtual void getSize(uint *w, uint *h) const = 0;

  virtual void getScreenSize(uint *w, uint *h) const = 0;

  // destrow window
  virtual void destroy() = 0;

  // move window
  virtual void move(int x, int y) = 0;

  // resize window
  virtual void resize(uint width, uint height) = 0;

  // map/unmap window
  virtual void map  () = 0;
  virtual void unmap() = 0;

  // get if window is mapped
  virtual bool isMapped() = 0;

  // iconize/deiconize window
  virtual void iconize  () = 0;
  virtual void deiconize() = 0;

  // maximize/demaximize window
  virtual void maximize  () = 0;
  virtual void demaximize() = 0;

  // lower/raise window
  virtual void lower() = 0;
  virtual void raise() = 0;

  // set window/icon title
  virtual void setWindowTitle(const std::string &title) = 0;
  virtual void setIconTitle(const std::string &) { }

  // get window/icon title
  virtual void getWindowTitle(std::string &title) const = 0;
  virtual void getIconTitle(std::string &title) const { title = ""; }

  // signal expose event
  virtual void expose() = 0;

  // set event adapter
  virtual void setEventAdapter(CEventAdapter *eventAdpater) = 0;

  // event notifications ...

  // button press, motion, release
  virtual bool buttonPressEvent  (const CMouseEvent &) { return false; }
  virtual bool buttonMotionEvent (const CMouseEvent &) { return false; }
  virtual bool buttonReleaseEvent(const CMouseEvent &) { return false; }

  // key press, release
  virtual bool keyPressEvent  (const CKeyEvent &) { return false; }
  virtual bool keyReleaseEvent(const CKeyEvent &) { return false; }

  // pointer motion
  virtual bool pointerMotionEvent(const CMouseEvent &) { return false; }

  // expose, resize
  virtual bool exposeEvent() { return false; }
  virtual bool resizeEvent() { return false; }

  // enter, leave
  virtual bool enterEvent() { return false; }
  virtual bool leaveEvent() { return false; }

  // show, hide
  virtual bool hideEvent() { return false; }
  virtual bool showEvent() { return false; }

  // close
  virtual bool closeEvent() { return false; }

  // selection
  virtual bool setSelectText(const std::string &text) = 0;

  // property
  virtual bool setProperty(const std::string &name, const std::string &value) = 0;

  // beep
  virtual void beep();

  // draw window contents
  virtual void redraw() { }

 private:
  CWindow(const CWindow &rhs);
  CWindow &operator=(const CWindow &rhs);
};

#endif
