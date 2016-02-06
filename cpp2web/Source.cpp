#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "Utility.h"
#include "cpp2web.h"

using namespace std;



int main(int argc, char * argv[])
{
	vector<string> args(argv+1, argv+ argc); 
	
	cpp2web inst(args);

}