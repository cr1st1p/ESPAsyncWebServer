#include "ESPAsyncWebServer.h"



void LightString::changeToStringType() {
	if (type == STRING)
		return;
	//flashString.~FlashStringStorage();

	type = STRING;
	new (&string) String();
}

LightString::LightString(FlashString s) :
	type(FLASH) {
	flashString = (FlashStringStorage)s;

}

LightString::LightString(const String& s) :
	type(STRING) {
	new (&string) String(s);
}

LightString::LightString(String&& rval) :
	type(STRING) {
	new (&string) String(std::move(rval));
}


LightString::LightString(const LightString& s) :
		type(s.type)
{
	if (type == FLASH)
		flashString = std::move(s.flashString);
	else
		new (&string) String(std::move(s.string));
}

LightString::LightString(LightString&& s) :
		type(std::move(s.type))
{
	if (type == FLASH)
		flashString = s.flashString;
	else
		new (&string) String(std::move(s.string));
}


LightString::LightString(const char* s)
	: type(STRING) {
	new (&string) String(s);
}



LightString::~LightString() {
	if (type == STRING) {
		   string.~String();
	} else {
		   //flashString.~FlashStringStorage();
	}
}

String LightString::asString() const {
	if (type == FLASH)
		return String(FPSTR(flashString));
	return string;
}


LightString& LightString::operator=(const String& rhs) {
	changeToStringType();
	string = rhs;
	return *this;
}

LightString& LightString::operator=(String&& rvalue) {
	if (type == FLASH) {
		type = STRING;
		new (&string) String(std::move(rvalue));
	} else {
		string = std::move(rvalue);
	}
	return *this;
}


int LightString::atoi() const {
	if (type == FLASH) {
		char s[30];
		strncpy_P(s, flashString, sizeof(s) / sizeof(s[0] - 1));
		s[29] = '\0';
		return ::atoi(&s[0]);
	}
	return ::atoi(string.c_str());
}

long LightString::toInt(void) const {
	if (type == FLASH) {
		char s[30];
		strncpy_P(s, flashString, sizeof(s) / sizeof(s[0] - 1));
		s[29] = '\0';
		return atol(s);
	}

	return string.toInt();
}

unsigned char LightString::equalsIgnoreCase(const String &s) const {
	if (type == STRING)
		return string.equalsIgnoreCase(s);
	return strcasecmp_P(s.c_str(), flashString) == 0;
}


unsigned char LightString::equals(const char *cstr) const {
	if (type == STRING)
		return string.equals(cstr);
	return strcmp(cstr, flashString) == 0;
}


void LightString::appendTo(String& dest) const {
	if (type == STRING) {
		dest.concat(string);
	} else {
		dest.concat(FPSTR(flashString));
	}
}


unsigned char LightString::startsWith(const String &prefix) const {
	if (type == STRING)
		return string.startsWith(prefix);
	return asString().startsWith(prefix);
}


unsigned char LightString::startsWith(const String &prefix, unsigned int offset) const {
	if (type == STRING)
		return string.startsWith(prefix, offset);
	return asString().startsWith(prefix, offset);
}

String LightString::substring(unsigned int beginIndex) const {
	if (type == STRING)
		return string.substring(beginIndex);
	return asString().substring(beginIndex);
}

String LightString::substring(unsigned int beginIndex, unsigned int endIndex) const {
	if (type == STRING)
		return string.substring(beginIndex, endIndex);
	return asString().substring(beginIndex, endIndex);
}


