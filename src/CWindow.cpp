#include <CWindow.h>
#include <cassert>

CWindowMgr *
CWindowMgr::
getInstance()
{
  static CWindowMgr *instance;

  if (! instance)
    instance = new CWindowMgr;

  return instance;
}

CWindowMgr::
CWindowMgr() :
 factory_(NULL)
{
}

CWindowMgr::
~CWindowMgr()
{
}

CWindow *
CWindowMgr::
createWindow(int x, int y, uint width, uint height)
{
  assert(factory_);

  return factory_->createWindow(x, y, width, height);
}

CWindow *
CWindowMgr::
createWindow(CWindow *parent, int x, int y, uint width, uint height)
{
  assert(factory_);

  return factory_->createWindow(parent, x, y, width, height);
}

//-------

CWindow::
CWindow(CWindow *parent) :
 parent_(parent)
{
}

CWindow::
~CWindow()
{
}

void
CWindow::
beep()
{
}
