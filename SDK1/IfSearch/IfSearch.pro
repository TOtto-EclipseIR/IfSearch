# -------------------------------------------------
# Project created by QtCreator 2010-03-22T14:38:14
# -------------------------------------------------

QT *= widgets
QT *= network
QT *= xml

TARGET = IfSearch
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

QMAKE_CXXFLAGS += -fpermissive \
    -DQ_DECL_IMPORT= \
    -DQ_DECL_EXPORT=
QMAKE_LFLAGS += --enable-stdcall-fixup -Wl,--enable-auto-import -Wl,--enable-runtime-pseudo-reloc
DEFINES *= IJM
#DEFINES *= ENABLE_WATCHDOG
#DEFINES *= BUILD_NETCAM
#DEFINES *= ENABLE_CLOTHES
QMAKE_LFLAGS -= -Wl,--enable-auto-import
QMAKE_LFLAGS -= -Wl,--enable-runtime-pseudo-reloc
message("QMAKE_LFLAGS" = $$QMAKE_LFLAGS)

include(../../common/OpenCV4.pri)

INCLUDEPATH *= ../../INDI2/oldEIRlibs
INCLUDEPATH *= ../../INDI2/INDIlibs
INCLUDEPATH += .

OUR_DEST1 = ../../common/<dir><ver>
#!build_pass:message($$OUR_DEST1)
CONFIG(debug, debug|release) {
    OUR_DEST2 = $$replace(OUR_DEST1, <dir>, dbg)
}
else {
    OUR_DEST2 = $$replace(OUR_DEST1, <dir>, bin)
}
#!build_pass:message($$OUR_DEST2)

OUR_DEST3 = $$replace(OUR_DEST2, <ver>, $$QT_MAJOR_VERSION)
#!build_pass:message($$OUR_DEST3)
OUR_DEST = $$OUR_DEST3

CONFIG += debug_and_release
CONFIG(debug, debug|release) {
        DESTDIR = $$OUR_DEST
        LIBS += -L$$OUR_DEST
}
else {
        DESTDIR = $$OUR_DEST
        LIBS += -L$$OUR_DEST
}
!build_pass:message(DESTDIR = $$DESTDIR)


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
    DetectorResult.cpp \
    HaarDetector-group.cpp \
    InfoOutputSetting.cpp \
    Return.cpp \
    EigenFaceData.cpp \
    EigenFaceGenerator.cpp \
    HaarDetector.cpp \
    WindowsProcess.cpp \
    InputHotdir.cpp \
    ImageCacheEntry.cpp \
    FileWriteProfile.cpp \
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
    EigenFaceSearcher.cpp \
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
    ../../INDI2/oldEIRlibs/eirBase/BaseIdBehavior.cpp \
    ../../INDI2/oldEIRlibs/eirBase/AbstractIdString.cpp \
    ../../INDI2/oldEIRlibs/eirCore/VariableId.cpp \
    ../../INDI2/oldEIRlibs/eirCore/VariableGroup.cpp \
    ../../INDI2/oldEIRlibs/eirCore/Variable.cpp \
    ../../INDI2/oldEIRlibs/eirIb2/FramePerformance.cpp \
    ../../INDI2/oldEIRlibs/eirCore/VariableType.cpp \
    ../../INDI2/oldEIRlibs/eirIb2/FrameStatistics.cpp \
    ../../INDI2/oldEIRlibs/eirCore/eirCore.cpp \
    ../../INDI2/oldEIRlibs/eirFile/QQDir.cpp \
    ../../INDI2/oldEIRlibs/eirFile/CsvWriter.cpp \
    IfSearchAuthenticate.cpp \
    ../../INDI2/oldEIRlibs/eirCore/VariableIdList.cpp \
    ../../INDI2/oldEIRlibs/eirTypes/MillisecondTime.cpp \
    ../../INDI2/oldEIRlibs/eirTypes/MillisecondDelta.cpp \
    ../../INDI2/oldEIRlibs/eirCore/VariableSet.cpp \
    ../../INDI2/oldEIRlibs/eirCore/IdGenerator.cpp \
    ../../INDI2/oldEIRlibs/eirCore/BasicId.cpp \
    ../../INDI2/oldEIRlibs/eirBase/Severity.cpp \
    ../../INDI2/oldEIRlibs/eirBase/Enumeration.cpp \
    IfSearchEnroll2.cpp \
    ../../INDI2/oldEIRlibs/eirBase/BaseLog.cpp \
    ../../INDI2/oldEIRlibs/eirBase/eirBase.cpp \
    ../../INDI2/oldEIRlibs/eirBase/Library.cpp \
    EigenFaceSearchSettings.cpp \
    EigenFaceSearchTier.cpp \
    EigenFaceStatus.cpp \
    EigenFaceSearchPerson.cpp \
    EigenFaceSearchResultList.cpp \
    EigenFaceSearchResult.cpp \
    EigenFaceSimilarity.cpp \
    EigenFaceSimilarityEntry.cpp \
    EigenFaceSimilarityResult.cpp \
    EigenFaceSimilarityResultList.cpp \
    IfSearchSimilarity.cpp \
    ../../INDI2/oldEIRlibs/eirExe/WatchDog.cpp
HEADERS += IfSearch.h \
    DetectorResult.h \
    EigenFaceSearchSettings.h \
    EigenFaceGenerator.h \
    EyeDetector.h \
    FrontalFaceDetector.h \
    HaarDetector.h \
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
    ../../INDI2/oldEIRlibs/eirBase/BaseIdBehavior.h \
    ../../INDI2/oldEIRlibs/eirBase/AbstractIdString.h \
    ../../INDI2/oldEIRlibs/eirCore/VariableId.h \
    ../../INDI2/oldEIRlibs/eirCore/VariableGroup.h \
    ../../INDI2/oldEIRlibs/eirCore/Variable.h \
    ../../INDI2/oldEIRlibs/eirIb2/FramePerformance.h \
    ../../INDI2/oldEIRlibs/eirCore/VariableType.h \
    ../../INDI2/oldEIRlibs/eirIf2/ObjDetParameters.h \
    ../../INDI2/oldEIRlibs/eirIb2/FrameStatistics.h \
    ../../INDI2/oldEIRlibs/eirCore/eirCore.h \
    ../../INDI2/oldEIRlibs/eirFile/QQDir.h \
    ../../INDI2/oldEIRlibs/eirFile/CsvWriter.h \
    ../../INDI2/oldEIRlibs/eirCore/VariableIdList.h \
    ../../INDI2/oldEIRlibs/eirTypes/MillisecondTime.h \
    ../../INDI2/oldEIRlibs/eirTypes/MillisecondDelta.h \
    ../../INDI2/oldEIRlibs/eirCore/VariableSet.h \
    ../../INDI2/oldEIRlibs/eirCore/IdGenerator.h \
    ../../INDI2/oldEIRlibs/eirCore/BasicId.h \
    ../../INDI2/oldEIRlibs/eirBase/Severity.h \
    ../../INDI2/oldEIRlibs/eirBase/Enumeration.h \
    EigenFaceStatus.h \
    EigenFaceSearcher.h \
    EigenFaceSearchTier.h \
    EigenFaceSimilarity.h \
    EigenFaceSimilarityEntry.h \
    EigenFaceSimilarityResult.h \
    EigenFaceSimilarityResultList.h \
    ../../INDI2/oldEIRlibs/eirBase/DataProperty.h \
    ../../INDI2/oldEIRlibs/eirExe/WatchDog.h
