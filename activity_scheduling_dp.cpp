#include <iostream>
#include <algorithm>
#include <cassert>
#include <util.h>
#include "limits.h"

using namespace std;

struct Activity {
    int S; int T; int V; int E;
    int initial_i; // the i at which its read in (for printing at the end)
};

struct Triple {
    int parent_i; // interval number parent_i is the parent of current
    int parent_E;
    // say we are checking par[i][j]
    //  if par[i][j].include_current == true, then we add i to our knapsack
    //  otherwise we just continue
    //  for next we check par[parent_i][parent_E] and so on
    bool include_current;
};

bool activityLeq(Activity a1, Activity a2) {
    return (a1.S <= a2.S);
}


// int knapsack(int i, int E, int N, bool (&visited)[100 * 1500], int (&answer)[100*1500]) {
//     if (visited[i + E * 100]) return answer[i + E*100];
//     if (i > N) return 0; // should not be possible?
//     if (E < 0) return INT_MIN;  // should not be possible?


// }



int main() {
    int N;
    int M;
    cin >> N >> M;

    Activity *intervals = new Activity[N+1];

    for (int i = 1; i <= N; ++i) { // intervals is 1 indexed
        intervals[i] = Activity();
        cin >> intervals[i].S >> intervals[i].T >> intervals[i].V >> intervals[i].E;
        intervals[i].initial_i = i;
    }




    // TEST
    // cout << "unsorted:" <<endl;
    // for (int i = 1; i <= N; ++i) {
    //     cout << "S: " << intervals[i].S << " T: " << intervals[i].T << " V: " << intervals[i].V << " E: " << intervals[i].E << endl;
    // }

    //




    sort(intervals+1, intervals + N + 1, activityLeq); // sort by non-decreasing start time





    // cout << "sorted:" <<endl;
    // for (int i = 1; i <= N; ++i) {
    //     cout << "S: " << intervals[i].S << " T: " << intervals[i].T << " V: " << intervals[i].V << " E: " << intervals[i].E << endl;
    // }




    int next[N+1]; // next is 1 indexed
    next[N] = N+1;


    // cout << "N: " << N << endl;


    // populating next array
    for (int i = 1; i < N; ++i) {
        int l = i; int r = N;

        // optimization for when interval finishes 
        //  after all other intervals
        if (intervals[r].S < intervals[i].T) {
            // cout << "i: " << i << " | " << "r: " << r << endl;
            // cout << "next[i] = N + 1" << endl;
            next[i] = N+1;
            continue;
        }

        // cout << "while loop: " << endl;
        bool found = false;
        while (l < r) {
            // cout << "l: " << l << " | r: " << r;

            int mid = l + (r - l) / 2;

            // cout << " mid: " << mid << endl;

            if (intervals[mid].S == intervals[i].T) { // not possible for S to be lower
                found = true;
                next[i] = mid;
                break;
            } else if (intervals[mid].S > intervals[i].T) { // disjoint but maybe exists earlier S
                r = mid;
            } else {
                l = mid + 1;
            }
        }
        // assert interval l is disjoint from interval i; if its not i fked smthn up (pls no)
        if (!found) assert(intervals[l].S >= intervals[i].T);                                                       // <------------- THIS FAILED!!!!!!!!
        // assert interval l-1 is intersecting with interval i; if its not i fked smthn up (pls no)
        if (l - 1 >= 0) assert(intervals[l-1].S < intervals[i].T);
        if (!found) next[i] = l;
    }

    // dp[i + L * (N+1)]

    /*
    
        MIGHT HAVE TO STORE some sort of duble in dp - first part for cur_K, second for cur_V(the thing it holds now)
    
    */

    pair<int, int> *dp = new pair<int, int>[(N + 1) * 931]; // rows is 1 indexed - [1, N] and cols is [0, 930]
    Triple *par = new Triple[(N+1) * 931];
    for (int i = N; i >= 1; --i) {
        for (int E = 0; E <= 930; ++E) {
            int do_i = 0;
            int dont_do_i = 0;
            int do_i_K = 0;
            int dont_do_i_K = 0;
            if (E >= intervals[i].E) { // have enough energy to do interval i
                if (next[i] == N+1) { // rest of intervals overlap with this one
                    do_i = intervals[i].V;
                    do_i_K = 1;
                } else {
                    // Vi + dp[next[i]][E - Ei + (S_next[i] - Ti)]
                    do_i = intervals[i].V + dp[next[i] + (E - intervals[i].E + (intervals[next[i]].S - intervals[i].T)) * (N+1)].second;
                    do_i_K = dp[next[i] + (E - intervals[i].E + (intervals[next[i]].S - intervals[i].T)) * (N+1)].first + 1;
                }
            }

            if ((i+1) <= N) { // there exists another interval after this one
                // dp[i+1][E + (S_(i+1) - Si)]
                dont_do_i = dp[i+1 + (E + (intervals[i+1].S - intervals[i].S)) * (N+1)].second;
                dont_do_i_K = dp[i+1 + (E + (intervals[i+1].S - intervals[i].S)) * (N+1)].first;
            }

            // dp[i][E] = max(do_i, dont_do_i);
            if (do_i >= dont_do_i) {
                Triple t;
                t.include_current = true;
                t.parent_i = next[i];
                t.parent_E = next[i] <= N ? E - intervals[i].E + (intervals[next[i]].S - intervals[i].T) : 0;
                dp[i + E * (N+1)] = pair<int, int>{do_i_K, do_i};
                par[i + E * (N+1)] = t;//Triple();
                // par[i + E * (N+1)].include_current = true;
                // par[i + E * (N+1)].parent_i = next[i];
                // par[i + E * (N+1)].parent_E = E - intervals[i].E + (intervals[next[i]].S - intervals[i].T);
            } else {
                dp[i + E * (N+1)] = pair<int, int>{dont_do_i_K, dont_do_i};
                Triple t;
                t.include_current = false;
                t.parent_i = i+1;
                t.parent_E = i+1 <= N ? E + intervals[i+1].S - intervals[i].S : 0;
                par[i + E * (N+1)] = t;//Triple();
                // par[i + E * (N+1)].include_current = false;
                // par[i + E * (N+1)].parent_i = i+1;
                // par[i + E * (N+1)].parent_E = E + intervals[i+1].S - intervals[i].S;
            }
        }
    }

    // Printing output
    int K = dp[1 + M * (N+1)].first; //count
    int V = dp[1 + M * (N+1)].second; // dp[1][M]
    cout << K << " " << V << endl;

    int cur_i = 1;
    int cur_E = M;
    while (cur_i <= N) {
        int new_i = par[cur_i + cur_E * (N+1)].parent_i;
        int new_E = par[cur_i + cur_E * (N+1)].parent_E;
        if (par[cur_i + cur_E * (N+1)].include_current) {
            cout << intervals[cur_i].initial_i;
            --K;
            if (K > 0) cout << " ";
        }
        cur_i = new_i;
        cur_E = new_E;
    }
    cout << endl;
}