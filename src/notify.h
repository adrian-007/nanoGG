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

#ifndef NOTIFY_H
#define NOTIFY_H

#include "version.h"

#if defined(Q_OS_SYMBIAN) || defined(Q_WS_S60)
class CHWRMVibra;
#else

#endif

class Notify {
public:
    Notify();

    static void init();
    static void cleanup();

    static void message();
    static void presence();

    static void setEnabled(bool e) {
        m_bEnabled = e;
    }

private:
    static bool m_bEnabled;
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_S60)
    static CHWRMVibra* vibra;
#else

#endif

};

#endif // NOTIFY_H
