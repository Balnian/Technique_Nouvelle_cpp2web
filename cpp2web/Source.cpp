#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "Utility.h"
#include "cpp2web.h"

using namespace std;



int main(int argc, char * argv[])
{
	vector<string> args(argv+1, argv+ argc); // = Utility::extractWords(sarg);

	
	cpp2web inst(args);

	/*vector<string> fichiers;
	fichiers.emplace_back("Source.cpp" );
	cpp2web inst;

	inst["-stats"](fichiers);
	inst["-couleur"](fichiers);*/

	//inst.execute("-stats", fichiers);

	/*cout << *
	find_if(begin(args), end(args), [](string s) {
		return Utility::str_begin_not_with(s,"-") && Utility::str_begin_not_with(s,"/") && s.size() > 1 && !Utility::str_end_with(s,".h")  && !Utility::str_end_with(s,".cpp");
	});*/
	//for_each(begin(args), end(args), [](string s) {
	//	cout << s << endl;
	//});
}