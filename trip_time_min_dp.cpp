#include <iostream>
#include <util.h>

using namespace std;

#define TIME_MAX 510001; //  ** max time + 1 **

struct Triple {
    int i; int f; int m;
};

int get_index(int i, int f, int m, int N, int C, int M) {
    return i*(C+1)*(M+1) + f*(M+1) + m;
}

int main() {

    int N, M, C;
    cin >> N >> M >> C;
    int *T = new int[N];    // 1 indexed
    int *R = new int[N+1];  // |    |   |
    int *D = new int[N+1];  // V    V   V

    // travel time
    for (int i = 1; i <= N - 1; ++i) {
        cin >> T[i];
    }

    // fuel time
    for (int i = 1; i <= N; ++i) {
        cin >> R[i];
    }

    // fuel cost
    for (int i = 1; i <= N; ++i) {
        cin >> D[i];
    }



    // tabulation
    // dp.first = min time, dp.second = parent{i, fuel, money}
    pair<int, Triple> *dp = new pair<int, Triple>[N * (C+1) * (M+1)];
    for (int i = N - 1; i >= 2; --i) {
        for (int f = C; f >= 0; --f) {
            for (int m = 0; m <= M; ++m) {
                int refuel = TIME_MAX;
                int nextcity = TIME_MAX;

                // special case for trip from city N-1 to N
                // everything at dp[N] is 0 so for dp[N-1] we just set to T_N-1 if enough fuel
                if (i == N - 1 and f >=  T[N-1]) {
                    dp[get_index(i, f, m, N, C, M)].first = T[N-1]; // since dp[N][][] = 0 so we just need to get to last city
                    dp[get_index(i, f, m, N, C, M)].second = Triple{N, -1, -1}; // only i = N matters cus that breaks out of parent while loop
                    continue;
                }
                
                // if enough fuel: dp[i][f][m] = T_i + dp[i+1][f-T_i][m];
                if (f >= T[i]) {
                    nextcity = T[i] + dp[get_index(i+1, f-T[i], m, N, C, M)].first;
                }

                // if have enough capacity and money: dp[i][f][m] = R_i + dp[i][f+1][m-D_i];
                if (f < C and m >= D[i]) {
                    refuel = R[i] + dp[get_index(i, f+1, m-D[i], N, C, M)].first;
                }


                if (nextcity >= TIME_MAX and refuel >= TIME_MAX) {
                    dp[get_index(i, f, m, N, C, M)].first = TIME_MAX;
                    dp[get_index(i, f, m, N, C, M)].second = Triple{-1, -1, -1};
                // going to next city is faster than refueling before leaving
                } else if (nextcity <= refuel) {
                    dp[get_index(i, f, m, N, C, M)].first = nextcity;
                    dp[get_index(i, f, m, N, C, M)].second = Triple{i+1, f-T[i], m};
                // faster to refuel before heading to next city
                } else { // refuel < nextcity
                    dp[get_index(i, f, m, N, C, M)].first = refuel;
                    dp[get_index(i, f, m, N, C, M)].second = Triple{i, f+1, m-D[i]};
                }
            }
        }
    }

    // set the first value (cus we never refuel on first one)
    if (C >= T[1]) { // only if we have enough fuel for first trip
        dp[get_index(1, C, M, N, C, M)].first = T[1] + dp[get_index(2, C-T[1], M, N, C, M)].first;
        dp[get_index(1, C, M, N, C, M)].second = Triple{2, C-T[1], M};
    } else {
        dp[get_index(1, C, M, N, C, M)].first = TIME_MAX;
    }

    // printing re-fuel values
    if (dp[get_index(1, C, M, N, C, M)].first >= TIME_MAX) { // if impossible to make trip
        cout << -1 << endl;
        return 0;
    }

    int cur_i = 1;
    int cur_f = C;
    int cur_m = M;
    int fi_counter = 0;
    while (cur_i != N) {
        int next_i = dp[get_index(cur_i, cur_f, cur_m, N, C, M)].second.i;
        int next_f = dp[get_index(cur_i, cur_f, cur_m, N, C, M)].second.f;
        int next_m = dp[get_index(cur_i, cur_f, cur_m, N, C, M)].second.m;

        if (next_i == cur_i) {
            ++fi_counter;
        } else {
            cout << fi_counter << " "<< endl;
            fi_counter = 0;
        }
        cur_i = next_i;
        cur_f = next_f;
        cur_m = next_m;


        // this mean something broken
        assert(cur_i!=-1 and cur_f != -1 and cur_m != -1);
    }

    // print out refuel at city N (always zero)
    cout << 0 << endl;
}