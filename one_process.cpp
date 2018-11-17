#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

using namespace std;

vector<vector<int> > a;
vector<int> v;

int main() {
	int n, m;
	int C = 10;
	freopen("input.txt", "w", stdout);
	cin >> n >> m;
	a.resize(n+1);
	cout << n << ' '<< m << endl;
	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++) {
			a[i].push_back(rand() % C);
			cout << a[i][j] << ' ';
		}
		cout << endl;
	}
	v.resize(m + 1);
	for (int i=0; i<m; i++) {
		v[i] = rand() % C;
		cout << v[i] << ' ';
	}
	cout << endl;
	freopen("correct.txt", "w", stdout);
	cout << n << endl;
	for (int i=0; i<n; i++) {
		int s = 0;
		for (int j=0; j<m; j++)
			s += a[i][j] * v[j];
		cout << s << ' ';
	}
}