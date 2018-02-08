SUBDIRS += eirIb2
LIBS *= -leirIb2
eirIb2.file = ../../EIRlibs/eirIb2/eirIb2.pro
eirIb2.target = $$(DESTDIR)/eirIb2.dll
eirIb2.depends = eirBase eirTypes eirCore
