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
#include "colorbalance.h"
#include <gst/video/colorbalance.h>

namespace QGst {

QString ColorBalanceChannel::label() const
{
    return QString::fromUtf8(object<GstColorBalanceChannel>()->label);
}

int ColorBalanceChannel::minValue() const
{
    return object<GstColorBalanceChannel>()->min_value;
}

int ColorBalanceChannel::maxValue() const
{
    return object<GstColorBalanceChannel>()->max_value;
}


ColorBalanceType ColorBalance::type() const
{
    return static_cast<ColorBalanceType>(gst_color_balance_get_balance_type(object<GstColorBalance>()));
}

QList<ColorBalanceChannelPtr> ColorBalance::channels() const
{
    QList<ColorBalanceChannelPtr> result;
    const GList *list = gst_color_balance_list_channels(object<GstColorBalance>());
    while(list) {
        result.append(ColorBalanceChannelPtr::wrap(GST_COLOR_BALANCE_CHANNEL(list->data)));
        list = list->next;
    }
    return result;
}

int ColorBalance::value(const ColorBalanceChannelPtr & channel) const
{
    return gst_color_balance_get_value(object<GstColorBalance>(), channel);
}

void ColorBalance::setValue(const ColorBalanceChannelPtr & channel, int value)
{
    gst_color_balance_set_value(object<GstColorBalance>(), channel, value);
}

} //namespace QGst
