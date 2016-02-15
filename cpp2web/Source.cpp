#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

#include "Utility.h"
#include "cpp2web.h"

using namespace std;
using namespace std::chrono;



int main(int argc, char * argv[])
{
	auto start = high_resolution_clock::now();
	vector<string> args(argv+1, argv+ argc); 
	
	cpp2web inst(args);

	auto end = high_resolution_clock::now();
	milliseconds temp= duration_cast<milliseconds>( end - start);
	cout << temp.count() << endl;

}