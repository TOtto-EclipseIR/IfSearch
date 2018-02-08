# -------------------------------------------------
# Project created by QtCreator 2010-03-22T14:38:14
# -------------------------------------------------
QMAKE_CXXFLAGS += -fpermissive
QMAKE_LFLAGS += --enable-stdcall-fixup -Wl,--enable-auto-import -Wl,--enable-runtime-pseudo-reloc

INCLUDEPATH *= ../EIRlibs

DEFINES += IJM
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
        DESTDIR = ../../common/dbg
        LIBS += -L../../common/dbg
}
else {
        DESTDIR = ../../common/bin
        LIBS += -L../../common/bin
}
INCLUDEPATH += .

include(../../common/OpenCV22.pri)
QT += network \
    xml
TARGET = IfSearch
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
win32:RC_FILE = IfSearch.rc
OTHER_FILES = ../../../IJM.pri \
    rebuild.bat \
    IfSearch.rc \
    DOSbox.bat \
    eIRonly24.ico \
    INDIface24.ico \
    ../../common/WinRes.rc \
    Rectangle.hh \
    Key.hh \
    Eyes.hh \
    ../../common/libtests.pri \
    ../../common/libs.pri \
    ../../common/IJM.pri \
    ../../common/apps.pri \
    detectorscale.txt \
    ../../INDI2/common/HeightGridSample.jpg \
    ../../INDI2/common/HeightGrid.cdf
SOURCES += main.cpp \
    InfoOutputSetting.cpp \
    Return.cpp \
    EigenFaceData.cpp \
    EigenFaceGenerator.cpp \
    HaarDetector.cpp \
    WindowsProcess.cpp \
    InputHotdir.cpp \
    ImageCacheEntry.cpp \
    FileWriteProfile.cpp \
    GroupObjects.cpp \
    FrontalFaceDetector.cpp \
    EyeDetector.cpp \
    EigenFaceVector.cpp \
    EigenFaceTemplate.cpp \
    EigenFaceParameters.cpp \
    EigenFaceMaskedArray-IO.cpp \
    EigenFaceMaskedArray-Image.cpp \
    EigenFaceMaskedArray.cpp \
    EigenFaceGenerator2.cpp \
    EigenFaceBitMask.cpp \
    EigenFace.cpp \
    InfoOutputFile.cpp \
    FeatureSet.cpp \
    ImageMarker.cpp \
    FaceBase-File.cpp \
    FaceBase.cpp \
    Info.cpp \
    Settings.cpp \
    SettingProperty.cpp \
    SettingItem.cpp \
    Setting.cpp \
    InfoItem.cpp \
    ImageSource.cpp \
    ImageCache.cpp \
    ImageCacheWriter.cpp \
    ImageCacheCleaner.cpp \
    GreyImage.cpp \
    QQRect.cpp \
    InfoFunction.cpp \
    EigenFaceFace.cpp \
    ImageInfo.cpp \
    FileWriter.cpp \
    SettingsScanner.cpp \
    IfSearch-ProcessGrab.cpp \
    IfSearch-ProcessFace.cpp \
    IfSearchEnroll.cpp \
    IfSearch.cpp \
    IfSearchInit.cpp \
    IfSearchSlots.cpp \
    IfSearchSearch.cpp \
    IfSearchRetrieve.cpp \
    InfoOutputs.cpp \
    ddtcore.cpp \
    AttributeSet.cpp \
    InfoThread.cpp \
    FaceBase-Access.cpp \
    fbPerson.cpp \
    fbValue.cpp \
    IfSearch-AvgFace.cpp \
    TransformProperties.cpp \
    AnyColor.cpp \
    ColorModel.cpp \
    SkinMatcher.cpp \
    SkinDetector.cpp \
    ClothesMatchProperties.cpp \
    ClothesMatcher.cpp \
    CharacteristicColor.cpp \
    SkinMatchProperties.cpp \
    Resolver.cpp \
    HeightGrid.cpp \
    IdString.cpp \
    VersionInfo.cpp \
    Key.cpp \
    AverageFace.cpp \
    AnyColorMatrix.cpp \
    ../EIRlibs/eirBase/BaseIdBehavior.cpp \
    ../EIRlibs/eirBase/AbstractIdString.cpp \
    ../EIRlibs/eirCore/VariableId.cpp \
    ../EIRlibs/eirCore/VariableGroup.cpp \
    ../EIRlibs/eirCore/Variable.cpp \
    ../EIRlibs/eirIb2/FramePerformance.cpp \
    ../EIRlibs/eirCore/VariableType.cpp \
    ../EIRlibs/eirIb2/FrameStatistics.cpp \
    ../EIRlibs/eirCore/eirCore.cpp \
    ../EIRlibs/eirFile/QQDir.cpp \
    ../EIRlibs/eirFile/CsvWriter.cpp \
    IfSearchAuthenticate.cpp \
    ../EIRlibs/eirCore/VariableIdList.cpp \
    ../EIRlibs/eirTypes/MillisecondTime.cpp \
    ../EIRlibs/eirTypes/MillisecondDelta.cpp \
    ../EIRlibs/eirCore/VariableSet.cpp \
    ../EIRlibs/eirCore/IdGenerator.cpp \
    ../EIRlibs/eirCore/BasicId.cpp \
    ../EIRlibs/eirBase/Severity.cpp \
    ../EIRlibs/eirBase/Enumeration.cpp \
    IfSearchEnroll2.cpp \
    ../EIRlibs/eirBase/BaseLog.cpp \
    ../EIRlibs/eirBase/eirBase.cpp \
    ../EIRlibs/eirBase/Library.cpp \
    EigenFaceSearchTier.cpp \
    EigenFaceSearchSettings.cpp \
    EigenFaceSearcher.cpp \
    EigenFaceSearchResultList.cpp \
    EigenFaceSearchPerson.cpp \
    EigenFaceSearchResult.cpp \

HEADERS += IfSearch.h \
    EigenFaceSearchSettings.h \
    EigenFaceGenerator.h \
    INDIffd.h \
    Detector.h \
    InputHotdir.h \
    INDIef.h \
    FileWriteProfile.h \
    EigenFaceTemplate.h \
    EigenFaceData.h \
    EigenFace.h \
    iSettings.h \
    InfoMacros.h \
    FaceBase.h \
    Settings.h \
    Return.h \
    ImageSource.h \
    ImageCache.h \
    FileWriter.h \
    FeatureSet.h \
    DDTcore.h \
    iImageCache.h \
    Info.h \
    NamedArray.h \
    InfoThread.h \
    InfoSeverity.h \
    InfoOutputSetting.h \
    InfoOutputs.h \
    InfoOutputQDebug.h \
    InfoOutputFile.h \
    InfoOutputBase.h \
    InfoItem.h \
    InfoFunction.h \
    NamedEnum.h \
    NamedOrdinal.h \
    EigenFaceMaskedArray.h \
    EigenFaceBitMask.h \
    AttributeSet.h \
    QQRect.h \
    GreyImage.h \
    EigenFaceVector.h \
    EigenFaceParameters.h \
    ImageInfo.h \
    WindowsProcess.h \
    EigenFaceFace.h \
    fbPerson.h \
    DualMap.h \
    fbValue.h \
    ImageMarker.h \
    version.h \
    version.h \
    ../../common/version.h \
    TransformProperties.h \
    Eyes.h \
    AnyColor.h \
    ColorModel.h \
    SkinMatcher.h \
    SkinDetector.h \
    NamedEnum2.h \
    ClothesMatchProperties.h \
    ClothesMatcher.h \
    CharacteristicColor.h \
    SkinMatchProperties.h \
    Histogram.h \
    Resolver.h \
    Rectangle.h \
    HeightGrid.h \
    IdString.h \
    Matrix.h \
    VersionInfo.h \
    Property.h \
    Key.h \
    AverageFace.h \
    AnyColorMatrix.h \
    SkinColorTables.inc \
    ColorRangeWeightVector.inc \
    QProperty.h \
    ../EIRlibs/eirBase/BaseIdBehavior.h \
    ../EIRlibs/eirBase/AbstractIdString.h \
    ../EIRlibs/eirCore/VariableId.h \
    ../EIRlibs/eirCore/VariableGroup.h \
    ../EIRlibs/eirCore/Variable.h \
    ../EIRlibs/eirIb2/FramePerformance.h \
    ../EIRlibs/eirCore/VariableType.h \
    ../EIRlibs/eirIf2/ObjDetParameters.h \
    ../EIRlibs/eirIb2/FrameStatistics.h \
    ../EIRlibs/eirCore/eirCore.h \
    ../EIRlibs/eirFile/QQDir.h \
    ../EIRlibs/eirFile/CsvWriter.h \
    ../EIRlibs/eirCore/VariableIdList.h \
    ../EIRlibs/eirTypes/MillisecondTime.h \
    ../EIRlibs/eirTypes/MillisecondDelta.h \
    ../EIRlibs/eirCore/VariableSet.h \
    ../EIRlibs/eirCore/IdGenerator.h \
    ../EIRlibs/eirCore/BasicId.h \
    ../EIRlibs/eirBase/Severity.h \
    ../EIRlibs/eirBase/Enumeration.h \
    EigenFaceSearchTier.h \
    EigenFaceSearcher.h \
    EigenFaceSearchResultList.h \
    EigenFaceSearchPerson.h \
    EigenFaceSearchResult.h \
    ../EIRlibs/eirBase/DataProperty.h \
    ../EIRlibs/eirBase/Property.h











































