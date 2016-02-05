#include "Utility.h"
#include <algorithm>
#include <locale>





Utility::~Utility()
{
}

vector<string> Utility::extractWords(string s)
{
	vector<string> str;

	auto debut = begin(s), fin = end(s);
	//S�pare les string et les espaces
	while (debut != fin)
	{
		auto t = find_if(debut, fin, [](char c) {return !isspace(c, locale{ "" }); });
		//if(debut!=t)

		auto t2 = find_if(t, fin, [](char c) {return isspace(c, locale{ "" }); });

		str.emplace_back(string(t, t2));
		debut = t2;
	}

	return str;
}

vector<string> Utility::regex_Split(string& str, regex expr)
{
   vector<string> out;
   
   auto sbegin = sregex_iterator(begin(str), end(str), expr);
   auto send = sregex_iterator();

   for (auto i = sbegin; i != send; ++i)
   {
      out.emplace_back(i->str());
   }
   return out;
}
