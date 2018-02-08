SUBDIRS += eirFrameSource
LIBS *= -leirFrameSource
eirFrameSource.file = ../../EIRlibs/eirFrameSource/eirFrameSource.pro
eirFrameSource.target = $$(DESTDIR)/eirFrameSource.dll
eirFrameSource.depends = eirImage eirTypes eirCore eirExe eirFile eirImage eirNetwork
