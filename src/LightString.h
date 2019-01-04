#ifndef _LightString_H_
#define _LightString_H_


class LightString {
public:
	typedef const __FlashStringHelper* FlashString ;
	typedef PGM_P FlashStringStorage;

private:
	union {
		FlashStringStorage flashString;
		String string;
	};
	enum Type {FLASH, STRING};
	uint8_t _type; // enum type is taking more memory


public:

	LightString(FlashString s = nullptr);
	LightString(const String& s);
	LightString(String&& rval);

	LightString(const LightString& s);
	LightString(LightString&& rval);

	LightString(const char* s);

	~LightString();

	void clear();

	inline Type type() const { return (Type) _type; }

	void changeToStringType(bool keepData = false);
	void changeToFlashStringType();
	String asString() const;

	LightString& operator=(const char* s);
	LightString& operator=(FlashString s);
	LightString& operator=(const String& rhs);
	LightString& operator=(const LightString& rhs);
	LightString& operator=(LightString&& rhs);
	LightString& operator=(String&& rvalue);


	unsigned int length(void) const;
	bool isEmpty() const; // use this instead of length if you do not really care about the length

	bool operator!() const; // @returns true if string is not set

	int atoi() const;
	long toInt(void) const;
	unsigned char equalsIgnoreCase(const String &s) const;
    unsigned char equals(const char *cstr) const;
    unsigned char equals(const String& s) const {
    	return equals(s.c_str());
    }
    unsigned char operator ==(const String &rhs) const {
    	return equals(rhs.c_str());
    }

    void appendTo(String& dest) const;

    unsigned char startsWith(const String &prefix) const;
    unsigned char startsWith(const String &prefix, unsigned int offset) const;

    String substring(unsigned int beginIndex) const;
    String substring(unsigned int beginIndex, unsigned int endIndex) const;

    const char* c_str();
};



#endif //_LightString_H_
