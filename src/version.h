/*
 * Copyright (C) 2010 adrian_007, adrian-007 on o2 point pl
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef VERSION_H
#define VERSION_H

#include <QtGlobal>

#define APPNAME "nanoGG"
#define VERSIONSTRING "0.074"

#if defined(Q_OS_SYMBIAN) || defined(Q_WS_S60)
#define OS_NAME "Symbian"
#elif defined(Q_OS_WIN)
#define OS_NAME "Windows"
#elif defined(Q_OS_LINUX)
#define OS_NAME "Linux"
#else
#define OS_NAME "Unknown"
#endif

#define FULLNAME APPNAME " v" VERSIONSTRING

#define NANOGG_DEBUG 0

#endif // VERSION_H
