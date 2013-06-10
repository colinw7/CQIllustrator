#include <CQApp.h>
#include <CQFont.h>
#include <CQImage.h>
#include <CQWindow.h>

//#define USE_OBJEDIT 1

#ifdef USE_OBJEDIT
#include <CQObjEdit.h>
#endif

#include <QKeyEvent>

CQApp *CQApp::app_;

#ifdef USE_OBJEDIT
extern bool CQAppXIsObjEditKey(void *);
#endif

#ifdef USE_OBJEDIT
bool
CQAppEventFilter(void *message, long *)
{
  bool show = CQAppXIsObjEditKey(message);

  if (show) {
    CQObjEdit *objEdit = CQObjEdit::createInstance();

    objEdit->show();
  }

  return show;
}
#endif

#ifdef USE_OBJEDIT
bool
CQAppObjEditFilter::
eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

    if (keyEvent->key() == Qt::Key_Home) {
      CQObjEdit *objEdit = CQObjEdit::createInstance();

      objEdit->show();

      return true;
    }
  }

  // standard event processing
  return QObject::eventFilter(obj, event);
}
#endif

/*! Constructor to create QApplication and correctly
    initialize CQImage and CQFont prototypes
*/
CQApp::
CQApp(int &argc, char **argv) :
 QApplication(argc, argv)
{
  CQImage::setPrototype();

  CQFontMgrInst->setPrototype();

#ifdef USE_OBJEDIT
  setEventFilter(CQAppEventFilter);
#endif

  CQWindow::setFactory();
}

#ifdef USE_OBJEDIT
void
CQApp::
addObjEditFilter(QWidget *widget)
{
  widget->installEventFilter(new CQAppObjEditFilter);
}
#endif
