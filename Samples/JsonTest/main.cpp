#include <iostream>
#include <twinkle/twinkle.h>
#include <twinkle/json/json.h>

using namespace std;
using namespace twinkle;

int main()
{
	json::data json = 4;
	json = "ciao";


	cout << "Premi un tasto per continuare...";
	getchar();

	return 0;
}