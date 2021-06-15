#include <bits/stdc++.h>
#define endl '\n'

#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()
#define lb lower_bound
#define ub upper_bound
#define pb push_back

using namespace std;
template<class T, class T1> int chkmin(T &x, const T1 &y) { return x > y ? x = y, 1 : 0; }
template<class T, class T1> int chkmax(T &x, const T1 &y) { return x < y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);

template<typename T>
struct slope_trick {
	T offset_l, offset_r, zero_height;
	priority_queue<T> L;
	priority_queue<T, vector<T>, greater<T>> R;

	slope_trick() {
		offset_l = 0;
		offset_r = 0;
		zero_height = 0;
	}

	T zero_start() {
		if(L.empty()) {
			return numeric_limits<T>::min();
		} else {
			return L.top() + offset_l;
		}
	}

	T zero_end() {
		if(R.empty()) {
			return numeric_limits<T>::max();
		} else {
			return R.top() + offset_r;
		} 
	}

	/* Add the function f(x) = height + |x - center| to the current slope trick. */
	void add_simple(const T &center, const T &height = 0) {
		T s = zero_start(), e = zero_end();
		zero_height += height;
		if(s <= center && center <= e) {
			L.push(center - offset_l);
			R.push(center - offset_r);
		} else if(center < s) {
			L.push(center - offset_l);
			L.push(center - offset_l);
			L.pop();
			R.push(s - offset_r);					
			zero_height += abs(center - s);	
		} else {
			R.push(center - offset_r);
			R.push(center - offset_r);
			R.pop();	
			L.push(e - offset_l);
			zero_height += abs(center - e);	
		}
	}

	/* Update the current slope trick to its prefix minimum - i.e. f'(x) = min f(x') for all x' <= x.  */ 
	void prefix_min() {
		offset_r = 0;
		while(!R.empty()) {
			R.pop();
		}
	}

	/* Update the current slope trick to its prefix minimum - i.e. f'(x) = min f(x') for all x' >= x.  */ 
	void suffix_min() {
		offset_l = 0;
		while(!L.empty()) {
			L.pop();
		}
	}

	/* Update the current slope trick to its local minimum - i.e. f'(x) = min f(x') for all x - strip_width <= x' <= x + strip_width.  */ 
	void local_min(const T &strip_width) {
		if(!L.empty()) {
			offset_l -= strip_width;
		}

		if(!R.empty()) {
			offset_r += strip_width;
		}
	}
};

int n;
int a[MAXN];

void read() {
	cin >> n;
	for(int i = 0; i < n; i++) {
		cin >> a[i];
	}
}

void solve() {
	slope_trick<int64_t> dp;
	for(int i = 0; i < n; i++) {
		a[i] -= i;
		dp.add_simple(a[i]);
		dp.prefix_min();
	}

	cout << dp.zero_height << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	int T = 1;
	//cin >> T;
	for(int test = 1; test <= T; test++) {
		read();
		//cout << "Case #" << test << ": ";
		solve();
	}

	return 0;
}

