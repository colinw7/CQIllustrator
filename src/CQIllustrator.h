#ifndef CQILLUSTRATOR_H
#define CQILLUSTRATOR_H

#include <CQMainWindow.h>
#include <CAutoPtr.h>
#include <CPoint2D.h>
#include <CImageLib.h>
#include <CEvent.h>
#include <CBooleanOp.h>

#include <CStack.h>

class CQIllustrator;
class CQIllustratorCanvas;
class CQIllustratorToolbar;

class CQMenu;
class CQMenuItem;
class CQToolBar;
class CQColorChooser;
class CQStrokeOptionTool;
class CQFillOptionTool;
//class CQFontOptionTool;
class CQObjectOptionTool;
class CQLayerOptionTool;
class CQPropertiesOptionTool;
class CQIllustratorAlignToolbar;
class CQIllustratorTranformToolbar;
class CQIllustratorLayerStack;
class CQIllustratorUndoDock;
class CQIllustratorPreferenceDock;
class CQIllustratorSnapDock;

class QLabel;
class QLineEdit;
class QStackedWidget;

class CSVGObject;

class CQIllustrator;
class CQIllustratorCmd;
class CQIllustratorCmdMgr;

#include <CQIllustratorSaveData.h>
#include <CQIllustratorMode.h>
#include <CQIllustratorSelection.h>
#include <CQIllustratorUndo.h>
#include <CQIllustratorSandbox.h>
#include <CQIllustratorShapeDrawer.h>
#include <CQIllustratorLayer.h>
#include <CQIllustratorShape.h>

class CQIllustratorGrid {
 public:
  CQIllustratorGrid() :
   enabled_(false), origin_(0,0), dx_(10), dy_(10) {
  }

  bool getEnabled() const { return enabled_; }
  void setEnabled(bool enabled) { enabled_ = enabled; }

  void draw(CQIllustratorShapeDrawer *drawer, const CBBox2D &bbox);

 private:
  bool     enabled_;
  CPoint2D origin_;
  double   dx_, dy_;
};

class CQIllustratorSnap {
 public:
  CQIllustratorSnap() :
   enabled_(false), xpitch_(1), ypitch_(1) {
  }

  bool getEnabled() const { return enabled_; }
  void setEnabled(bool enabled) { enabled_ = enabled; }

  double getXPitch() const { return xpitch_; }
  void setXPitch(double xpitch) { xpitch_ = xpitch; }

  double getYPitch() const { return ypitch_; }
  void setYPitch(double ypitch) { ypitch_ = ypitch; }

  CPoint2D snapPoint(const CPoint2D &point) const;

 private:
  bool   enabled_;
  double xpitch_, ypitch_;
};

class CQIllustrator : public CQMainWindow {
  Q_OBJECT

 public:
  enum Mode {
    MODE_SELECT,
    MODE_POINT_SELECT,
    MODE_RECT,
    MODE_ELLIPSE,
    MODE_POLYGON,
    MODE_PATH,
    MODE_STAR,
    MODE_TEXT,
    MODE_LGRADIENT,
    MODE_RGRADIENT,
    MODE_IMAGE,
    MODE_ZOOM,
    MODE_SLICE,
    MODE_PAN,
    MODE_ALIGN,
    MODE_TRANSFORM,
    MODE_ANCHOR_OBJECT,
    MODE_ANCHOR_POSITION,
    MODE_OFFSET_PATH
  };

  enum AlignSide {
    ALIGN_LEFT,
    ALIGN_BOTTOM,
    ALIGN_RIGHT,
    ALIGN_TOP,
    ALIGN_HORIZONTAL,
    ALIGN_VERTICAL
  };

  class PreviewObject {
   public:
    PreviewObject() { }

    virtual ~PreviewObject() { }

    virtual void draw(CQIllustrator *illustrator, QPainter *painter) const = 0;
  };

  class PreviewShape : public PreviewObject {
   private:
    CQIllustratorShape *shape_;
    CPoint2D            d_;

   public:
    PreviewShape(CQIllustratorShape *shape, const CPoint2D &d) :
     shape_(shape), d_(d) {
    }

    const CBBox2D &getBBox() const;

    void draw(CQIllustrator *illustrator, QPainter *painter) const;
  };

  class PreviewLine : public PreviewObject {
   private:
    CPoint2D p1_, p2_;

   public:
    PreviewLine(const CPoint2D &p1, const CPoint2D &p2) :
     p1_(p1), p2_(p2) {
    }

    void draw(CQIllustrator *illustrator, QPainter *painter) const;
  };

  typedef CQIllustratorMode::MouseEvent MouseEvent;
  typedef CQIllustratorMode::KeyEvent   KeyEvent;
  typedef std::vector<PreviewObject *>  PreviewObjectList;

 public:
  CQIllustrator();
 ~CQIllustrator();

  void addMode(CQIllustratorMode *mode);

  CQIllustratorMode *getMode(Mode id) const;

  QWidget *createCentralWidget();

  CQIllustratorShapeDrawer *getDrawer() const { return drawer_; }

  void initTerm();
  void createWorkspace();
  void createMenus();
  void createToolBars();
  void createStatusBar();
  void createDockWindows();

  void emitFillChanged();

  void addModeToolbar(CQIllustratorMode *mode, CQIllustratorToolbar *toolbar);

  void updateTitle();

  void resizeCanvas(int w, int h);

  void mousePress  (const MouseEvent &e);
  void mouseRelease(const MouseEvent &e);
  void mouseDrag   (const MouseEvent &e);
  void mouseMove   (const MouseEvent &e);

  void keyPress(const KeyEvent &e);

  bool getFlipY() const { return flip_y_; }

  void setFlipY(bool flip);

  const CQIllustratorData::ShapeStack &getShapes() const;

  const CQIllustratorSelectedShapes *getSelection() const { return selection_; }

  CQIllustratorSelectedShapes *getSelection() { return selection_; }

  void setLayer(const std::string &name);

  void checkoutShape(CQIllustratorShape *shape, CQIllustratorData::ChangeType changeType);
  void checkinShape (CQIllustratorShape *shape, CQIllustratorData::ChangeType changeType);

#if 0
  uint getNumSelected() const;

  CQIllustratorSelectedShapes::iterator getSelectionBegin();
  CQIllustratorSelectedShapes::iterator getSelectionEnd();

  CQIllustratorSelectedShapes::const_iterator getSelectionBegin() const;
  CQIllustratorSelectedShapes::const_iterator getSelectionEnd() const;

  CQIllustratorSelectedShape &getSelectionFront();

  const CQIllustratorSelectedShape &getSelectionFront() const;
#endif

  void moveSelectedShapes  (const CPoint2D &d);
  void moveSelectedPoints  (const CPoint2D &d);
  void resizeSelectedShapes(const CBBox2D  &d);

  void deleteSelectedShapes();
  void deleteSelectedPoints();

  CQIllustratorShape *selectAt(const CPoint2D &p, bool add=false, bool remove=false);

  void selectIn(const CBBox2D &bbox, bool add=false, bool remove=false);

  void selectOverlap(const CBBox2D &bbox, bool add=false, bool remove=false);

  void selectPointAt(const CPoint2D &p, CQIllustratorShape::ControlType type,
                     bool add=false, bool remove=false);

  void selectPointAt(CQIllustratorShape *shape, const CPoint2D &p,
                     CQIllustratorShape::ControlType type,
                     bool add=false, bool remove=false);

  void selectPointsIn(const CBBox2D &bbox, CQIllustratorShape::ControlType type,
                      bool add=false, bool remove=false);

  void selectLinesIn(const CBBox2D &bbox, bool add=false, bool remove=false);

  CQIllustratorShape *getObjectAt(const CPoint2D &p) const;

  void cyclePrev();
  void cycleNext();

  void zoomIn();
  void zoomOut();

  void zoomFull();
  void zoomFit();
  void zoomSelected();

  CBBox2D getFitBBox() const;

  void setMode(Mode mode);

  void setCursor(QCursor cursor);

  bool getDimmed() const { return dimmed_; }
  void setDimmed(bool dimmed);

  void clearSelection();

  void setSelectShape(const std::string &name);

  void setSelectShape(CQIllustratorShape *shape);

  void addSelectShape(CQIllustratorShape *shape);

  void removeSelectShape(CQIllustratorShape *shape);

  void removeShapeFromSelection(CQIllustratorShape *shape);

  CQIllustratorShape *getShape(const std::string &name) const;
  CQIllustratorShape *getShape(uint id) const;

  void draw(QPainter *painter);

  void dimQImage(const QImage &qimage, QImage &dim_qimage);

  void drawContents(QPainter *painter);

  void drawOverlay(QPainter *painter);

  void drawQuadTree(QPainter *painter);
  void drawQuadTree(QPainter *painter, const CQIllustratorData::QuadTree *tree, uint max_num);

  void setDefaultStroke(const CQIllustratorShapeStroke &stroke);
  void setDefaultFill(const CQIllustratorShapeFill &fill);

  const CQIllustratorShapeStroke &getDefaultStroke() const;
  const CQIllustratorShapeFill   &getDefaultFill() const;

  void acceptMode();
  void cancelMode();

  const CBBox2D &getBBox() const { return bbox_; }

  void setBBox(const CBBox2D &bbox);

  const CBBox2D &getFullBBox() const { return fullBBox_; }

  void setFullBBox(const CBBox2D &bbox);

  CQIllustratorCanvas *getCanvas() const { return canvas_; }

  const QTransform &getTransform () const;
  const QTransform &getITransform() const;

  CQIllustratorLayerStack &getLayerStack() { return *layerStack_; }

  CQIllustratorData *getData() const;

  CQIllustratorUndo *getUndo() const { return undo_; }

  CQIllustratorSandbox *getSandbox() const { return sandbox_; }

  const CRGBA &getBackground() const { return bg_; }

  void setBackground(const CRGBA &bg);

  void showConsole();

  void setCanvasFocus();

  void loadFile(const QString &filename);

  void loadSVG(const QString &filename);
  void loadPS (const QString &filename);

  void saveSVG(const QString &filename);
  void saveCmd(const QString &filename);

  CQIllustratorShape *addSVGObject(CSVGObject *parent, CSVGObject *object);

  void addShape(CQIllustratorShape *shape);

  void removeShape(CQIllustratorShape *shape);

  //QFont getCurrentFont() const;

  CQIllustratorAlignToolbar *getAlignToolbar() const;

  void selectAll();
  void selectNone();

  void addCommand(CQIllustratorCmd *cmd);

  bool loadCmd(const std::string &filename);

  bool execCommand(const std::string &cmd);

  void offsetPath(double o);
  void strokePath(double d);

  void toCurve();
  void flatten();

  void geomOp(CBooleanOp op);

  void intersectOp(CBooleanOp op, bool split=false, bool keepOld=false);

  void addLayer();

  void clearPreviewObjects();
  void addPreviewObject(PreviewObject *object);
  void drawPreviewObjects(QPainter *painter);

  void addLinearGradient(const CPoint2D &p1, const CPoint2D &p2);
  void addRadialGradient(const CPoint2D &p1, const CPoint2D &p2);

  void setStroke(QPainter *painter, CQIllustratorShape *shape,
                 const CQIllustratorShapeStroke &stroke);
  void setStroke(QPainter *painter, const CQIllustratorShape *shape,
                 const CQIllustratorShapeStroke &stroke);

  void setFill(QPainter *painter, CQIllustratorShape *shape,
               const CQIllustratorShapeFill &fill, const CBBox2D &bbox);
  void setFill(QPainter *painter, const CQIllustratorShape *shape,
               const CQIllustratorShapeFill &fill, const CBBox2D &bbox);

  void redrawOverlay();

  bool isInUndoGroup() const;

  void startUndoGroup(const std::string &str);
  void endUndoGroup();

  void setDeltaLabel(const QString &title, const QString &str);

  //------

  bool getSnapEnabled() const { return snap_.getEnabled(); }
  void setSnapEnabled(bool enabled) { snap_.setEnabled(enabled); }

  double getSnapXPitch() const { return snap_.getXPitch(); }
  void setSnapXPitch(double xpitch) { snap_.setXPitch(xpitch); }

  double getSnapYPitch() const { return snap_.getYPitch(); }
  void setSnapYPitch(double ypitch) { snap_.setYPitch(ypitch); }

  CPoint2D snapPoint(const CPoint2D &point) const { return snap_.snapPoint(point); }

  CQIllustratorCmdMgr *getCmdMgr() const { return cmdMgr_; }

 signals:
  void selectionChanged();

  void fillChanged();

  void fullBBoxChanged();

 public slots:
  void cancelAnchorSlot();

 private slots:
  void selectionChangedSlot();

  void layerChangedSlot();
  void layersModifiedSlot();

  void selectAllSlot();
  void selectNoneSlot();
  void groupSlot();
  void ungroupSlot();
  void copySlot();
  void deleteSlot();
  void raiseSlot();
  void lowerSlot();
  void flipXSlot();
  void flipYSlot();
  void lockSlot();
  void unlockSlot();

  void copyStrokeSlot();
  void pasteStrokeSlot();

  void anchorObjectSlot();
  void anchorPositionSlot();

  void loadFileSlot();
  void saveSVGSlot();
  void saveCmdSlot();

  void snapShotSlot();

  void undoSlot();
  void redoSlot();
  void undoChangedSlot();

  void addLayerSlot();

  void zoomFullSlot();
  void zoomFitSlot();
  void zoomSelSlot();

  void gridSlot();

  void flipViewSlot(bool);

  void toPolygonSlot();
  void toPathSlot();

  void offsetPathSlot();
  void strokePathSlot();

  void geomAndSlot();
  void geomOrSlot();
  void geomXorSlot();
  void geomNotSlot();

  void toCurveSlot();
  void flattenSlot();

  void intersectOrSlot();
  void intersectAndSlot();
  void intersectXor1Slot();
  void intersectXor2Slot();

  void triangulateSlot();
  void giftWrapSlot();
  void delaunaySlot();

  void combineSlot();

  void aboutSlot();

  void consoleExecSlot();

  void strokeSlot(const CQIllustratorShapeStroke &);
  void fillSlot(const CQIllustratorShapeFill &);
  void clipSlot(bool clip);
//void fontSlot(const QFont &font);
  void objectSlot(const QString &);
  void propSlot();

  void setShapeSVGStrokeAndFill(CQIllustratorShape *shape, CSVGObject *object);

  void setSVGShapeName(CQIllustratorShape *shape, CSVGObject *object);

  void setShapeName(CQIllustratorShape *shape);

 public:
  CQIllustratorRectShape    *createRectShape(const CPoint2D &p1, const CPoint2D &p2,
                                             double rx=0.0, double ry=0.0);
  CQIllustratorEllipseShape *createEllipseShape(const CPoint2D &p1, const CPoint2D &p2);
  CQIllustratorPolygonShape *createPolygonShape(const std::vector<CPoint2D> &points=
                                                 std::vector<CPoint2D>());
  CPathShape                *createPathShape();
  CQIllustratorTextShape    *createTextShape(const CPoint2D &p1, const CPoint2D &p2,
                                             const std::string &str);
  CQIllustratorNPolyShape   *createNPolyShape(const CPoint2D &c, uint n, double r, double a);
  CQIllustratorStarShape    *createStarShape(const CPoint2D &c, uint n, double r1, double r2,
                                             double a1, double a2);
  CQIllustratorGroupShape   *createGroupShape();

 public slots:
  void redraw();

 private:
  typedef std::map<Mode,CQIllustratorMode *> ModeMap;

  CQIllustratorCanvas               *canvas_;

  // Menu and Toolbar
  CQMenu                            *fileMenu_;
  CQMenu                            *createMenu_;
  CQMenu                            *modeMenu_;
  CQMenu                            *pathMenu_;
  CQMenu                            *selectMenu_;
  CQMenu                            *editMenu_;
  CQMenuItem                        *undoItem_;
  CQMenuItem                        *redoItem_;
  CQMenu                            *layerMenu_;
  CQMenuItem                        *addLayerItem_;
  CQMenu                            *viewMenu_;
  CQMenu                            *helpMenu_;
  CQToolBar                         *modeToolBar_;
  CQToolBar                         *pathModeToolBar_;
  CQToolBar                         *selectToolBar_;
  CQToolBar                         *editToolBar_;
  CQToolBar                         *layerToolBar_;
  CQToolBar                         *mouseToolToolBar_;
  QStackedWidget                    *mouseToolStack_;
  CQToolBar                         *toolsToolBar_;
  CQToolBar                         *consoleToolBar_;
  CQMenuItem                        *alignItem_;
  CQMenuItem                        *groupItem_;
  CQMenuItem                        *ungroupItem_;
  CQMenuItem                        *copyItem_;
  CQMenuItem                        *deleteItem_;
  CQMenuItem                        *raiseItem_;
  CQMenuItem                        *lowerItem_;
  CQMenuItem                        *flipXItem_;
  CQMenuItem                        *flipYItem_;
  CQMenuItem                        *lockItem_;
  CQMenuItem                        *unlockItem_;
  CQMenuItem                        *copyStroke_;
  CQMenuItem                        *pasteStroke_;
  CQMenuItem                        *selectAllItem_;
  CQMenuItem                        *selectNoneItem_;
  CQMenuItem                        *flipViewItem_;

  // Tools
  CQStrokeOptionTool                *strokeTool_;
  CQFillOptionTool                  *fillTool_;
//CQFontOptionTool                  *fontTool_;
  CQObjectOptionTool                *objectTool_;
  CQLayerOptionTool                 *layerTool_;
  CQPropertiesOptionTool            *propTool_;

  // Dock Widgets
  CQIllustratorUndoDock             *undoDock_;
  CQIllustratorPreferenceDock       *preferenceDock_;
  CQIllustratorSnapDock             *snapDock_;

  // Console
  QLineEdit                         *consoleEdit_;
  QAction                           *consoleAction_;

  // Status Labels
  QLabel                            *modeLabel_;
  QLabel                            *selLabel_;
  QLabel                            *posLabel_;
  QLabel                            *deltaLabel_;

  // Current File
  QString                            fileName_;
  CFileType                          fileType_;

  bool                               flip_y_;
  bool                               changed_;
  bool                               escape_;
  QImage                             qimage_;
  QImage                             dim_qimage_;
  bool                               dimmed_;
  bool                               dim_valid_;
  bool                               quad_tree_;
  CAutoPtr<CQIllustratorShapeDrawer> drawer_;
  Mode                               mode_;
  ModeMap                            modeMap_;
  CQIllustratorMode                 *currentMode_;
  CQIllustratorLayerStack           *layerStack_;
  CBBox2D                            fullBBox_;
  CBBox2D                            bbox_;
  CAutoPtr<CQIllustratorUndo>        undo_;
  CAutoPtr<CQIllustratorSandbox>     sandbox_;
  CRGBA                              bg_;
  CQIllustratorSelectedShapes       *selection_;
  PreviewObjectList                  previewObjects_;
  QPointF                            press_wpos_;
  QPoint                             current_ppos_;
  CQIllustratorGrid                  grid_;
  CQIllustratorSnap                  snap_;
  CQIllustratorCmdMgr               *cmdMgr_;
  CQIllustratorShapeStroke           def_stroke_;
  CQIllustratorShapeFill             def_fill_;
  CQIllustratorShapeStroke           save_stroke_;
  CQIllustratorShapeFill             save_fill_;
};

namespace CQIllustratorUtil {
  template<typename T>
  T *getCurrentShape(CQIllustrator *illustrator) {
    T *shape = NULL;

    CQIllustratorSelectedShapes *selection = illustrator->getSelection();

    CQIllustratorSelectedShapes::iterator ps1, ps2;

    for (ps1 = selection->begin(), ps2 = selection->end(); ps1 != ps2; ++ps1) {
      CQIllustratorShape *shape1 = (*ps1).getShape();

      T *tshape = dynamic_cast<T *>(shape1);

      if (tshape == NULL) continue;

      if (shape == NULL)
        shape = tshape;
      else
        break;
    }

    return shape;
  }
}

#endif
