#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <fstream>


using namespace std;

class cpp2web
{

	map < string, function<void(vector<string>)>> switchAction;
	vector<string> keywords;

	string htmlHeader(string title);
	string htmlFooter();

public:
	cpp2web();
	cpp2web(vector<string> args);
	~cpp2web();

	function<void(vector<string>)> operator[] (string swtch)
	{
		switchAction.try_emplace(swtch, [](vector<string>) {});
		return switchAction[swtch];
	}

	void execute(string swtch, vector<string> files)
	{
		switchAction.try_emplace(swtch, [](vector<string>) {});
		switchAction[swtch](files);
	}



	void stats(vector<string> files);

	void couleur(vector<string> files);

	void htmlSanitize(string & s);

};

