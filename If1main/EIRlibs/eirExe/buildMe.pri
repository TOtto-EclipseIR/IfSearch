SUBDIRS += eirExe
LIBS *= -leirExe
eirExe.file = ../../EIRlibs/eirExe/eirExe.pro
eirExe.target = $$(DESTDIR)/eirExe.dll
eirExe.depends = eirBase eirTypes eirCore
