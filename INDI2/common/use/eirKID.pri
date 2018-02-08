SUBDIRS += eirKID
LIBS *= -leirKID
eirKID.file = ../../EIRlibs/eirKID/eirKID.pro
eirKID.target = $$(DESTDIR)/eirKID.dll
eirKID.depends = eirBase
