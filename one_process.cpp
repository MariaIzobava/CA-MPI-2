#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int a[2010][2010], v[2010];

int main() {
	int n, m;
	int C = 10;
	freopen("input.txt", "w", stdout);
	cin >> n >> m;
	cout << n << ' '<< m << endl;
	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++) {
			a[i][j] = rand() % C;
			cout << a[i][j] << ' ';
		}
		cout << endl;
	}
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