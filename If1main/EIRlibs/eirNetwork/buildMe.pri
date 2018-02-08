SUBDIRS += eirNetwork
LIBS *= -leirNetwork
eirNetwork.file = ../../EIRlibs/eirNetwork/eirNetwork.pro
eirNetwork.target = $$(DESTDIR)/eirNetwork.dll
eirNetwork.depends = eirTypes eirCore
