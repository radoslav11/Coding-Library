#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
template<class T, class T2> inline void chkmax(T &x, const T2 &y) { if(x < y) x = y; }
template<class T, class T2> inline void chkmin(T &x, const T2 &y) { if(x > y) x = y; }
const double PI = acos(-1);
const int MAXN = (1 << 19);
int mod;

inline void addmod(int& x, int y, int mod) { (x += y) >= mod && (x -= mod); }
inline int mulmod(int x, int y, int mod) { return x * 1ll * y % mod; }

struct complex_base
{
	double x, y;
	complex_base(double _x = 0, double _y = 0) { x = _x; y = _y; }
	friend complex_base operator-(const complex_base &a, const complex_base &b) { return complex_base(a.x - b.x, a.y - b.y); }
	friend complex_base operator+(const complex_base &a, const complex_base &b) { return complex_base(a.x + b.x, a.y + b.y); }
	friend complex_base operator*(const complex_base &a, const complex_base &b) { return complex_base(a.x * b.x - a.y * b.y, a.y * b.x + b.y * a.x); }
	friend void operator/=(complex_base &a, const double &P) { a.x /= P; a.y /= P; }
};

int bit_rev[MAXN];
int last_n_fft = -1, ilast_n_fft = -1;
complex_base root[MAXN], iroot[MAXN];

void fft(complex_base *a, int lg)
{
	int n = (1 << lg);
	if(last_n_fft != n)
	{
		double ang = 2 * PI / n;
		for(int i = 0; i < (n >> 1); i++)
			root[i] = complex_base(cos(ang * i), sin(ang * i));

		last_n_fft = n;
	}

	for(int i = 1; i < n; i++)
	{
		bit_rev[i] = (bit_rev[i >> 1] >> 1) | ((i & 1) << (lg - 1));
		if(bit_rev[i] < i) swap(a[i], a[bit_rev[i]]);
	}

	for(int len = 2; len <= n; len <<= 1)
	{
		int step = (n / len);
		for(int j = 0; j < (len >> 1); j++)
			for(int i = 0; i < n; i += len)
			{
				complex_base u = a[i + j], v = root[step * j] * a[i + j + (len >> 1)];
				a[i + j] = u + v;
				a[i + j + (len >> 1)] = u - v;
			}
	}
}

void inv_fft(complex_base *a, int lg)
{
	int n = (1 << lg);
	if(ilast_n_fft != n)
	{
		double ang = -2 * PI / n;
		for(int i = 0; i < (n >> 1); i++)
			iroot[i] = complex_base(cos(ang * i), sin(ang * i));

		ilast_n_fft = n;
	}

	for(int i = 1; i < n; i++)
	{
		bit_rev[i] = (bit_rev[i >> 1] >> 1) | ((i & 1) << (lg - 1));
		if(bit_rev[i] < i) swap(a[i], a[bit_rev[i]]);
	}

	for(int len = 2; len <= n; len <<= 1)
	{
		int step = (n / len);
		for(int j = 0; j < (len >> 1); j++)
			for(int i = 0; i < n; i += len)
			{
				complex_base u = a[i + j], v = iroot[step * j] * a[i + j + (len >> 1)];
				a[i + j] = u + v;
				a[i + j + (len >> 1)] = u - v;
			}
	}

	for(int i = 0; i < n; i++)
		a[i] /= n;
}

complex_base A[MAXN], B[MAXN];

vector<int> mult(const vector<int> &a, const vector<int> &b)
{
	if(a.size() * b.size() <= 128)
	{
		vector<int> ans(a.size() + b.size(), 0);
		for(int i = 0; i < (int)a.size(); i++)
			for(int j = 0; j < (int)b.size(); j++)
				ans[i + j] = (ans[i + j] + a[i] * 1ll * b[j]) % mod;

		return ans;
	}

	int lg = 0; while((1 << lg) < (int)(a.size() + b.size())) ++lg;
	for(int i = 0; i < (1 << lg); i++) A[i] = B[i] = complex_base(0, 0);
	for(int i = 0; i < (int)a.size(); i++) A[i] = complex_base(a[i], 0);
	for(int i = 0; i < (int)b.size(); i++) B[i] = complex_base(b[i], 0);

	fft(A, lg); fft(B, lg);
	for(int i = 0; i < (1 << lg); i++)
		A[i] = A[i] * B[i];
	inv_fft(A, lg);

	vector<int> ans(a.size() + b.size(), 0);
	for(int i = 0; i < (int)ans.size(); i++)
		ans[i] = (int64_t)(A[i].x + 0.5) % mod;

	return ans;
}

vector<int> mult_mod(const vector<int> &a, const vector<int> &b)
{
	/// Thanks pavel.savchenkov

	// a = a0 + sqrt(MOD) * a1
	// a = a0 + base * a1
	int base = (int)sqrtl(mod);

	vector<int> a0(a.size()), a1(a.size());
	for(int i = 0; i < (int)a.size(); i++) 
	{
		a0[i] = a[i] % base;
		a1[i] = a[i] / base;
	}

	vector<int> b0(b.size()), b1(b.size());
	for(int i = 0; i < (int)b.size(); i++)
	{
		b0[i] = b[i] % base;
		b1[i] = b[i] / base;
	}

	vector<int> a01 = a0;
	for(int i = 0; i < (int)a.size(); i++) 
		addmod(a01[i], a1[i], mod);  	

	vector<int> b01 = b0;
	for(int i = 0; i < (int)b.size(); i++) 
		addmod(b01[i], b1[i], mod);

	vector<int> C = mult(a01, b01);  // 1

	vector<int> a0b0 = mult(a0, b0); // 2
	vector<int> a1b1 = mult(a1, b1); // 3

	vector<int> mid = C;
	for(int i = 0; i < (int)mid.size(); i++) 
	{
		addmod(mid[i], -a0b0[i] + mod, mod);
		addmod(mid[i], -a1b1[i] + mod, mod);
	}

	vector<int> res = a0b0;
	for(int i = 0; i < (int)res.size(); i++)
		addmod(res[i], mulmod(base, mid[i], mod), mod);

	base = mulmod(base, base, mod);
	for(int i = 0; i < (int)res.size(); i++)
		addmod(res[i], mulmod(base, a1b1[i], mod), mod);

	return res;
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

