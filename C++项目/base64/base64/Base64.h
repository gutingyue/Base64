#pragma once
#include <string>

using namespace std;

class Base64{
public:
	//±àÂë
	string Encode(const string& strData);
	//½âÂë
	string Decode(const string& strData);

};