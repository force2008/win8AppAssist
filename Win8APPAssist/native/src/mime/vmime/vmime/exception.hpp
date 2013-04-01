//
// VMime library (http://www.vmime.org)
// Copyright (C) 2002-2009 Vincent Richard <vincent@vincent-richard.net>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 3 of
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// Linking this library statically or dynamically with other modules is making
// a combined work based on this library.  Thus, the terms and conditions of
// the GNU General Public License cover the whole combination.
//

#ifndef VMIME_EXCEPTION_HPP_INCLUDED
#define VMIME_EXCEPTION_HPP_INCLUDED


#include <stdexcept>

#include "vmime/config.hpp"
#include "vmime/base.hpp"

namespace vmime
{


/** Base class for VMime exceptions.
  */

class exception : public std::exception
{
private:

	string m_what;
	exception* m_other;

	exception();

public:

	exception(const string& what, const exception& other = NO_EXCEPTION);
	exception(const exception& e);

	virtual ~exception() throw();

	/** Return a description of the error.
	  *
	  * @return error message
	  */
	const char* what() const throw();

	/** Return a description of the error.
	  *
	  * @return error message
	  */
	const char* what() throw();

	/** Return the next exception in the chain (encapsulated exception).
	  *
	  * @return next exception in the chain
	  */
	const exception* other() const throw();

	/** Return a name identifying the exception.
	  *
	  * @return exception name
	  */
	virtual const char* name() const throw();

	/** Clone this object.
	  *
	  * @return a new copy of this object
	  */
	virtual exception* clone() const;

protected:

	static const exception NO_EXCEPTION;
};



/** List of all VMime exceptions. */

namespace exceptions
{


class bad_field_type : public vmime::exception
{
public:

	bad_field_type(const exception& other = NO_EXCEPTION);
	~bad_field_type() throw();

	exception* clone() const;
	const char* name() const throw();
};


class charset_conv_error : public vmime::exception
{
public:

	charset_conv_error(const string& what = "", const exception& other = NO_EXCEPTION);
	~charset_conv_error() throw();

	exception* clone() const;
	const char* name() const throw();
};


/** No encoder has been found for the specified encoding name.
  */

class no_encoder_available : public vmime::exception
{
public:

	no_encoder_available(const string& name, const exception& other = NO_EXCEPTION);
	~no_encoder_available() throw();

	exception* clone() const;
	const char* name() const throw();
};


/** No algorithm has been found for the specified name.
  */

class no_digest_algorithm_available : public vmime::exception
{
public:

	no_digest_algorithm_available(const string& name, const exception& other = NO_EXCEPTION);
	~no_digest_algorithm_available() throw();

	exception* clone() const;
	const char* name() const throw();
};


class no_such_parameter : public vmime::exception
{
public:

	no_such_parameter(const string& name, const exception& other = NO_EXCEPTION);
	~no_such_parameter() throw();

	exception* clone() const;
	const char* name() const throw();
};


class no_such_field : public vmime::exception
{
public:

	no_such_field(const exception& other = NO_EXCEPTION);
	~no_such_field() throw();

	exception* clone() const;
	const char* name() const throw();
};


class no_such_part : public vmime::exception
{
public:

	no_such_part(const exception& other = NO_EXCEPTION);
	~no_such_part() throw();

	exception* clone() const;
	const char* name() const throw();
};


class no_such_mailbox : public vmime::exception
{
public:

	no_such_mailbox(const exception& other = NO_EXCEPTION);
	~no_such_mailbox() throw();

	exception* clone() const;
	const char* name() const throw();
};


class no_such_message_id : public vmime::exception
{
public:

	no_such_message_id(const exception& other = NO_EXCEPTION);
	~no_such_message_id() throw();

	exception* clone() const;
	const char* name() const throw();
};


class no_such_address : public vmime::exception
{
public:

	no_such_address(const exception& other = NO_EXCEPTION);
	~no_such_address() throw();

	exception* clone() const;
	const char* name() const throw();
};


class open_file_error : public vmime::exception
{
public:

	open_file_error(const exception& other = NO_EXCEPTION);
	~open_file_error() throw();

	exception* clone() const;
	const char* name() const throw();
};


class no_factory_available : public vmime::exception
{
public:

	no_factory_available(const exception& other = NO_EXCEPTION);
	~no_factory_available() throw();

	exception* clone() const;
	const char* name() const throw();
};


class no_platform_handler : public vmime::exception
{
public:

	no_platform_handler(const exception& other = NO_EXCEPTION);
	~no_platform_handler() throw();

	exception* clone() const;
	const char* name() const throw();
};


/** No expeditor specified.
  */

class no_expeditor : public vmime::exception
{
public:

	no_expeditor(const exception& other = NO_EXCEPTION);
	~no_expeditor() throw();

	exception* clone() const;
	const char* name() const throw();
};


/** No recipient specified.
  */

class no_recipient : public vmime::exception
{
public:

	no_recipient(const exception& other = NO_EXCEPTION);
	~no_recipient() throw();

	exception* clone() const;
	const char* name() const throw();
};


class no_object_found : public vmime::exception
{
public:

	no_object_found(const exception& other = NO_EXCEPTION);
	~no_object_found() throw();

	exception* clone() const;
	const char* name() const throw();
};


/** There is no property with that name in the set.
  */

class no_such_property : public vmime::exception
{
public:

	no_such_property(const string& name, const exception& other = NO_EXCEPTION);
	~no_such_property() throw();

	exception* clone() const;
	const char* name() const throw();
};


/** Bad type specified when reading property.
  */

class invalid_property_type : public vmime::exception
{
public:

	invalid_property_type(const exception& other = NO_EXCEPTION);
	~invalid_property_type() throw();

	exception* clone() const;
	const char* name() const throw();
};


/** Bad argument was passed to the function.
  */

class invalid_argument : public vmime::exception
{
public:

	invalid_argument(const exception& other = NO_EXCEPTION);
	~invalid_argument() throw();

	exception* clone() const;
	const char* name() const throw();
};


/** Underlying operating system error.
  */

class system_error : public vmime::exception
{
public:

	system_error(const string& what, const exception& other = NO_EXCEPTION);
	~system_error() throw();

	exception* clone() const;
	const char* name() const throw();
};


/** The URL is malformed.
  */

class malformed_url : public exception
{
public:

	malformed_url(const string& error, const exception& other = NO_EXCEPTION);
	~malformed_url() throw();

	exception* clone() const;
	const char* name() const throw();
};

/** Authentication exception.
  */

class authentication_exception : public vmime::exception
{
public:

	authentication_exception(const string& what, const exception& other = NO_EXCEPTION);
	~authentication_exception() throw();

	exception* clone() const;
	const char* name() const throw();
};


/** The requested information cannot be provided.
  */

class no_auth_information : public authentication_exception
{
public:

	no_auth_information(const exception& other = NO_EXCEPTION);
	~no_auth_information() throw();

	exception* clone() const;
	const char* name() const throw();
};
} // exceptions


} // vmime


#endif // VMIME_EXCEPTION_HPP_INCLUDED
