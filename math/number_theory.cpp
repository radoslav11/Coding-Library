#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);
const int bound = 1000000;
const int64_t mod = (int64_t)1e9 + 7;

int64_t pow(int64_t x, int64_t p, int64_t MOD)
{
    if(p == 0) return 1;
    if(p % 2 == 1) return (x * pow(x, p - 1, MOD)) % MOD;
    int64_t half = pow(x, p / 2, MOD);
    return (half * half) % MOD;
}

int lp[bound + 1], phi[bound + 1];
vector<int> pr;

template<class T>
vector<T> get_divs(T x)
{
    if(x <= bound)
    {
        vector<T> ret;
        while(x != 1) { ret.push_back(lp[x]); x /= lp[x]; }
        return ret;
    }

    vector<T> ret;
    for(T d = 2; d * 1ll * d <= x; d++)
        while(x % d == 0) ret.push_back(d), x /= d;

    if(x != 1) ret.push_back(x);
    return ret;
}

int64_t gcd(int64_t a, int64_t b, int64_t &x, int64_t &y)
{
	if(a == 0)
	{
		x = 0;
		y = 1;
		return b;
	}

	int64_t x1, y1, g = gcd(b % a, a, x1, y1);
	x = y1 - (b / a) * x1;
	y = x1;

	return g;
}

void prepare()
{
    phi[1] = 1;
    for(int i = 2; i <= bound; i++)
    {
        if(lp[i] == 0) lp[i] = i, phi[i] = i - 1, pr.push_back(i);
        else if(lp[i] == lp[i / lp[i]]) phi[i] = phi[i / lp[i]] * lp[i];
        else phi[i] = phi[i / lp[i]] * (lp[i] - 1);
        for(int j = 0; j < pr.size() && i * 1ll * pr[j] <= bound && pr[j] <= lp[i]; j++)
            lp[i * pr[j]] = pr[j];
    }
}

int64_t gcd(int64_t a, int64_t b)
{
    if(a == 0 || b == 0) return a + b;
    return __gcd(a, b);
}

int64_t get_phi(int64_t x)
{
    int64_t cop = x, ret = x;
    for(int64_t div = 2; div * div <= cop; div++)
        if(cop % div == 0)
        {
            while(cop % div == 0) cop /= div;
            ret /= div;
            ret *= (div - 1);
        }

    if(cop != 1) ret /= cop, ret *= (cop - 1);
    return ret;
}

int64_t pinv(int64_t x, int64_t MOD) { return pow(x, MOD - 2, MOD); }

int64_t inv(int64_t x, int64_t MOD)
{
    if(gcd(x, MOD) != 1) return -1;
    if(MOD <= bound) return pow(x, phi[MOD] - 1, MOD);
    return pow(x, get_phi(MOD) - 1, MOD);
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
