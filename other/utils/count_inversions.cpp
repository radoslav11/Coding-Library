#include <bits/stdc++.h>
using namespace std;

int64_t count_inversions(int *start, int *end) {
    if(end - start <= 1) {
        return 0;
    }
 
    int *mid = start + (end - start) / 2;
    int64_t ans = count_inversions(start, mid) + count_inversions(mid, end);
 
    vector<int> temp;
    for(int *i = start, *j = mid; i != mid || j != end; ) {
        if(i == mid) {
            temp.push_back(*j++);
        } else if(j == end) {
            temp.push_back(*i++);
        } else if(*i <= *j) {
            temp.push_back(*i++);
        } else {
            ans += mid - i;
            temp.push_back(*j++);
        }
    }
 
    copy(temp.begin(), temp.end(), start);
    return ans;
}
