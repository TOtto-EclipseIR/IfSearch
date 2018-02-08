# -------------------------------------------------
# Project created by QtCreator 2010-03-22T14:38:14
# -------------------------------------------------
QMAKE_CXXFLAGS *= -fpermissive \
    -DIFSEARCH_STATIC \
#    -DQ_DECL_IMPORT= \
#    -DQ_DECL_EXPORT=

QMAKE_CXXFLAGS_DEBUG *= -static-libgcc -static-libstdc++ -ggdb -O0

QMAKE_LFLAGS += --enable-stdcall-fixup -Wl,--enable-auto-import -Wl,--enable-runtime-pseudo-reloc

include(QtHttp.pri)

INCLUDEPATH *= ../../INDI2/EIRlibs
INCLUDEPATH *= ../../INDI2/INDIlibs

#LIBS *= -lCrp32DLL -L"C:/CrypKey SDK/Build 7723/Lib/DLL"

DEFINES += IJM
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
        DESTDIR = ../../common/dbg5
        LIBS += -L../../common/dbg5
}
else {
        DESTDIR = ../../common/bin5
        LIBS += -L../../common/bin5
}
INCLUDEPATH += .

#LIBS *= -lFSBridge
#LIBS *= -leirBase -leirTypes -leirCore -leirExe
#LIBS *= -leirFrameSource -leirImage -leirFile -leirNetwork

include(../../common/OpenCV22.pri)
QT += network \
    xml
TARGET = IfSearchB04
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
    ../../INDI2/common/HeightGrid.cdf \
    Qt5PortNotes.txt \
    QtHttp.pri
SOURCES += main.cpp \
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
    EigenFaceSearcher.cpp \
    InfoOutputFile.cpp \
    FeatureSet.cpp \
    ImageMarker.cpp \
    FaceBase-File.cpp \
    FaceBase.cpp \
    Info.cpp \
    Settings.cpp \
    SettingProperty.cpp \
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
    ../../INDI2/EIRlibs/eirBase/BaseIdBehavior.cpp \
    ../../INDI2/EIRlibs/eirBase/AbstractIdString.cpp \
    ../../INDI2/EIRlibs/eirCore/VariableId.cpp \
    ../../INDI2/EIRlibs/eirCore/VariableGroup.cpp \
    ../../INDI2/EIRlibs/eirCore/Variable.cpp \
    ../../INDI2/EIRlibs/eirIb2/FramePerformance.cpp \
    ../../INDI2/EIRlibs/eirCore/VariableType.cpp \
    ../../INDI2/EIRlibs/eirIb2/FrameStatistics.cpp \
    ../../INDI2/EIRlibs/eirCore/eirCore.cpp \
    ../../INDI2/EIRlibs/eirFile/QQDir.cpp \
    ../../INDI2/EIRlibs/eirFile/CsvWriter.cpp \
    IfSearchAuthenticate.cpp \
    ../../INDI2/EIRlibs/eirCore/VariableIdList.cpp \
    ../../INDI2/EIRlibs/eirTypes/MillisecondTime.cpp \
    ../../INDI2/EIRlibs/eirTypes/MillisecondDelta.cpp \
    ../../INDI2/EIRlibs/eirCore/VariableSet.cpp \
    ../../INDI2/EIRlibs/eirCore/IdGenerator.cpp \
    ../../INDI2/EIRlibs/eirCore/BasicId.cpp \
    ../../INDI2/EIRlibs/eirBase/Severity.cpp \
    ../../INDI2/EIRlibs/eirBase/Enumeration.cpp \
    IfSearchEnroll2.cpp \
    ../../INDI2/EIRlibs/eirBase/BaseLog.cpp \
    ../../INDI2/EIRlibs/eirBase/eirBase.cpp \
    ../../INDI2/EIRlibs/eirBase/Library.cpp \
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
    QtHttp/qhttp.cpp \
    QtHttp/qhttpauthenticator.cpp
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
    ../../INDI2/EIRlibs/eirBase/BaseIdBehavior.h \
    ../../INDI2/EIRlibs/eirBase/AbstractIdString.h \
    ../../INDI2/EIRlibs/eirCore/VariableId.h \
    ../../INDI2/EIRlibs/eirCore/VariableGroup.h \
    ../../INDI2/EIRlibs/eirCore/Variable.h \
    ../../INDI2/EIRlibs/eirIb2/FramePerformance.h \
    ../../INDI2/EIRlibs/eirCore/VariableType.h \
    ../../INDI2/EIRlibs/eirIf2/ObjDetParameters.h \
    ../../INDI2/EIRlibs/eirIb2/FrameStatistics.h \
    ../../INDI2/EIRlibs/eirCore/eirCore.h \
    ../../INDI2/EIRlibs/eirFile/QQDir.h \
    ../../INDI2/EIRlibs/eirFile/CsvWriter.h \
    ../../INDI2/EIRlibs/eirCore/VariableIdList.h \
    ../../INDI2/EIRlibs/eirTypes/MillisecondTime.h \
    ../../INDI2/EIRlibs/eirTypes/MillisecondDelta.h \
    ../../INDI2/EIRlibs/eirCore/VariableSet.h \
    ../../INDI2/EIRlibs/eirCore/IdGenerator.h \
    ../../INDI2/EIRlibs/eirCore/BasicId.h \
    ../../INDI2/EIRlibs/eirBase/Severity.h \
    ../../INDI2/EIRlibs/eirBase/Enumeration.h \
    EigenFaceStatus.h \
    EigenFaceSearcher.h \
    EigenFaceSearchTier.h \
    EigenFaceSimilarity.h \
    EigenFaceSimilarityEntry.h \
    EigenFaceSimilarityResult.h \
    EigenFaceSimilarityResultList.h \
    ../../INDI2/EIRlibs/eirBase/DataProperty.h \
    QtHttp/qhttp.h \
    QtHttp/qhttpauthenticator_p.h \
    QtHttp/qringbuffer_p.h \
    EigenFaceSearchPerson.h \
    EigenFaceSearchResult.h \
    EigenFaceSearchResultList.h














































