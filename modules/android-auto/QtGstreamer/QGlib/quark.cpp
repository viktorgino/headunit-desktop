/*
    Copyright (C) 2009-2010  George Kiagiadakis <kiagiadakis.george@gmail.com>
    Copyright (C) 2010 Collabora Ltd.
      @author George Kiagiadakis <george.kiagiadakis@collabora.co.uk>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "quark.h"
#include <glib.h>

namespace QGlib {

//static
Quark Quark::fromString(const char *str)
{
    return g_quark_from_string(str);
}

//static
Quark Quark::tryString(const char *str)
{
    return g_quark_try_string(str);
}

QString Quark::toString() const
{
    return QString::fromUtf8(g_quark_to_string(m_quark));
}

} //namespace QGlib
