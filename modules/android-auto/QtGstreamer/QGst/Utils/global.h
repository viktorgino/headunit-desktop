/*
    Copyright (C) 2011 Collabora Ltd.
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
#ifndef QGST_UTILS_GLOBAL_H
#define QGST_UTILS_GLOBAL_H

// workaround for https://bugreports.qt-project.org/browse/QTBUG-22829
#if defined(Q_MOC_RUN) && !defined(BOOST_TT_HAS_OPERATOR_HPP_INCLUDED)
#define BOOST_TT_HAS_OPERATOR_HPP_INCLUDED
#endif

#include <QtCore/QtGlobal>

/* defined by cmake when building this library */
#if defined(QtGStreamerUtils_EXPORTS) || defined(Qt5GStreamerUtils_EXPORTS)
# define QTGSTREAMERUTILS_EXPORT Q_DECL_EXPORT
#else
# define QTGSTREAMERUTILS_EXPORT Q_DECL_IMPORT
#endif

#if !defined(Q_OS_WIN) && !defined(Q_CC_NOKIAX86) && \
    !defined(Q_CC_RVCT) && defined(QT_VISIBILITY_AVAILABLE)
# define QTGSTREAMERUTILS_NO_EXPORT __attribute__((visibility("hidden")))
#else
# define QTGSTREAMERUTILS_NO_EXPORT
#endif

#endif
