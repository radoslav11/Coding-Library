#include <bits/stdc++.h>
#define endl '\n'

//#pragma GCC optimize ("O3")
//#pragma GCC target ("sse4")

using namespace std;
template<class T, class T2> inline int chkmax(T &x, const T2 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline int chkmin(T &x, const T2 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);
const int mod = (int)1e9 + 7;

template<class T>
T pw(T a, int pw)
{
	T ret(1);
	while(pw)
	{
		if(pw & 1) ret *= a; 
		a *= a;
		pw >>= 1;
	}

	return ret;
}

template<unsigned mod>
class Modint 
{
	private:
		unsigned x;
	
	public:
		Modint() { x = 0; }
		Modint(unsigned _x) { x = _x; }
		operator unsigned() { return x; }
		
		Modint operator==(const Modint& m) const { return x == m.x; }
		Modint operator!=(const Modint& m) const { return x != m.x; }
		
		Modint operator+=(const Modint& m) { x = (x + m.x >= mod ? x + m.x - mod : x + m.x); return *this; }
		Modint operator-=(const Modint& m) { x = (x < m.x ? x - m.x + mod : x - m.x); return *this; }
		Modint operator*=(const Modint& m) { x = 1ULL * x * m.x % mod; return *this; }
	
		Modint operator+=(const int32_t m) { x = (x + (m % mod) >= mod ? x + (m % mod) - mod : x + (m % mod)); return *this; }
		Modint operator-=(const int32_t m) { x = (x < (m % mod) ? x - (m % mod) + mod : x - (m % mod)); return *this; }
		Modint operator*=(const int32_t m) { x = 1ULL * x * (m % mod) % mod; return *this; }

		Modint operator+=(const int64_t m) { x = (x + (m % mod) >= mod ? x + (m % mod) - mod : x + (m % mod)); return *this; }
		Modint operator-=(const int64_t m) { x = (x < (m % mod) ? x - (m % mod) + mod : x - (m % mod)); return *this; }
		Modint operator*=(const int64_t m) { x = 1ULL * x * (m % mod) % mod; return *this; }

		Modint operator+(const Modint& m) const { return Modint(*this) += m; }
		Modint operator-(const Modint& m) const { return Modint(*this) -= m; }
		Modint operator*(const Modint& m) const { return Modint(*this) *= m; }

		Modint operator+(const int32_t m) const { return Modint(*this) += m; }
		Modint operator-(const int32_t m) const { return Modint(*this) -= m; }
		Modint operator*(const int32_t m) const { return Modint(*this) *= m; }

		Modint operator+(const int64_t m) const { return Modint(*this) += m; }
		Modint operator-(const int64_t m) const { return Modint(*this) -= m; }
		Modint operator*(const int64_t m) const { return Modint(*this) *= m; }

		Modint inv() { return pw(Modint(*this), mod - 2); }		
};

Modint<mod> fact[MAXN], ifact[MAXN];

void precompute()
{
	fact[0] = 1;
	for(int i = 1; i < MAXN; i++) fact[i] = fact[i - 1] * i;
	ifact[MAXN - 1] = fact[MAXN - 1].inv();
	for(int i = MAXN - 2; i >= 0; i--) ifact[i] = ifact[i + 1] * (i + 1);
}

Modint<mod> C(int n, int k) 
{  
	if(n < k || n < 0 || k < 0) return Modint<mod>(0);
	return fact[n] * ifact[n - k] * ifact[k];
} 

void read()
{
	precompute();
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

