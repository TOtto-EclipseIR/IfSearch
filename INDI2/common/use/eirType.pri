SUBDIRS += eirType
LIBS *= -leirType
eirType.file = ../../EIRlibs/eirType/eirType.pro
eirType.target = $$(DESTDIR)/eirType.dll
eirType.depends = eirBase
