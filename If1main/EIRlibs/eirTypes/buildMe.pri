SUBDIRS += eirTypes
LIBS *= -leirTypes
eirTypes.file = ../../EIRlibs/eirTypes/eirTypes.pro
eirTypes.target = $$(DESTDIR)/eirTypes.dll
eirTypes.depends = eirBase
