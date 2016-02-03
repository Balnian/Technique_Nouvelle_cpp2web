#include "cpp2web.h"
#include <sstream>
#include <iostream>
#include "Utility.h"
#include <regex>



cpp2web::cpp2web()
{
	switchAction["-stats"] = [&](vector<string> files) -> void { this->stats(files); };

	switchAction["-couleur"] = [&](vector<string> files) -> void { this->couleur(files); };
}

cpp2web::cpp2web(vector<string> args) :cpp2web()
{
	//check if request was well formed
	if (end(args) != find_if(begin(args), end(args), [](string s) {
		return Utility::str_begin_not_with(s, "-")
			&& Utility::str_begin_not_with(s, "/")
			&& !s.size() > 1
			&& Utility::str_end_not_with(s, ".h")
			&& Utility::str_end_not_with(s, ".cpp");
	}))
	{
		cout << "Paramêtre invalide!!!" << endl;
	}
	else
	{
		vector<string> files{ args.size() }, actions{ args.size() };
		//split switch from files
		auto point = partition_copy(begin(args), end(args), begin(files), begin(actions), [](string s) {return Utility::str_begin_not_with(s, "-") && Utility::str_begin_not_with(s, "/"); });

		//if there is no switch(s) or no file(s) print an error
		if (files.size() <= 0 || actions.size() <= 0)
		{
			cout << "Aucun fichier ou option passer en parametre" << endl;
		}
		else
		{
			for_each(begin(actions), end(actions), [&](string swtch) 
			{
				(*this)[swtch](files);
			});
		}
	}
	
}


cpp2web::~cpp2web()
{
}

void cpp2web::stats(vector<string> files)
{
	for_each(begin(files), end(files), [](string s)-> void 
	{
		stringstream src;
		src << ifstream{ s, ios::binary }.rdbuf();
		string file = src.str();
		regex keywords("");
		//Replace Keywords
		//wrap comments
		//Sanetize characters

		//regex_replace(file, keywords,)
		
	});
}

void cpp2web::couleur(vector<string> files)
{
	cout << "couleur" << endl;
}