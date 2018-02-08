SUBDIRS += TokenAuth
TokenAuth.file = ../../apps/TokenAuth/TokenAuth.pro
TokenAuth.target = $$(DESTDIR)/TokenAuth.exe
TokenAuth.depends = eirBase eirTypes eirCore eirFile eirGUI eirSDK1
