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
#ifndef QGST_INIT_H
#define QGST_INIT_H

#include "global.h"

namespace QGst {

    /*! \overload
     * \note You need to include <QGst/Init> to use this function.
     */
    QTGSTREAMER_EXPORT void init();

    /*! Initializes the GStreamer library, setting up internal path lists,
     * registering built-in elements, and loading standard plugins.
     * \note
     * \li This function also calls QGlib::init(), so there is no need to call it explicitly.
     * \li You need to include <QGst/Init> to use this function.
     *
     * \param argc pointer to the application's argc
     * \param argv pointer to the application's argv
     * \throws QGlib::Error when initialization fails
     */
    QTGSTREAMER_EXPORT void init(int *argc, char **argv[]);

    /*! Clean up any resources created by GStreamer in init().
     *
     * It is normally not needed to call this function in a normal application as the resources
     * will automatically be freed when the program terminates. This function is therefore mostly
     * used by testsuites and other memory profiling tools.
     *
     * After this call GStreamer (including this method) should not be used anymore.
     *
     * \note You need to include <QGst/Init> to use this function.
     */
    QTGSTREAMER_EXPORT void cleanup();

} //namespace QGst

#endif //QGST_INIT_H
