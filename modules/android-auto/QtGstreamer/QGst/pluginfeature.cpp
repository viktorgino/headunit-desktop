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
#include "pluginfeature.h"
#include <gst/gst.h>

namespace QGst {

uint PluginFeature::rank() const
{
    return gst_plugin_feature_get_rank(object<GstPluginFeature>());
}

void PluginFeature::setRank(uint rank)
{
    gst_plugin_feature_set_rank(object<GstPluginFeature>(), rank);
}

QString PluginFeature::name() const
{
    return QString::fromUtf8(gst_plugin_feature_get_name(object<GstPluginFeature>()));
}

void PluginFeature::setName(const char *name)
{
    gst_plugin_feature_set_name(object<GstPluginFeature>(), name);
}

bool PluginFeature::isAtLeastVersion(uint major, uint minor, uint micro) const
{
    return gst_plugin_feature_check_version(object<GstPluginFeature>(), major, minor, micro);
}

PluginFeaturePtr PluginFeature::load()
{
    return PluginFeaturePtr::wrap(gst_plugin_feature_load(object<GstPluginFeature>()), false);
}

}
