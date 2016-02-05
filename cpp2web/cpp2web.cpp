#include "cpp2web.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <regex>
#include <map>
#include "Utility.h"

/**/

cpp2web::cpp2web()
{
   /*1.1 + 2.2;return*/
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
         //Chargement des Keywords
         ifstream fKwords{ "Keywords.txt" };
         for (string kword; fKwords >> kword;)
         {
            keywords.emplace_back(kword);
         }

         // Execution des commandes
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
   <title>)"+ title + R"(</title>
   <link rel="stylesheet" type="text/css" href="cpp2web.css">
   </head>

   <body>
      <pre>
)" ;
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

   ofstream ofs{ "stats.txt" };
   for (const auto& elem : stat)
   {
      ofs << elem.first << " : " << elem.second << endl;
   }

}

void cpp2web::couleur(vector<string> files)
{
   cout << "couleur" << endl;
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
      file = regex_replace(file, regex(R"("[^"]+")"), "<span class=\"str\">$&</span>");
      file = regex_replace(file, regex(R"(/\*.*\*/)"), "<span class=\"comment\">$&</span>");
      file = regex_replace(file, regex(R"(//.*)"), "<span class=\"comment\">$&</span>");
      

      ofstream ofs{ s + ".html" };
      string h = htmlHeader(s);
      string f = htmlFooter();
      ofs << h << file << f;

   });

   //Replace Keywords
   //wrap comments
   //Sanetize characters
}

