//#define SVG_FLIP 1

#include <CQIllustrator.h>
#include <CQIllustratorEllipseShape.h>
#include <CQIllustratorGroupShape.h>
#include <CQIllustratorNPolyShape.h>
#include <CQIllustratorPathShape.h>
#include <CQIllustratorPolygonShape.h>
#include <CQIllustratorRectShape.h>
#include <CQIllustratorStarShape.h>
#include <CQIllustratorTextShape.h>
#include <CQIllustratorShapeControlLine.h>

#include <CQIllustratorCanvas.h>
#include <CQIllustratorLayer.h>
#include <CQIllustratorUndo.h>
#include <CQIllustratorShapeDrawer.h>
#include <CQIllustratorAlignMode.h>
#include <CQIllustratorTransformMode.h>
#include <CQIllustratorCreateRectMode.h>
#include <CQIllustratorCreateEllipseMode.h>
#include <CQIllustratorCreatePathMode.h>
#include <CQIllustratorCreatePolygonMode.h>
#include <CQIllustratorCreateStarMode.h>
#include <CQIllustratorCreateTextMode.h>
#include <CQIllustratorPanMode.h>
#include <CQIllustratorPointSelectMode.h>
#include <CQIllustratorSelectMode.h>
#include <CQIllustratorSetAnchorObjectMode.h>
#include <CQIllustratorSetAnchorPositionMode.h>
#include <CQIllustratorSetLGradientMode.h>
#include <CQIllustratorSetRGradientMode.h>
#include <CQIllustratorSetImageMode.h>
#include <CQIllustratorSliceMode.h>
#include <CQIllustratorZoomMode.h>
#include <CQIllustratorOffsetPathMode.h>
#include <CQIllustratorUndoDock.h>
#include <CQIllustratorPreference.h>
#include <CQIllustratorSnapDock.h>
#include <CQIllustratorHandle.h>
#include <CQIllustratorCmd.h>
#include <CQIllustratorPropertiesDlg.h>

#include <CSVGCircle.h>
#include <CSVGEllipse.h>
#include <CSVGImage.h>
#include <CSVGLine.h>
#include <CSVGLinearGradient.h>
#include <CSVGPath.h>
#include <CSVGPathPart.h>
#include <CSVGPolygon.h>
#include <CSVGPolyLine.h>
#include <CSVGRadialGradient.h>
#include <CSVGRect.h>
#include <CSVGStop.h>
#include <CSVGText.h>
#include <CSVGTSpan.h>
#include <CSVGUse.h>
#include <CSVGUtil.h>
#include <CQSVGRenderer.h>

#include <CLinearGradient.h>
#include <CRadialGradient.h>

#include <CQStrokeOptionTool.h>
#include <CQFillOptionTool.h>
//#include <CQFontOption.h>
#include <CQObjectOption.h>
#include <CQLayerOption.h>
#include <CQPropertiesOption.h>

#ifdef PSVIEW
#include <CPSViewShapeRenderer.h>
#endif

#include <CQApp.h>
#include <CQStyle.h>
#include <CQMenu.h>
#include <CQImage.h>
#include <CQToolBar.h>
#include <CQDockWidget.h>
#include <CQUtil.h>
#include <CSVG.h>
#include <CGenPoly.h>
#include <CFileUtil.h>
#include <CAxis2D.h>
#include <CQAccelerate.h>
#include <CTriangulate2D.h>
#include <CGiftWrap.h>
#include <CDelaunay.h>

#include <COSFile.h>

#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QStatusBar>
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>
#include <QToolBar>

#include <svg/select_all_svg.h>
#include <svg/select_none_svg.h>

#include <svg/group_svg.h>
#include <svg/ungroup_svg.h>
#include <svg/copy_svg.h>
#include <svg/copy_color_svg.h>
#include <svg/delete_svg.h>
#include <svg/raise_svg.h>
#include <svg/lower_svg.h>
#include <svg/flip_x_svg.h>
#include <svg/flip_y_svg.h>
#include <svg/lock_svg.h>
#include <svg/unlock_svg.h>
#include <svg/add_layer_svg.h>
#include <svg/undo_svg.h>
#include <svg/redo_svg.h>

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  app.setStyle(new CQStyle);

  CQIllustrator *w = new CQIllustrator;

  w->init();

  if (argc > 1)
    w->loadFile(argv[1]);

  w->show();

  return app.exec();
}

//----------------

class CQIllustratorSourceCmd : public CQIllustratorCmd {
 public:
  CQIllustratorSourceCmd();

  const char *getName() const { return "source"; }

  bool exec(const std::vector<std::string> &words);
};

class CQIllustratorSelectCmd : public CQIllustratorCmd {
 public:
  CQIllustratorSelectCmd();

  const char *getName() const { return "select"; }

  bool exec(const std::vector<std::string> &words);
};

class CQIllustratorMoveCmd : public CQIllustratorCmd {
 public:
  CQIllustratorMoveCmd();

  const char *getName() const { return "move"; }

  bool exec(const std::vector<std::string> &words);
};

class CQIllustratorMovePointCmd : public CQIllustratorCmd {
 public:
  CQIllustratorMovePointCmd();

  const char *getName() const { return "move_point"; }

  bool exec(const std::vector<std::string> &words);
};

class CQIllustratorUndoCmd : public CQIllustratorCmd {
 public:
  CQIllustratorUndoCmd();

  const char *getName() const { return "undo"; }

  bool exec(const std::vector<std::string> &words);
};

class CQIllustratorRedoCmd : public CQIllustratorCmd {
 public:
  CQIllustratorRedoCmd();

  const char *getName() const { return "redo"; }

  bool exec(const std::vector<std::string> &words);
};

class CQIllustratorOffsetPathCmd : public CQIllustratorCmd {
 public:
  CQIllustratorOffsetPathCmd();

  const char *getName() const { return "offset_path"; }

  bool exec(const std::vector<std::string> &words);
};

class CQIllustratorStrokePathCmd : public CQIllustratorCmd {
 public:
  CQIllustratorStrokePathCmd();

  const char *getName() const { return "stroke_path"; }

  bool exec(const std::vector<std::string> &words);
};

class CQIllustratorIntersectCmd : public CQIllustratorCmd {
 public:
  CQIllustratorIntersectCmd();

  const char *getName() const { return "intersect"; }

  bool exec(const std::vector<std::string> &words);
};

//----------------

CQIllustrator::
CQIllustrator() :
 CQMainWindow("Illustrator")
{
  drawer_ = new CQIllustratorShapeDrawer(this);

  selection_ = new CQIllustratorSelectedShapes(this);

  connect(selection_, SIGNAL(selectionChanged()), this, SLOT(selectionChangedSlot()));

  layerStack_ = new CQIllustratorLayerStack(this);

  connect(layerStack_, SIGNAL(objectPostModify(CQIllustratorShape *, ChangeType)),
          this, SLOT(selectionChangedSlot()));

  connect(layerStack_, SIGNAL(layerChanged()), this, SLOT(layerChangedSlot()));

  connect(layerStack_, SIGNAL(layersModified()), this, SLOT(layersModifiedSlot()));

  undo_ = new CQIllustratorUndo(this);

  connect(undo_, SIGNAL(undoChanged()), this, SLOT(undoChangedSlot()));

  sandbox_ = new CQIllustratorSandbox(this);

  cmdMgr_ = new CQIllustratorCmdMgr(this);

  double size = 1000.0;

  setFullBBox(CBBox2D(CPoint2D(0.0, 0.0), CPoint2D(size, size)));

  setBBox(getFullBBox());

  //------

  addCommand(new CQIllustratorSourceCmd);
  addCommand(new CQIllustratorSelectCmd);
  addCommand(new CQIllustratorMoveCmd);
  addCommand(new CQIllustratorMovePointCmd);
  addCommand(new CQIllustratorUndoCmd);
  addCommand(new CQIllustratorRedoCmd);
  addCommand(new CQIllustratorOffsetPathCmd);
  addCommand(new CQIllustratorStrokePathCmd);
  addCommand(new CQIllustratorIntersectCmd);

  //------

  updateTitle();
}

CQIllustrator::
~CQIllustrator()
{
}

void
CQIllustrator::
initTerm()
{
  addMode(new CQIllustratorSelectMode       (this));
  addMode(new CQIllustratorPointSelectMode  (this));
  addMode(new CQIllustratorCreateRectMode   (this));
  addMode(new CQIllustratorCreateEllipseMode(this));
  addMode(new CQIllustratorCreatePolygonMode(this));
  addMode(new CQIllustratorCreatePathMode   (this));
  addMode(new CQIllustratorCreateStarMode   (this));
  addMode(new CQIllustratorCreateTextMode   (this));
  addMode(new CQIllustratorSetLGradientMode (this));
  addMode(new CQIllustratorSetRGradientMode (this));
  addMode(new CQIllustratorSetImageMode     (this));
  addMode(new CQIllustratorSliceMode        (this));
  addMode(new CQIllustratorZoomMode         (this));
  addMode(new CQIllustratorPanMode          (this));
  addMode(new CQIllustratorAlignMode        (this));
  addMode(new CQIllustratorTransformMode    (this));
  addMode(new CQIllustratorOffsetPathMode   (this));

  addMode(new CQIllustratorSetAnchorObjectMode  (this));
  addMode(new CQIllustratorSetAnchorPositionMode(this));

  for (auto &modePair : modeMap_) {
    CQIllustratorMode *mode = modePair.second;

    if (mode->getParentMode() == 0) {
      if      (mode->isCreateMode())
        mode->createMenuItem(createMenu_);
      else if (mode->isSelectMode())
        mode->createMenuItem(selectMenu_);
      else
        mode->createMenuItem(modeMenu_);

      modeToolBar_->addItem(mode->getMenuItem());
    }
  }

  setMode(Mode::SELECT);
}

void
CQIllustrator::
selectionChangedSlot()
{
  emit selectionChanged();

  redraw();
}

void
CQIllustrator::
layerChangedSlot()
{
  layerTool_->updateLayer();

  redraw();
}

void
CQIllustrator::
layersModifiedSlot()
{
  layerTool_->reloadLayers();

  redraw();
}

void
CQIllustrator::
emitFillChanged()
{
  emit fillChanged();
}

void
CQIllustrator::
addMode(CQIllustratorMode *mode)
{
  mode->init();

  modeMap_[(Mode) mode->getId()] = mode;
}

CQIllustratorMode *
CQIllustrator::
getMode(Mode id) const
{
  auto p = modeMap_.find(id);

  if (p == modeMap_.end())
    return 0;

  return (*p).second;
}

QWidget *
CQIllustrator::
createCentralWidget()
{
  canvas_ = new CQIllustratorCanvas(this);

  return canvas_;
}

void
CQIllustrator::
createWorkspace()
{
}

void
CQIllustrator::
createMenus()
{
  fileMenu_ = new CQMenu(this, "File");

  CQMenuItem *loadFileItem = new CQMenuItem(fileMenu_, "Load File");

  connect(loadFileItem->getAction(), SIGNAL(triggered()), this, SLOT(loadFileSlot()));

  CQMenuItem *saveSvgItem = new CQMenuItem(fileMenu_, "Save SVG");

  connect(saveSvgItem->getAction(), SIGNAL(triggered()), this, SLOT(saveSVGSlot()));

  CQMenuItem *saveCmdItem = new CQMenuItem(fileMenu_, "Save Cmd");

  connect(saveCmdItem->getAction(), SIGNAL(triggered()), this, SLOT(saveCmdSlot()));

  CQMenuItem *snapshotItem = new CQMenuItem(fileMenu_, "Snapshot");

  snapshotItem->setStatusTip("Screen dump");

  connect(snapshotItem->getAction(), SIGNAL(triggered()), this, SLOT(snapShotSlot()));

  CQMenuItem *objectsItem = new CQMenuItem(fileMenu_, "Shapes");

  connect(objectsItem->getAction(), SIGNAL(triggered()), this, SLOT(objectsSlot()));

  CQMenuItem *quitItem = new CQMenuItem(fileMenu_, "Quit");

  quitItem->setShortcut("Ctrl+Q");
  quitItem->setStatusTip("Quit the application");

  connect(quitItem->getAction(), SIGNAL(triggered()), this, SLOT(close()));

  //--------

  createMenu_ = new CQMenu(this, "Create");
  selectMenu_ = new CQMenu(this, "Select");
  modeMenu_   = new CQMenu(this, "Mode");

  //--------

  selectAllItem_ = new CQMenuItem(selectMenu_,
    CQPixmapCacheInst->getIcon("SELECT_ALL"), "Select All");

  selectAllItem_->setStatusTip("Select All");

  connect(selectAllItem_->getAction(), SIGNAL(triggered()), this, SLOT(selectAllSlot()));

  selectNoneItem_ = new CQMenuItem(selectMenu_,
    CQPixmapCacheInst->getIcon("SELECT_NONE"), "Select None");

  selectNoneItem_->setStatusTip("Select None");

  connect(selectNoneItem_->getAction(), SIGNAL(triggered()), this, SLOT(selectNoneSlot()));

  //--------

  editMenu_ = new CQMenu(this, "Edit");

  groupItem_ = new CQMenuItem(editMenu_,
    CQPixmapCacheInst->getIcon("GROUP"), "Group Objects");

  groupItem_->setStatusTip("Create Group From Selected Objects");

  connect(groupItem_->getAction(), SIGNAL(triggered()), this, SLOT(groupSlot()));

  ungroupItem_ = new CQMenuItem(editMenu_,
    CQPixmapCacheInst->getIcon("UNGROUP"), "Ungroup Objects");

  ungroupItem_->setStatusTip("Ungroup Selected Groups");

  connect(ungroupItem_->getAction(), SIGNAL(triggered()), this, SLOT(ungroupSlot()));

  copyItem_ = new CQMenuItem(editMenu_,
    CQPixmapCacheInst->getIcon("COPY"), "Copy Objects");

  copyItem_->setStatusTip("Copy Selected Objects");

  connect(copyItem_->getAction(), SIGNAL(triggered()), this, SLOT(copySlot()));

  deleteItem_ = new CQMenuItem(editMenu_,
    CQPixmapCacheInst->getIcon("DELETE"), "Delete Objects");

  deleteItem_->setStatusTip("Delete Selected Objects");

  connect(deleteItem_->getAction(), SIGNAL(triggered()), this, SLOT(deleteSlot()));

  raiseItem_ = new CQMenuItem(editMenu_,
    CQPixmapCacheInst->getIcon("RAISE"), "Raise Objects");

  raiseItem_->setStatusTip("Lower Selected Objects");

  connect(raiseItem_->getAction(), SIGNAL(triggered()), this, SLOT(raiseSlot()));

  lowerItem_ = new CQMenuItem(editMenu_,
    CQPixmapCacheInst->getIcon("LOWER"), "Lower Objects");

  lowerItem_->setStatusTip("Raise Selected Objects");

  connect(lowerItem_->getAction(), SIGNAL(triggered()), this, SLOT(lowerSlot()));

  flipXItem_ = new CQMenuItem(editMenu_,
    CQPixmapCacheInst->getIcon("FLIP_X"), "Flip X");

  flipXItem_->setStatusTip("Flip Objects in X Axis");

  connect(flipXItem_->getAction(), SIGNAL(triggered()), this, SLOT(flipXSlot()));

  flipYItem_ = new CQMenuItem(editMenu_,
    CQPixmapCacheInst->getIcon("FLIP_Y"), "Flip Y");

  flipYItem_->setStatusTip("Flip Objects in Y Axis");

  connect(flipYItem_->getAction(), SIGNAL(triggered()), this, SLOT(flipYSlot()));

  lockItem_ = new CQMenuItem(editMenu_, CQPixmapCacheInst->getIcon("LOCK"), "Lock");

  lockItem_->setStatusTip("Lock Selected Objects");

  connect(lockItem_->getAction(), SIGNAL(triggered()), this, SLOT(lockSlot()));

  unlockItem_ = new CQMenuItem(editMenu_, CQPixmapCacheInst->getIcon("UNLOCK"), "Unlock");

  unlockItem_->setStatusTip("Unlock Selected Objects");

  connect(unlockItem_->getAction(), SIGNAL(triggered()), this, SLOT(unlockSlot()));

  copyStroke_ = new CQMenuItem(editMenu_,
    CQPixmapCacheInst->getIcon("COPY_COLOR"), "Copy Stroke and Fill");

  copyStroke_->setStatusTip("Copy Stroke and Fill");

  connect(copyStroke_->getAction(), SIGNAL(triggered()), this, SLOT(copyStrokeSlot()));

  pasteStroke_ = new CQMenuItem(editMenu_, "Paste Stroke and Fill");

  pasteStroke_->setStatusTip("Paste Stroke and Fill");

  connect(pasteStroke_->getAction(), SIGNAL(triggered()), this, SLOT(pasteStrokeSlot()));

  undoItem_ = new CQMenuItem(editMenu_, CQPixmapCacheInst->getIcon("UNDO"), "Undo");

  undoItem_->setEnabled(false);

  undoItem_->setShortcut("Ctrl+Z");
  undoItem_->setStatusTip("Undo last change");

  connect(undoItem_->getAction(), SIGNAL(triggered()), this, SLOT(undoSlot()));

  redoItem_ = new CQMenuItem(editMenu_, CQPixmapCacheInst->getIcon("REDO"), "Redo");

  redoItem_->setEnabled(false);

  redoItem_->setShortcut("Ctrl+Y");
  redoItem_->setStatusTip("Redo last undo");

  connect(redoItem_->getAction(), SIGNAL(triggered()), this, SLOT(redoSlot()));

  //----

  layerMenu_ = new CQMenu(this, "Layer");

  addLayerItem_ = new CQMenuItem(layerMenu_,
    CQPixmapCacheInst->getIcon("ADD_LAYER"), "Add Layer");

  addLayerItem_->setStatusTip("Add New Layer");

  connect(addLayerItem_->getAction(), SIGNAL(triggered()), this, SLOT(addLayerSlot()));

  //----

  viewMenu_ = new CQMenu(this, "View");

  CQMenuItem *zoomFullItem = new CQMenuItem(viewMenu_, "Zoom Full");
  CQMenuItem *zoomFitItem  = new CQMenuItem(viewMenu_, "Zoom Fit");
  CQMenuItem *zoomSelItem  = new CQMenuItem(viewMenu_, "Zoom Selected");
  CQMenuItem *gridItem     = new CQMenuItem(viewMenu_, "Grid");

  connect(zoomFullItem->getAction(), SIGNAL(triggered()), this, SLOT(zoomFullSlot()));
  connect(zoomFitItem ->getAction(), SIGNAL(triggered()), this, SLOT(zoomFitSlot()));
  connect(zoomSelItem ->getAction(), SIGNAL(triggered()), this, SLOT(zoomSelSlot()));
  connect(gridItem    ->getAction(), SIGNAL(triggered()), this, SLOT(gridSlot()));

  flipViewItem_ = new CQMenuItem(viewMenu_, "Flip Y", CQMenuItem::CHECKABLE);

  connect(flipViewItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(flipViewSlot(bool)));

  //----

  viewMenu_ = new CQMenu(this, "Geometry");

  CQMenuItem *toPolygonItem = new CQMenuItem(viewMenu_, "To Polygon");
  CQMenuItem *toPathItem    = new CQMenuItem(viewMenu_, "To Path"   );

  connect(toPolygonItem ->getAction(), SIGNAL(triggered()), this, SLOT(toPolygonSlot()));
  connect(toPathItem    ->getAction(), SIGNAL(triggered()), this, SLOT(toPathSlot   ()));

  CQMenuItem *offsetPathItem = new CQMenuItem(viewMenu_, "Offset Path");
  CQMenuItem *strokePathItem = new CQMenuItem(viewMenu_, "Stroke Path");

  connect(offsetPathItem->getAction(), SIGNAL(triggered()), this, SLOT(offsetPathSlot()));
  connect(strokePathItem->getAction(), SIGNAL(triggered()), this, SLOT(strokePathSlot()));

  CQMenuItem *geomAndItem = new CQMenuItem(viewMenu_, "And");
  CQMenuItem *geomOrItem  = new CQMenuItem(viewMenu_, "Or" );
  CQMenuItem *geomXorItem = new CQMenuItem(viewMenu_, "Xor");
  CQMenuItem *geomNotItem = new CQMenuItem(viewMenu_, "Not");

  connect(geomAndItem->getAction(), SIGNAL(triggered()), this, SLOT(geomAndSlot()));
  connect(geomOrItem ->getAction(), SIGNAL(triggered()), this, SLOT(geomOrSlot ()));
  connect(geomXorItem->getAction(), SIGNAL(triggered()), this, SLOT(geomXorSlot()));
  connect(geomNotItem->getAction(), SIGNAL(triggered()), this, SLOT(geomNotSlot()));

  CQMenuItem *toCurveItem = new CQMenuItem(viewMenu_, "To Curve");
  CQMenuItem *flattenItem = new CQMenuItem(viewMenu_, "Flatten" );

  connect(toCurveItem->getAction(), SIGNAL(triggered()), this, SLOT(toCurveSlot()));
  connect(flattenItem->getAction(), SIGNAL(triggered()), this, SLOT(flattenSlot()));

  CQMenuItem *intersectOrItem   = new CQMenuItem(viewMenu_, "Intersect (OR)");
  CQMenuItem *intersectAndItem  = new CQMenuItem(viewMenu_, "Intersect (AND)");
  CQMenuItem *intersectXor1Item = new CQMenuItem(viewMenu_, "Intersect (XOR Single)");
  CQMenuItem *intersectXor2Item = new CQMenuItem(viewMenu_, "Intersect (XOR Split)");
  CQMenuItem *combineItem       = new CQMenuItem(viewMenu_, "Combine");

  connect(intersectOrItem  ->getAction(), SIGNAL(triggered()), this, SLOT(intersectOrSlot()));
  connect(intersectAndItem ->getAction(), SIGNAL(triggered()), this, SLOT(intersectAndSlot()));
  connect(intersectXor1Item->getAction(), SIGNAL(triggered()), this, SLOT(intersectXor1Slot()));
  connect(intersectXor2Item->getAction(), SIGNAL(triggered()), this, SLOT(intersectXor2Slot()));
  connect(combineItem      ->getAction(), SIGNAL(triggered()), this, SLOT(combineSlot()));

  CQMenuItem *triangulateItem = new CQMenuItem(viewMenu_, "Triangulate");

  connect(triangulateItem->getAction(), SIGNAL(triggered()), this, SLOT(triangulateSlot()));

  CQMenuItem *giftWrapItem = new CQMenuItem(viewMenu_, "Gift Wrap");

  connect(giftWrapItem->getAction(), SIGNAL(triggered()), this, SLOT(giftWrapSlot()));

  CQMenuItem *delaunayItem = new CQMenuItem(viewMenu_, "Delaunay");

  connect(delaunayItem->getAction(), SIGNAL(triggered()), this, SLOT(delaunaySlot()));

  //----

  CQDockMgrInst->addToolsMenu(this);

  //----

  helpMenu_ = new CQMenu(this, "Help");

  //----

  CQMenuItem *aboutItem = new CQMenuItem(helpMenu_, "About");

  aboutItem->setStatusTip("Show the application's About box");

  connect(aboutItem->getAction(), SIGNAL(triggered()), this, SLOT(aboutSlot()));

  //----

  CQAccelerate::accelerateMenu(editMenu_->getMenu());
}

void
CQIllustrator::
createToolBars()
{
  QFontMetrics fm(font());

  int is = style()->pixelMetric(QStyle::PM_LargeIconSize);

  int ts = 2*is + 4;

  //-------

  modeToolBar_ = new CQToolBar(this, "Mode");

  //-------

  selectToolBar_ = new CQToolBar(this, "Select");

  selectToolBar_->addItem(selectAllItem_);
  selectToolBar_->addItem(selectNoneItem_);

  //-------

  editToolBar_ = new CQToolBar(this, "Edit");

  editToolBar_->addItem(groupItem_);
  editToolBar_->addItem(ungroupItem_);

  editToolBar_->addItem(copyItem_);
  editToolBar_->addItem(deleteItem_);
  editToolBar_->addItem(raiseItem_);
  editToolBar_->addItem(lowerItem_);

  editToolBar_->addItem(lockItem_);
  editToolBar_->addItem(unlockItem_);

  editToolBar_->addItem(undoItem_);
  editToolBar_->addItem(redoItem_);

  //-------

  layerToolBar_ = new CQToolBar(this, "Layer");

  layerToolBar_->addItem(addLayerItem_);

  //-------

  addToolBarBreak();

  //-------

  mouseToolToolBar_ = new CQToolBar(this, "Mode");

  mouseToolStack_ = new QStackedWidget;

  mouseToolToolBar_->addWidget(mouseToolStack_);

  mouseToolToolBar_->getToolBar()->setFixedHeight(ts);
  mouseToolToolBar_->getToolBar()->setMovable(false);
  mouseToolToolBar_->getToolBar()->setFloatable(false);

  mouseToolToolBar_->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

  //-------

#if 0
  consoleToolBar_ = new CQToolBar(this, "Console", Qt::BottomToolBarArea);

  //consoleToolBar_->getToolBar()->setFixedHeight(ts);
  consoleToolBar_->getToolBar()->setMovable(false);
  consoleToolBar_->getToolBar()->setFloatable(false);

  consoleToolBar_->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

  consoleEdit_ = new QLineEdit;

  consoleToolBar_->addWidget(consoleEdit_);

  connect(consoleEdit_, SIGNAL(returnPressed()), this, SLOT(consoleExecSlot()));

  consoleAction_ = consoleToolBar_->addWidget(consoleEdit_);

  consoleAction_->setVisible(false);
#endif

  //-------

  toolsToolBar_ = new CQToolBar(this, "Tools", Qt::BottomToolBarArea);

  toolsToolBar_->getToolBar()->setFixedHeight(ts);
  toolsToolBar_->getToolBar()->setMovable(false);
  toolsToolBar_->getToolBar()->setFloatable(false);

  toolsToolBar_->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

  //-------

  CQOptionToolSet *styleSet = new CQOptionToolSet;

  styleSet->setPopupArea(canvas_);

  strokeTool_ = new CQStrokeOptionTool(this);
  fillTool_   = new CQFillOptionTool(this);
//fontTool_   = new CQFontOptionTool;

  connect(strokeTool_, SIGNAL(valueChanged(const CQIllustratorShapeStroke &)),
          this, SLOT(strokeSlot(const CQIllustratorShapeStroke &)));
  connect(fillTool_  , SIGNAL(valueChanged(const CQIllustratorShapeFill &)),
          this, SLOT(fillSlot(const CQIllustratorShapeFill &)));
  connect(fillTool_  , SIGNAL(clipChanged(bool)),
          this, SLOT(clipSlot(bool)));
//connect(fontTool_  , SIGNAL(valueChanged(const QFont &)),
//        this, SLOT(fontSlot(const QFont &)));

  styleSet->addOption(strokeTool_);
  styleSet->addOption(fillTool_  );
//styleSet->addOption(fontTool_  );

  //------

  CQOptionToolSet *objectSet = new CQOptionToolSet;

  objectSet->setPopupArea(canvas_);

  propTool_   = new CQPropertiesOptionTool(this);
  objectTool_ = new CQObjectOptionTool(this);
  layerTool_  = new CQLayerOptionTool(this);

  connect(propTool_  , SIGNAL(valueChanged()),
          this, SLOT(propSlot()));
  connect(objectTool_, SIGNAL(valueChanged(const QString &)),
          this, SLOT(objectSlot(const QString &)));

  objectSet->addOption(propTool_  );
  objectSet->addOption(objectTool_);
  objectSet->addOption(layerTool_ );

  //------

  modeLabel_ = new QLabel("<small><b>Mode</b></small><br>");

  modeLabel_->setAlignment(Qt::AlignLeft);
  modeLabel_->setFixedWidth(fm.width("Set Radial Gradient"));

  posLabel_ = new QLabel("<small><b>Position</b></small><br>");

  posLabel_->setAlignment(Qt::AlignLeft);
  posLabel_->setFixedWidth(fm.width("(XXXX.XXX, XXXX.XXX)"));

  deltaLabel_ = new QLabel("<small><b>Delta</b></small><br>");

  deltaLabel_->setAlignment(Qt::AlignLeft);
  deltaLabel_->setFixedWidth(fm.width("XXXX.XXX"));

  //------

  toolsToolBar_->addWidget(styleSet);
  toolsToolBar_->addWidget(objectSet);
  toolsToolBar_->addWidget(modeLabel_);
  toolsToolBar_->addWidget(posLabel_);
  toolsToolBar_->addWidget(deltaLabel_);

  insertToolBarBreak(toolsToolBar_->getToolBar());
}

void
CQIllustrator::
createStatusBar()
{
}

void
CQIllustrator::
createDockWindows()
{
  undoDock_ = new CQIllustratorUndoDock(this);

  CQDockWidget *undoDock = new CQDockWidget(this, "Undo", undoDock_);

  undoDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  undoDock->getDockWidget()->setVisible(false);

  //------

  preferenceDock_ = new CQIllustratorPreferenceDock(this);

  CQDockWidget *preferenceDock = new CQDockWidget(this, "Preference", preferenceDock_);

  preferenceDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  preferenceDock->getDockWidget()->setVisible(false);

  //------

  snapDock_ = new CQIllustratorSnapDock(this);

  CQDockWidget *snapDock = new CQDockWidget(this, "Snap", snapDock_);

  snapDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  snapDock->getDockWidget()->setVisible(false);
}

void
CQIllustrator::
addModeToolbar(CQIllustratorMode *mode, CQIllustratorToolbar *toolbar)
{
  mouseToolStack_->addWidget(toolbar);

  mode->setStack(mouseToolStack_);

  toolbar->init();
}

void
CQIllustrator::
updateTitle()
{
  if (fileName_ != "") {
    QString fileType;

    if      (fileType_ == CFILE_TYPE_IMAGE_SVG)
      fileType = "SVG";
    else if (fileType_ == CFILE_TYPE_IMAGE_PS)
      fileType = "PS";
    else
      fileType = "None";

    setWindowTitle(QString("CQIllustrator - %1 (%2)").arg(fileName_).arg(fileType));
  }
  else
    setWindowTitle("CQIllustrator");
}

void
CQIllustrator::
resizeCanvas(int w, int h)
{
  qimage_ = QImage(w, h, QImage::Format_ARGB32);

  dim_valid_ = false;

  changed_ = true;
}

void
CQIllustrator::
mousePress(const MouseEvent &e)
{
  escape_ = false;

  press_wpos_   = e.window;
  current_ppos_ = e.pixel;

  currentMode_->mousePress(e);

  redrawOverlay();
}

void
CQIllustrator::
mouseRelease(const MouseEvent &e)
{
  if (escape_) {
    escape_ = false;

    if (isInUndoGroup()) {
      endUndoGroup();

      undoSlot();
    }

    canvas_->updateStatus();

    return;
  }

  //------

  if (isInUndoGroup())
    endUndoGroup();

  //------

  currentMode_->mouseRelease(e);

  canvas_->updateStatus();
}

void
CQIllustrator::
mouseDrag(const MouseEvent &e)
{
  //QPointF prev_wpos = getITransform().map(QPointF(current_ppos_));

  current_ppos_ = e.pixel;

  posLabel_->setText(QString("<small><b>Position</b></small><br>"
                             "(%1,%2)").arg(e.window.x()).arg(e.window.y()));

  double pdx = e.window.x() - press_wpos_.x();
  double pdy = e.window.y() - press_wpos_.y();

  QString msg = QString("(%1,%2)").arg(pdx).arg(pdy);

  setDeltaLabel("Delta", msg);

  currentMode_->mouseDrag(e);
}

void
CQIllustrator::
mouseMove(const MouseEvent &e)
{
  current_ppos_ = e.pixel;

  CPoint2D p = CQUtil::fromQPoint(e.window);

  posLabel_->setText(QString("<small><b>Position</b></small><br>"
                             "(%1,%2)").arg(p.x).arg(p.y));

  currentMode_->mouseMove(e);
}

void
CQIllustrator::
keyPress(const KeyEvent &e)
{
  if (currentMode_->keyPress(e))
    return;

  CKeyType key = e.event->getType();

  switch (key) {
    case CKEY_TYPE_BracketLeft:
      cyclePrev();
      break;
    case CKEY_TYPE_Tab:
    case CKEY_TYPE_BracketRight:
      cycleNext();
      break;
    case CKEY_TYPE_Plus:
    case CKEY_TYPE_Equal:
      zoomIn();
      break;
    case CKEY_TYPE_Minus:
    case CKEY_TYPE_Underscore:
      zoomOut();
      break;
    case CKEY_TYPE_DEL:
      deleteSlot();
      break;
    case CKEY_TYPE_Up: {
      if (! getFlipY()) {
        if (e.event->isShiftKey())
          resizeSelectedShapes(CBBox2D(0,0,0,1));
        else
          moveSelectedShapes(CPoint2D(0,1));
      }
      else {
        if (e.event->isShiftKey())
          resizeSelectedShapes(CBBox2D(0,0,0,-1));
        else
          moveSelectedShapes(CPoint2D(0,-1));
      }

      break;
    }
    case CKEY_TYPE_Down: {
      if (! getFlipY()) {
        if (e.event->isShiftKey())
          resizeSelectedShapes(CBBox2D(0,-1,0,0));
        else
          moveSelectedShapes(CPoint2D(0,-1));
      }
      else {
        if (e.event->isShiftKey())
          resizeSelectedShapes(CBBox2D(0,1,0,0));
        else
          moveSelectedShapes(CPoint2D(0,1));
      }

      break;
    }
    case CKEY_TYPE_Left:
      if (e.event->isShiftKey())
        resizeSelectedShapes(CBBox2D(-1,0,0,0));
      else
        moveSelectedShapes(CPoint2D(-1,0));

      break;
    case CKEY_TYPE_Right:
      if (e.event->isShiftKey())
        resizeSelectedShapes(CBBox2D(0,0,1,0));
      else
        moveSelectedShapes(CPoint2D(1,0));

      break;
    case CKEY_TYPE_Return:
    case CKEY_TYPE_KP_Enter:
      acceptMode();
      break;
    case CKEY_TYPE_Escape:
      cancelMode();
      break;
    case CKEY_TYPE_a:
    case CKEY_TYPE_A:
      if (e.event->isControlKey())
        selectAll();
      break;
    case CKEY_TYPE_d:
    case CKEY_TYPE_D:
      if (e.event->isControlKey())
        selectNone();
      break;
    case CKEY_TYPE_e:
    case CKEY_TYPE_E:
      setMode(Mode::ELLIPSE);
      break;
    case CKEY_TYPE_p:
    case CKEY_TYPE_P:
      if (e.event->isControlKey())
        setMode(Mode::PAN);
      else
        setMode(Mode::PATH);
      break;
    case CKEY_TYPE_q:
    case CKEY_TYPE_Q:
      quad_tree_ = ! quad_tree_;
      redraw();
      break;
    case CKEY_TYPE_r:
    case CKEY_TYPE_R:
      if (e.event->isControlKey())
        redraw();
      else
        setMode(Mode::RECT);
      break;
    case CKEY_TYPE_s:
    case CKEY_TYPE_S:
      setMode(Mode::SELECT);
      break;
    case CKEY_TYPE_t:
    case CKEY_TYPE_T:
      setMode(Mode::TEXT);
      break;
    case CKEY_TYPE_v:
    case CKEY_TYPE_V:
      if (e.event->isControlKey())
        copySlot();
      break;
    case CKEY_TYPE_z:
    case CKEY_TYPE_Z:
      setMode(Mode::ZOOM);
      break;
    default:
      break;
  }
}

void
CQIllustrator::
setDeltaLabel(const QString &title, const QString &str)
{
  QString label = QString("<small><b>%1</b></small><br>%2").arg(title).arg(str);

  deltaLabel_->setText(label);

  redraw();
}

void
CQIllustrator::
setFlipY(bool flip)
{
  flipY_ = flip;

  flipViewItem_->setChecked(flipY_);
}

const CQIllustratorData::ShapeStack &
CQIllustrator::
getShapes() const
{
  return getData()->getShapes();
}

CQIllustratorShape *
CQIllustrator::
selectAt(const CPoint2D &p, bool add, bool remove)
{
  CQIllustratorShape *select_shape = getObjectAt(p);

  if      (add)
    addSelectShape(select_shape);
  else if (remove)
    removeSelectShape(select_shape);
  else
    setSelectShape(select_shape);

  return select_shape;
}

CQIllustratorShape *
CQIllustrator::
getObjectAt(const CPoint2D &p) const
{
  return getData()->getObjectAt(p);
}

void
CQIllustrator::
selectIn(const CBBox2D &bbox, bool add, bool remove)
{
  if (! add && ! remove)
    clearSelection();

  CQIllustratorData::ShapeList shapes;

  getData()->getObjectsInside(bbox, shapes);

  selection_->startSelect();

  for (auto &shape : shapes) {
    if (! remove)
      addSelectShape(shape);
    else
      removeSelectShape(shape);
  }

  selection_->endSelect();
}

void
CQIllustrator::
selectOverlap(const CBBox2D &bbox, bool add, bool remove)
{
  if (! add && ! remove)
    clearSelection();

  CQIllustratorData::ShapeList shapes;

  getData()->getObjectsTouching(bbox, shapes);

  selection_->startSelect();

  for (auto &shape : shapes) {
    if (! remove)
      addSelectShape(shape);
    else
      removeSelectShape(shape);
  }

  selection_->endSelect();
}

void
CQIllustrator::
selectPointAt(const CPoint2D &p, CQIllustratorShape::ControlType type, bool add, bool remove)
{
  clearSelection();

  double                            dist  = 0.0;
  CQIllustratorShape               *shape = 0;
  CQIllustratorShapeNearestPoint2D  npoint;

  const CQIllustratorData::ShapeStack &shapes = getShapes();

  CQIllustratorData::ShapeStack::const_reverse_iterator ps1, ps2;

  for (ps1 = shapes.rbegin(), ps2 = shapes.rend(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape1 = *ps1;

    CQIllustratorShapeNearestPoint2D point = shape1->nearestPoint(p, type);

    if (shape == 0 || point.getDist() < dist) {
      shape  = shape1;
      npoint = point;
    }
  }

  if (shape) {
    setSelectShape(shape);

    CQIllustratorSelectedShape &sshape = selection_->front();

    if (! add && ! remove)
      selection_->clearShapePoints(sshape);

    if (! remove)
      selection_->addShapePoint(sshape, npoint.getPoint());
    else
      selection_->removeShapePoint(sshape, npoint.getPoint());
  }
}

void
CQIllustrator::
selectPointAt(CQIllustratorShape *shape, const CPoint2D &p, CQIllustratorShape::ControlType type,
              bool add, bool remove)
{
  CQIllustratorShapeNearestPoint2D npoint = shape->nearestPoint(p, type);

  if (! npoint.isSet())
    return;

  if      (add)
    addSelectShape(shape);
  else if (! remove)
    setSelectShape(shape);

  CQIllustratorSelectedShape &sshape = selection_->front();

  if (! add && ! remove)
    selection_->clearShapePoints(sshape);

  if (! remove)
    selection_->addShapePoint(sshape, npoint.getPoint());
  else
    selection_->removeShapePoint(sshape, npoint.getPoint());
}

void
CQIllustrator::
selectPointsIn(const CBBox2D &bbox, CQIllustratorShape::ControlType type,
               bool add, bool remove)
{
  if (! add && ! remove)
    clearSelection();

  CQIllustratorData::ShapeList shapes;

  getData()->getObjectsTouching(bbox, shapes);

  selection_->startSelect();

  for (auto &shape : shapes) {
    CQIllustratorShape::ControlPointList controlPoints;

    shape->getControlPoints(controlPoints, type);

    uint numControlPoints = controlPoints.size();

    for (uint i = 0; i < numControlPoints; ++i) {
      CPoint2D p = controlPoints[i]->getPoint(shape);

      if (! bbox.inside(p)) continue;

      addSelectShape(shape);

      CQIllustratorSelectedShape &sshape = selection_->get(shape);

      if (! remove)
        selection_->addShapePoint(sshape, controlPoints[i]);
      else
        selection_->removeShapePoint(sshape, controlPoints[i]);
    }
  }

  selection_->endSelect();
}

void
CQIllustrator::
selectLinesIn(const CBBox2D &bbox, bool add, bool remove)
{
  if (! add && ! remove)
    clearSelection();

  CQIllustratorData::ShapeList shapes;

  getData()->getObjectsTouching(bbox, shapes);

  selection_->startSelect();

  for (auto &shape : shapes) {
    CQIllustratorShape::ControlLineList controlLines;

    shape->getControlLines(controlLines);

    uint numControlLines = controlLines.size();

    for (uint i = 0; i < numControlLines; ++i) {
      CLine2D l = controlLines[i]->getLine(shape);

      const CPoint2D &p1 = l.start();
      const CPoint2D &p2 = l.end();

      if (! bbox.inside(p1) || ! bbox.inside(p2)) continue;

      addSelectShape(shape);

      CQIllustratorSelectedShape &sshape = selection_->get(shape);

      if (! remove)
        selection_->addShapeLine(sshape, controlLines[i]);
      else
        selection_->removeShapeLine(sshape, controlLines[i]);
    }
  }

  selection_->endSelect();
}

void
CQIllustrator::
cyclePrev()
{
  CQIllustratorShape *select_shape = 0;

  if (selection_->size() == 1)
    select_shape = selection_->front().getShape();

  CQIllustratorShape *shape = 0;

  const CQIllustratorData::ShapeStack &shapes = getShapes();

  CQIllustratorData::ShapeStack::const_reverse_iterator ps1, ps2;

  for (ps1 = shapes.rbegin(), ps2 = shapes.rend(); ps1 != ps2; ++ps1) {
    if      (select_shape == 0) {
      shape = *ps1;

      break;
    }
    else if (select_shape == *ps1) {
      ++ps1;

      if (ps1 != ps2)
        shape = *ps1;
      else
        shape = shapes.back();

      break;
    }
  }

  setSelectShape(shape);
}

void
CQIllustrator::
cycleNext()
{
  CQIllustratorShape *select_shape = 0;

  if (selection_->size() == 1)
    select_shape = selection_->front().getShape();

  CQIllustratorShape *shape = 0;

  const CQIllustratorData::ShapeStack &shapes = getShapes();

  CQIllustratorData::ShapeStack::const_iterator ps1, ps2;

  for (ps1 = shapes.begin(), ps2 = shapes.end(); ps1 != ps2; ++ps1) {
    if      (select_shape == 0) {
      shape = *ps1;

      break;
    }
    else if (select_shape == *ps1) {
      ++ps1;

      if (ps1 != ps2)
        shape = *ps1;
      else
        shape = shapes.front();

      break;
    }
  }

  setSelectShape(shape);
}

void
CQIllustrator::
draw(QPainter *painter)
{
  if (changed_) {
    QPainter ipainter(&qimage_);

    if (! canvas_->getPressed())
      ipainter.setRenderHints(QPainter::Antialiasing);

    ipainter.fillRect(qimage_.rect(), QBrush(CQUtil::rgbaToColor(bg_)));

    //ipainter.setWorldTransform(painter->worldTransform());
    ipainter.setTransform(painter->transform());

    drawContents(&ipainter);

    dim_valid_ = false;
  }

  painter->setWorldMatrixEnabled(false);

//painter->fillRect(qimage_.rect(), QBrush(CQUtil::rgbaToColor(bg_)));

  if (! dimmed_)
    painter->drawImage(QPoint(0, 0), qimage_);
  else {
    if (! dim_valid_) {
      dim_qimage_ = qimage_.copy();

      dimQImage(qimage_, dim_qimage_);

      dim_valid_ = true;
    }

    painter->drawImage(QPoint(0, 0), dim_qimage_);
  }

  painter->setWorldMatrixEnabled(true);

  if (! dimmed_)
    drawOverlay(painter);

  drawPreviewObjects(painter);

  if (quad_tree_)
    drawQuadTree(painter);

  changed_ = false;
}

void
CQIllustrator::
dimQImage(const QImage &qimage, QImage &dim_qimage)
{
  uint w = qimage.width();
  uint h = qimage.height();

  for (uint y = 0; y < h; ++y) {
    for (uint x = 0; x < w; ++x) {
      QRgb rgb = qimage.pixel(x, y);

      int g = std::min(int(127.0*(qGray(rgb)/255.0) + 128.0), 255);

      dim_qimage.setPixel(x, y, qRgb(g, g, g));
    }
  }
}

void
CQIllustrator::
drawContents(QPainter *painter)
{
  int w = qimage_.width ();
  int h = qimage_.height();

  QPointF tl = getITransform().map(QPointF(0    , h - 1));
  QPointF br = getITransform().map(QPointF(w - 1, 0    ));

  CBBox2D bbox = CQUtil::fromQRect(QRectF(tl, br));

  //------

  QPen pen;

  pen.setColor(QColor(0,0,0));
  pen.setStyle(Qt::SolidLine);

  painter->setPen(pen);

  drawer_->setPainter(painter);

  drawer_->pushMatrix(canvas_->getMatrix());

  //------

  grid_.draw(drawer_, bbox);

  //------

  const CQIllustratorLayerStack::LayerStack &layerStack = layerStack_->getLayerStack();

  CQIllustratorLayerStack::LayerStack::const_iterator p1, p2;

  for (p1 = layerStack.begin(), p2 = layerStack.end(); p1 != p2; ++p1) {
    const CQIllustratorLayer *layer = layerStack_->getLayer(*p1);

    CQIllustratorData *data = layer->getData();

    CQIllustratorData::ShapeList shapes;

    data->getOrderedObjectsTouching(bbox, shapes);

    CQIllustratorData::ShapeList::const_iterator ps1, ps2;

    for (ps1 = shapes.begin(), ps2 = shapes.end(); ps1 != ps2; ++ps1) {
      CQIllustratorShape *shape = *ps1;

      if (! shape->getVisible()) continue;

      const CBBox2D &bbox = shape->getFlatBBox();

      drawer_->setBBox(bbox);

      shape->draw(drawer_);
    }
  }

  drawer_->popMatrix();
}

void
CQIllustrator::
drawOverlay(QPainter *painter)
{
  drawer_->setPainter(painter);

  drawer_->pushMatrix(canvas_->getMatrix());

  currentMode_->drawSelection(drawer_);

  sandbox_->draw(drawer_);

  currentMode_->drawOverlay(drawer_);

  drawer_->popMatrix();
}

void
CQIllustrator::
drawQuadTree(QPainter *painter)
{
  const CQIllustratorData::QuadTree *tree = getData()->getQuadTree();

  uint max_num = tree->maxElements();

  drawQuadTree(painter, tree, max_num);
}

void
CQIllustrator::
drawQuadTree(QPainter *painter, const CQIllustratorData::QuadTree *tree, uint max_num)
{
  const CQIllustratorData::QuadTree::DataList &dataList = tree->getDataList();

  const CBBox2D &bbox = tree->getBBox();

  //uint num = dataList.size();

  //double gray = 0.1;

  //if (num > 0) gray = (1.0*num)/max_num;

  painter->fillRect(CQUtil::toQRect(bbox), QBrush(CQUtil::rgbaToColor(CRGBA(0.0,0.0,0.8,0.5))));

  uint depth = tree->getDepth();

  double r = 1.0 - 0.1*depth;

  CQIllustratorData::QuadTree::DataList::const_iterator p1 = dataList.begin();
  CQIllustratorData::QuadTree::DataList::const_iterator p2 = dataList.end  ();

  for ( ; p1 != p2; ++p1) {
    const CBBox2D &bbox = (*p1)->getBBox();

    painter->fillRect(CQUtil::toQRect(bbox), QBrush(CQUtil::rgbaToColor(CRGBA(0.0,0.0,r,0.5))));
  }

  if (tree->getBLTree() != 0) {
    const CQIllustratorData::QuadTree *bl_tree = tree->getBLTree();
    const CQIllustratorData::QuadTree *br_tree = tree->getBRTree();
    const CQIllustratorData::QuadTree *tl_tree = tree->getTLTree();
    const CQIllustratorData::QuadTree *tr_tree = tree->getTRTree();

    drawQuadTree(painter, bl_tree, max_num);
    drawQuadTree(painter, br_tree, max_num);
    drawQuadTree(painter, tl_tree, max_num);
    drawQuadTree(painter, tr_tree, max_num);
  }
}

void
CQIllustrator::
anchorObjectSlot()
{
  setMode(Mode::ANCHOR_OBJECT);
}

void
CQIllustrator::
anchorPositionSlot()
{
  setMode(Mode::ANCHOR_POSITION);
}

void
CQIllustrator::
cancelAnchorSlot()
{
  setMode(Mode::SELECT);
}

void
CQIllustrator::
selectAllSlot()
{
  selectAll();
}

void
CQIllustrator::
selectNoneSlot()
{
  selectNone();
}

void
CQIllustrator::
selectAll()
{
  selection_->startSelect();

  clearSelection();

  for (const auto &shape : getShapes())
    addSelectShape(shape);

  selection_->endSelect();

  redrawOverlay();
}

void
CQIllustrator::
selectNone()
{
  clearSelection();

  redrawOverlay();
}

void
CQIllustrator::
addCommand(CQIllustratorCmd *cmd)
{
  cmdMgr_->addCommand(cmd);
}

bool
CQIllustrator::
loadCmd(const std::string &filename)
{
  CFile file(filename);

  std::vector<std::string> lines;

  if (! file.toLines(lines))
    return false;

  uint num_lines = lines.size();

  for (uint i = 0; i < num_lines; ++i)
    execCommand(lines[i]);

  return true;
}

bool
CQIllustrator::
execCommand(const std::string &cmd)
{
  return cmdMgr_->execCmd(cmd);
}

void
CQIllustrator::
setCursor(QCursor cursor)
{
  canvas_->setCursor(cursor);
}

void
CQIllustrator::
setDimmed(bool dimmed)
{
  dimmed_ = dimmed;
}

void
CQIllustrator::
groupSlot()
{
  if (selection_->empty()) return;

  CQIllustratorGroupShape *group = createGroupShape();

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    shape->setParent(group);

    // TODO: this is not removing from database just moving in hierarchy
    //removeShape(shape);
  }

  addShape(group);

  setSelectShape(group);
}

void
CQIllustrator::
ungroupSlot()
{
  std::vector<CQIllustratorShape *> groups;

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    // get parent group matrix
    CQIllustratorShape *shape = (*ps1).getShape();

    const CQIllustratorGroupShape *group = dynamic_cast<CQIllustratorGroupShape *>(shape);
    if (! group) continue;

    const CMatrix2D &m = group->getMatrix();

    //---

    // adjust child shape matrices
    std::vector<CQIllustratorShape *> shapes;

    for (const auto &groupShape : group->getChildren()) {
      const CMatrix2D &m1 = groupShape->getMatrix();

      groupShape->setMatrix(m*m1);

      shapes.push_back(groupShape);
    }

    //---

    // save group
    groups.push_back(shape);

    //---

    // move shapes to new parent (or none)
    CQIllustratorShape *parent = group->getParent();

    for (auto &groupShape : shapes)
      groupShape->setParent(parent);
  }

  //---

  // reparent groups (now empty)
  for (auto &group : groups) {
    CQIllustratorShape *parent = group->getParent();

    group->setParent(parent);
  }

  //---

  // reset selection
  clearSelection();
}

void
CQIllustrator::
copySlot()
{
  std::vector<CQIllustratorShape *> shapes;

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    CQIllustratorShape *shape1 = shape->dup();

    //shape1->moveBy(CPoint2D(1,1));

    getData()->addShape(shape1);

    shapes.push_back(shape1);
  }

  selection_->startSelect();

  clearSelection();

  std::vector<CQIllustratorShape *>::const_iterator pcs1, pcs2;

  for (pcs1 = shapes.begin(), pcs2 = shapes.end(); pcs1 != pcs2; ++pcs1) {
    CQIllustratorShape *shape = *pcs1;

    addSelectShape(shape);
  }

  selection_->endSelect();

  redraw();
}

void
CQIllustrator::
deleteSlot()
{
  currentMode_->deleteCurrent();
}

void
CQIllustrator::
raiseSlot()
{
  if (selection_->size() == 1) {
    CQIllustratorShape *shape = selection_->front().getShape();

    getData()->raiseShape(shape);

    redraw();
  }
}

void
CQIllustrator::
lowerSlot()
{
  if (selection_->size() == 1) {
    CQIllustratorShape *shape = selection_->front().getShape();

    getData()->lowerShape(shape);

    redraw();
  }
}

void
CQIllustrator::
flipXSlot()
{
  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    checkoutShape(shape, CQIllustratorData::ChangeType::GEOMETRY);

    shape->flip(true);

    checkinShape(shape, CQIllustratorData::ChangeType::GEOMETRY);
  }
}

void
CQIllustrator::
flipYSlot()
{
  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    checkoutShape(shape, CQIllustratorData::ChangeType::GEOMETRY);

    shape->flip(false);

    checkinShape(shape, CQIllustratorData::ChangeType::GEOMETRY);
  }
}

void
CQIllustrator::
lockSlot()
{
  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    checkoutShape(shape, CQIllustratorData::ChangeType::GEOMETRY);

    shape->setFixed(true);

    checkinShape(shape, CQIllustratorData::ChangeType::GEOMETRY);
  }
}

void
CQIllustrator::
unlockSlot()
{
  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    checkoutShape(shape, CQIllustratorData::ChangeType::GEOMETRY);

    shape->setFixed(false);

    checkinShape(shape, CQIllustratorData::ChangeType::GEOMETRY);
  }
}

void
CQIllustrator::
copyStrokeSlot()
{
  if (selection_->size() == 1) {
    CQIllustratorShape *shape = selection_->front().getShape();

    save_stroke_ = shape->getStroke();
    save_fill_   = shape->getFill  ();
  }
}

void
CQIllustrator::
pasteStrokeSlot()
{
  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    //------

    checkoutShape(shape, CQIllustratorData::ChangeType::STROKE);

    shape->setStroke(save_stroke_);

    checkinShape(shape, CQIllustratorData::ChangeType::STROKE);

    //------

    checkoutShape(shape, CQIllustratorData::ChangeType::FILL);

    shape->setFill(save_fill_);

    checkinShape(shape, CQIllustratorData::ChangeType::FILL);
  }
}

void
CQIllustrator::
loadFileSlot()
{
  QString title  = "Load Vector File";
  QString cwd    = QString(COSFile::getCurrentDir().c_str());
  QString filter = "SVG, Postscript, Cmd (*.svg *.ps *.cmd)";

  QStringList filenames = QFileDialog::getOpenFileNames(this, title, cwd, filter);

  if (filenames.size() == 0)
    return;

  QStringListIterator fi(filenames);

  while (fi.hasNext())
    loadFile(fi.next());
}

void
CQIllustrator::
saveSVGSlot()
{
  QString title  = "Save SVG File";
  QString cwd    = QString(COSFile::getCurrentDir().c_str());
  QString filter = "SVG (*.svg)";

  QString fileName = QFileDialog::getSaveFileName(this, title, cwd, filter);

  if (fileName.length() == 0)
    return;

  saveSVG(fileName);
}

void
CQIllustrator::
saveCmdSlot()
{
  QString title  = "Save Cmd File";
  QString cwd    = QString(COSFile::getCurrentDir().c_str());
  QString filter = "Cmd (*.cmd)";

  QString fileName = QFileDialog::getSaveFileName(this, title, cwd, filter);

  if (fileName.length() == 0)
    return;

  saveCmd(fileName);
}

void
CQIllustrator::
setDefaultStroke(const CQIllustratorShapeStroke &stroke)
{
  def_stroke_ = stroke;
}

const CQIllustratorShapeStroke &
CQIllustrator::
getDefaultStroke() const
{
  return def_stroke_;
}

void
CQIllustrator::
setDefaultFill(const CQIllustratorShapeFill &fill)
{
  def_fill_ = fill;
}

const CQIllustratorShapeFill &
CQIllustrator::
getDefaultFill() const
{
  return def_fill_;
}

void
CQIllustrator::
acceptMode()
{
  currentMode_->accept();
}

void
CQIllustrator::
cancelMode()
{
  escape_ = true;

  if (currentMode_->cancel()) {
    setMode(Mode::SELECT);

    setCursor(Qt::ArrowCursor);
  }
}

const QTransform &
CQIllustrator::
getTransform() const
{
  return canvas_->getTransform();
}

const QTransform &
CQIllustrator::
getITransform() const
{
  return canvas_->getITransform();
}

CQIllustratorData *
CQIllustrator::
getData() const
{
  return layerStack_->getCurrentData();
}

void
CQIllustrator::
setMode(Mode mode)
{
  static bool locked;

  if (locked) return;

  locked = true;

  //------

  if (currentMode_)
    currentMode_->showToolbar(false);

  //------

  mode_ = mode;

  currentMode_ = getMode(mode_);

  ModeMap::const_iterator p1, p2;

  for (p1 = modeMap_.begin(), p2 = modeMap_.end(); p1 != p2; ++p1) {
    CQMenuItem *menuItem = (*p1).second->getMenuItem();

    if (menuItem)
      menuItem->setChecked(mode_ == (*p1).first);
  }

  modeLabel_->setText(QString("<small><b>Mode</b></small><br>"
                              "%1").arg(currentMode_->getTitle()));

  setCursor(currentMode_->getCursor());

  if (mode_ != Mode::ANCHOR_OBJECT && mode_ != Mode::ANCHOR_POSITION)
    getAlignToolbar()->resetSelectMode();

  redrawOverlay();

  //------

  if (currentMode_)
    currentMode_->showToolbar(true);

  //------

  locked = false;
}

void
CQIllustrator::
setBackground(const CRGBA &bg)
{
  bg_ = bg;

  redraw();
}

QColor
CQIllustrator::
getQBackground() const
{
  return CQUtil::rgbaToColor(bg_);
}

void
CQIllustrator::
setQBackground(const QColor &c)
{
  bg_ = CQUtil::colorToRGBA(c);
}

void
CQIllustrator::
showConsole()
{
  if (consoleAction_)
    consoleAction_->setVisible(true);

  if (consoleEdit_)
    consoleEdit_->setFocus();
}

void
CQIllustrator::
consoleExecSlot()
{
  if (consoleEdit_) {
    execCommand(consoleEdit_->text().toStdString());

    consoleEdit_->setText("");
  }

  if (consoleAction_)
    consoleAction_->setVisible(false);

  setCanvasFocus();
}

void
CQIllustrator::
setCanvasFocus()
{
  canvas_->setFocus();
}

void
CQIllustrator::
loadFile(const QString &filename)
{
  std::string filename1 = filename.toStdString();

  CFile file(filename1);

  if (file.getSuffix() == "cmd") {
    loadCmd(filename1);
  }
  else {
    CFileType type = CFileUtil::getType(filename.toStdString());

    if (type != CFILE_TYPE_IMAGE_SVG && type != CFILE_TYPE_IMAGE_PS)
      type = CFILE_TYPE_IMAGE_SVG;

    if      (type == CFILE_TYPE_IMAGE_SVG)
      loadSVG(filename);
    else if (type == CFILE_TYPE_IMAGE_PS)
      loadPS(filename);

    zoomFitSlot();
  }
}

void
CQIllustrator::
loadSVG(const QString &filename)
{
#ifndef SVG_FLIP
  setFlipY(true);
#endif

  CSVG svg;

  CQSVGRenderer renderer;

  svg.setRenderer(&renderer);

  svg.setAutoName(true);

  if (! svg.read(filename.toStdString()))
    return;

  startUndoGroup("Load SVG");

#ifdef SVG_FLIP
  CPoint2D c(0,0);
#endif

  CSVGBlock *block = svg.getBlock();

  for (const auto &object : block->children()) {
    CQIllustratorShape *shape = addSVGObject(0, object);

    if (! shape) continue;

#ifdef SVG_FLIP
    CPoint2D c1 = shape->getRotateCenter();

    shape->setRotateCenter(c);

    shape->flip();

    shape->setRotateCenter(c1);
#endif

    setSVGShapeName(shape, object);

    addShape(shape);
  }

  endUndoGroup();

  //------

  fileType_ = CFILE_TYPE_IMAGE_SVG;
  fileName_ = filename;

  updateTitle();
}

void
CQIllustrator::
loadPS(const QString &filename)
{
#ifdef PSVIEW
  setFlipY(true);

  startUndoGroup("Load PS");

  CPSViewShapeRenderer renderer(this);

  CPSView psview;

  psview.setRenderer(&renderer);

  psview.openCurrentFile(filename.toStdString());

  psview.executeCurrentFile();

  psview.closeCurrentFile();

  endUndoGroup();

  //------

  fileType_ = CFILE_TYPE_IMAGE_PS;
  fileName_ = filename;

  updateTitle();
#endif
}

void
CQIllustrator::
saveSVG(const QString &filename)
{
  //CPoint2D c = bbox.getCenter();

  CFile file(filename.toStdString());

  if (! file.open(CFileBase::WRITE))
    return;

  CQIllustratorSaveData saveData;

  saveData.bbox = getBBox();
  saveData.file = &file;

  file.printf("<svg>\n");

  const CQIllustratorData::ShapeStack &shapes = getShapes();

  CQIllustratorData::ShapeStack::const_iterator ps1, ps2;

  for (ps1 = shapes.begin(), ps2 = shapes.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    if (shape->getParent()) continue;

    //CPoint2D c1 = shape->getRotateCenter();

    //shape->setRotateCenter(c);

    //shape->flip();

    shape->saveSVG(saveData);

    //shape->flip();

    //shape->setRotateCenter(c1);
  }

  file.printf("</svg>\n");

  file.flush();

  file.close();

  //------

  fileType_ = CFILE_TYPE_IMAGE_PS;
  fileName_ = filename;

  updateTitle();
}

void
CQIllustrator::
saveCmd(const QString &filename)
{
  CFile file(filename.toStdString());

  if (! file.open(CFileBase::WRITE))
    return;

  CQIllustratorSaveData saveData;

  saveData.bbox = getBBox();
  saveData.file = &file;

  const CQIllustratorData::ShapeStack &shapes = getShapes();

  CQIllustratorData::ShapeStack::const_iterator ps1, ps2;

  for (ps1 = shapes.begin(), ps2 = shapes.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    if (shape->getParent()) continue;

    shape->saveCmd(saveData);
  }

  file.flush();

  file.close();

  //------

  fileType_ = CFILE_TYPE_IMAGE_PS;
  fileName_ = filename;

  updateTitle();
}

CQIllustratorShape *
CQIllustrator::
addSVGObject(CSVGObject *, CSVGObject *object, bool force)
{
  if (! force) {
    if (! object->isHierDrawable())
      return 0;

    if (! object->isDrawable())
      return 0;
  }

  //---

  CMatrix2D m = object->getTransform().getMatrix();

  CQIllustratorShape *shape = 0;

  CSVGObjTypeId id = object->getObjTypeId();

  switch (id) {
    case CSVGObjTypeId::PATH: {
      CPoint2D lp(0,0);

      CQIllustratorPathShape *pathShape = createPathShape();

      pathShape->startGroup();

      shape = pathShape;

      CSVGPath *path = dynamic_cast<CSVGPath *>(object);

      const CSVGPathPartList &parts = path->getPartList();

      for (const auto part : parts.parts()) {
        CSVGPathPartType type = part->getType();

        switch (type) {
          case CSVGPathPartType::MOVE_TO: {
            CSVGPathMoveTo const *pp = dynamic_cast<CSVGPathMoveTo const *>(part);

            CPoint2D pp1 = pp->getPoint();

            pathShape->addMoveTo(pp1);

            lp = pp1;

            break;
          }
          case CSVGPathPartType::RMOVE_TO: {
            CSVGPathRMoveTo const *pp = dynamic_cast<CSVGPathRMoveTo const *>(part);

            CPoint2D pp1 = lp + pp->getPoint();

            pathShape->addMoveTo(pp1);

            lp = pp1;

            break;
          }
          case CSVGPathPartType::LINE_TO: {
            CSVGPathLineTo const *pp = dynamic_cast<CSVGPathLineTo const *>(part);

            CPoint2D pp1 = pp->getPoint();

            pathShape->addLineTo(pp1);

            lp = pp1;

            break;
          }
          case CSVGPathPartType::RLINE_TO: {
            CSVGPathRLineTo const *pp = dynamic_cast<CSVGPathRLineTo const *>(part);

            CPoint2D pp1 = lp + pp->getPoint();

            pathShape->addLineTo(pp1);

            lp = pp1;

            break;
          }
          case CSVGPathPartType::HLINE_TO: {
            CSVGPathHLineTo const *pp = dynamic_cast<CSVGPathHLineTo const *>(part);

            CPoint2D pp1 = CPoint2D(pp->getX(), lp.y);

            pathShape->addLineTo(pp1);

            lp = pp1;

            break;
          }
          case CSVGPathPartType::RHLINE_TO: {
            CSVGPathRHLineTo const *pp = dynamic_cast<CSVGPathRHLineTo const *>(part);

            CPoint2D pp1 = lp + CPoint2D(pp->getDistance(), 0);

            pathShape->addLineTo(pp1);

            lp = pp1;

            break;
          }
          case CSVGPathPartType::VLINE_TO: {
            CSVGPathVLineTo const *pp = dynamic_cast<CSVGPathVLineTo const *>(part);

            CPoint2D pp1 = lp + CPoint2D(lp.x, pp->getY());

            pathShape->addLineTo(pp1);

            lp = pp1;

            break;
          }
          case CSVGPathPartType::RVLINE_TO: {
            CSVGPathRVLineTo const *pp = dynamic_cast<CSVGPathRVLineTo const *>(part);

            CPoint2D pp1 = lp + CPoint2D(0, pp->getDistance());

            pathShape->addLineTo(pp1);

            lp = pp1;

            break;
          }
          case CSVGPathPartType::ARC_TO: {
            CSVGPathArcTo const *pp = dynamic_cast<CSVGPathArcTo const *>(part);

            double xa = pp->getXA();
            int    fa = pp->getFA();
            int    fs = pp->getFS();

            CPoint2D r = CPoint2D(pp->getRadiusX(), pp->getRadiusY());

            double cx, cy, rx, ry, theta, delta;

            bool unit_circle = false;

            CPoint2D pp2 = pp->getPoint2();

            CSVGUtil::convertArcCoords(lp.x, lp.y, pp2.x, pp2.y, xa, r.x, r.y, fa, fs,
                                       unit_circle, &cx, &cy, &rx, &ry, &theta, &delta);

            CPoint2D c(cx, cy);

            pathShape->addArcTo(c, rx, ry, CMathGen::DegToRad(theta),
                                CMathGen::DegToRad(theta + delta));

            lp = pp2;

            break;
          }
          case CSVGPathPartType::RARC_TO: {
            CSVGPathRArcTo const *pp = dynamic_cast<CSVGPathRArcTo const *>(part);

            double xa = pp->getXA();
            int    fa = pp->getFA();
            int    fs = pp->getFS();

            CPoint2D r = CPoint2D(pp->getRadiusX(), pp->getRadiusY());

            double cx, cy, rx, ry, theta, delta;

            bool unit_circle = false;

            CPoint2D pp2 = lp + pp->getPoint2();

            CSVGUtil::convertArcCoords(lp.x, lp.y, pp2.x, pp2.y, xa, r.x, r.y, fa, fs,
                                       unit_circle, &cx, &cy, &rx, &ry, &theta, &delta);

            CPoint2D c(cx, cy);

            pathShape->addArcTo(c, rx, ry, CMathGen::DegToRad(theta),
                                CMathGen::DegToRad(theta + delta));

            lp = pp2;

            break;
          }
          case CSVGPathPartType::BEZIER2_TO: {
            CSVGPathBezier2To const *pp = dynamic_cast<CSVGPathBezier2To const *>(part);

            pathShape->addCurveTo(pp->getPoint1(), pp->getPoint2());

            lp = pp->getPoint2();

            break;
          }
          case CSVGPathPartType::MBEZIER2_TO: {
           //CSVGPathMBezier2To const *pp = dynamic_cast<CSVGPathMBezier2To const *>(part);

           std::cerr << "MBEZIER2_TO not handled" << std::endl;

           break;
          }
          case CSVGPathPartType::RBEZIER2_TO: {
            CSVGPathRBezier2To const *pp = dynamic_cast<CSVGPathRBezier2To const *>(part);

            pathShape->addCurveTo(lp + pp->getPoint1(), lp + pp->getPoint2());

            lp = lp + pp->getPoint2();

            break;
          }
          case CSVGPathPartType::MRBEZIER2_TO: {
            //CSVGPathMRBezier2To const *pp = dynamic_cast<CSVGPathMRBezier2To const *>(part);

            std::cerr << "MRBEZIER2_TO not handled" << std::endl;

            break;
          }
          case CSVGPathPartType::BEZIER3_TO: {
            CSVGPathBezier3To const *pp = dynamic_cast<CSVGPathBezier3To const *>(part);

            pathShape->addCurveTo(pp->getPoint1(), pp->getPoint2(), pp->getPoint3());

            lp = pp->getPoint3();

            break;
          }
          case CSVGPathPartType::MBEZIER3_TO: {
            //CSVGPathMBezier3To const *pp = dynamic_cast<CSVGPathMBezier3To const *>(part);

            std::cerr << "MBEZIER3_TO not handled" << std::endl;

            break;
          }
          case CSVGPathPartType::RBEZIER3_TO: {
            CSVGPathRBezier3To const *pp = dynamic_cast<CSVGPathRBezier3To const *>(part);

            pathShape->addCurveTo(lp + pp->getPoint1(), lp + pp->getPoint2(), lp + pp->getPoint3());

            lp = lp + pp->getPoint3();

            break;
          }
          case CSVGPathPartType::MRBEZIER3_TO: {
            //CSVGPathMRBezier3To const *pp = dynamic_cast<CSVGPathMRBezier3To const *>(part);

            std::cerr << "MRBEZIER3_TO not handled" << std::endl;

            break;
          }
          case CSVGPathPartType::CLOSE_PATH: {
            pathShape->addClose();

            break;
          }
          default:
            std::cerr << "Unhandled path part " << uint(type) << std::endl;
            break;
        }
      }

      pathShape->endGroup();

      setShapeSVGStrokeAndFill(pathShape, object);

      break;
    }
    case CSVGObjTypeId::POLYGON: {
      CQIllustratorPathShape *pathShape = createPathShape();

      shape = pathShape;

      CSVGPolygon *polygon = dynamic_cast<CSVGPolygon *>(object);

      const CSVGPolygon::PointList &points = polygon->getPoints();

      uint num_points = points.size();

      CPoint2D pp1 = points[0];

      pathShape->addMoveTo(pp1);

      for (uint i = 1; i < num_points; ++i) {
        CPoint2D pp1 = points[i];

        pathShape->addLineTo(pp1);
      }

      pathShape->addClose();

      setShapeSVGStrokeAndFill(pathShape, object);

      break;
    }
    case CSVGObjTypeId::RECT: {
      CSVGRect *rect = dynamic_cast<CSVGRect *>(object);

      const CBBox2D &bbox = rect->getBBox();

      //---

      // option corner radius (1 or 2 values)
      double rx = 0, ry = 0;

      if (rect->hasRX() && rect->hasRY()) {
        rx = fabs(rect->getRX());
        ry = fabs(rect->getRY());
      }
      else if (rect->hasRX()) {
        rx = fabs(rect->getRX());
        ry = rx;
      }
      else if (rect->hasRY()) {
        ry = fabs(rect->getRY());
        rx = ry;
      }

      //---

      CPoint2D p1 = bbox.getUL();
      CPoint2D p2 = bbox.getLR();

      CQIllustratorRectShape *rectShape = createRectShape(p1, p2);

      shape = rectShape;

      //---

      if (rx > 1E-6 && ry >= 1E-6) {
        rectShape->setRadiusX(rx);
        rectShape->setRadiusY(ry);
      }

      setShapeSVGStrokeAndFill(rectShape, object);

      break;
    }
    case CSVGObjTypeId::CIRCLE: {
      CSVGCircle *circle = dynamic_cast<CSVGCircle *>(object);

      const CPoint2D &c = circle->getCenter();
      double          r = circle->getRadius();

      CBBox2D bbox(CPoint2D(c.x - r, c.y - r), CPoint2D(c.x + r, c.y + r));

      CPoint2D p1 = bbox.getUL();
      CPoint2D p2 = bbox.getLR();

      CQIllustratorEllipseShape *ellipseShape = createEllipseShape(p1, p2);

      shape = ellipseShape;

      setShapeSVGStrokeAndFill(ellipseShape, object);

      break;
    }
    case CSVGObjTypeId::ELLIPSE: {
      CSVGEllipse *ellipse = dynamic_cast<CSVGEllipse *>(object);

      const CPoint2D &c  = ellipse->getCenter();
      double          rx = ellipse->getRadiusX();
      double          ry = ellipse->getRadiusY();

      CBBox2D bbox(CPoint2D(c.x - rx, c.y - ry), CPoint2D(c.x + rx, c.y + ry));

      CPoint2D p1 = bbox.getUL();
      CPoint2D p2 = bbox.getLR();

      CQIllustratorEllipseShape *ellipseShape = createEllipseShape(p1, p2);

      shape = ellipseShape;

      setShapeSVGStrokeAndFill(ellipseShape, object);

      break;
    }
    case CSVGObjTypeId::LINE: {
      CQIllustratorPathShape *pathShape = createPathShape();

      shape = pathShape;

      CSVGLine *line = dynamic_cast<CSVGLine *>(object);

      CPoint2D p1 = line->getStart();
      CPoint2D p2 = line->getEnd  ();

      pathShape->addMoveTo(p1);
      pathShape->addLineTo(p2);

      setShapeSVGStrokeAndFill(pathShape, object);

      break;
    }
    case CSVGObjTypeId::POLYLINE: {
      CQIllustratorPathShape *pathShape = createPathShape();

      shape = pathShape;

      CSVGPolyLine *poly = dynamic_cast<CSVGPolyLine *>(object);

      const CSVGPolyLine::PointList &points = poly->getPoints();

      uint numPoints = points.size();

      if (numPoints > 0) {
        CPoint2D p = points[0];

        pathShape->addMoveTo(p);
      }

      for (uint i = 1; i < numPoints; ++i) {
        CPoint2D p = points[i];

        pathShape->addLineTo(p);
      }

      setShapeSVGStrokeAndFill(pathShape, object);

      break;
    }
    case CSVGObjTypeId::TEXT: {
      CSVGText *text = dynamic_cast<CSVGText *>(object);

      CBBox2D bbox;

      text->getBBox(bbox);

      const std::string &str = text->getText();

      CQIllustratorTextShape *textShape = createTextShape(bbox.getLL(), bbox.getUR(), str);

      textShape->setFont(object->getFont());

      shape = textShape;

      setShapeSVGStrokeAndFill(textShape, object);

      //---

      for (const auto &childObject : object->children()) {
        CSVGTSpan *tspan = dynamic_cast<CSVGTSpan *>(childObject);

        if (! tspan)
          continue;

        const std::string &str = tspan->getText();

        if (textShape->getText() == "")
          textShape->setText(str);
        else
          textShape->setText(textShape->getText() + " " + str);
      }

      break;
    }
    case CSVGObjTypeId::TSPAN: {
      break;
    }
    case CSVGObjTypeId::IMAGE: {
      CSVGImage *image = dynamic_cast<CSVGImage *>(object);

      CBBox2D bbox;

      image->getBBox(bbox);

      CSVGObject *childObject = image->getObject();

      if (childObject) {
        CQIllustratorGroupShape *groupShape = createGroupShape();

        shape = groupShape;

        setShapeSVGStrokeAndFill(groupShape, object);

        CQIllustratorShape *childShape = addSVGObject(object, childObject);

        if (childShape) {
          groupShape->addChild(childShape);

          setSVGShapeName(childShape, childObject);

          addShape(childShape);

          childShape->setParent(groupShape);
        }
      }
      else {
        CPoint2D p1 = bbox.getUL();
        CPoint2D p2 = bbox.getLR();

        CQIllustratorRectShape *rectShape = createRectShape(p1, p2);

        shape = rectShape;

        setShapeSVGStrokeAndFill(rectShape, object);

        CImagePtr img = image->getImage();

        if (img.isValid()) {
          CQIllustratorShapeFill &fill = shape->getFill();

          fill.setImage(img);

          fill.setImageScale(CQIllustratorShapeFill::ImageScale::FIT);
        }
      }

      break;
    }
    case CSVGObjTypeId::TITLE: {
      break;
    }
    case CSVGObjTypeId::USE: {
      CQIllustratorGroupShape *groupShape = createGroupShape();

      shape = groupShape;

      setShapeSVGStrokeAndFill(groupShape, object);

      CSVGUse *use = dynamic_cast<CSVGUse *>(object);

      CSVGObject *childObject = use->getLinkObject();

      CSVGObject *parent = childObject->getParent();

      childObject->setParent(use);

      CQIllustratorShape *childShape = addSVGObject(object, childObject, /*force*/true);

      if (childShape) {
        groupShape->addChild(childShape);

        setSVGShapeName(childShape, childObject);

        addShape(childShape);

        childShape->setParent(groupShape);
      }

      childObject->setParent(parent);

      break;
    }
    case CSVGObjTypeId::GROUP: {
      CQIllustratorGroupShape *groupShape = createGroupShape();

      shape = groupShape;

      setShapeSVGStrokeAndFill(groupShape, object);

      for (const auto &childObject : object->children()) {
        CQIllustratorShape *childShape = addSVGObject(object, childObject);

        if (! childShape) continue;

        groupShape->addChild(childShape);

        setSVGShapeName(childShape, childObject);

        addShape(childShape);

        childShape->setParent(groupShape);
      }

      break;
    }
    case CSVGObjTypeId::SYMBOL: {
      CQIllustratorGroupShape *groupShape = createGroupShape();

      shape = groupShape;

      setShapeSVGStrokeAndFill(groupShape, object);

      for (const auto &childObject : object->children()) {
        CQIllustratorShape *childShape = addSVGObject(object, childObject);

        if (! childShape) continue;

        groupShape->addChild(childShape);

        setSVGShapeName(childShape, childObject);

        addShape(childShape);

        childShape->setParent(groupShape);
      }

      break;
    }
    default:
      std::cerr << "Unhandled: " << object->getObjType().getName() << std::endl;
      break;
  }

  if (shape)
    shape->setMatrix(m);

  return shape;
}

void
CQIllustrator::
snapShotSlot()
{
  qimage_.save("snapshot.png", "PNG");
}

void
CQIllustrator::
objectsSlot()
{
  if (! propertiesDlg_)
    propertiesDlg_ = new CQIllustratorPropertiesDlg(this);

  propertiesDlg_->load();

  propertiesDlg_->show();
}

bool
CQIllustrator::
isInUndoGroup() const
{
  return undo_->isInGroup();
}

void
CQIllustrator::
startUndoGroup(const std::string &str)
{
  undo_->startGroup();

  undo_->getCurrentGroup()->setDesc(str);
}

void
CQIllustrator::
endUndoGroup()
{
  undo_->endGroup();
}

void
CQIllustrator::
undoSlot()
{
  undo_->undo();
}

void
CQIllustrator::
redoSlot()
{
  undo_->redo();
}

void
CQIllustrator::
undoChangedSlot()
{
  redraw();

  undoItem_->setEnabled(undo_->canUndo());
  redoItem_->setEnabled(undo_->canRedo());
}

void
CQIllustrator::
addLayerSlot()
{
  addLayer();
}

void
CQIllustrator::
zoomFullSlot()
{
  zoomFull();
}

void
CQIllustrator::
zoomFitSlot()
{
  zoomFit();
}

void
CQIllustrator::
zoomSelSlot()
{
  zoomSelected();
}

void
CQIllustrator::
gridSlot()
{
  grid_.setEnabled(! grid_.getEnabled());

  redraw();
}

void
CQIllustrator::
flipViewSlot(bool flip)
{
  setFlipY(flip);
}

void
CQIllustrator::
toPolygonSlot()
{
  std::vector<CQIllustratorShape *> old_shapes;
  std::vector<CQIllustratorShape *> new_shapes;

  startUndoGroup("To Polygon");

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    CQIllustratorGroupShape *group = dynamic_cast<CQIllustratorGroupShape *>(shape);

    if (group) continue;

    CPolygon2D polygon;

    if (! shape->getPolygon(polygon))
      continue;

    CQIllustratorPolygonShape *pshape = createPolygonShape();

    pshape->setStroke(shape->getStroke());
    pshape->setFill  (shape->getFill ());

    pshape->setPoints(polygon.getPoints());

    addShape(pshape);

    old_shapes.push_back(shape);
    new_shapes.push_back(pshape);
  }

  endUndoGroup();

  uint num_old_shapes = old_shapes.size();

  for (uint i = 0; i < num_old_shapes; ++i)
    removeShape(old_shapes[i]);

  setSelectShape(0);

  uint num_new_shapes = new_shapes.size();

  for (uint i = 0; i < num_new_shapes; ++i)
    addSelectShape(new_shapes[i]);
}

void
CQIllustrator::
toPathSlot()
{
  std::vector<CQIllustratorShape *> old_shapes;
  std::vector<CQIllustratorShape *> new_shapes;

  startUndoGroup("To Path");

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    CQIllustratorGroupShape *group = dynamic_cast<CQIllustratorGroupShape *>(shape);

    if (group) continue;

    CPathShapePartList parts;

    if (! shape->getPath(parts))
      continue;

    CQIllustratorPathShape *pshape = createPathShape();

    pshape->setParts(parts);

    pshape->setStroke(shape->getStroke());
    pshape->setFill  (shape->getFill ());

    addShape(pshape);

    old_shapes.push_back(shape);
    new_shapes.push_back(pshape);
  }

  endUndoGroup();

  uint num_old_shapes = old_shapes.size();

  for (uint i = 0; i < num_old_shapes; ++i)
    removeShape(old_shapes[i]);

  setSelectShape(0);

  uint num_new_shapes = new_shapes.size();

  for (uint i = 0; i < num_new_shapes; ++i)
    addSelectShape(new_shapes[i]);
}

void
CQIllustrator::
toCurveSlot()
{
  toCurve();
}

void
CQIllustrator::
toCurve()
{
  std::vector<CQIllustratorShape *> old_shapes;
  std::vector<CQIllustratorShape *> new_shapes;

  startUndoGroup("To Curve");

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    CQIllustratorGroupShape *group = dynamic_cast<CQIllustratorGroupShape *>(shape);

    if (group) continue;

    CPathShapePartList parts;

    if (! shape->getPath(parts))
      continue;

    CPathShapePartList parts1;

    if (! parts.removeArcs(parts1))
      parts1 = parts;

    CQIllustratorPathShape *pshape = createPathShape();

    pshape->setParts(parts1);

    pshape->setStroke(shape->getStroke());
    pshape->setFill  (shape->getFill ());

    addShape(pshape);

    old_shapes.push_back(shape);
    new_shapes.push_back(pshape);
  }

  endUndoGroup();

  uint num_old_shapes = old_shapes.size();

  for (uint i = 0; i < num_old_shapes; ++i)
    removeShape(old_shapes[i]);

  setSelectShape(0);

  uint num_new_shapes = new_shapes.size();

  for (uint i = 0; i < num_new_shapes; ++i)
    addSelectShape(new_shapes[i]);
}

void
CQIllustrator::
offsetPathSlot()
{
  offsetPath(10);
}

void
CQIllustrator::
offsetPath(double o)
{
  std::vector<CQIllustratorShape *> new_shapes;

  startUndoGroup("Offset Path");

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    CQIllustratorGroupShape *group = dynamic_cast<CQIllustratorGroupShape *>(shape);

    if (group) continue;

    CPathShapePartList parts;

    if (! shape->getPath(parts)) continue;

    CPathShapePartList parts1;

    parts.offsetPath(o, shape->getCenter(), parts1);

    CQIllustratorPathShape *pshape = createPathShape();

    pshape->setParts(parts1);

    pshape->setStroke(shape->getStroke());
    pshape->setFill  (shape->getFill ());

    addShape(pshape);

    new_shapes.push_back(pshape);
  }

  endUndoGroup();

  setSelectShape(0);

  uint num_new_shapes = new_shapes.size();

  for (uint i = 0; i < num_new_shapes; ++i)
    addSelectShape(new_shapes[i]);
}

void
CQIllustrator::
strokePathSlot()
{
  strokePath(10);
}

void
CQIllustrator::
strokePath(double d)
{
  std::vector<CQIllustratorShape *> old_shapes;
  std::vector<CQIllustratorShape *> new_shapes;

  startUndoGroup("Stroke Path");

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    CQIllustratorGroupShape *group = dynamic_cast<CQIllustratorGroupShape *>(shape);

    if (group) continue;

    CPathShapePartList parts;

    if (! shape->getPath(parts))
      continue;

    CPathShapePartList parts1;

    parts.strokePath(d, parts1);

    CQIllustratorPathShape *pshape = createPathShape();

    pshape->setParts(parts1);

    pshape->setStroke(shape->getStroke());
    pshape->setFill  (shape->getFill ());

    addShape(pshape);

    old_shapes.push_back(shape);
    new_shapes.push_back(pshape);
  }

  endUndoGroup();

  uint num_old_shapes = old_shapes.size();

  for (uint i = 0; i < num_old_shapes; ++i)
    removeShape(old_shapes[i]);

  setSelectShape(0);

  uint num_new_shapes = new_shapes.size();

  for (uint i = 0; i < num_new_shapes; ++i)
    addSelectShape(new_shapes[i]);
}

void
CQIllustrator::
geomAndSlot()
{
  geomOp(BOOLEAN_AND);
}

void
CQIllustrator::
geomOrSlot()
{
  geomOp(BOOLEAN_OR);
}

void
CQIllustrator::
geomXorSlot()
{
  geomOp(BOOLEAN_XOR);
}

void
CQIllustrator::
geomNotSlot()
{
  geomOp(BOOLEAN_NOT);
}

void
CQIllustrator::
geomOp(CBooleanOp op)
{
  CGenPolygon subject, clip;

  bool subjectSet = false, clipSet = false;

  CQIllustratorShape *shape1;

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    CQIllustratorGroupShape *group = dynamic_cast<CQIllustratorGroupShape *>(shape);

    if (! group) {
      CPolygon2D polygon;

      if (! shape->getPolygon(polygon))
        continue;

      if (! subjectSet && ! shape->getFixed()) {
        subject.addPolygon(polygon.getX(), polygon.getY(), polygon.getNumPoints());

        shape1 = shape;

        subjectSet = true;
      }
      else {
        clip.addPolygon(polygon.getX(), polygon.getY(), polygon.getNumPoints());

        clipSet = true;
      }
    }
    else {
      std::vector<CPolygon2D> polygons;

      group->getPolygons(polygons);

      uint num_polygons = polygons.size();

      for (uint i = 0; i <  num_polygons; ++i) {
        const CPolygon2D &polygon = polygons[i];

        if (! subjectSet && ! shape->getFixed()) {
          subject.addPolygon(polygon.getX(), polygon.getY(), polygon.getNumPoints());

          shape1 = shape;

          subjectSet = true;
        }
        else {
          clip.addPolygon(polygon.getX(), polygon.getY(), polygon.getNumPoints());

          clipSet = true;
        }
      }
    }
  }

  if (! subjectSet || ! clipSet) return;

  CGenPolygon result, result1;

  if      (op == BOOLEAN_AND) {
    if (! subject.andOp(clip, result))
      return;
  }
  else if (op == BOOLEAN_OR) {
    if (! subject.orOp(clip, result))
      return;
  }
  else if (op == BOOLEAN_XOR) {
    if (! subject.xorOp(clip, result))
      return;
  }
  else if (op == BOOLEAN_NOT) {
    if (! subject.diffOp(clip, result))
      return;

    //bool rc1 = subject.diffOp(clip, result);
    //bool rc2 = clip.diffOp(subject, result1);

    //if (! rc1 && ! rc2)
    //  return;
  }
  else
    return;

  //------

  startUndoGroup("Boolean Geometry Op");

  setSelectShape(0);

  std::vector<CQIllustratorShape *> shapes;

  uint num_contours = result.getNumContours();

  for (uint i = 0; i < num_contours; ++i) {
    CQIllustratorPolygonShape *pshape = createPolygonShape();

    pshape->setStroke(shape1->getStroke());
    pshape->setFill  (shape1->getFill ());

    CGenPolygonContourRef contour = result.getContour(i);

    uint num_vertices = contour.getNumVertices();

    for (uint j = 0; j < num_vertices; ++j) {
      CGenPolygonVertexRef vertex = contour.getVertex(j);

      pshape->addPoint(CPoint2D(vertex.getX(), vertex.getY()));
    }

    addShape(pshape);

    shapes.push_back(pshape);

    setSelectShape(pshape);
  }

  if (op == BOOLEAN_NOT) {
    uint num_contours = result1.getNumContours();

    for (uint i = 0; i < num_contours; ++i) {
      CQIllustratorPolygonShape *pshape = createPolygonShape();

      pshape->setStroke(shape1->getStroke());
      pshape->setFill  (shape1->getFill ());

      CGenPolygonContourRef contour = result1.getContour(i);

      uint num_vertices = contour.getNumVertices();

      for (uint j = 0; j < num_vertices; ++j) {
        CGenPolygonVertexRef vertex = contour.getVertex(j);

        pshape->addPoint(CPoint2D(vertex.getX(), vertex.getY()));
      }

      addShape(pshape);

      shapes.push_back(pshape);

      setSelectShape(pshape);
    }
  }

  removeShape(shape1);

  endUndoGroup();
}

void
CQIllustrator::
flattenSlot()
{
  flatten();
}

void
CQIllustrator::
flatten()
{
  std::vector<CQIllustratorShape *> shapes;

  startUndoGroup("Flatten");

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    CPathShapePartList parts;

    if (! shape->getPath(parts)) continue;

    std::vector<CPolygon2D> polygons;

    parts.flatten(shape->getFlatMatrix(), polygons);

    uint num_polygons = polygons.size();

    for (uint i = 0; i < num_polygons; ++i) {
      const CPolygon2D &polygon = polygons[i];

      const std::vector<CPoint2D> &points = polygon.getPoints();

      CQIllustratorPolygonShape *poly = createPolygonShape();

      poly->setStroke(shape->getStroke());
      poly->setFill  (shape->getFill ());

      uint num_points = points.size();

      for (uint i = 0; i < num_points; ++i)
        poly->addPoint(points[i]);

      addShape(poly);

      shapes.push_back(poly);
    }
  }

  setSelectShape(0);

  uint num_shapes = shapes.size();

  for (uint i = 0; i < num_shapes; ++i)
    addSelectShape(shapes[i]);

  endUndoGroup();
}

void
CQIllustrator::
intersectOrSlot()
{
  intersectOp(BOOLEAN_OR, false, false);
}

void
CQIllustrator::
intersectAndSlot()
{
  intersectOp(BOOLEAN_AND, false, false);
}

void
CQIllustrator::
intersectXor1Slot()
{
  intersectOp(BOOLEAN_XOR, false, false);
}

void
CQIllustrator::
intersectXor2Slot()
{
  intersectOp(BOOLEAN_XOR, true, false);
}

void
CQIllustrator::
intersectOp(CBooleanOp op, bool split, bool keepOld)
{
  CQIllustratorShape *shape1 = 0, *shape2 = 0;

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    if      (! shape1) shape1 = shape;
    else if (! shape2) shape2 = shape;
  }

  if (! shape1 || ! shape2) return;

  CPathShapePartList parts1, parts2;

  if (! shape1->getPath(parts1) || ! shape2->getPath(parts2)) return;

  CPathShapePartListArray ipartsList;

  CPathShapePartList::intersect(op, parts1, shape1->getFlatMatrix(),
                                    parts2, shape2->getFlatMatrix(), ipartsList, split);

  uint numParts = ipartsList.size();

  if (numParts == 0) return;

  //------

  startUndoGroup("Intersect");

  setSelectShape(0);

  if (! keepOld) {
    removeShape(shape1);
    removeShape(shape2);
  }

  for (uint i = 0; i < numParts; ++i) {
    CQIllustratorPathShape *path1 = createPathShape();

    path1->setParts(ipartsList[i]);

    path1->setStroke(shape1->getStroke());
    path1->setFill  (shape1->getFill ());

    addShape(path1);

    addSelectShape(path1);
  }

  endUndoGroup();
}

void
CQIllustrator::
combineSlot()
{
  CQIllustratorShape *shape1 = 0, *shape2 = 0;

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    if      (! shape1) shape1 = shape;
    else if (! shape2) shape2 = shape;
  }

  if (! shape1 || ! shape2) return;

  CPathShapePartList parts1, parts2;

  if (! shape1->getPath(parts1) || ! shape2->getPath(parts2)) return;

  CPathShapePartList cparts;

  if (! CPathShapePartList::combine(parts1, shape1->getFlatMatrix(),
                                    parts2, shape2->getFlatMatrix(), cparts))
    return;

  //------

  startUndoGroup("Combine");

  removeShape(shape1);
  removeShape(shape2);

  CQIllustratorPathShape *path1 = createPathShape();

  path1->setParts(cparts);

  path1->setStroke(shape1->getStroke());
  path1->setFill  (shape1->getFill ());

  addShape(path1);

  setSelectShape(path1);

  endUndoGroup();
}

void
CQIllustrator::
triangulateSlot()
{
  std::vector<CQIllustratorShape *> old_shapes;
  std::vector<CQIllustratorShape *> new_shapes;

  startUndoGroup("Gift Wrap");

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    CQIllustratorGroupShape *group = dynamic_cast<CQIllustratorGroupShape *>(shape);

    if (group) continue;

    CPolygon2D polygon;

    if (! shape->getPolygon(polygon))
      continue;

    const std::vector<CPoint2D> &points = polygon.getPoints();

    std::vector<CTriangle2D> triangleList;

    if (! CTriangulate2D::triangulate(points, triangleList))
      continue;

    uint numTriangleList = triangleList.size();

    for (uint i = 0; i < numTriangleList; ++i) {
      const CTriangle2D &t = triangleList[i];

      CQIllustratorPolygonShape *pshape = createPolygonShape();

      pshape->setStroke(shape->getStroke());
      pshape->setFill  (shape->getFill ());

      pshape->addPoint(t.getPoint1());
      pshape->addPoint(t.getPoint2());
      pshape->addPoint(t.getPoint3());

      addShape(pshape);

      new_shapes.push_back(pshape);
    }

    old_shapes.push_back(shape);
  }

  uint num_old_shapes = old_shapes.size();

  for (uint i = 0; i < num_old_shapes; ++i)
    removeShape(old_shapes[i]);

  endUndoGroup();

  setSelectShape(0);

  uint num_new_shapes = new_shapes.size();

  for (uint i = 0; i < num_new_shapes; ++i)
    addSelectShape(new_shapes[i]);
}

void
CQIllustrator::
giftWrapSlot()
{
  std::vector<CQIllustratorShape *> old_shapes;
  std::vector<CQIllustratorShape *> new_shapes;

  startUndoGroup("Gift Wrap");

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    CQIllustratorGroupShape *group = dynamic_cast<CQIllustratorGroupShape *>(shape);

    if (group) continue;

    CPolygon2D polygon;

    if (! shape->getPolygon(polygon))
      continue;

    const std::vector<CPoint2D> &points = polygon.getPoints();

    CGiftWrap giftWrap;

    std::vector<uint> poly_inds;

    if (! giftWrap.calc(points, poly_inds))
      continue;

    std::vector<CPoint2D> gpoints;

    uint num_inds = poly_inds.size();

    for (uint i = 0; i < num_inds; ++i)
      gpoints.push_back(points[poly_inds[i]]);

    CQIllustratorPolygonShape *pshape = createPolygonShape();

    pshape->setStroke(shape->getStroke());
    pshape->setFill  (shape->getFill ());

    pshape->setPoints(gpoints);

    addShape(pshape);

    old_shapes.push_back(shape);
    new_shapes.push_back(pshape);
  }

  endUndoGroup();

  uint num_old_shapes = old_shapes.size();

  for (uint i = 0; i < num_old_shapes; ++i)
    removeShape(old_shapes[i]);

  setSelectShape(0);

  uint num_new_shapes = new_shapes.size();

  for (uint i = 0; i < num_new_shapes; ++i)
    addSelectShape(new_shapes[i]);
}

void
CQIllustrator::
delaunaySlot()
{
  std::vector<CQIllustratorShape *> old_shapes;
  std::vector<CQIllustratorShape *> new_shapes;

  startUndoGroup("Delaunay");

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    CQIllustratorGroupShape *group = dynamic_cast<CQIllustratorGroupShape *>(shape);

    if (group) continue;

    CPolygon2D polygon;

    if (! shape->getPolygon(polygon))
      continue;

    const std::vector<CPoint2D> &points = polygon.getPoints();

    CDelaunay delaunay;

    uint num_points = points.size();

    for (uint i = 0; i < num_points; ++i)
      delaunay.addVertex(points[i].x, points[i].y);

    delaunay.calc();

    CDelaunay::FaceIterator pf1, pf2;

    for (pf1 = delaunay.facesBegin(), pf2 = delaunay.facesEnd(); pf1 != pf2; ++pf1) {
      const CDelaunay::Face *f = *pf1;

      if (! f->isLower()) continue;

      CDelaunay::Vertex *v1 = f->vertex(0);
      CDelaunay::Vertex *v2 = f->vertex(1);
      CDelaunay::Vertex *v3 = f->vertex(2);

      CTriangle2D triangle(CPoint2D(v1->x(), v1->y()),
                           CPoint2D(v2->x(), v2->y()),
                           CPoint2D(v3->x(), v3->y()));

      CPoint2D c = triangle.centroid();

      if (! polygon.insideEvenOdd(c)) continue;

      CQIllustratorPolygonShape *pshape = createPolygonShape();

      pshape->setStroke(shape->getStroke());
      pshape->setFill  (shape->getFill ());

      pshape->addPoint(triangle.getPoint1());
      pshape->addPoint(triangle.getPoint2());
      pshape->addPoint(triangle.getPoint3());

      addShape(pshape);

      new_shapes.push_back(pshape);
    }

    old_shapes.push_back(shape);
  }

  uint num_old_shapes = old_shapes.size();

  for (uint i = 0; i < num_old_shapes; ++i)
    removeShape(old_shapes[i]);

  endUndoGroup();

  setSelectShape(0);

  uint num_new_shapes = new_shapes.size();

  for (uint i = 0; i < num_new_shapes; ++i)
    addSelectShape(new_shapes[i]);
}

void
CQIllustrator::
addLayer()
{
  layerStack_->addLayer();
}

void
CQIllustrator::
zoomFull()
{
  setBBox(getFullBBox());
}

void
CQIllustrator::
zoomFit()
{
  CBBox2D bbox = getFitBBox();

  setBBox(bbox);
}

CBBox2D
CQIllustrator::
getFitBBox() const
{
  CBBox2D bbox;

  for (const auto &shape : getShapes())
    bbox += shape->getFlatBBox();

  return bbox;
}

void
CQIllustrator::
zoomSelected()
{
  if (! selection_->empty()) {
    CBBox2D bbox;

    CQIllustratorSelectedShapes::iterator ps1, ps2;

    for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
      CQIllustratorShape *shape = (*ps1).getShape();

      bbox += shape->getFlatBBox();
    }

    setBBox(bbox);
  }
}

#if 0
uint
CQIllustrator::
getNumSelected() const
{
  return selection_->size();
}

CQIllustratorSelectedShapes::iterator
CQIllustrator::
getSelectionBegin()
{
  return selection_->begin();
}

CQIllustratorSelectedShapes::iterator
CQIllustrator::
getSelectionEnd()
{
  return selection_->end();
}

CQIllustratorSelectedShapes::const_iterator
CQIllustrator::
getSelectionBegin() const
{
  return selection_->begin();
}

CQIllustratorSelectedShapes::const_iterator
CQIllustrator::
getSelectionEnd() const
{
  return selection_->end();
}

CQIllustratorSelectedShape &
CQIllustrator::
getSelectionFront()
{
  return selection_->front();
}

const CQIllustratorSelectedShape &
CQIllustrator::
getSelectionFront() const
{
  return selection_->front();
}
#endif

void
CQIllustrator::
setLayer(const std::string &name)
{
  layerStack_->setLayer(name);

  selectNone();
}

void
CQIllustrator::
checkoutShape(CQIllustratorShape *shape, CQIllustratorData::ChangeType changeType)
{
  getData()->checkoutShape(shape, changeType);
}

void
CQIllustrator::
checkinShape(CQIllustratorShape *shape, CQIllustratorData::ChangeType changeType)
{
  getData()->checkinShape(shape, changeType);
}

void
CQIllustrator::
moveSelectedShapes(const CPoint2D &d)
{
  currentMode_->moveCurrent(d);
}

void
CQIllustrator::
moveSelectedPoints(const CPoint2D &d)
{
  currentMode_->moveCurrentPoint(d);
}

void
CQIllustrator::
resizeSelectedShapes(const CBBox2D &d)
{
  currentMode_->resizeCurrent(d);
}

void
CQIllustrator::
deleteSelectedShapes()
{
  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    removeShape(shape);

    delete shape;
  }

  clearSelection();
}

void
CQIllustrator::
deleteSelectedPoints()
{
  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    const CQIllustratorSelectedShape &sshape = *ps1;

    CQIllustratorSelectedShape &sshape1 =
      selection_->checkoutShape(sshape, CQIllustratorData::ChangeType::GEOMETRY);

    uint num_points = sshape.numPoints();

    for (uint i = 0; i < num_points; ++i) {
      const CQIllustratorShapeControlPoint *point = sshape.getPoint(i);

      sshape1.getShape()->removePoint(point);
    }
  }

  redraw();
}

void
CQIllustrator::
zoomIn()
{
  CPoint2D c = getBBox().getCenter();

  double scale = sqrt(2);

  double size = getBBox().getWidth()/scale;

  CBBox2D bbox(CPoint2D(c.x - size/2, c.y - size/2), CPoint2D(c.x + size/2, c.y + size/2));

  setBBox(bbox);
}

void
CQIllustrator::
zoomOut()
{
  CPoint2D c = getBBox().getCenter();

  double scale = sqrt(2);

  double size = getBBox().getWidth()*scale;

  CBBox2D bbox(CPoint2D(c.x - size/2, c.y - size/2), CPoint2D(c.x + size/2, c.y + size/2));

  setBBox(bbox);
}

void
CQIllustrator::
aboutSlot()
{
}

void
CQIllustrator::
strokeSlot(const CQIllustratorShapeStroke &stroke)
{
  if (! selection_->empty()) {
    CQIllustratorSelectedShapes::iterator ps1, ps2;

    for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
      CQIllustratorShape *shape = (*ps1).getShape();

      shape->setStroke(stroke);
    }

    redraw();
  }
}

void
CQIllustrator::
fillSlot(const CQIllustratorShapeFill &fill)
{
  if (! selection_->empty()) {
    CQIllustratorSelectedShapes::iterator ps1, ps2;

    for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
      CQIllustratorShape *shape = (*ps1).getShape();

      shape->setFill(fill);
    }

    redraw();
  }
}

void
CQIllustrator::
clipSlot(bool clip)
{
  if (! selection_->empty()) {
    CQIllustratorSelectedShapes::iterator ps1, ps2;

    for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
      CQIllustratorShape *shape = (*ps1).getShape();

      shape->setClip(clip);
    }

    redraw();
  }
}

#if 0
void
CQIllustrator::
fontSlot(const QFont &font)
{
  CFontPtr cfont = CQUtil::fromQFont(font);

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    CQIllustratorTextShape *text = dynamic_cast<CQIllustratorTextShape *>(shape);

    if (text) {
      checkoutShape(shape, CQIllustratorData::ChangeType::FONT);

      CQIllustratorTextShape *text = dynamic_cast<CQIllustratorTextShape *>(shape);

      text->setFont(cfont);

      checkinShape(shape, CQIllustratorData::ChangeType::FONT);
    }
  }

  redraw();
}
#endif

void
CQIllustrator::
objectSlot(const QString &)
{
}

void
CQIllustrator::
propSlot()
{
  redraw();
}

void
CQIllustrator::
setShapeSVGStrokeAndFill(CQIllustratorShape *shape, CSVGObject *object)
{
  CQIllustratorShapeStroke stroke;
  CQIllustratorShapeFill   fill;

  double opacity = object->getOpacity();

  const CSVGStroke &objStroke = object->getStroke();
  const CSVGFill   &objFill   = object->getFill();

  stroke.setStroked (! object->getFlatStrokeNoColor());
  stroke.setColor   (object->getFlatStrokeColor());
  stroke.setOpacity (opacity*object->getFlatStrokeOpacity());
  stroke.setWidth   (object->getFlatStrokeWidth());
  stroke.setLineDash(object->getFlatStrokeLineDash());
  stroke.setLineCap (objStroke.getLineCap());
  stroke.setLineJoin(objStroke.getLineJoin());

  fill.setFilled  (! object->getFlatFillNoColor());
  fill.setColor   (object->getFlatFillColor());
  fill.setOpacity (opacity*object->getFlatFillOpacity());
  fill.setFillRule(objFill.getRule());

  CSVGObject *fill_object = object->getFillObject();

  if (fill_object) {
    CSVGLinearGradient *lg = dynamic_cast<CSVGLinearGradient *>(fill_object);
    CSVGRadialGradient *rg = dynamic_cast<CSVGRadialGradient *>(fill_object);

    if      (lg) {
      CLinearGradient *lgradient = new CLinearGradient;

      for (const auto &stop : lg->stops()) {
        double o = stop->getOffset().ratioValue(1);

        lgradient->addStop(o, stop->getAlphaColor());
      }

      CMatrix2D m1;

      if (lg->getGTransformValid())
        m1 = lg->getGTransform().getMatrix();
      else
        m1.setIdentity();

      CPoint2D p1(lg->getX1().pxValue(1), lg->getY1().pxValue(1));
      CPoint2D p2(lg->getX2().pxValue(1), lg->getY2().pxValue(1));

      p1 = m1*p1;
      p2 = m1*p2;

      if (lg->getUnitsValid() && lg->getUnits() != CSVGCoordUnits::OBJECT_BBOX) {
        const CBBox2D &bbox = shape->getBBox(); // Flat ?

        p1.x = (p1.x - bbox.getXMin())/(bbox.getXMax() - bbox.getXMin());
        p1.y = (p1.y - bbox.getYMin())/(bbox.getYMax() - bbox.getYMin());
        p2.x = (p2.x - bbox.getXMin())/(bbox.getXMax() - bbox.getXMin());
        p2.y = (p2.y - bbox.getYMin())/(bbox.getYMax() - bbox.getYMin());
      }

      lgradient->setLine(p1.x, p1.y, p2.x, p2.y);

      lgradient->init(1, 1);

      fill.setGradient(lgradient);

      delete lgradient;
    }
    else if (rg) {
      CRadialGradient *rgradient = new CRadialGradient;

      CSVGRadialGradient::StopList::const_iterator ps1, ps2;

      for (const auto &stop : rg->stops()) {
        double o = stop->getOffset().ratioValue(1);

        rgradient->addStop(o, stop->getAlphaColor());
      }

      CMatrix2D m1;

      if (rg->getGTransformValid())
        m1 = rg->getGTransform().getMatrix();
      else
        m1.setIdentity();

      CPoint2D c(rg->getCenterX().pxValue(1), rg->getCenterY().pxValue(1));
      CPoint2D f(rg->getFocusX(), rg->getFocusY());

      CVector2D r(rg->getRadius().pxValue(1), rg->getRadius().pxValue(1));

      c = m1*c;
      f = m1*f;
      r = m1*r;

      if (rg->getUnitsValid() && rg->getUnits() != CSVGCoordUnits::OBJECT_BBOX) {
        const CBBox2D &bbox = shape->getBBox(); // Flat ?

        c.x = (c.x - bbox.getXMin())/(bbox.getXMax() - bbox.getXMin());
        c.y = (c.y - bbox.getYMin())/(bbox.getYMax() - bbox.getYMin());
        f.x = (f.x - bbox.getXMin())/(bbox.getXMax() - bbox.getXMin());
        f.y = (f.y - bbox.getYMin())/(bbox.getYMax() - bbox.getYMin());

        r.setX(2*r.getX()/(bbox.getXMax() - bbox.getXMin()));
        r.setY(2*r.getY()/(bbox.getYMax() - bbox.getYMin()));
      }

      rgradient->setCenter(c.x, c.y);
      rgradient->setFocus (f.x, f.y);
      rgradient->setRadius(std::max(r.getX(), r.getY()));

      rgradient->init(1, 1);

      fill.setGradient(rgradient);

      delete rgradient;
    }
  }

  shape->setStroke(stroke);
  shape->setFill  (fill);
}

void
CQIllustrator::
addLinearGradient(const CPoint2D &p1, const CPoint2D &p2)
{
  CLinearGradient *lgradient = new CLinearGradient;

  lgradient->addStop(0.0, CRGBA(1,1,1));
  lgradient->addStop(1.0, CRGBA(0,0,0));

  // no selection so create rectangle
  if (selection_->empty()) {
    CQIllustratorRectShape *rect = createRectShape(p1, p2);

    addShape(rect);

    setSelectShape(rect);
  }

  // apply gradient to selected shape
  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    const CBBox2D &bbox = shape->getBBox(); // Flat ?

    double x1 = (p1.x - bbox.getXMin())/(bbox.getXMax() - bbox.getXMin());
    double y1 = (p1.y - bbox.getYMin())/(bbox.getYMax() - bbox.getYMin());
    double x2 = (p2.x - bbox.getXMin())/(bbox.getXMax() - bbox.getXMin());
    double y2 = (p2.y - bbox.getYMin())/(bbox.getYMax() - bbox.getYMin());

    lgradient->setLine(x1, y1, x2, y2);

    lgradient->init(1, 1);

    CQIllustratorShapeFill fill = shape->getFill();

    fill.setGradient(lgradient);

    shape->setFill(fill);
  }

  delete lgradient;
}

void
CQIllustrator::
addRadialGradient(const CPoint2D &p1, const CPoint2D &p2)
{
  CRadialGradient *rgradient = new CRadialGradient;

  rgradient->addStop(0.0, CRGBA(1,1,1));
  rgradient->addStop(1.0, CRGBA(0,0,0));

  // no selection so create ellipse
  if (selection_->empty()) {
    CQIllustratorEllipseShape *ellipse = createEllipseShape(p1, p2);

    addShape(ellipse);

    setSelectShape(ellipse);
  }

  // apply gradient to selected shape
  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    const CBBox2D &bbox = shape->getBBox(); // Flat ?

    double x1 = (p1.x - bbox.getXMin())/(bbox.getXMax() - bbox.getXMin());
    double y1 = (p1.y - bbox.getYMin())/(bbox.getYMax() - bbox.getYMin());
    double x2 = (p2.x - bbox.getXMin())/(bbox.getXMax() - bbox.getXMin());
    double y2 = (p2.y - bbox.getYMin())/(bbox.getYMax() - bbox.getYMin());

    rgradient->setCenter(x1, y1);

    rgradient->setRadius(CPoint2D(x1, y1).distanceTo(CPoint2D(x2, y2)));

    rgradient->setFocus((x1 + x2)/2, (y1 + y2)/2);

    rgradient->init(1, 1);

    CQIllustratorShapeFill fill = shape->getFill();

    fill.setGradient(rgradient);

    shape->setFill(fill);
  }

  delete rgradient;
}

void
CQIllustrator::
setSVGShapeName(CQIllustratorShape *shape, CSVGObject *object)
{
  std::string id = object->getId();

  if (id != "") {
    CQIllustratorData *data = shape->getData();

    shape->setData(0);

    shape->setName(id);

    shape->setData(data);
  }
  else
    setShapeName(shape);
}

void
CQIllustrator::
addShape(CQIllustratorShape *shape)
{
  getData()->addShape(shape);

  setShapeName(shape);
}

void
CQIllustrator::
removeShape(CQIllustratorShape *shape)
{
  getData()->removeShape(shape);
}

void
CQIllustrator::
setShapeName(CQIllustratorShape *shape)
{
  static int num = 1;

  if (shape->getName() == "") {
    CQIllustratorData *data = shape->getData();

    shape->setData(0);

    shape->setName(CStrUtil::strprintf("%s%d", shape->getClassName(), num));

    shape->setData(data);

    ++num;
  }
}

void
CQIllustrator::
clearSelection()
{
  selection_->clear();
}

void
CQIllustrator::
setSelectShape(const std::string &name)
{
  CQIllustratorShape *shape = getShape(name);

  setSelectShape(shape);
}

void
CQIllustrator::
setSelectShape(CQIllustratorShape *shape)
{
  clearSelection();

  if (shape)
    addSelectShape(shape);
}

void
CQIllustrator::
addSelectShape(CQIllustratorShape *shape)
{
  if (shape == 0)
    return;

  selection_->add(shape);
}

void
CQIllustrator::
removeSelectShape(CQIllustratorShape *shape)
{
  if (shape == 0)
    return;

  removeShapeFromSelection(shape);
}

void
CQIllustrator::
removeShapeFromSelection(CQIllustratorShape *shape)
{
  if (shape == 0)
    return;

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection_->begin(), ps2 = selection_->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape1 = (*ps1).getShape();

    if (shape == shape1) {
      selection_->remove(ps1);

      break;
    }
  }
}

CQIllustratorShape *
CQIllustrator::
getShape(const std::string &name) const
{
  const CQIllustratorData::ShapeStack &shapes = getShapes();

  CQIllustratorData::ShapeStack::const_iterator ps1, ps2;

  for (ps1 = shapes.begin(), ps2 = shapes.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    if (shape->getName() == name)
      return shape;
  }

  return 0;
}

CQIllustratorShape *
CQIllustrator::
getShape(uint id) const
{
  const CQIllustratorData::ShapeStack &shapes = getShapes();

  CQIllustratorData::ShapeStack::const_iterator ps1, ps2;

  for (ps1 = shapes.begin(), ps2 = shapes.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    if (shape->getId() == int(id))
      return shape;
  }

  return 0;
}

void
CQIllustrator::
setBBox(const CBBox2D &bbox)
{
  if (! bbox.isSet())
    return;

  if (REAL_EQ(bbox.getWidth(), 0.0) || REAL_EQ(bbox.getHeight(), 0.0))
    return;

  bbox_ = bbox;

  int w = bbox_.getWidth();
  int h = bbox_.getHeight();

  if (w > h) {
    double dy = (w - h)/2.0;

    bbox_.expand(0, -dy, 0, dy);
  }
  else {
    double dx = (h - w)/2.0;

    bbox_.expand(-dx, 0, dx, 0);
  }

  redraw();
}

QRectF
CQIllustrator::
getQRect() const
{
  return CQUtil::toQRect(bbox_);
}

void
CQIllustrator::
setQRect(const QRectF &r)
{
  bbox_ = CQUtil::fromQRect(r);
}

void
CQIllustrator::
setFullBBox(const CBBox2D &bbox)
{
  if (REAL_EQ(bbox.getWidth(), 0.0) || REAL_EQ(bbox.getHeight(), 0.0))
    return;

  fullBBox_ = bbox;

  emit fullBBoxChanged();
}

QRectF
CQIllustrator::
getQFullRect() const
{
  return CQUtil::toQRect(fullBBox_);
}

void
CQIllustrator::
setQFullRect(const QRectF &r)
{
  fullBBox_ = CQUtil::fromQRect(r);
}

#if 0
QFont
CQIllustrator::
getCurrentFont() const
{
  return fontTool_->getFont();
}
#endif

void
CQIllustrator::
redraw()
{
  if (! changed_) {
    changed_ = true;

    redrawOverlay();
  }
}

void
CQIllustrator::
redrawOverlay()
{
  canvas_->update();
}

CQIllustratorAlignToolbar *
CQIllustrator::
getAlignToolbar() const
{
  CQIllustratorMode *mode = getMode(Mode::ALIGN);

  CQIllustratorAlignMode *alignMode = dynamic_cast<CQIllustratorAlignMode *>(mode);

  if (alignMode)
    return alignMode->getToolbar();
  else
    return 0;
}

void
CQIllustrator::
clearPreviewObjects()
{
  PreviewObjectList::const_iterator p1, p2;

  for (p1 = previewObjects_.begin(), p2 = previewObjects_.end(); p1 != p2; ++p1)
    delete *p1;

  previewObjects_.clear();
}

void
CQIllustrator::
addPreviewObject(PreviewObject *object)
{
  previewObjects_.push_back(object);
}

void
CQIllustrator::
drawPreviewObjects(QPainter *painter)
{
  PreviewObjectList::const_iterator p1, p2;

  for (p1 = previewObjects_.begin(), p2 = previewObjects_.end(); p1 != p2; ++p1)
    (*p1)->draw(this, painter);
}

void
CQIllustrator::
setStroke(QPainter *painter, CQIllustratorShape *shape, const CQIllustratorShapeStroke &stroke)
{
  setStroke(painter, shape, stroke);
}

void
CQIllustrator::
setStroke(QPainter *painter, const CQIllustratorShape *shape,
          const CQIllustratorShapeStroke &stroke)
{
  QPen pen;

  CRGBA color = stroke.getColor();

  color.setAlpha(stroke.getOpacity());

  pen.setColor(CQUtil::rgbaToColor(color));

  double width = shape->getStrokeWidth();

  if (width <= 0.0) width = 0.01;

  pen.setWidthF(width);

  const CLineDash &dash = stroke.getLineDash();

  int num = dash.getNumLengths();

  if (num > 0) {
    pen.setStyle(Qt::CustomDashLine);

    pen.setDashOffset(dash.getOffset());

    QVector<qreal> dashes;

    for (int i = 0; i < num; ++i)
      dashes << dash.getLength(i)/width;

    if (num & 1)
      dashes << dash.getLength(0)/width;

    pen.setDashPattern(dashes);
  }
  else
    pen.setStyle(Qt::SolidLine);

  switch (stroke.getLineCap()) {
    case LINE_CAP_TYPE_BUTT  : pen.setCapStyle(Qt::FlatCap  ); break;
    case LINE_CAP_TYPE_ROUND : pen.setCapStyle(Qt::RoundCap ); break;
    case LINE_CAP_TYPE_SQUARE: pen.setCapStyle(Qt::SquareCap); break;
    default                  : break;
  }

  switch (stroke.getLineJoin()) {
    case LINE_JOIN_TYPE_MITRE: pen.setJoinStyle(Qt::MiterJoin); break;
    case LINE_JOIN_TYPE_ROUND: pen.setJoinStyle(Qt::RoundJoin); break;
    case LINE_JOIN_TYPE_BEVEL: pen.setJoinStyle(Qt::BevelJoin); break;
    default                  : break;
  }

  pen.setMiterLimit(stroke.getMitreLimit());

  painter->setPen(pen);
}

void
CQIllustrator::
setFill(QPainter *painter, CQIllustratorShape *shape,
        const CQIllustratorShapeFill &fill, const CBBox2D &bbox)
{
  setFill(painter, shape, fill, bbox);
}

void
CQIllustrator::
setFill(QPainter *painter, const CQIllustratorShape *,
        const CQIllustratorShapeFill &fill, const CBBox2D &bbox)
{
  if      (fill.hasGradient()) {
    painter->setBrushOrigin(QPointF(0,0));

    const CGenGradient *g = fill.getGradient();

    const CLinearGradient *lg = 0;
    const CRadialGradient *rg = 0;

    if      ((lg = dynamic_cast<const CLinearGradient *>(g)) != 0) {
      QBrush brush(CQUtil::toQGradient(lg));

      painter->setBrush(brush);
    }
    else if ((rg = dynamic_cast<const CRadialGradient *>(g)) != 0) {
      QBrush brush(CQUtil::toQGradient(rg));

      painter->setBrush(brush);
    }
  }
  else if (fill.hasImage()) {
    CImagePtr                          image  = fill.getImage();
    CQIllustratorShapeFill::ImageScale scale  = fill.getImageScale();
    CHAlignType                        halign = fill.getImageHAlign();
    CVAlignType                        valign = fill.getImageVAlign();

    QBrush brush;

    if (image.isValid()) {
      QImage qimage = image.cast<CQImage>()->getQImage();

      brush.setTextureImage(qimage);

      painter->setBrushOrigin(QPointF(0,0));

      int w = image->getWidth();
      int h = image->getHeight();

      QTransform t  = getTransform();
      QTransform it = getITransform();

      if      (scale == CQIllustratorShapeFill::ImageScale::NONE) {
        double x, y;

        if      (halign == CHALIGN_TYPE_LEFT)   x = bbox.getXMin();
        else if (halign == CHALIGN_TYPE_RIGHT)  x = bbox.getXMax();
        else                                    x = bbox.getXMid();

        if      (valign == CVALIGN_TYPE_BOTTOM) y = bbox.getYMin();
        else if (valign == CVALIGN_TYPE_TOP   ) y = bbox.getYMax();
        else                                    y = bbox.getYMid();

        double px, py;

        t.map(x, y, &px, &py);

        if (halign == CHALIGN_TYPE_CENTER) px -= w/2;

        QTransform t1;

        t1.translate(px, py);

        brush.setTransform(t1*it);
      }
      else if (scale == CQIllustratorShapeFill::ImageScale::FIT) {
        double x = bbox.getXMin();
        double y = bbox.getYMin();

        double px, py;

        t.map(x, y, &px, &py);

        QTransform t1;

        t1.translate(px, py);

        QTransform t2;

        double px1, py1, px2, py2;

        t.map(bbox.getXMin(), bbox.getYMin(), &px1, &py1);
        t.map(bbox.getXMax(), bbox.getYMax(), &px2, &py2);

        double xs = fabs(px2 - px1)/w;
        double ys = fabs(py2 - py1)/h;

        t2.scale(xs, ys);

        brush.setTransform(t2*t1*it);
      }
      else if (scale == CQIllustratorShapeFill::ImageScale::EQUAL) {
        double x = bbox.getXMin();
        double y = bbox.getYMin();

        double px, py;

        t.map(x, y, &px, &py);

        QTransform t1;

        t1.translate(px, py);

        QTransform t2;

        double px1, py1, px2, py2;

        t.map(bbox.getXMin(), bbox.getYMin(), &px1, &py1);
        t.map(bbox.getXMax(), bbox.getYMax(), &px2, &py2);

        double xs = fabs(px2 - px1)/w;
        double ys = fabs(py2 - py1)/h;

        double s = std::min(xs, ys);

        t2.scale(s, s);

        QTransform t3;

        double dx = fabs(px2 - px1) - s*w;
        double dy = fabs(py2 - py1) - s*h;

        double dx1, dy1;

        if      (halign == CHALIGN_TYPE_LEFT  ) dx1 = 0;
        else if (halign == CHALIGN_TYPE_RIGHT ) dx1 = dx;
        else                                    dx1 = dx/2.0;

        if      (valign == CVALIGN_TYPE_BOTTOM) dy1 = 0;
        else if (valign == CVALIGN_TYPE_TOP   ) dy1 = dy;
        else                                    dy1 = dy/2.0;

        t3.translate(dx1, dy1);

        brush.setTransform(t3*t2*t1*it);
      }
      else {
        brush.setTransform(it);
      }
    }

    painter->setBrush(brush);
  }
  else {
    CRGBA color = fill.getColor();

    color.setAlpha(fill.getOpacity());

    QBrush brush(CQUtil::rgbaToColor(color));

    painter->setBrush(brush);
  }
}

CQIllustratorRectShape *
CQIllustrator::
createRectShape(const CPoint2D &p1, const CPoint2D &p2, double rx, double ry)
{
  CQIllustratorRectShape *rect = new CQIllustratorRectShape(p1, p2, rx, ry);

  rect->setStroke(def_stroke_);
  rect->setFill  (def_fill_);

  return rect;
}

CQIllustratorEllipseShape *
CQIllustrator::
createEllipseShape(const CPoint2D &p1, const CPoint2D &p2)
{
  CQIllustratorEllipseShape *ellipse = new CQIllustratorEllipseShape(p1, p2);

  ellipse->setStroke(def_stroke_);
  ellipse->setFill  (def_fill_);

  return ellipse;
}

CQIllustratorPolygonShape *
CQIllustrator::
createPolygonShape(const std::vector<CPoint2D> &points)
{
  CQIllustratorPolygonShape *polygon = new CQIllustratorPolygonShape(points);

  polygon->setStroke(def_stroke_);
  polygon->setFill  (def_fill_);

  return polygon;
}

CQIllustratorPathShape *
CQIllustrator::
createPathShape()
{
  CQIllustratorPathShape *path = new CQIllustratorPathShape();

  path->setStroke(def_stroke_);
  path->setFill  (def_fill_);

  return path;
}

CQIllustratorNPolyShape *
CQIllustrator::
createNPolyShape(const CPoint2D &c, uint n, double r, double a)
{
  CQIllustratorNPolyShape *npoly = new CQIllustratorNPolyShape(c, n, r, a);

  npoly->setStroke(def_stroke_);
  npoly->setFill  (def_fill_);

  return npoly;
}

CQIllustratorStarShape *
CQIllustrator::
createStarShape(const CPoint2D &c, uint n, double r1, double r2, double a1, double a2)
{
  CQIllustratorStarShape *star = new CQIllustratorStarShape(c, n, r1, r2, a1, a2);

  star->setStroke(def_stroke_);
  star->setFill  (def_fill_);

  return star;
}

CQIllustratorTextShape *
CQIllustrator::
createTextShape(const CPoint2D &p1, const CPoint2D &p2, const std::string &str)
{
  CQIllustratorTextShape *text = new CQIllustratorTextShape(p1, p2, str);

  text->setStroke(def_stroke_);
  text->setFill  (def_fill_);

  return text;
}

CQIllustratorGroupShape *
CQIllustrator::
createGroupShape()
{
  CQIllustratorGroupShape *group = new CQIllustratorGroupShape();

  group->setStroke(def_stroke_);
  group->setFill  (def_fill_);

  return group;
}

QSize
CQIllustrator::
sizeHint() const
{
  QFontMetrics fm(font());

  int tw = fm.width("X");
  int th = fm.height();

  return QSize(120*tw, 50*th);
}

//----------------

void
CQIllustrator::PreviewShape::
draw(CQIllustrator *illustrator, QPainter *painter) const
{
  CQIllustratorShapeDrawer *drawer = illustrator->getDrawer();

  drawer->setPainter(painter);

  QPen pen;

  pen.setColor(QColor(0,0,0));
  pen.setStyle(Qt::DashLine);
  pen.setWidth(0);

  painter->setPen(pen);
  painter->setBrush(Qt::NoBrush);

  painter->drawRect(CQUtil::toQRect(shape_->getFlatBBox()));

  CQIllustratorData *data = shape_->getData();

  shape_->setData(0);

  shape_->moveBy(d_);

  shape_->draw(illustrator->getDrawer());

  pen.setColor(QColor(255,0,0));
  pen.setStyle(Qt::DashLine);

  painter->setPen(pen);
  painter->setBrush(Qt::NoBrush);

  painter->drawRect(CQUtil::toQRect(shape_->getFlatBBox()));

  shape_->moveBy(-d_);

  shape_->setData(data);
}

//------

void
CQIllustrator::PreviewLine::
draw(CQIllustrator *, QPainter *painter) const
{
  QPen pen;

  pen.setColor(QColor(255,0,0));
  pen.setStyle(Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(Qt::NoBrush);

  painter->drawLine(CQUtil::toQPoint(p1_), CQUtil::toQPoint(p2_));
}

//------

void
CQIllustratorGrid::
draw(CQIllustratorShapeDrawer *drawer, const CBBox2D &bbox)
{
  if (! enabled_) return;

  //QTransform transform = drawer->getPainter()->worldTransform();
  QTransform transform = drawer->getPainter()->transform();

  QTransform itransform = transform.inverted();

  QPointF p1 = itransform.map(QPointF(0   , 0   ));
  QPointF p2 = itransform.map(QPointF(1000, 1000));

  CAxis2D axis(0, p2.x() - p1.x());

  double dx  = axis.getMajorIncrement();
  double dx1 = axis.getMinorIncrement();

  double dy  = dx;
  double dy1 = dx1;

  double xm = (p1.x() + p2.x())/2.0;
  double ym = (p1.y() + p2.y())/2.0;

  double xo = int(ym/dx)*dx;
  double yo = int(xm/dy)*dy;

  uint n1 = axis.getNumMinorTicks();

#if 0
  double dx = dx_;
  double dy = dy_;

  int nx = int(bbox.getWidth ()/dx);
  int ny = int(bbox.getHeight()/dy);

  while (nx > 40) {
    dx *= 2;

    nx = int(bbox.getWidth ()/dx);
  }

  while (ny > 40) {
    dy *= 2;

    ny = int(bbox.getHeight()/dy);
  }

  dx = std::min(dx, dy);
  dy = dx;

  xo = origin_.x;
  yo = origin_.y;
#endif

  for (double x1 = xo, x2 = x1 + dx; x1 <= bbox.getRight() + dx; x1 = x2, x2 += dx) {
    drawer->drawGridLine(CPoint2D(x1, bbox.getBottom()), CPoint2D(x1, bbox.getTop()));

    for (uint i = 1; i < n1; i++) {
      double x = x1 + i*dx1;

      drawer->drawGridSubLine(CPoint2D(x, bbox.getBottom()), CPoint2D(x, bbox.getTop()));
    }
  }

  for (double x1 = xo, x2 = x1 - dx; x1 >= bbox.getLeft() - dx; x1 = x2, x2 -= dx) {
    drawer->drawGridLine(CPoint2D(x1, bbox.getBottom()), CPoint2D(x1, bbox.getTop()));

    for (uint i = 1; i < n1; i++) {
      double x = x1 + i*dx1;

      drawer->drawGridSubLine(CPoint2D(x, bbox.getBottom()), CPoint2D(x, bbox.getTop()));
    }
  }

  //---

  for (double y1 = yo, y2 = y1 + dy; y1 <= bbox.getTop() + dy; y1 = y2, y2 += dy) {
    drawer->drawGridLine(CPoint2D(bbox.getLeft(), y1), CPoint2D(bbox.getRight(), y1));

    for (uint i = 1; i < n1; i++) {
      double y = y1 + i*dy1;

      drawer->drawGridSubLine(CPoint2D(bbox.getLeft(), y), CPoint2D(bbox.getRight(), y));
    }
  }

  for (double y1 = yo, y2 = y1 - dy; y1 >= bbox.getBottom() - dy; y1 = y2, y2 -= dy) {
    drawer->drawGridLine(CPoint2D(bbox.getLeft(), y1), CPoint2D(bbox.getRight(), y1));

    for (uint i = 1; i < n1; i++) {
      double y = y1 + i*dy1;

      drawer->drawGridSubLine(CPoint2D(bbox.getLeft(), y), CPoint2D(bbox.getRight(), y));
    }
  }
}

//------

CPoint2D
CQIllustratorSnap::
snapPoint(const CPoint2D &point) const
{
  if (enabled_) {
    double x = xpitch_*CMathGen::Round(point.x/xpitch_);
    double y = ypitch_*CMathGen::Round(point.y/ypitch_);

    return CPoint2D(x, y);
  }
  else
    return point;
}

//----------

static const char *source_opts = "\
-file:s=\"\"";

CQIllustratorSourceCmd::
CQIllustratorSourceCmd() :
 CQIllustratorCmd(source_opts)
{
}

bool
CQIllustratorSourceCmd::
exec(const std::vector<std::string> &words)
{
  args_.parse(words);

  std::string filename = args_.getStringArg("-file");

  if (filename == "")
    return false;

  CFile file(filename);

  std::vector<std::string> lines;

  if (! file.toLines(lines))
    return false;

  uint num_lines = lines.size();

  for (uint i = 0; i < num_lines; ++i)
    illustrator_->execCommand(lines[i]);

  return true;
}

//----------

static const char *select_opts = "\
-all:f";

CQIllustratorSelectCmd::
CQIllustratorSelectCmd() :
 CQIllustratorCmd(select_opts)
{
}

bool
CQIllustratorSelectCmd::
exec(const std::vector<std::string> &words)
{
  args_.parse(words);

  bool is_all = args_.getBooleanArg("-all");

  if (is_all)
    illustrator_->selectAll();

  return true;
}

//----------

static const char *move_opts = "\
-x:r=0 \
-y:r=0 \
-dx:r=0 \
-dy:r=0";

CQIllustratorMoveCmd::
CQIllustratorMoveCmd() :
 CQIllustratorCmd(move_opts)
{
}

bool
CQIllustratorMoveCmd::
exec(const std::vector<std::string> &words)
{
  args_.parse(words);

  //double x = args_.getRealArg("-x");
  //double y = args_.getRealArg("-y");

  double dx = args_.getRealArg("-dx");
  double dy = args_.getRealArg("-dy");

  illustrator_->moveSelectedShapes(CPoint2D(dx,dy));

  return true;
}

//----------

static const char *move_point_opts = "\
-x:r=0 \
-y:r=0 \
-dx:r=0 \
-dy:r=0";

CQIllustratorMovePointCmd::
CQIllustratorMovePointCmd() :
 CQIllustratorCmd(move_point_opts)
{
}

bool
CQIllustratorMovePointCmd::
exec(const std::vector<std::string> &words)
{
  args_.parse(words);

  //double x = args_.getRealArg("-x");
  //double y = args_.getRealArg("-y");

  double dx = args_.getRealArg("-dx");
  double dy = args_.getRealArg("-dy");

  illustrator_->moveSelectedPoints(CPoint2D(dx,dy));

  return true;
}

//----------

static const char *undo_opts = "\
-all:f";

CQIllustratorUndoCmd::
CQIllustratorUndoCmd() :
 CQIllustratorCmd(undo_opts)
{
}

bool
CQIllustratorUndoCmd::
exec(const std::vector<std::string> &words)
{
  args_.parse(words);

  illustrator_->getUndo()->undo();

  return true;
}

//----------

static const char *redo_opts = "\
-all:f";

CQIllustratorRedoCmd::
CQIllustratorRedoCmd() :
 CQIllustratorCmd(redo_opts)
{
}

bool
CQIllustratorRedoCmd::
exec(const std::vector<std::string> &words)
{
  args_.parse(words);

  illustrator_->getUndo()->redo();

  return true;
}

//----------

static const char *offset_path_opts = "\
-offset:r=10";

CQIllustratorOffsetPathCmd::
CQIllustratorOffsetPathCmd() :
 CQIllustratorCmd(offset_path_opts)
{
}

bool
CQIllustratorOffsetPathCmd::
exec(const std::vector<std::string> &words)
{
  args_.parse(words);

  double offset = args_.getRealArg("-offset");

  illustrator_->offsetPath(offset);

  return true;
}

//----------

static const char *stroke_path_opts = "\
-width:r=10";

CQIllustratorStrokePathCmd::
CQIllustratorStrokePathCmd() :
 CQIllustratorCmd(stroke_path_opts)
{
}

bool
CQIllustratorStrokePathCmd::
exec(const std::vector<std::string> &words)
{
  args_.parse(words);

  double width = args_.getRealArg("-width");

  illustrator_->strokePath(width);

  return true;
}

//----------

static const char *intersect_opts = "\
-op:s \
-split:f";

CQIllustratorIntersectCmd::
CQIllustratorIntersectCmd() :
 CQIllustratorCmd(intersect_opts)
{
}

bool
CQIllustratorIntersectCmd::
exec(const std::vector<std::string> &words)
{
  args_.parse(words);

  std::string op = args_.getStringArg("-op");

  if (op == "")
    return false;

  bool is_split = args_.getBooleanArg("-split");

  if      (op == "or")
    illustrator_->intersectOp(BOOLEAN_OR, false);
  else if (op == "and")
    illustrator_->intersectOp(BOOLEAN_AND, false);
  else if (op == "xor")
    illustrator_->intersectOp(BOOLEAN_XOR, is_split);
  else
    return false;

  return true;
}
