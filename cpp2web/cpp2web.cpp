#include "cpp2web.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <regex>
#include <map>
#include <thread>
#include <chrono>
#include "Utility.h"
#include "ThreadAPI.h"

using namespace std::chrono;
/**/

cpp2web::cpp2web()
{
	/*1.1 + 2.2;return
	sdsd*/
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
		files.erase(remove_if(files.begin(), files.end(), [](string s) { return  s.empty(); }), files.end());
		actions.erase(remove_if(actions.begin(), actions.end(), [](string s) { return  s.empty(); }), actions.end());

		//if there is no switch(s) or no file(s) print an error
		if (files.size() <= 0 || actions.size() <= 0)
		{
			cout << "Aucun fichier ou option passer en parametre" << endl;
		}
		else
		{
			//Chargement des Keywords
			ifstream fKwords{ "Keywords.txt" };
			for (string kword; fKwords >> kword;)
			{
				keywords.emplace_back(kword);
			}

			const size_t MULTIPLICATEUR = 1;

			const char csvSepa = ';';

			ofstream tb{ "ThreadBenchmark.csv" };
			for (size_t i = 0; i < 10; i++)
			{


				cout << "No Thread" << endl;
				// No Thread
				{
					ThreadAPI<NO_THREAD> tAPI;

					auto start = high_resolution_clock::now();

					//for (size_t i = 0; i < MULTIPLICATEUR; i++)
					// Execution des commandes
					for_each(begin(actions), end(actions), [&](string swtch)
					{
						tAPI.execute((*this)[swtch], files);
					});
					auto end = high_resolution_clock::now();
					tb << "NO Thread" << csvSepa << duration_cast<microseconds>(end - start).count() << endl;
				}

				cout << "Thread on-the-spot" << endl;
				// Thread on-the-spot
				{
					ThreadAPI<ADHOC_THREAD> tAPI;
					auto start = high_resolution_clock::now();

					vector<thread> TPool;
					//for (size_t i = 0; i < MULTIPLICATEUR; i++)
					// Execution des commandes
					for_each(begin(actions), end(actions), [&](string swtch)
					{
						TPool.emplace_back(tAPI.execute((*this)[swtch], files));
					});

					for_each(begin(TPool), end(TPool), [](thread& t)
					{
						if (t.joinable())
							t.join();
					});

					auto end = high_resolution_clock::now();
					tb << "Thread on-the-spot" << csvSepa << duration_cast<microseconds>(end - start).count() << endl;
				}

				cout << "Thread Pool" << endl;
				// Thread Pool
				{
					for (size_t i = 1; i < 40; i++)
					{

						system("del gen\*");
						ThreadAPI<THREAD_POOL> tAPI(i);
						auto start = high_resolution_clock::now();
						//for (size_t i = 0; i < MULTIPLICATEUR; i++)
						tAPI.execute([=]()
						{
							cout << i << flush;
							stats(files);
							couleur(files);
							//switchAction[swtch](files);
						});

						// Execution des commandes
						//for_each(begin(actions), end(actions), [&/*,&tAPI*/](string swtch)
						//{
						//	cout << swtch << flush;
						//	bind(switchAction[swtch], this, files);
						//	tAPI.execute(bind(switchAction[swtch], this, files) /*[&,swtch]()
						//	{
						//		cout << swtch << flush;
						//		switchAction[swtch](files);
						//	}*/);
						//});
						tAPI.wait();
						auto end = high_resolution_clock::now();
						tb << "Thread Pool(" << i << ")" << csvSepa << duration_cast<microseconds>(end - start).count() << endl;

						//this_thread::sleep_for(seconds(1));
					}
				}
			}
		}
	}
}

cpp2web::~cpp2web()
{
}

void cpp2web::htmlSanitize(string & s)
{
	s = regex_replace(s, regex("&"), "&amp");
	s = regex_replace(s, regex("<"), "&lt");
	s = regex_replace(s, regex(">"), "&gt");
}

string cpp2web::htmlHeader(string title)
{

	return R"(
   <!DOCTYPE html> 
   <html>
   <head>
   <title>)" + title + R"(</title>
   <link rel="stylesheet" type="text/css" href="cpp2web.css">
   </head>

													   <body>
      <pre>
)";
}

string cpp2web::htmlFooter()
{
	return R"(
         </pre>
      </body>
   </html>
      )";
}


void cpp2web::stats(vector<string> files)
{
	map<string, int> stat;
	for_each(begin(files), end(files), [&stat](string s)-> void
	{

		stringstream src;
		src << ifstream{ s, ios::binary }.rdbuf();
		string file = src.str();
		regex split(R"(\d+\.\d+|[\w_]+)");

		auto data = Utility::regex_Split(file, split);

		for (auto& w : data)
		{
			stat[w]++;
		}
	});
	// Mettre en ordre
	vector<pair<string, int>> vecStat;
	copy(begin(stat), end(stat), back_inserter(vecStat));
	sort(begin(vecStat), end(vecStat), [](pair<string, int> a, pair<string, int> b)
	{
		if (a.second == b.second)
		{
			return lexicographical_compare(begin(a.first), end(a.first), begin(b.first), end(b.first));
		}
		else
		{
			return b.second < a.second;
		}
	});

	//Afficher
	ofstream ofs{ "gen\\stats.txt" };
	for (const auto& elem : vecStat)
	{
		ofs << elem.first << " : " << elem.second << endl;
	}
}

void cpp2web::couleur(vector<string> files)
{
	for_each(begin(files), end(files), [&](string s)-> void
	{
		stringstream src;
		src << ifstream{ s, ios::binary }.rdbuf();
		string file = src.str();

		// création du regex
		string reg = accumulate(begin(keywords), end(keywords), string{}, [](const std::string& a, string b)
		{
			return a.empty() ? "\\b" + b + "\\b"
				: a + '|' + "\\b" + b + "\\b";
		});

		regex keyW(reg);

		htmlSanitize(file);

		file = regex_replace(file, keyW, "<span class=\"Kword\">$&</span>");
		// Code pour les string interfere avec le remplacement de keyword,
		// mais si passer en premier keywords interfere avec le class dans le span
		//file = regex_replace(file, regex(R"("[^"]+")"), "<span class=\"str\">$&</span>");
		file = regex_replace(file, regex(R"(\/\*[^(\*\)]*\*\/)"), "<span class=\"comment\">$&</span>"); //old: \/\*.*\*\/
		file = regex_replace(file, regex(R"(//.*)"), "<span class=\"comment\">$&</span>");


		ofstream ofs{ "gen\\"+ s + ".html" };
		string h = htmlHeader(s);
		string f = htmlFooter();
		ofs << h << file << f;

	});
}

