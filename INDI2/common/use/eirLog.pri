SUBDIRS += eirLog
LIBS *= -leirLog
eirLog.file = ../../EIRlibs/eirLog/eirLog.pro
eirLog.target = $$(DESTDIR)/eirLog.dll
eirLog.depends = eirBase eirType
