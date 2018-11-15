#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int v1[2010], v2[2010];

int main() {
	freopen("correct.txt", "r", stdin);
	int n1;
	cin >> n1;
	for (int i=0; i<n1; i++)
		cin >> v1[i];
	freopen("output.txt", "r", stdin);
	int n2;
	cin >> n2;
	for (int i=0; i<n2; i++)
		cin >> v2[i];
	if (n1 != n2 || !std::equal(v1, v1+n1, v2))
		cout << "WRONG!!!!!!!\n"<< ' '<< n1 << ' '<< n2<< ' '<< std::equal(v1, v1+n1, v2);
	else
		cout << "CORRECT! GOOD JOB!\n";
}