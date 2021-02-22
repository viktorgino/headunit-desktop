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
#ifndef QGST_COLORBALANCE_H
#define QGST_COLORBALANCE_H

#include "global.h"
#include "../QGlib/object.h"

namespace QGst {

/*! \headerfile colorbalance.h <QGst/ColorBalance>
 * \brief Wrapper class for GstColorBalanceChannel
 */
class QTGSTREAMER_EXPORT ColorBalanceChannel : public QGlib::Object
{
    QGST_WRAPPER(ColorBalanceChannel)
public:
    QString label() const;
    int minValue() const;
    int maxValue() const;
};

/*! \headerfile colorbalance.h <QGst/ColorBalance>
 * \brief Wrapper class for GstColorBalance
 */
class QTGSTREAMER_EXPORT ColorBalance : public QGlib::Interface
{
    QGST_WRAPPER(ColorBalance)
public:
    ColorBalanceType type() const;
    QList<ColorBalanceChannelPtr> channels() const;
    int value(const ColorBalanceChannelPtr & channel) const;
    void setValue(const ColorBalanceChannelPtr & channel, int value);
};

} //namespace QGst

QGST_REGISTER_TYPE(QGst::ColorBalanceChannel)
QGST_REGISTER_TYPE(QGst::ColorBalance)
QGLIB_REGISTER_INTERFACE(QGst::ColorBalance)

#endif // QGST_COLORBALANCE_H
