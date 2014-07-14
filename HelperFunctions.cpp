#include "HelperFunctions.h"
#include <regex>

/** Function to remove the leading and trailing spaces from a string */
string trim(string the_string)
{
	size_t p = the_string.find_first_not_of(" ");
	if (p == string::npos) return "";
	size_t q = the_string.find_last_not_of(" ");
	return the_string.substr(p, q - p + 1);
}

bool isDate(string s){
	return regex_match(s, regex("^(\\d{1,2})/(\\d{1,2})/(\\d{4})$"));
}