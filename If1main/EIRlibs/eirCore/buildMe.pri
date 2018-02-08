SUBDIRS += eirCore
LIBS *= -leirCore
eirCore.file = ../../EIRlibs/eirCore/eirCore.pro
eirCore.target = $$(DESTDIR)/eirCore.dll
eirCore.depends = eirBase eirTypes
