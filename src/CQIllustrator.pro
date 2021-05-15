TEMPLATE = app

TARGET = CQIllustrator

DEPENDPATH += .

QT += widgets

QMAKE_CXXFLAGS += \
-std=c++14 \
-DCQUTIL_PEN \
-DCQUTIL_BRUSH \
-DCQUTIL_FONT \
-DCQUTIL_IMAGE \
-DCBRUSH_IMAGE \

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
CQSVGFontObj.cpp \
CQSVGImageData.cpp \
\
CQAccelerate.cpp \
CQFillOptionDialog.cpp \
CQFillOptionTool.cpp \
CQFontOptionDialog.cpp \
CQFontOptionTool.cpp \
CQGradientStopList.cpp \
CQImagePreview.cpp \
CQLayerOption.cpp \
CQLineCap.cpp \
CQLineJoin.cpp \
CQMatrix2D.cpp \
CQObjectOption.cpp \
CQOptionToolSet.cpp \
CQPointEdit.cpp \
CQPropertiesOption.cpp \
CQScrollArea.cpp \
CQStrokeOptionDialog.cpp \
CQStrokeOptionTool.cpp \
CQSwatch.cpp \
\
CAxis2D.cpp \
CGenPoly.cpp \
CKuhnMunkres.cpp \
CLog.cpp \
CPathShape.cpp \

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
CQSVGFontObj.h \
CQSVGImageData.h \
\
CQAccelerate.h \
CQDockWidget.h \
CQFillOptionDialog.h \
CQFillOptionTool.h \
CQFontOptionDialog.h \
CQFontOptionTool.h \
CQGradientStopList.h \
CQImagePreview.h \
CQLayerOption.h \
CQLineCap.h \
CQLineJoin.h \
CQMatrix2D.h \
CQObjectOption.h \
CQOptionToolSet.h \
CQPointEdit.h \
CQPropertiesOption.h \
CQScrollArea.h \
CQStrokeOptionDialog.h \
CQStrokeOptionTool.h \
CQSwatch.h \
CQuadTree.h \
\
CAxis2D.h \
CBooleanOp.h \
CGenPoly.h \
CKuhnMunkres.h \
CLog.h \
CPathPartType.h \
CPathShape.h \
CStack.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
../include \
../../CSVG/include \
../../CCSS/include \
../../CQPropertyTree/include \
../../CQUtil/include \
../../CImageLib/include \
../../CConfig/include \
../../CArgs/include \
../../CFileUtil/include \
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
-L../../CJavaScript/lib \
-L../../CCSS/lib \
-L../../CJson/lib \
-L../../CXML/lib \
-L../../CImageLib/lib \
-L../../CArgs/lib \
-L../../CConfig/lib \
-L../../CFont/lib \
-L../../CRegExp/lib \
-L../../CFile/lib \
-L../../CFileUtil/lib \
-L../../CMath/lib \
-L../../CStrUtil/lib \
-L../../CRGBName/lib \
-L../../CUndo/lib \
-L../../CUtil/lib \
-L../../COS/lib \
-lCSVG \
-lCJavaScript \
-lCCSS \
-lCJson \
-lCXML \
-lCQPropertyTree \
-lCQUtil \
-lCImageLib \
-lCFont \
-lCArgs \
-lCConfig \
-lCUndo \
-lCFile \
-lCFileUtil \
-lCMath \
-lCRGBName \
-lCUtil \
-lCOS \
-lCRegExp \
-lCStrUtil \
-ljpeg -lpng -ltre
