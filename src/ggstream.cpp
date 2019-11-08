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

#include "ggstream.h"
#include "debug.h"

GGStream::GGStream(QByteArray* a, QIODevice::OpenMode mode) : QDataStream(a, mode) {
    this->setByteOrder(QDataStream::LittleEndian);
}

void GGStream::appendString(const QString& str, bool prependLength /*= false*/) {
    if(prependLength) {
        (*this) << (unsigned int)str.length();
    }

    for(int i = 0; i < str.length(); ++i) {
        *this << (unsigned char)str[i].toAscii();
    }
}

void GGStream::appendStringNullTerminated(const QString& str) {
    appendString(str);
    *this << (unsigned char)'\0';
}

void GGStream::appendAndRepeat(unsigned char c, int count) {
    for(int i = 0; i < count; ++i) {
        *this << c;
    }
}

void GGStream::appendByteArray(const QByteArray& array) {
    for(int i = 0; i < array.length(); ++i) {
        (*this) << (unsigned char)array[i];
    }
}

void GGStream::appendVariableValue(unsigned int val)
{
    unsigned int parts = 0;
    int i = 1;

    if(val != 0)
    {
        for(i = 0; i < 4; ++i)
        {
            if(val >= (1 << 7))
            {
                parts |= ((unsigned int)(val & 0x000000FF) | (1 << 7)) << (8*(3-i));
                val >>= 7;
            }
            else
            {
                parts |= ((unsigned int)(val & 0x000000FF)) << (8*(3-i));
                break;
            }
        }
    }

    for(int j = 0; j <= i; ++j)
    {
        (*this) << (unsigned char)((parts >> (8*(3-j))) & 0x000000FF);
    }
}

QString GGStream::readString(int len) {
    QByteArray ret;
    ret.reserve(len);
    unsigned char c;

    for(int i = 0; i < len; ++i) {
        (*this) >> c;
        ret[i] = c;
    }
    return QString(ret);
}

QString GGStream::readStringNullTerminated() {
    QByteArray ret;
    unsigned char c;

    while(true) {
        (*this) >> c;
        if(c == 0) {
            break;
        }
        ret += c;
    }
    return QString(ret);
}

QByteArray GGStream::read(unsigned int len) {
    QByteArray buf;
    buf.resize(len);
    this->readRawData((char*)buf.data(), buf.length());
    return buf;
}

unsigned int GGStream::readVariableValue(int* len /*= 0*/)
{
    unsigned char byte;
    int i = 0;
    unsigned int ret = 0;

    do
    {
        if(this->readRawData((char*)&byte, 1) != 1)
        {
            ret = (unsigned int)-1;

            if(len != 0)
            {
                *len = -1;
            }

            break;
        }

        ret |= ((unsigned int)(byte & ~(1 << 7))) << (7*i);

        ++i;
    }
    while(byte & (1 << 7));

    if(len != 0)
    {
        *len = i;
    }

    return ret;
}

void GGStream::foward(int bytes) {
    unsigned char c;
    for(int i = 0; i < bytes; ++i) {
        (*this) >> c;
    }
}
