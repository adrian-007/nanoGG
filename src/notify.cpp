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

#include "notify.h"
#include "debug.h"

#if defined(Q_OS_SYMBIAN) || defined(Q_WS_S60)
#define _SYMBIAN
#endif

#if defined(_SYMBIAN)
#include <hwrmvibra.h>
#include <e32std.h>

CHWRMVibra* Notify::vibra = 0;
#else

#endif

bool Notify::m_bEnabled = false;

Notify::Notify()
{
}

void Notify::init()
{
#if defined(_SYMBIAN)
    if(vibra == 0)
    {
        vibra = CHWRMVibra::NewL();
    }

#endif
}

void Notify::cleanup()
{
#if defined(_SYMBIAN)
    if(vibra)
    {
        delete vibra;
        vibra = 0;
    }
#endif
}

void Notify::message()
{
#if defined(_SYMBIAN)
    if(vibra)
    {
        CHWRMVibra::TVibraStatus iStatus = vibra->VibraStatus();
        CHWRMVibra::TVibraModeState iState = vibra->VibraSettings();

        if(iStatus == CHWRMVibra::EVibraStatusStopped && iState == CHWRMVibra::EVibraModeON)
        {
            // czas, sila
            vibra->StartVibraL(250, -30);
            User::ResetInactivityTime();
        }
    }
#endif
}

void Notify::presence()
{
#if defined(_SYMBIAN)
    if(vibra)
    {
        CHWRMVibra::TVibraStatus iStatus = vibra->VibraStatus();
        CHWRMVibra::TVibraModeState iState = vibra->VibraSettings();

        if(iStatus == CHWRMVibra::EVibraStatusStopped && iState == CHWRMVibra::EVibraModeON)
        {
            // czas, sila
            //vibra->StartVibraL(100, 50);
        }
    }
#endif
}
