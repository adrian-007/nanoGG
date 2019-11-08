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

#ifndef GGSTREAM_H
#define GGSTREAM_H

#include <QDataStream>
#include <QByteArray>

class GGStream : public QDataStream
{
public:
    GGStream(QByteArray* a, QIODevice::OpenMode mode = QIODevice::ReadWrite);

    void appendString(const QString& str, bool prependLength = false);
    void appendStringNullTerminated(const QString& str);
    void appendAndRepeat(unsigned char c, int count);
    void appendByteArray(const QByteArray& array);
    void appendVariableValue(const unsigned int val);

    QString readString(int len);
    QString readStringNullTerminated();
    QByteArray read(unsigned int len);
    unsigned int readVariableValue(int* length = 0);

    void foward(int bytes);
};

#endif // GGSTREAM_H
