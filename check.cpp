#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

vector<int> v1, v2;

int main() {
	freopen("correct.txt", "r", stdin);
	int n1;
	cin >> n1;
	v1.resize(n1+1);
	for (int i=0; i<n1; i++)
		cin >> v1[i];
	freopen("output.txt", "r", stdin);
	int n2;
	cin >> n2;
	v2.resize(n2+1);
	for (int i=0; i<n2; i++)
		cin >> v2[i];
	if (n1 != n2 || !std::equal(v1.begin(), v1.end(), v2.begin()))
		cout << "WRONG!!!!!!!\n"<< ' '<< n1 << ' '<< n2<< ' '<< std::equal(v1.begin(), v1.end(), v2.begin()) << endl;
	else
		cout << "CORRECT! GOOD JOB!\n";
}