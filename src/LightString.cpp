#include "ESPAsyncWebServer.h"



void LightString::changeToStringType(bool keepData ) {
	if (_type == STRING)
		return;
	FlashStringStorage temp = flashString;

	flashString.~FlashStringStorage();

	_type = STRING;
	if (keepData)
		new (&string) String(temp);
	else
		new (&string) String();
}


void LightString::changeToFlashStringType() {
	if (_type == FLASH)
		return;

	string.~String();
	_type = FLASH;
	new (&flashString) FlashStringStorage();
}


LightString::LightString(FlashString s) :
	_type(FLASH) {
	new (&flashString) FlashStringStorage((FlashStringStorage)s);
}

LightString::LightString(const String& s) :
	_type(STRING) {
	new (&string) String(s);
}

LightString::LightString(String&& rval) :
	_type(STRING) {
	new (&string) String(std::move(rval));
}


LightString::LightString(const LightString& s) :
		_type(s._type)
{
	if (_type == FLASH)
		new (&flashString) FlashStringStorage(s.flashString);
	else
		new (&string) String(s.string);
}

LightString::LightString(LightString&& s) :
		_type(std::move(s._type))
{
	if (_type == FLASH)
		new (&flashString) FlashStringStorage(std::move(s.flashString));
	else
		new (&string) String(std::move(s.string));
}


LightString::LightString(const char* s)
	: _type(STRING) {
	new (&string) String(s);
}



LightString::~LightString() {
	if (_type == STRING) {
		   string.~String();
	} else {
		   flashString.~FlashStringStorage();
	}
}


void LightString::clear() {
	if (_type == STRING) {
		string.~String();
		_type = FLASH;
		new (&flashString) FlashStringStorage();
	} else {
		flashString = nullptr;
	}
}


String LightString::asString() const {
	if (_type == FLASH)
		return String(FPSTR(flashString));
	return string;
}

LightString& LightString::operator=(const char* s) {
	changeToStringType();
	string = s;
	return *this;
}

LightString& LightString::operator=(FlashString s) {
	*this = LightString(s);
	return *this;
}


LightString& LightString::operator=(const String& rhs) {
	changeToStringType();
	string = rhs;
	return *this;
}

LightString& LightString::operator=(String&& rvalue) {
	if (_type == FLASH) {
		flashString.~FlashStringStorage();

		_type = STRING;
		new (&string) String(std::move(rvalue));
	} else {
		string = std::move(rvalue);
	}
	return *this;
}

LightString& LightString::operator=(const LightString& rhs) {
	if (_type == FLASH) {
		if (rhs._type == FLASH) {
			flashString = rhs.flashString;
		} else {
			flashString.~FlashStringStorage();
			_type = STRING;
			new (&string) String(rhs.string);
		}
	} else {
		if (rhs._type == FLASH) {
			string.~String();
			_type = FLASH;
			new (&flashString) FlashStringStorage(rhs.flashString);
		} else
			string = rhs.string;
	}
	return *this;
}

LightString& LightString::operator=(LightString&& rhs) {
	if (_type == FLASH) {
		if (rhs._type == FLASH) {
			flashString = std::move(rhs.flashString);
		} else {
			flashString.~FlashStringStorage();
			_type = STRING;
			new (&string) String(std::move(rhs.string));
		}
	} else {
		if (rhs._type == FLASH) {
			string.~String();
			_type = FLASH;
			new (&flashString) FlashStringStorage(std::move(rhs.flashString));
		} else
			string = std::move(rhs.string);
	}
	return *this;
}

bool LightString::isEmpty() const {
	if (_type == STRING)
		return string.length() == 0;
	if (!flashString)
		return true;
	// we check first byte to see if it is a C string end terminator
	return 0 == pgm_read_byte_inlined(flashString);
}


unsigned int LightString::length(void) const {
	if (_type == STRING)
		return string.length();
	if (!flashString)
		return 0;
	return strlen_P(flashString);
}

int LightString::atoi() const {
	if (_type == FLASH) {
		if (!flashString)
			return -1; // HMM

		char s[30];
		strncpy_P(s, flashString, sizeof(s) / sizeof(s[0] - 1));
		s[29] = '\0';
		return ::atoi(&s[0]);
	}
	return ::atoi(string.c_str());
}

long LightString::toInt(void) const {
	if (_type == FLASH) {
		if (!flashString)
			return -1;

		char s[30];
		strncpy_P(s, flashString, sizeof(s) / sizeof(s[0] - 1));
		s[29] = '\0';
		return atol(s);
	}

	return string.toInt();
}

unsigned char LightString::equalsIgnoreCase(const String &s) const {
	if (_type == STRING)
		return string.equalsIgnoreCase(s);
	return strcasecmp_P(s.c_str(), flashString) == 0;
}


unsigned char LightString::equals(const char *cstr) const {
	if (_type == STRING)
		return string.equals(cstr);
	return strcmp_P(cstr, flashString) == 0;
}


void LightString::appendTo(String& dest) const {
	if (_type == STRING) {
		dest.concat(string);
	} else {
		dest.concat(FPSTR(flashString));
	}
}


unsigned char LightString::startsWith(const String &prefix) const {
	if (_type == STRING)
		return string.startsWith(prefix);

	return strncmp_P(prefix.c_str(), flashString, prefix.length()) == 0;
}


unsigned char LightString::startsWith(const String &prefix, unsigned int offset) const {
	if (_type == STRING)
		return string.startsWith(prefix, offset);
	return asString().startsWith(prefix, offset);
}

String LightString::substring(unsigned int beginIndex) const {
	if (_type == STRING)
		return string.substring(beginIndex);
	return asString().substring(beginIndex);
}

String LightString::substring(unsigned int beginIndex, unsigned int endIndex) const {
	if (_type == STRING)
		return string.substring(beginIndex, endIndex);
	return asString().substring(beginIndex, endIndex);
}

const char* LightString::c_str() {
	changeToStringType(true);
	return string.c_str();
}
