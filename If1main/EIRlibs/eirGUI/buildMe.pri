SUBDIRS += eirGUI
LIBS *= -leirGUI
eirGUI.file = ../../EIRlibs/eirGUI/eirGUI.pro
eirGUI.target = $$(DESTDIR)/eirGUI.dll
eirGUI.depends = eirTypes eirCore eirExe
