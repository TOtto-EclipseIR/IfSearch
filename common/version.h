#ifndef COMMONVERSION_H
#define COMMONVERSION_H

#define CVER_STRING     "v2.02"
#define CVER_MAJOR      2
#define CVER_MINOR      2
#define CVER_BRANCH     8
#define CVER_RELEASE    1
#define CVER_COPYRIGHT  "Copyright (c) 2003-2015, " \
                        "Eclipse Identity Recognition Corporation. " \
                        "All rights reserved worldwide."
#define CVER_PRODNAME   "Eclipse FusionIR"
#define CVER_COMPANY    "Eclipse Identity Recognition Corporation"
#define CVER_ORGNAME    "EclipseIR"
#define CVER_ICON       "../../../common/art/eIRcairo.ico"

#define VOS_NT          0x00040000L
#define VFT_APP         0x00000001L
#define VFT_DLL         0x00000002L

#ifndef VER_STRING
# define VER_STRING CVER_STRING
#endif

#ifndef VER_MAJOR
# define VER_MAJOR CVER_MAJOR
#endif

#ifndef VER_MINOR
# define VER_MINOR CVER_MINOR
#endif

#ifndef VER_BRANCH
# define VER_BRANCH CVER_BRANCH
#endif

#ifndef VER_RELEASE
# define VER_RELEASE CVER_RELEASE
#endif

#ifndef VER_COPYRIGHT
# define VER_COPYRIGHT CVER_COPYRIGHT
#endif

#ifndef VER_PRODNAME
# define VER_PRODNAME CVER_PRODNAME
#endif

#ifndef VER_COMPANY
# define VER_COMPANY CVER_COMPANY
#endif

#ifndef VER_ORGNAME
# define VER_ORGNAME CVER_ORGNAME
#endif

#ifndef VER_ICON
# define VER_ICON CVER_ICON
#endif
#endif // COMMONVERSION_H
