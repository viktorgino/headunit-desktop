/*
    Copyright (C) 2010  George Kiagiadakis <kiagiadakis.george@gmail.com>

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
#ifndef QGST_PARSE_H
#define QGST_PARSE_H

#include "element.h"
#include <QtCore/QString>

/*! \namespace QGst::Parse
 * \brief Wrappers for the GstParse methods
 * \note The gst_parse_bin_from_description() method is available as QGst::Bin::fromDescription().
 */

namespace QGst {
namespace Parse {

/*! Creates a pipeline out of a pipeline description.
 * The syntax is the same as the one used in the gst-launch tool.
 * \throws QGlib::Error when there was a problem creating the pipeline
 */
QTGSTREAMER_EXPORT ElementPtr launch(const char *description);

/*! \overload */
inline ElementPtr launch(const QString & description)
{
    return launch(description.toUtf8().constData());
}

/*! \overload
 * This function takes an array of strings, which are joined before they are
 * parsed as a description. This is the function used by the gst-launch tool.
 * \throws QGlib::Error when there was a problem creating the pipeline
 */
QTGSTREAMER_EXPORT ElementPtr launch(const char *argv[]);

} //namespace Parse
} //namespace QGst

#endif // QGST_PARSE_H
