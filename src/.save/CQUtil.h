#include <CRGBA.h>
#include <CPoint2D.h>
#include <CIPoint2D.h>
#include <CMatrix2D.h>
#include <CBBox2D.h>
#include <CIBBox2D.h>
#include <CFont.h>
#include <CImagePtr.h>
#include <CEvent.h>
#include <CAlignType.h>

#include <QLinearGradient>
#include <QRadialGradient>

class CLinearGradient;
class CRadialGradient;
class QMouseEvent;
class QKeyEvent;

namespace CQUtil {
  CRGBA colorToRGBA(const QColor &c);
  QColor rgbaToColor(const CRGBA &rgba);

  CPoint2D fromQPoint(const QPointF &p);
  CIPoint2D fromQPoint(const QPoint &p);

  QPointF toQPoint(const CPoint2D &p);
  QPoint toQPoint(const CIPoint2D &p);

  CBBox2D  fromQRect(const QRectF &rect);
  CIBBox2D fromQRect(const QRect &rect);

  QRectF toQRect(const CBBox2D &rect);
  QRect  toQRect(const CIBBox2D &rect);

  QTransform toQTransform(const CMatrix2D &m);

  QLinearGradient toQGradient(const CLinearGradient *lgradient);
  QRadialGradient toQGradient(const CRadialGradient *rgradient);

  QFont    toQFont  (CFontPtr font);
  CFontPtr fromQFont(QFont font);

  void setBackground(QWidget *widget, const QColor &color);

  CMouseEvent *convertEvent(QMouseEvent *event);
  CKeyEvent   *convertEvent(QKeyEvent *event);

  Qt::Alignment toQAlign(CHAlignType halign);
  Qt::Alignment toQAlign(CVAlignType valign);

  QImage toQImage(CImagePtr image);
}
