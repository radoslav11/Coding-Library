#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);
const int mod = (int)1e9 + 7;
const int64_t inf = (int64_t)1e16 + 42;

template<class T>
struct matrix
{
	int n;
	vector<vector<T> > t;

	matrix(int _n, T val) {n = _n; t.assign(n, vector<T>(n, val)); }
	matrix(int _n) {n = _n; t.assign(n, vector<T>(n, 0)); }
    matrix() { n = 0; t.clear(); }

	matrix operator * (matrix b)
	{
		matrix c = matrix(n, 0);
		for(int i = 0; i < n; i++)
			for(int k = 0; k < n; k++)
				for(int j = 0; j < n; j++)
					c.t[i][j] = (c.t[i][j] + t[i][k] * b.t[k][j]) % mod;
		return c;
	}

	vector<T> operator * (vector<T> b)
	{
		vector<T> c = vector<T>(n, 0);
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
                c[i] = (c[i] + t[i][j] * b[j]) % mod;

		return c;
	}

	matrix<T> operator ^ (matrix<T> b)
	{
		matrix<T> c = matrix(n, inf);
		for(int i = 0; i < n; i++)
			for(int k = 0; k < n; k++)
				for(int j = 0; j < n; j++)
					c.t[i][j] = min(c.t[i][j], t[i][k] + b.t[k][j]);
		return c;
	}

	matrix<T> operator + (matrix<T> b)
	{
		matrix<T> c = matrix<T>(n);
		for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                c.t[i][j] =  (t[i][j] + b.t[i][j]) % mod;

		return c;
	}

	matrix<T> operator - (matrix<T> b)
	{
		matrix<T> c = matrix<T>(n);
		for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                c.t[i][j] = (t[i][j] - b.t[i][j] + mod) % mod;

		return c;
	}

	matrix operator & (matrix b)
	{
		matrix<T> c = matrix<T>(n, -inf);
		for(int i = 0; i < n; i++)
			for(int k = 0; k < n; k++)
				for(int j = 0; j < n; j++)
					c.t[i][j] = max(c.t[i][j], t[i][k] + b.t[k][j]);

		return c;
	}
};

template<class T>
matrix<T> pow_min(matrix<T> base, int64_t p)
{
	if(p == 1) return base;

	if(p % 2ll == 0ll)
	{
		matrix<T> d = pow_min(base, p / 2ll);
		return d ^ d;
	}

	return base ^ pow_min(base, p - 1);
}

template<class T>
matrix<T> pow_max(matrix<T> base, int64_t p)
{
	if(p == 1) return base;

	if(p % 2ll == 0ll)
	{
		matrix<T> d = pow_max(base, p / 2ll);
		return d & d;
	}

	return base & pow_max(base, p - 1);
}

template<class T>
matrix<T> pow(matrix<T> base, int64_t p)
{
	if(p == 1) return base;

	if(p % 2ll == 0ll)
	{
		matrix<T> d = pow(base, p / 2ll);
		return d * d;
	}

	return base * pow(base, p - 1);
}

template<class T>
void print(matrix<T> mat)
{
	for(int i = 0; i < mat.n; i++)
		for(int j = 0; j < mat.n; j++)
			if(mat.t[i][j] < inf) cout << mat.t[i][j] << " \n"[j == mat.n - 1];
			else cout << "inf" << " \n"[j == mat.n - 1];

	cout << endl;
}

void read()
{

}

void solve()
{

}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

