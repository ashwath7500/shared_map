#include <bits/stdc++.h>
#include "shared_map.hpp"
using namespace std;

int main () {
	shared_map<string ,int> m1;
	m1["helo"] = 2;
	m1["world"] = 3;
	auto m2 = m1;
	cout<<m2["helo"]<<"\n";
	m1["helo"] = 4;
	cout<<m2["helo"]<<"\n";
	cout<<m1["helo"]<<" "<<m2["helo"]<<"\n";
	return 0;
}