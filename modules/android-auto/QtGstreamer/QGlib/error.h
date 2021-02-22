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
#ifndef QGLIB_ERROR_H
#define QGLIB_ERROR_H

#include "global.h"
#include "type.h"
#include "quark.h"
#include <exception>

namespace QGlib {

/*! \headerfile error.h <QGlib/Error>
 * \brief Wrapper class for GError
 */
class QTGLIB_EXPORT Error : public std::exception
{
public:
    /*! Wraps an existing GError into an Error.
     * \note the constructed Error takes ownership of \a error */
    Error(GError *error = NULL);

    /*! Creates a new Error with the given \a domain, \a code and \a message */
    Error(Quark domain, int code, const QString & message);

    static Error copy(GError *error);

    Error(const Error & other);
    Error & operator=(const Error & other);
    virtual ~Error() throw();

    /*! Use message() instead. This method is provided
     * to implement the std::exception interface. */
    virtual const char* what() const throw();

    /*! \returns the domain of the error.
     * The domain indicates the module where the error has happened. */
    Quark domain() const;

    /*! \returns a code that describes the error */
    int code() const;

    /*! \returns a human-readable message that describes the error */
    QString message() const;

    /*! \returns a pointer to the underlying GError
     * \note no copy is made */
    operator GError *();

    /*! \returns a const pointer to the underlying GError
     * \note no copy is made */
    operator const GError *() const;

private:
    GError *m_error;
};

/*! \relates QGlib::Error */
QTGLIB_EXPORT QDebug operator<<(QDebug dbg, const Error & error);

} //namespace QGlib

QGLIB_REGISTER_TYPE(QGlib::Error)

#endif // QGLIB_ERROR_H
