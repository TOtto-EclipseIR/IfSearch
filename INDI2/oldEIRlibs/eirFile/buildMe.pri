SUBDIRS += eirFile
LIBS *= -leirFile
eirFile.file = ../../EIRlibs/eirFile/eirFile.pro
eirFile.target = $$(DESTDIR)/eirFile.dll
eirFile.depends = eirTypes eirCore
