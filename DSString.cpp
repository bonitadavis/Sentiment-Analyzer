#include "DSString.h"

/* 
 * Implement the functions defined in DSString.h. You may add more functions as needed
 * for the project. 
 *
 * Note that c-strings don's store an explicit length but use `\0` as the terminator symbol
 * but your class should store its length in a member variable. 
 * DDO NOT USE C-STRING FUNCTIONS <string.h> or <cstring>.
 */  

DSString::DSString() {
    len = 0;
    data = new char[1];
    data[0] = '\0';
}

DSString::DSString(const char *cstring) {  // constructor that converts a cstring
    len = 0;
    while (cstring[len] != '\0')
        len++;
    
    data = new char[len+1];
    for (size_t i = 0; i < len; i++)
        data[i] = cstring[i];
    data[len] = '\0';
}
// you can also provide DSString(const std::string &); for std::string

// Rule of three is needed if dynamic memory allocation is used
DSString::DSString(const DSString &word) { // copy constructor
    len = word.length();
    data = new char[len+1];
    for (size_t i = 0; i < len; i++)
        data[i] = word.data[i];
    data[len] = '\0';
}

DSString::~DSString(){ // destructor
    delete[] data;
}

DSString& DSString::operator=(const DSString &cstring) { // assignment operator
    // check for self-assignment first
    if (this == &cstring)
        return *this;

    // deep copy of word
    delete[] data;
    len = cstring.len;
    data = new char[len+1];
    for (size_t i = 0; i < len; i++)
        data[i] = cstring.data[i];
    data[len] = '\0';
    return *this;
}

// you can also implement the move versions for the big 5 (C+11)

size_t DSString::length() const { // returns the length of the string
    return len;
}

char& DSString::operator[](size_t i) { // returns a reference to the character at the given index
    return data[i];
}

/**
 * Overloaded operator+ which appends the string in the argument to this string
 */
DSString DSString::operator+(const DSString &cstring) const {
    int newlen = this->length() + cstring.length() + 1;
    char* newdata = new char[newlen];

    for (size_t i = 0; i < this->length(); i++)
        newdata[i] = this->data[i];
    for (size_t i = 0; i < cstring.length(); i++)
        newdata[i + this->length()] = cstring.data[i];

    newdata[newlen-1] = '\0';
    DSString ret(newdata);
    delete[] newdata;
    return ret;
}

/**
 * Standard relational operators to compare and order your strings.
 * Feel free to add additional.
 **/
bool DSString::operator==(const DSString &cstring) const {
    if (cstring.data==nullptr && data==nullptr)
        return true;
    else if (cstring.len != len)
        return false;
    else {
        for (size_t i = 0; i < len; i++) {
            if (cstring.data[i] != data[i]) {
                return false;
            }
        }
        return true;
    }
}

bool DSString::operator<(const DSString &cstring) const {
    if (data == cstring.data)
        return false;

    for (size_t i = 0; i < len; i++) {
        if (data[i] < cstring.data[i])
            return true;
        else if (data[i] > cstring.data[i])
            return false;
    }
    return false;
}

/**
 * The substring method returns a new string object that contains a
 * sequence of characters from this string object.
 *
 * param start - the index of where to start
 * param numChars - the number (count) of characters to copy into
 *    the substring
 * @return a DSString object containing the requested substring
 **/
DSString DSString::substring(size_t start, size_t numChars) const {
    DSString substring;
    substring.len = numChars;
    delete[] substring.data;
    substring.data = new char[substring.len+1];
    substring.data[numChars] = '\0';

    for (size_t i = start; i < start+numChars; i++) {
        substring.data[i-start] = data[i];
    }

    return substring;
    delete[] substring.data;
}

/**
 * @brief Returns a new string object with all characters in lowercase
 *
 * @return DSString
 */
DSString DSString::toLower() const { // look at the ASCII table for this!
    DSString newString;
    size_t length = len;
    newString.len = length;
    delete[] newString.data;
    newString.data = new char[length+1];
    newString.data[len] = '\0';

    for (size_t i = 0; i < length; i++) {
        if (data[i] >= 'A' && data[i] <= 'Z')
            newString.data[i] = data[i] + 32;
        else
            newString.data[i] = data[i];
    }

    return newString;
    delete[] newString.data;
}

/**
 * the c_str function returns a pointer a null-terminated c-string holding the
 * contents of this object. Since data already has a `\0`
 * at the end of the string in DSString so you can just return a pointer to data.
 **/
char* DSString::c_str() const {
    return data;
}

// a conversion to std::string would also be nice: string string() const;
string DSString::toString() const {
    ostringstream stringStream;
    stringStream << *this;
    return stringStream.str();
}

/**
 * Overloaded stream insertion operator to print the contents of this
 * string to the output stream in the first argument. Remember:
 * This operator needs to be implemented outside of the class as
 * a friend because it operates on the stream and not the DSString object.
 **/
std::ostream &operator<<(std::ostream &output, const DSString &cstring) {
    output << cstring.c_str();
    return output;
}

DSString DSString::removeSymbols() { // removes non-letters
    char* newData = new char[this->len+1];
    size_t j = 0;
    size_t count = 0;

    // iterates through each character in original DSString data and copies non-characters to newData
    for (size_t i = 0; i < this->len; i++) {
        if (((data[i] >= 'a' && data[i] <= 'z') || (data[i] >= 'A' && data[i] <= 'Z')) || (data[i] == ' ')) {
            newData[j] = data[i];
            j++;
        }
        else
            count++;
    }

    newData[len-count] = '\0'; // add null-terminator
    DSString newString(newData);
    delete[] newData;
    return newString;
}

// returns index of char or -1 if char not found
size_t DSString::findCharIndex (char character) {
    for (size_t i = 0; i < len; i++) {
        if (data[i] == character)
            return i;
    }
    return -1;
}

DSString DSString::stem() {
    string str = this->toString();
    std::wstring wStr(str.begin(), str.end());

    stemming::english_stem<> StemEnglish;
    StemEnglish(wStr);

    string str2 = string(wStr.begin(), wStr.end());

    DSString ret(str2.c_str());
    return ret;
}