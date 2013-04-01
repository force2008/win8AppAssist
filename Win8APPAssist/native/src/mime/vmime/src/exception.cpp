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

#include "vmime/exception.hpp"


namespace vmime {


//
// exception
//

const exception exception::NO_EXCEPTION;


exception::exception()
	: m_what(""), m_other(NULL)
{
}


exception::exception(const string& what, const exception& other)
	: m_what(what), m_other(&other != &NO_EXCEPTION ? other.clone() : NULL)
{
}


exception::exception(const exception& e)
	: std::exception(), m_what(e.what()), m_other(e.m_other == NULL ? NULL : e.m_other->clone())
{
}


exception::~exception() throw()
{
	delete (m_other);
}


const char* exception::what() const throw()
{
	return (m_what.c_str());
}


const char* exception::what() throw()
{
	return (m_what.c_str());
}


const exception* exception::other() const throw()
{
	return (m_other);
}


const char* exception::name() const throw()
{
	return "exception";
}


exception* exception::clone() const
{
	return new exception(*this);
}



namespace exceptions
{


//
// bad_field_type
//

bad_field_type::~bad_field_type() throw() {}
bad_field_type::bad_field_type(const exception& other)
	: exception("Bad field type.", other) {}

exception* bad_field_type::clone() const { return new bad_field_type(*this); }
const char* bad_field_type::name() const throw() { return "bad_field_type"; }



//
// charset_conv_error
//

charset_conv_error::~charset_conv_error() throw() {}
charset_conv_error::charset_conv_error(const string& what, const exception& other)
	: exception(what.empty() ? "Charset conversion error." : what, other) {}

exception* charset_conv_error::clone() const { return new charset_conv_error(*this); }
const char* charset_conv_error::name() const throw() { return "charset_conv_error"; }


//
// no_encoder_available
//

no_encoder_available::~no_encoder_available() throw() {}
no_encoder_available::no_encoder_available(const string& name, const exception& other)
	: exception("No encoder available: '" + name + "'.", other) {}

exception* no_encoder_available::clone() const { return new no_encoder_available(*this); }
const char* no_encoder_available::name() const throw() { return "no_encoder_available"; }


//
// no_digest_algorithm_available
//

no_digest_algorithm_available::~no_digest_algorithm_available() throw() {}
no_digest_algorithm_available::no_digest_algorithm_available(const string& name, const exception& other)
	: exception("No algorithm available: '" + name + "'.", other) {}

exception* no_digest_algorithm_available::clone() const { return new no_digest_algorithm_available(*this); }
const char* no_digest_algorithm_available::name() const throw() { return "no_digest_algorithm_available"; }


//
// no_such_parameter
//

no_such_parameter::~no_such_parameter() throw() {}
no_such_parameter::no_such_parameter(const string& name, const exception& other)
	: exception(string("Parameter not found: '") + name + string("'."), other) {}

exception* no_such_parameter::clone() const { return new no_such_parameter(*this); }
const char* no_such_parameter::name() const throw() { return "no_such_parameter"; }


//
// no_such_field
//

no_such_field::~no_such_field() throw() {}
no_such_field::no_such_field(const exception& other)
	: exception("Field not found.", other) {}

exception* no_such_field::clone() const { return new no_such_field(*this); }
const char* no_such_field::name() const throw() { return "no_such_field"; }


//
// no_such_part
//

no_such_part::~no_such_part() throw() {}
no_such_part::no_such_part(const exception& other)
	: exception("Part not found.", other) {}

exception* no_such_part::clone() const { return new no_such_part(*this); }
const char* no_such_part::name() const throw() { return "no_such_part"; }


//
// no_such_mailbox
//

no_such_mailbox::~no_such_mailbox() throw() {}
no_such_mailbox::no_such_mailbox(const exception& other)
	: exception("Mailbox not found.", other) {}

exception* no_such_mailbox::clone() const { return new no_such_mailbox(*this); }
const char* no_such_mailbox::name() const throw() { return "no_such_mailbox"; }


//
// no_such_message_id
//

no_such_message_id::~no_such_message_id() throw() {}
no_such_message_id::no_such_message_id(const exception& other)
	: exception("Message-Id not found.", other) {}

exception* no_such_message_id::clone() const { return new no_such_message_id(*this); }
const char* no_such_message_id::name() const throw() { return "no_such_message_id"; }


//
// no_such_address
//

no_such_address::~no_such_address() throw() {}
no_such_address::no_such_address(const exception& other)
	: exception("Address not found.", other) {}

exception* no_such_address::clone() const { return new no_such_address(*this); }
const char* no_such_address::name() const throw() { return "no_such_address"; }


//
// open_file_error
//

open_file_error::~open_file_error() throw() {}
open_file_error::open_file_error(const exception& other)
	: exception("Error opening file.", other) {}

exception* open_file_error::clone() const { return new open_file_error(*this); }
const char* open_file_error::name() const throw() { return "open_file_error"; }


//
// no_factory_available
//

no_factory_available::~no_factory_available() throw() {}
no_factory_available::no_factory_available(const exception& other)
	: exception("No factory available.", other) {}

exception* no_factory_available::clone() const { return new no_factory_available(*this); }
const char* no_factory_available::name() const throw() { return "no_factory_available"; }


//
// no_platform_handler
//

no_platform_handler::~no_platform_handler() throw() {}
no_platform_handler::no_platform_handler(const exception& other)
	: exception("No platform handler installed.", other) {}

exception* no_platform_handler::clone() const { return new no_platform_handler(*this); }
const char* no_platform_handler::name() const throw() { return "no_platform_handler"; }


//
// no_expeditor
//

no_expeditor::~no_expeditor() throw() {}
no_expeditor::no_expeditor(const exception& other)
	: exception("No expeditor specified.", other) {}

exception* no_expeditor::clone() const { return new no_expeditor(*this); }
const char* no_expeditor::name() const throw() { return "no_expeditor"; }


//
// no_recipient
//

no_recipient::~no_recipient() throw() {}
no_recipient::no_recipient(const exception& other)
	: exception("No recipient specified.", other) {}

exception* no_recipient::clone() const { return new no_recipient(*this); }
const char* no_recipient::name() const throw() { return "no_recipient"; }


//
// no_object_found
//

no_object_found::~no_object_found() throw() {}
no_object_found::no_object_found(const exception& other)
	: exception("No object found.", other) {}

exception* no_object_found::clone() const { return new no_object_found(*this); }
const char* no_object_found::name() const throw() { return "no_object_found"; }


//
// no_such_property
//

no_such_property::~no_such_property() throw() {}
no_such_property::no_such_property(const string& name, const exception& other)
	: exception(std::string("No such property: '") + name + string("'."), other) { }

exception* no_such_property::clone() const { return new no_such_property(*this); }
const char* no_such_property::name() const throw() { return "no_such_property"; }


//
// invalid_property_type
//

invalid_property_type::~invalid_property_type() throw() {}
invalid_property_type::invalid_property_type(const exception& other)
	: exception("Invalid property type.", other) {}

exception* invalid_property_type::clone() const { return new invalid_property_type(*this); }
const char* invalid_property_type::name() const throw() { return "invalid_property_type"; }


//
// invalid_argument
//

invalid_argument::~invalid_argument() throw() {}
invalid_argument::invalid_argument(const exception& other)
	: exception("Invalid argument.", other) {}

exception* invalid_argument::clone() const { return new invalid_argument(*this); }
const char* invalid_argument::name() const throw() { return "invalid_argument"; }


//
// system_error
//

system_error::~system_error() throw() { }
system_error::system_error(const string& what, const exception& other)
	: exception(what, other) {}

exception* system_error::clone() const { return new system_error(*this); }
const char* system_error::name() const throw() { return "system_error"; }


//
// malformed_url
//

malformed_url::~malformed_url() throw() {}
malformed_url::malformed_url(const string& error, const exception& other)
	: exception("Malformed URL: " + error + ".", other) {}

exception* malformed_url::clone() const { return new malformed_url(*this); }
const char* malformed_url::name() const throw() { return "malformed_url"; }



//
// authentication_exception
//

authentication_exception::~authentication_exception() throw() {}
authentication_exception::authentication_exception(const string& what, const exception& other)
	: exception(what, other) {}

exception* authentication_exception::clone() const { return new authentication_exception(*this); }
const char* authentication_exception::name() const throw() { return "authentication_exception"; }


//
// no_auth_information
//

no_auth_information::~no_auth_information() throw() {}
no_auth_information::no_auth_information(const exception& other)
	: authentication_exception("Information cannot be provided.", other) {}

exception* no_auth_information::clone() const { return new no_auth_information(*this); }
const char* no_auth_information::name() const throw() { return "no_auth_information"; }

} // exceptions


} // vmime

