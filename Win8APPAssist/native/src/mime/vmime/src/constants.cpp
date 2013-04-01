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

#include "vmime/constants.hpp"


namespace vmime
{


// Media Types
namespace mediaTypes
{
	// Types
	const string::value_type* const TEXT = "text";
	const string::value_type* const MULTIPART = "multipart";
	const string::value_type* const MESSAGE = "message";
	const string::value_type* const APPLICATION = "application";
	const string::value_type* const IMAGE = "image";
	const string::value_type* const AUDIO = "audio";
	const string::value_type* const VIDEO = "video";

	// Sub-types
	const string::value_type* const TEXT_PLAIN = "plain";
	const string::value_type* const TEXT_HTML = "html";
	const string::value_type* const TEXT_RICHTEXT = "richtext";
	const string::value_type* const TEXT_ENRICHED = "enriched";
	const string::value_type* const TEXT_RFC822_HEADERS = "rfc822-headers";  // RFC-1892
	const string::value_type* const TEXT_DIRECTORY = "directory";  // RFC-2426

	const string::value_type* const MULTIPART_MIXED = "mixed";
	const string::value_type* const MULTIPART_RELATED = "related";
	const string::value_type* const MULTIPART_ALTERNATIVE = "alternative";
	const string::value_type* const MULTIPART_PARALLEL = "parallel";
	const string::value_type* const MULTIPART_DIGEST = "digest";
	const string::value_type* const MULTIPART_REPORT = "report";  // RFC-1892

	const string::value_type* const MESSAGE_RFC822 = "rfc822";
	const string::value_type* const MESSAGE_PARTIAL = "partial";
	const string::value_type* const MESSAGE_EXTERNAL_BODY = "external-body";
	const string::value_type* const MESSAGE_DISPOSITION_NOTIFICATION = "disposition-notification";

	const string::value_type* const APPLICATION_OCTET_STREAM = "octet-stream";

	const string::value_type* const IMAGE_JPEG = "jpeg";
	const string::value_type* const IMAGE_GIF = "gif";

	const string::value_type* const AUDIO_BASIC = "basic";

	const string::value_type* const VIDEO_MPEG = "mpeg";
}


// Encoding types
namespace encodingTypes
{
	const string::value_type* const SEVEN_BIT = "7bit";
	const string::value_type* const EIGHT_BIT = "8bit";
	const string::value_type* const BASE64 = "base64";
	const string::value_type* const QUOTED_PRINTABLE = "quoted-printable";
	const string::value_type* const BINARY = "binary";
	const string::value_type* const UUENCODE = "uuencode";
}


// Content disposition types
namespace contentDispositionTypes
{
	const string::value_type* const INLINE = "inline";
	const string::value_type* const ATTACHMENT = "attachment";
}


// Charsets
namespace charsets
{
	const string::value_type* const IBM037 = "IBM037";
	const string::value_type* const IBM437 = "IBM437";
	const string::value_type* const IBM500 = "IBM500";
	const string::value_type* const ASMO_708 = "ASMO-708";
	const string::value_type* const DOS_720 = "DOS-720";
	const string::value_type* const IBM737 = "ibm737";
	const string::value_type* const IBM775 = "ibm775";
	const string::value_type* const IBM850 = "ibm850";
	const string::value_type* const IBM852 = "ibm852";
	const string::value_type* const IBM855 = "IBM855";
	const string::value_type* const IBM857 = "ibm857";
	const string::value_type* const IBM00858 = "IBM00858";
	const string::value_type* const IBM860 = "IBM860";
	const string::value_type* const IBM861 = "ibm861";
	const string::value_type* const DOS_862 = "DOS-862";
	const string::value_type* const IBM863 = "IBM863";
	const string::value_type* const IBM864 = "IBM864";
	const string::value_type* const IBM865 = "IBM865";
	const string::value_type* const CP866 = "cp866";
	const string::value_type* const IBM869 = "ibm869";
	const string::value_type* const IBM870 = "IBM870";
	const string::value_type* const WINDOWS_874 = "windows-874";
	const string::value_type* const CP875 = "cp875";
	const string::value_type* const SHIFT_JIS = "shift_jis";
	const string::value_type* const GB2312 = "gb2312";
	const string::value_type* const KS_C_5601_1987 = "ks_c_5601-1987";
	const string::value_type* const BIG5 = "big5";
	const string::value_type* const IBM1026 = "IBM1026";
	const string::value_type* const IBM01047 = "IBM01047";
	const string::value_type* const IBM01140 = "IBM01140";
	const string::value_type* const IBM01141 = "IBM01141";
	const string::value_type* const IBM01142 = "IBM01142";
	const string::value_type* const IBM01143 = "IBM01143";
	const string::value_type* const IBM01144 = "IBM01144";
	const string::value_type* const IBM01145 = "IBM01145";
	const string::value_type* const IBM01146 = "IBM01146";
	const string::value_type* const IBM01147 = "IBM01147";
	const string::value_type* const IBM01148 = "IBM01148";
	const string::value_type* const IBM01149 = "IBM01149";
	const string::value_type* const UTF_16 = "utf-16";
	const string::value_type* const UNICODEFFFE = "unicodeFFFE";
	const string::value_type* const WINDOWS_1250 = "windows-1250";
	const string::value_type* const WINDOWS_1251 = "windows-1251";
	const string::value_type* const WINDOWS_1252 = "windows-1252";
	const string::value_type* const WINDOWS_1253 = "windows-1253";
	const string::value_type* const WINDOWS_1254 = "windows-1254";
	const string::value_type* const WINDOWS_1255 = "windows-1255";
	const string::value_type* const WINDOWS_1256 = "windows-1256";
	const string::value_type* const WINDOWS_1257 = "windows-1257";
	const string::value_type* const WINDOWS_1258 = "windows-1258";
	const string::value_type* const JOHAB = "Johab";
	const string::value_type* const MACINTOSH = "macintosh";
	const string::value_type* const X_MAC_JAPANESE = "x-mac-japanese";
	const string::value_type* const X_MAC_CHINESETRAD = "x-mac-chinesetrad";
	const string::value_type* const X_MAC_KOREAN = "x-mac-korean";
	const string::value_type* const X_MAC_ARABIC = "x-mac-arabic";
	const string::value_type* const X_MAC_HEBREW = "x-mac-hebrew";
	const string::value_type* const X_MAC_GREEK = "x-mac-greek";
	const string::value_type* const X_MAC_CYRILLIC = "x-mac-cyrillic";
	const string::value_type* const X_MAC_CHINESESIMP = "x-mac-chinesesimp";
	const string::value_type* const X_MAC_ROMANIAN = "x-mac-romanian";
	const string::value_type* const X_MAC_UKRAINIAN = "x-mac-ukrainian";
	const string::value_type* const X_MAC_THAI = "x-mac-thai";
	const string::value_type* const X_MAC_CE = "x-mac-ce";
	const string::value_type* const X_MAC_ICELANDIC = "x-mac-icelandic";
	const string::value_type* const X_MAC_TURKISH = "x-mac-turkish";
	const string::value_type* const X_MAC_CROATIAN = "x-mac-croatian";
	const string::value_type* const UTF_32 = "utf-32";
	const string::value_type* const UTF_32BE = "utf-32BE";
	const string::value_type* const X_CHINESE_CNS = "x-Chinese_CNS";
	const string::value_type* const X_CP20001 = "x-cp20001";
	const string::value_type* const X_CHINESE_ETEN = "x_Chinese-Eten";
	const string::value_type* const X_CP20003 = "x-cp20003";
	const string::value_type* const X_CP20004 = "x-cp20004";
	const string::value_type* const X_CP20005 = "x-cp20005";
	const string::value_type* const X_IA5 = "x-IA5";
	const string::value_type* const X_IA5_GERMAN = "x-IA5-German";
	const string::value_type* const X_IA5_SWEDISH = "x-IA5-Swedish";
	const string::value_type* const X_IA5_NORWEGIAN = "x-IA5-Norwegian";
	const string::value_type* const US_ASCII = "us-ascii";
	const string::value_type* const X_CP20261 = "x-cp20261";
	const string::value_type* const X_CP20269 = "x-cp20269";
	const string::value_type* const IBM273 = "IBM273";
	const string::value_type* const IBM277 = "IBM277";
	const string::value_type* const IBM278 = "IBM278";
	const string::value_type* const IBM280 = "IBM280";
	const string::value_type* const IBM284 = "IBM284";
	const string::value_type* const IBM285 = "IBM285";
	const string::value_type* const IBM290 = "IBM290";
	const string::value_type* const IBM297 = "IBM297";
	const string::value_type* const IBM420 = "IBM420";
	const string::value_type* const IBM423 = "IBM423";
	const string::value_type* const IBM424 = "IBM424";
	const string::value_type* const X_EBCDIC_KOREANEXTENDED = "x-EBCDIC-KoreanExtended";
	const string::value_type* const IBM_THAI = "IBM-Thai";
	const string::value_type* const KOI8_R = "koi8-r";
	const string::value_type* const IBM871 = "IBM871";
	const string::value_type* const IBM880 = "IBM880";
	const string::value_type* const IBM905 = "IBM905";
	const string::value_type* const IBM00924 = "IBM00924";
	const string::value_type* const EUC_JP = "EUC-JP";
	const string::value_type* const X_CP20936 = "x-cp20936";
	const string::value_type* const X_CP20949 = "x-cp20949";
	const string::value_type* const CP1025 = "cp1025";
	const string::value_type* const KOI8_U = "koi8-u";
	const string::value_type* const ISO_8859_1 = "iso-8859-1";
	const string::value_type* const ISO_8859_2 = "iso-8859-2";
	const string::value_type* const ISO_8859_3 = "iso-8859-3";
	const string::value_type* const ISO_8859_4 = "iso-8859-4";
	const string::value_type* const ISO_8859_5 = "iso-8859-5";
	const string::value_type* const ISO_8859_6 = "iso-8859-6";
	const string::value_type* const ISO_8859_7 = "iso-8859-7";
	const string::value_type* const ISO_8859_8 = "iso-8859-8";
	const string::value_type* const ISO_8859_9 = "iso-8859-9";
	const string::value_type* const ISO_8859_13 = "iso-8859-13";
	const string::value_type* const ISO_8859_15 = "iso-8859-15";
	const string::value_type* const X_EUROPA = "x-Europa";
	const string::value_type* const ISO_8859_8_I = "iso-8859-8-i";
	const string::value_type* const ISO_2022_JP = "iso-2022-jp";
	const string::value_type* const CSISO2022JP = "csISO2022JP";
	const string::value_type* const ISO_2022_JP2 = "iso-2022-jp";
	const string::value_type* const ISO_2022_KR = "iso-2022-kr";
	const string::value_type* const X_CP50227 = "x-cp50227";
	const string::value_type* const EUC_JP2 = "euc-jp";
	const string::value_type* const EUC_CN = "EUC-CN";
	const string::value_type* const EUC_KR = "euc-kr";
	const string::value_type* const HZ_GB_2312 = "hz-gb-2312";
	const string::value_type* const GB18030 = "GB18030";
	const string::value_type* const X_ISCII_DE = "x-iscii-de";
	const string::value_type* const X_ISCII_BE = "x-iscii-be";
	const string::value_type* const X_ISCII_TA = "x-iscii-ta";
	const string::value_type* const X_ISCII_TE = "x-iscii-te";
	const string::value_type* const X_ISCII_AS = "x-iscii-as";
	const string::value_type* const X_ISCII_OR = "x-iscii-or";
	const string::value_type* const X_ISCII_KA = "x-iscii-ka";
	const string::value_type* const X_ISCII_MA = "x-iscii-ma";
	const string::value_type* const X_ISCII_GU = "x-iscii-gu";
	const string::value_type* const X_ISCII_PA = "x-iscii-pa";
	const string::value_type* const UTF_7 = "utf-7";
	const string::value_type* const UTF_8 = "utf-8";
}


// Fields
namespace fields
{
	const string::value_type* const RECEIVED = "Received";
	const string::value_type* const FROM = "From";
	const string::value_type* const SENDER = "Sender";
	const string::value_type* const REPLY_TO = "Reply-To";
	const string::value_type* const TO = "To";
	const string::value_type* const CC = "Cc";
	const string::value_type* const BCC = "Bcc";
	const string::value_type* const DATE = "Date";
	const string::value_type* const SUBJECT = "Subject";
	const string::value_type* const ORGANIZATION = "Organization";
	const string::value_type* const USER_AGENT = "User-Agent";
	const string::value_type* const DELIVERED_TO = "Delivered-To";
	const string::value_type* const RETURN_PATH = "Return-Path";
	const string::value_type* const MIME_VERSION = "Mime-Version";
	const string::value_type* const MESSAGE_ID = "Message-Id";
	const string::value_type* const CONTENT_TYPE = "Content-Type";
	const string::value_type* const CONTENT_TRANSFER_ENCODING = "Content-Transfer-Encoding";
	const string::value_type* const CONTENT_DESCRIPTION = "Content-Description";
	const string::value_type* const CONTENT_DISPOSITION = "Content-Disposition";
	const string::value_type* const CONTENT_ID = "Content-Id";
	const string::value_type* const CONTENT_LOCATION = "Content-Location";
	const string::value_type* const IN_REPLY_TO = "In-Reply-To";
	const string::value_type* const REFERENCES = "References";

	const string::value_type* const X_MAILER = "X-Mailer";
	const string::value_type* const X_PRIORITY = "X-Priority";

	// RFC-3798: Message Disposition
	const string::value_type* const ORIGINAL_MESSAGE_ID = "Original-Message-ID";
	const string::value_type* const DISPOSITION_NOTIFICATION_TO = "Disposition-Notification-To";
	const string::value_type* const DISPOSITION_NOTIFICATION_OPTIONS = "Disposition-Notification-Options";
	const string::value_type* const DISPOSITION = "Disposition";
	const string::value_type* const FAILURE = "Failure";
	const string::value_type* const ERROR = "Error";
	const string::value_type* const WARNING = "Warning";
	const string::value_type* const ORIGINAL_RECIPIENT = "Original-Recipient";
	const string::value_type* const FINAL_RECIPIENT = "Final-Recipient";
	const string::value_type* const REPORTING_UA = "Reporting-UA";
	const string::value_type* const MDN_GATEWAY = "MDN-Gateway";
}


// Constants for disposition action modes (RFC-3978).
namespace dispositionActionModes
{
	const string::value_type* const MANUAL = "manual";
	const string::value_type* const AUTOMATIC = "automatic";
}


// Constants for disposition sending modes (RFC-3798).
namespace dispositionSendingModes
{
	const string::value_type* const SENT_MANUALLY = "MDN-sent-manually";
	const string::value_type* const SENT_AUTOMATICALLY ="MDN-sent-automatically";
}


// Constants for disposition types (RFC-3798).
namespace dispositionTypes
{
	const string::value_type* const DISPLAYED = "displayed";
	const string::value_type* const DELETED = "deleted";
}


// Constants for disposition modifiers (RFC-3798).
namespace dispositionModifiers
{
	const string::value_type* const ERROR = "error";
}


} // vmime
