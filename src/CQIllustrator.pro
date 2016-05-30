TEMPLATE = app

TARGET = CQIllustrator

DEPENDPATH += .

QT += widgets

QMAKE_CXXFLAGS += -std=c++11

MOC_DIR = .moc

CONFIG += debug

# Input
SOURCES += \
CQIllustratorAlignMode.cpp \
CQIllustratorCanvas.cpp \
CQIllustratorCmd.cpp \
CQIllustrator.cpp \
CQIllustratorCreateEllipseMode.cpp \
CQIllustratorCreatePathMode.cpp \
CQIllustratorCreatePolygonMode.cpp \
CQIllustratorCreateRectMode.cpp \
CQIllustratorCreateStarMode.cpp \
CQIllustratorCreateTextMode.cpp \
CQIllustratorCreateTextToolbar.cpp \
CQIllustratorData.cpp \
CQIllustratorEllipseShape.cpp \
CQIllustratorGroupShape.cpp \
CQIllustratorHandle.cpp \
CQIllustratorImageFill.cpp \
CQIllustratorInfo.cpp \
CQIllustratorLayer.cpp \
CQIllustratorMode.cpp \
CQIllustratorNPolyShape.cpp \
CQIllustratorOffsetPathMode.cpp \
CQIllustratorPanMode.cpp \
CQIllustratorPathShapeControlLine.cpp \
CQIllustratorPathShapeControlPoint.cpp \
CQIllustratorPathShape.cpp \
CQIllustratorPointSelectMode.cpp \
CQIllustratorPolygonShape.cpp \
CQIllustratorPreference.cpp \
CQIllustratorPropertiesDlg.cpp \
CQIllustratorRectShape.cpp \
CQIllustratorSandbox.cpp \
CQIllustratorSelection.cpp \
CQIllustratorSelectMode.cpp \
CQIllustratorSetAnchorObjectMode.cpp \
CQIllustratorSetAnchorPositionMode.cpp \
CQIllustratorSetImageMode.cpp \
CQIllustratorSetLGradientMode.cpp \
CQIllustratorSetRGradientMode.cpp \
CQIllustratorShapeControlLine.cpp \
CQIllustratorShapeControlPoint.cpp \
CQIllustratorShape.cpp \
CQIllustratorShapeDrawer.cpp \
CQIllustratorShapeFill.cpp \
CQIllustratorShapeFilter.cpp \
CQIllustratorShapeLGradientControlPoint.cpp \
CQIllustratorShapeRGradientControlPoint.cpp \
CQIllustratorShapeStroke.cpp \
CQIllustratorSizer.cpp \
CQIllustratorSliceMode.cpp \
CQIllustratorSnapDock.cpp \
CQIllustratorStarShape.cpp \
CQIllustratorTextShape.cpp \
CQIllustratorToolbar.cpp \
CQIllustratorTransformMode.cpp \
CQIllustratorUndo.cpp \
CQIllustratorUndoDock.cpp \
CQIllustratorZoomMode.cpp \
\
CQSVGRenderer.cpp \
\
CArcToBezier.cpp \
CAxis2D.cpp \
CBezierToLine.cpp \
CCSS.cpp \
CDelaunay.cpp \
CEncode64.cpp \
CGenPoly.cpp \
CGiftWrap.cpp \
CHull3D.cpp \
CKuhnMunkres.cpp \
CLog.cpp \
CMathGeom2D.cpp \
CPathShape.cpp \
CQAccelerate.cpp \
CQAngleSpinBox.cpp \
CQFillOptionDialog.cpp \
CQFillOptionTool.cpp \
CQFontOptionDialog.cpp \
CQFontOptionTool.cpp \
CQGradientStopList.cpp \
CQImageButton.cpp \
CQImagePreview.cpp \
CQLayerOption.cpp \
CQLineCap.cpp \
CQLineDash.cpp \
CQLineJoin.cpp \
CQMatrix2D.cpp \
CQObjectOption.cpp \
CQOptionToolSet.cpp \
CQPointEdit.cpp \
CQPropertiesOption.cpp \
CQRadioButtons.cpp \
CQScrollArea.cpp \
CQStrokeOptionDialog.cpp \
CQStrokeOptionTool.cpp \
CQSwatch.cpp \
CQTableWidget.cpp \
CQWinWidget.cpp \
CTriangulate2D.cpp \

HEADERS += \
CQIllustratorAlignMode.h \
CQIllustratorCanvas.h \
CQIllustratorCmd.h \
CQIllustratorCreateEllipseMode.h \
CQIllustratorCreatePathMode.h \
CQIllustratorCreatePolygonMode.h \
CQIllustratorCreateRectMode.h \
CQIllustratorCreateStarMode.h \
CQIllustratorCreateTextMode.h \
CQIllustratorCreateTextToolbar.h \
CQIllustratorData.h \
CQIllustratorEllipseGeometry.h \
CQIllustratorEllipseShape.h \
CQIllustratorGrid.h \
CQIllustratorGroupGeometry.h \
CQIllustratorGroupShape.h \
CQIllustrator.h \
CQIllustratorHandle.h \
CQIllustratorImageFill.h \
CQIllustratorInfo.h \
CQIllustratorInfoTip.h \
CQIllustratorLayer.h \
CQIllustratorMode.h \
CQIllustratorNPolyGeometry.h \
CQIllustratorNPolyShape.h \
CQIllustratorOffsetPathMode.h \
CQIllustratorPanMode.h \
CQIllustratorPathGeometry.h \
CQIllustratorPathShapeControlLine.h \
CQIllustratorPathShapeControlPoint.h \
CQIllustratorPathShape.h \
CQIllustratorPointSelectMode.h \
CQIllustratorPolygonGeometry.h \
CQIllustratorPolygonShape.h \
CQIllustratorPreference.h \
CQIllustratorPropertiesDlg.h \
CQIllustratorQuadTree.h \
CQIllustratorRectGeometry.h \
CQIllustratorRectShape.h \
CQIllustratorSandbox.h \
CQIllustratorSaveData.h \
CQIllustratorSelection.h \
CQIllustratorSelectMode.h \
CQIllustratorSetAnchorObjectMode.h \
CQIllustratorSetAnchorPositionMode.h \
CQIllustratorSetImageMode.h \
CQIllustratorSetLGradientMode.h \
CQIllustratorSetRGradientMode.h \
CQIllustratorShapeControlLine.h \
CQIllustratorShapeControlPoint.h \
CQIllustratorShapeDrawer.h \
CQIllustratorShapeFill.h \
CQIllustratorShapeFilter.h \
CQIllustratorShapeGaussianFilter.h \
CQIllustratorShapeGeometry.h \
CQIllustratorShape.h \
CQIllustratorShapeLGradientControlPoint.h \
CQIllustratorShapeNearestPoint2D.h \
CQIllustratorShapeRGradientControlPoint.h \
CQIllustratorShapeStroke.h \
CQIllustratorSizer.h \
CQIllustratorSliceMode.h \
CQIllustratorSnapDock.h \
CQIllustratorSnap.h \
CQIllustratorStarGeometry.h \
CQIllustratorStarShape.h \
CQIllustratorTextGeometry.h \
CQIllustratorTextShape.h \
CQIllustratorToolbar.h \
CQIllustratorTransformMode.h \
CQIllustratorUndoDock.h \
CQIllustratorUndo.h \
CQIllustratorUtil.h \
CQIllustratorZoomMode.h \
\
CQSVGRenderer.h \
\
CAxis2D.h \
CBooleanOp.h \
CCSS.h \
CDelaunay.h \
CEncode64.h \
CGenPoly.h \
CHull3D.h \
CKuhnMunkres.h \
CListLink.h \
CLog.h \
CMathGeom2D.h \
CPathPartType.h \
CPathShape.h \
CQAccelerate.h \
CQAngleSpinBox.h \
CQDockWidget.h \
CQFillOptionDialog.h \
CQFillOptionTool.h \
CQFontOptionDialog.h \
CQFontOptionTool.h \
CQGradientStopList.h \
CQImageButton.h \
CQImagePreview.h \
CQLayerOption.h \
CQLineCap.h \
CQLineDash.h \
CQLineJoin.h \
CQMatrix2D.h \
CQObjectOption.h \
CQOptionToolSet.h \
CQPointEdit.h \
CQPropertiesOption.h \
CQRadioButtons.h \
CQScrollArea.h \
CQStrokeOptionDialog.h \
CQStrokeOptionTool.h \
CQSwatch.h \
CQTableWidget.h \
CQToolBar.h \
CQuadTree.h \
CQWinWidget.h \
CStack.h \
CStateIterator.h \
CTriangulate2D.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
../include \
../../CSVG/include \
../../CQPropertyTree/include \
../../CQUtil/include \
../../CImageLib/include \
../../CConfig/include \
../../CArgs/include \
../../CFile/include \
../../CFont/include \
../../CRegExp/include \
../../CStrUtil/include \
../../COS/include \
../../CMath/include \
../../CRGBName/include \
../../CUndo/include \
../../CUtil/include \
.

unix:LIBS += \
-L$$LIB_DIR \
-L../../CSVG/lib \
-L../../CQPropertyTree/lib \
-L../../CQUtil/lib \
-L../../CXML/lib \
-L../../CImageLib/lib \
-L../../CArgs/lib \
-L../../CConfig/lib \
-L../../CFont/lib \
-L../../CRegExp/lib \
-L../../CFile/lib \
-L../../CMath/lib \
-L../../CStrUtil/lib \
-L../../CRGBName/lib \
-L../../CUndo/lib \
-L../../CUtil/lib \
-L../../COS/lib \
-lCSVG \
-lCXML \
-lCQPropertyTree \
-lCQUtil \
-lCImageLib \
-lCFont \
-lCArgs \
-lCConfig \
-lCUndo \
-lCFile \
-lCRGBName \
-lCUtil \
-lCOS \
-lCRegExp \
-lCStrUtil \
-ljpeg -lpng -ltre
