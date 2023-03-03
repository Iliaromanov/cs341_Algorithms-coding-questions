#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;

struct Activity {
    int S; int T; int V; int E;
};





int main() {
    int N;
    int M;
    cin >> N >> M;

    Activity intervals[N];

    for (int i = 0; i < N; ++i) {
        intervals[i] = Activity();
        cin >> intervals[i].S >> intervals[i].T >> intervals[i].V >> intervals[i].E;
    }

    int next[N];

    // populating next array
    for (int i = 0; i <= N; ++i) {
        int l = i; int r = N;

        // optimization for when interval finishes 
        //  after all other intervals
        if (intervals[r].S < intervals[i].T) {
            next[i] = N+1; 
            continue;
        }
        while (l < r) {
            int mid = l + (r - l) / 2;
            if (intervals[mid].S == intervals[i].T) { // not possible for S to be lower
                next[i] = mid;
                break;
            } else if (intervals[mid].S > intervals[i].T) { // disjoint but maybe exists earlier S
                r = mid;
            } else {
                l = mid + 1;
            }
        }
        // assert interval l is disjoint from interval i; if its not i fked smthn up (pls no)
        assert(intervals[l].S >= intervals[i].T); 
        // assert interval l-1 is intersecting with interval i; if its not i fked smthn up (pls no)
        if (l - 1 >= 0) {assert(intervals[l-1].S < intervals[i].T);}
        next[i] = l;
    }


}