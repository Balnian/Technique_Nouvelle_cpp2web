#pragma once

#include <string>
#include <vector>
#include <regex>

using namespace std;


class Utility
{
	virtual void NonInstanciable()const noexcept = 0;
public:
	Utility() = delete;
	~Utility();

	

	static vector<string> extractWords(string str);

	static bool str_end_with(string str, string ending) { return (str.size()>ending.size()) ? str.substr(str.size() - ending.size()) == ending : false; };
	static bool str_begin_with(string str, string begin) { return (str.size() > begin.size()) ? str.substr(0, begin.size()) == begin : false;  }

	static bool str_end_not_with(string str, string ending) { return !str_end_with(str,ending); };
	static bool str_begin_not_with(string str, string begin) { return !str_begin_with(str,begin); }

   static vector<string> regex_Split(string& str, regex expr);
	
};

