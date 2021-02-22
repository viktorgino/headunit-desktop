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
#include "urihandler.h"
#include "element.h"
#include <gst/gst.h>
#include <QtCore/QUrl>
#include <QtCore/QStringList>

namespace QGst {

//static
bool UriHandler::protocolIsSupported(UriType type, const char *protocol)
{
    return gst_uri_protocol_is_supported(static_cast<GstURIType>(type), protocol);
}

//static
ElementPtr UriHandler::makeFromUri(UriType type, const QUrl & uri, const char *elementName)
{
    GError *error = NULL;
    GstElement *e = gst_element_make_from_uri(static_cast<GstURIType>(type), uri.toEncoded(), elementName, &error);
    if (error) {
	throw QGlib::Error(error);
    }
    if (e) {
        gst_object_ref_sink(e);
    }
    return ElementPtr::wrap(e, false);
}

UriType UriHandler::uriType() const
{
    return static_cast<UriType>(gst_uri_handler_get_uri_type(object<GstURIHandler>()));
}

QStringList UriHandler::supportedProtocols() const
{
    QStringList result;
    const char * const *protocols = gst_uri_handler_get_protocols(object<GstURIHandler>());
    if (protocols) {
        for (const char * const *p = protocols; p && *p; ++p) {
            result.append(QString::fromUtf8(*p));
        }
    }
    return result;
}

QUrl UriHandler::uri() const
{
    //QUrl::fromEncoded doesn't work because the returned URI
    //is encoded using percent encoding even for slashes.
    return QUrl::fromPercentEncoding(gst_uri_handler_get_uri(object<GstURIHandler>()));
}

bool UriHandler::setUri(const QUrl & uri)
{
    GError *error = NULL;
    bool result;
    result = gst_uri_handler_set_uri(object<GstURIHandler>(), uri.toEncoded(), &error);
    if (error) {
	throw QGlib::Error(error);
    }
    return result;
}

} //namespace QGst
