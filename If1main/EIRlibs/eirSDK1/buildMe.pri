SUBDIRS += eirSDK1
LIBS *= -leirSDK1
eirSDK1.file = ../../EIRlibs/eirSDK1/eirSDK1.pro
eirSDK1.target = $$(DESTDIR)/eirSDK1.dll
eirSDK1.depends = eirBase eirTypes eirCore eirFile eirGUI
