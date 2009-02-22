# -------------------------------------------------
# Project created by QtCreator 2009-02-14T15:30:46
# -------------------------------------------------
QT += opengl
TARGET = QFLR5
TEMPLATE = app
SOURCES += MainFrame.cpp \
    XDirect/XFoil.cpp \
    XDirect/XFoilAnalysisDlg.cpp \
    XDirect/XFoilAdvancedDlg.cpp \
    XDirect/XDirect.cpp \
    XDirect/TwoDPanelDlg.cpp \
    XDirect/InterpolateFoilsDlg.cpp \
    XDirect/FoilPolarDlg.cpp \
    XDirect/FoilGeomDlg.cpp \
    XDirect/TEGapDlg.cpp \
    XDirect/ReListDlg.cpp \
    XDirect/NacaFoilDlg.cpp \
    XDirect/LEDlg.cpp \
    XDirect/FoilCoordDlg.cpp \
    XDirect/FlapDlg.cpp \
    XDirect/CAddDlg.cpp \
    XDirect/BatchDlg.cpp \
    Objects/WPolar.cpp \
    Objects/WOpp.cpp \
    Objects/Wing.cpp \
    Objects/Surface.cpp \
    Objects/Spline.cpp \
    Objects/Panel.cpp \
    Objects/Sf.cpp \
    Objects/Quaternion.cpp \
    Objects/OpPoint.cpp \
    Objects/Frame.cpp \
    Objects/POpp.cpp \
    Objects/Polar.cpp \
    Objects/CVector.cpp \
    Objects/Plane.cpp \
    Objects/Pf.cpp \
    Objects/CRectangle.cpp \
    Objects/Foil.cpp \
    Objects/Body.cpp \
    Misc/UnitsDlg.cpp \
    Misc/RenameDlg.cpp \
    Misc/LinePickerDlg.cpp \
    Misc/LineDelegate.cpp \
    Misc/LineCbBox.cpp \
    Misc/LineButton.cpp \
    Misc/FloatEditDelegate.cpp \
    Misc/DisplaySettingsDlg.cpp \
    Misc/ColorButton.cpp \
    Misc/FloatEdit.cpp \
    Misc/AboutQ5.cpp \
    Miarex/WPolarDlg.cpp \
    Miarex/WingDlg.cpp \
    Miarex/WingDelegate.cpp \
    Miarex/VLMAnalysisDlg.cpp \
    Miarex/PlaneDlg.cpp \
    Miarex/PanelAnalysisDlg.cpp \
    Miarex/LLTAnalysisDlg.cpp \
    Miarex/ImportWingDlg.cpp \
    Miarex/GLWidget.cpp \
    Miarex/GLLightDlg.cpp \
    Miarex/GL3dViewDlg.cpp \
    Miarex/CpScaleDlg.cpp \
    Miarex/BodyTableDelegate.cpp \
    Miarex/Miarex.cpp \
    Miarex/ArcBall.cpp \
    Miarex/BodyGridDlg.cpp \
    Miarex/BodyScaleDlg.cpp \
    Graph/QGraph.cpp \
    Graph/GraphWidget.cpp \
    Graph/GraphVariableDlg.cpp \
    Graph/GraphDlg.cpp \
    Graph/Graph.cpp \
    Graph/Curve.cpp \
    Design/AFoil.cpp \
    Main.cpp \
    Globals.cpp \
    TwoDWidget.cpp \
    Miarex/GL3dBodyDlg.cpp
HEADERS += MainFrame.h \
    XDirect/XFoil.h \
    XDirect/XFoilAnalysisDlg.h \
    XDirect/XFoilAdvancedDlg.h \
    XDirect/XDirect.h \
    XDirect/TwoDPanelDlg.h \
    XDirect/TEGapDlg.h \
    XDirect/InterpolateFoilsDlg.h \
    XDirect/FoilGeomDlg.h \
    XDirect/FoilCoordDlg.h \
    XDirect/ReListDlg.h \
    XDirect/NacaFoilDlg.h \
    XDirect/LEDlg.h \
    XDirect/FoilPolarDlg.h \
    XDirect/FlapDlg.h \
    XDirect/CAddDlg.h \
    XDirect/BatchDlg.h \
    Objects/WPolar.h \
    Objects/WOpp.h \
    Objects/Wing.h \
    Objects/Surface.h \
    Objects/Spline.h \
    Objects/Sf.h \
    Objects/OpPoint.h \
    Objects/Quaternion.h \
    Objects/POpp.h \
    Objects/Polar.h \
    Objects/CVector.h \
    Objects/Plane.h \
    Objects/Pf.h \
    Objects/CRectangle.h \
    Objects/Panel.h \
    Objects/Frame.h \
    Objects/Foil.h \
    Objects/Body.h \
    Misc/UnitsDlg.h \
    Misc/RenameDlg.h \
    Misc/LinePickerDlg.h \
    Misc/LineDelegate.h \
    Misc/FloatEditDelegate.h \
    Misc/DisplaySettingsDlg.h \
    Misc/ColorButton.h \
    Misc/LineCbBox.h \
    Misc/LineButton.h \
    Misc/FloatEdit.h \
    Misc/AboutQ5.h \
    Miarex/WPolarDlg.h \
    Miarex/WingDlg.h \
    Miarex/WingDelegate.h \
    Miarex/VLMAnalysisDlg.h \
    Miarex/PlaneDlg.h \
    Miarex/PanelAnalysisDlg.h \
    Miarex/LLTAnalysisDlg.h \
    Miarex/ImportWingDlg.h \
    Miarex/GLLightDlg.h \
    Miarex/GL3dViewDlg.h \
    Miarex/CpScaleDlg.h \
    Miarex/Miarex.h \
    Miarex/GLWidget.h \
    Miarex/ArcBall.h \
    Miarex/BodyGridDlg.h \
    Miarex/BodyTableDelegate.h \
    Miarex/BodyScaleDlg.h \
    Graph/GraphWidget.h \
    Graph/GraphVariableDlg.h \
    Graph/Graph.h \
    Graph/GraphDlg.h \
    Graph/Curve.h \
    Graph/QGraph.h \
    Design/AFoil.h \
    Globals.h \
    TwoDWidget.h \
    Miarex/GL3dBodyDlg.h
RESOURCES += qflr5.qrc