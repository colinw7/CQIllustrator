#ifndef CQUTIL_H
#define CQUTIL_H

#include <Qt>
#include <QColor>
#include <QEvent>
#include <QVariant>
#include <QTableWidget>
#include <QPen>

#include <CEvent.h>
#include <CRGBA.h>
#include <CLineDash.h>
#include <CFontStyle.h>
#include <CFont.h>
#include <CImageLib.h>
#include <CPoint2D.h>
#include <CBBox2D.h>
#include <CMatrix2D.h>

class QMouseEvent;
class QKeyEvent;
class QMetaObject;
class QAbstractButton;
class QDockWidget;
class QLabel;

class CLinearGradient;
class CRadialGradient;

namespace CQUtil {
  class PropInfo {
   private:
    QString        _name;
    QVariant::Type _type;
    QString        _typeName;
    bool           _is_writable;
    bool           _is_enum_type;
    QStringList    _enumNames;

   public:
    void init(QMetaProperty mp);

    QString        name    () const { return _name; }
    QVariant::Type type    () const { return _type; }
    QString        typeName() const { return _typeName; }

    bool isWritable() const { return _is_writable ; }
    bool isEnumType() const { return _is_enum_type; }

    const QStringList &enumNames() { return _enumNames; }
  };

  CMouseEvent *convertEvent(QMouseEvent *event);
  CKeyEvent   *convertEvent(QKeyEvent *event);

  CMouseButton   convertButton(Qt::MouseButton button);
  CKeyType       convertKey(int key, Qt::KeyboardModifiers modifiers);
  CEventModifier convertModifier(Qt::KeyboardModifiers modifiers);

  QColor rgbToColor(const CRGB &rgb);
  QColor rgbaToColor(const CRGBA &rgba);

  uint rgbaToInt(const CRGBA &rgba);

  CRGB  colorToRGB(QColor color);
  CRGBA colorToRGBA(QColor color);

  void decodeFont(const QFont &font, QString &family, CFontStyle &style, int &pixelSize);

  QString fullName(const QObject* object);

  QObject *nameToObject(const QString &name);
  QObject *nameToObject(QObject *parent, const QString &name);

  QObject *childObject(QObject *parent, const QString &name);

  QWidget *getToplevelWidget(QWidget *widget);

  QStringList getPropertyList(const QObject *object, bool inherited);
  QStringList getPropertyList(const QObject *object, bool inherited,
                              const QMetaObject* metaObject);

  bool getProperty(const QObject *object, const QString &name, QVariant &value);

  bool setProperty(const QObject *object, const QString &name, const QVariant &value);

  bool getPropInfo(const QObject *object, const QString &name, PropInfo *info);

  const QMetaObject *baseClass(QMetaObject *metaObject, const char *name);

  QStringList signalNames(const QObject* object, bool inherited);
  QStringList slotNames(const QObject* object, bool inherited);

  QString eventTypeToName(QEvent::Type type);
  QString eventToString(QEvent *event);

  QColor getForeground(QWidget *widget);
  void   setForeground(QWidget *widget, const QColor &color);

  QColor getBackground(QWidget *widget);
  void   setBackground(QWidget *widget, const QColor &color);

  QPointF   toQPoint  (const CPoint2D &point);
  CPoint2D  fromQPoint(const QPointF &point);

  QPoint    toQPoint  (const CIPoint2D &point);
  CIPoint2D fromQPoint(const QPoint &point);

  QRectF  toQRect  (const CBBox2D &rect);
  CBBox2D fromQRect(const QRectF &rect);

  QRect    toQRect  (const CIBBox2D &rect);
  CIBBox2D fromQRect(const QRect &rect);

  QMatrix   toQMatrix  (const CMatrix2D &m);
  CMatrix2D fromQMatrix(const QMatrix &m);

  QTransform toQTransform  (const CMatrix2D &m);
  CMatrix2D  fromQTransform(const QTransform &t);

  QFont    toQFont  (CFontPtr font);
  CFontPtr fromQFont(QFont font);

  QString variantToString(QVariant var);

  bool paletteFromString(QPalette &palette, const QString &paletteDef);
  QString paletteToString(const QPalette &palette);

  bool activateSlot(QObject *receiver, const char *slotName, const char *valuesStr);
  bool activateSignal(QObject* sender, const char *signalName, const char *valuesStr);

  /*! template function to get table cell widget of type */
  template<typename T>
  T tableCellWidget(QTableWidget *table, int row, int col) {
    return qobject_cast<T>(table->cellWidget(row, col));
  }

  QIcon imageToIcon(CImagePtr image);

  QImage toQImage(CImagePtr image);

  QLinearGradient toQGradient(const CLinearGradient *lgradient);
  QRadialGradient toQGradient(const CRadialGradient *rgradient);

  void getScreenSize(uint *w, uint *h);

  void setSelectText(const std::string &text);

  void setDockVisible(QDockWidget *dock, bool visible);

  void penSetLineDash(QPen &pen, const CLineDash &dash);

  Qt::Alignment toQAlign(CHAlignType halign);
  Qt::Alignment toQAlign(CVAlignType valign);

  //-----

  QPixmap getPixmap(const QString &name);

  QIcon getIcon(const QString &name);

  //-----

  uint nameWidgetTree(QWidget *widget);

  void nameWidget(QAbstractButton *button);
  void nameWidget(QLabel *label);
  void nameWidget(QWidget *widget);

  //-----

  void recolorImage(QImage &image, const QColor &fg, const QColor &bg);
};

class CQWidgetPtr : public QObject {
  Q_OBJECT

 public:
  CQWidgetPtr(QWidget *w=NULL) :
   w_(w) {
    watch();
  }

  bool isValid() const {
    return (w_ != NULL);
  }

  void set(QWidget *w) {
    w_ = w;

    watch();
  }

  QWidget *get() const {
    return w_;
  }

  template<typename T>
  T *get() const {
    return dynamic_cast<T *>(w_);
  }

 private:
  void watch() {
    if (w_)
      connect(w_, SIGNAL(destroyed(QObject *)), this, SLOT(resetSlot()));
  }

 private slots:
  void resetSlot() {
    w_ = NULL;
  }

 private:
  QWidget *w_;
};

#endif
