SUBDIRS += eirImage
LIBS *= -leirBase -leirTypes -leirImage
eirImage.file = ../../EIRlibs/eirImage/eirImage.pro
eirImage.target = $$(DESTDIR)/eirImage.dll
eirImage.depends = eirBase eirTypes eirCore eirExe eirGUI
