#
# Copyright (C) 2010 adrian_007, adrian-007 on o2 point pl
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#

DEPLOYMENTFOLDERS =

symbian:TARGET.UID3 = 0xE2A478FD

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

symbian:LIBS += -lhwrmvibraclient -leuser

SOURCES += src/notify.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    src/ggstream.cpp \
    src/ggprotocol.cpp \
    src/database.cpp \
    src/conversationpage.cpp \
    src/contactlist.cpp \
    src/contactgroup.cpp \
    src/contact.cpp \
    zlib/zutil.c \
    zlib/uncompr.c \
    zlib/trees.c \
    zlib/inftrees.c \
    zlib/inflate.c \
    zlib/inffast.c \
    zlib/infback.c \
    zlib/gzread.c \
    zlib/gzlib.c \
    zlib/deflate.c \
    zlib/crc32.c \
    zlib/compress.c \
    zlib/adler32.c \
    src/debug.cpp \
    src/chattab.cpp \
    src/flickcharm.cpp \
    src/FlickCharmGlobal.cpp

HEADERS += src/version.h \
    src/notify.h \
    src/mainwindow.h \
    src/ggstream.h \
    src/ggprotocol.h \
    src/database.h \
    src/conversationpage.h \
    src/contactlist.h \
    src/contactgroup.h \
    src/contact.h \
    zlib/zutil.h \
    zlib/zlib.h \
    zlib/zconf.h \
    zlib/trees.h \
    zlib/inftrees.h \
    zlib/inflate.h \
    zlib/inffixed.h \
    zlib/inffast.h \
    zlib/gzguts.h \
    zlib/deflate.h \
    zlib/crc32.h \
    src/debug.h \
    src/chattab.h \
    src/flickcharm.h \
    src/FlickCharmGlobal.h

FORMS += mainwindow.ui \
    chattab.ui

QT += xml \
    sql \
    network \
    webkit

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    changelog.txt \
    default.css \
    chat.css

RESOURCES += \
    resources.qrc
