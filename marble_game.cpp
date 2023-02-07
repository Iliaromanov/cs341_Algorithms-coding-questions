#include <iostream>
#include <queue>
#include <cstring> // for memset

using namespace std;

// option, add parent pointer attribute instead of parent 

struct four_tuple {
    int arr[4];
    int dist;

    four_tuple &operator=(const four_tuple &rhs) {
        for (int i = 0; i < 4; ++i) {
            arr[i] = rhs.arr[i];
        }
        return *this;
    }

    void print() {
        cout << arr[0] << " " << arr[1] << " " << arr[2] << " " << arr[3] << endl;
    }

    bool operator==(const four_tuple &rhs) const {
        for (int i = 0; i < 4; ++i) {
            if (arr[i] != rhs.arr[i]) return false;
        }
        return true;
    }
};


int main() {
    int n;
    four_tuple S;
    four_tuple T;

    cin >> n; // read n
    for (int i = 0; i < 4; ++i) cin >> S.arr[i]; // read S
    for (int i = 0; i < 4; ++i) cin >> T.arr[i]; // read T


    // add 1 to indices since S is 1-indexed
    int visited[n-2][n-1][n][n+1];
    // int dist[n-2][n-1][n][n+1];
    four_tuple parent[n-2][n-1][n][n+1];
    queue<four_tuple> q;
    

    memset(visited, 0, sizeof(visited)); // visited[v] = false
    // dist[S.arr[0]][S.arr[1]][S.arr[2]][S.arr[3]] = 0; // dist[s] = 0
    S.dist = 0;
    visited[S.arr[0]][S.arr[1]][S.arr[2]][S.arr[3]] = 1; // visited[s] = true
    q.push(S); 

    cout << "starting while" << endl;

    bool done = false;
    while (!done) {
        four_tuple cur = q.front();
        // cout << "cur: ";
        // cur.print();
        q.pop();

        // check neighbours
        // for each val in four_tuple check:
        // 1. shift left
        // 2. shift right
        // 3. reflect left
        // 4. reflect right
        for (int i = 0; i < 4; ++i) {
            //1
            if (cur.arr[i] > 1 and (i == 0 or cur.arr[i-1] < cur.arr[i] - 1)) {
                bool is_T = true;
                four_tuple neighbor;
                for (int j = 0; j < 4; j++) {
                    if (j == i) {
                        neighbor.arr[j] = cur.arr[i] - 1;
                    } else {
                        neighbor.arr[j] = cur.arr[j];
                    }

                    // check for if this is end result
                    if (neighbor.arr[j] != T.arr[j]) is_T = false;
                }
                neighbor.dist = cur.dist + 1;

                if (!visited[neighbor.arr[0]][neighbor.arr[1]][neighbor.arr[2]][neighbor.arr[3]]) {
                    // cout << "SHIFT LEFT stack push (i = " << i << "), neighbor: ";
                    // neighbor.print();
                    parent[neighbor.arr[0]][neighbor.arr[1]][neighbor.arr[2]][neighbor.arr[3]] = cur;

                    if (is_T) {
                        T.dist = cur.dist + 1;
                        done = true;
                        break;
                    }

                    visited[neighbor.arr[0]][neighbor.arr[1]][neighbor.arr[2]][neighbor.arr[3]] = 1;
                    q.push(neighbor);
                }
            }
            //2
            if (cur.arr[i] < n and (i == 3 or cur.arr[i+1] > cur.arr[i] + 1)) {
                bool is_T = true;
                four_tuple neighbor;
                for (int j = 0; j < 4; j++) {
                    if (j == i) {
                        neighbor.arr[j] = cur.arr[i] + 1;
                    } else {
                        neighbor.arr[j] = cur.arr[j];
                    }

                    // check for if this is end result
                    if (neighbor.arr[j] != T.arr[j]) is_T = false;
                }
                neighbor.dist = cur.dist + 1;

                if (!visited[neighbor.arr[0]][neighbor.arr[1]][neighbor.arr[2]][neighbor.arr[3]]) {
                    // cout << "SHIFT RIGHT stack push (i = " << i << "), neighbor: ";
                    // neighbor.print();
                    parent[neighbor.arr[0]][neighbor.arr[1]][neighbor.arr[2]][neighbor.arr[3]] = cur;

                    if (is_T) {
                        T.dist = cur.dist + 1;
                        done = true;
                        break;
                    }

                    visited[neighbor.arr[0]][neighbor.arr[1]][neighbor.arr[2]][neighbor.arr[3]] = 1;
                    q.push(neighbor);
                }
            }
            //3
            if (i > 0 and 2 * cur.arr[i-1] - cur.arr[i] >= 1 and (i == 1 or cur.arr[i-2] < 2 * cur.arr[i-1] - cur.arr[i])) {
                // cout << "got into left reflect: " << endl;
                // cout << "2 * cur.arr[i-1] - cur.arr[i] = " << (2 * cur.arr[i-1] - cur.arr[i]) << endl;
                // cout << "cur.arr[i-2] < 2 * cur.arr[i-1] - cur.arr[i] = " << (cur.arr[i-2] < 2 * cur.arr[i-1] - cur.arr[i]) << endl;


                int two_q_minus_p = 2 * cur.arr[i-1] - cur.arr[i];
                bool is_T = true;
                four_tuple neighbor;
                for (int j = 0; j < 4; j++) {
                    if (j == i - 1) {
                        neighbor.arr[j] = two_q_minus_p;
                    } else if (j == i) {
                        neighbor.arr[j] = cur.arr[i-1];
                    } else {
                        neighbor.arr[j] = cur.arr[j];
                    }

                    // check for if this is end result
                    if (neighbor.arr[j] != T.arr[j]) is_T = false;
                }
                neighbor.dist = cur.dist + 1;

                if (!visited[neighbor.arr[0]][neighbor.arr[1]][neighbor.arr[2]][neighbor.arr[3]]) {
                    // cout << "REFLECT LEFT stack push (i = " << i << "), neighbor: ";
                    // neighbor.print();
                    parent[neighbor.arr[0]][neighbor.arr[1]][neighbor.arr[2]][neighbor.arr[3]] = cur;

                    if (is_T) {
                        T.dist = cur.dist + 1;
                        done = true;
                        break;
                    }

                    visited[neighbor.arr[0]][neighbor.arr[1]][neighbor.arr[2]][neighbor.arr[3]] = 1;
                    q.push(neighbor);
                }
            }
            //4
            if (i < 3 and 2 * cur.arr[i+1] - cur.arr[i] <= n and (i == 2 or cur.arr[i+2] > 2 * cur.arr[i+1] - cur.arr[i])) {


                // cout << "got into right reflect: " << endl;
                // cout << "2 * cur.arr[i-1] - cur.arr[i] = " << (2 * cur.arr[i-1] - cur.arr[i]) << endl;
                // cout << "cur.arr[i-2] < 2 * cur.arr[i-1] - cur.arr[i] = " << (cur.arr[i-2] < 2 * cur.arr[i-1] - cur.arr[i]) << endl;


                int two_q_minus_p = 2 * cur.arr[i+1] - cur.arr[i];
                bool is_T = true;
                four_tuple neighbor;
                for (int j = 0; j < 4; j++) {
                    if (j == i + 1) {
                        neighbor.arr[j] = two_q_minus_p;
                    } else if (j == i) {
                        neighbor.arr[j] = cur.arr[i+1];
                    } else {
                        neighbor.arr[j] = cur.arr[j];
                    }

                    // check for if this is end result
                    if (neighbor.arr[j] != T.arr[j]) is_T = false;
                }
                neighbor.dist = cur.dist + 1;

                if (!visited[neighbor.arr[0]][neighbor.arr[1]][neighbor.arr[2]][neighbor.arr[3]]) {
                    // cout << "REFLECT RIGHT stack push (i = " << i << "), neighbor: ";
                    // neighbor.print();
                    parent[neighbor.arr[0]][neighbor.arr[1]][neighbor.arr[2]][neighbor.arr[3]] = cur;

                    if (is_T) {
                        T.dist = cur.dist + 1;
                        done = true;
                        break;
                    }

                    visited[neighbor.arr[0]][neighbor.arr[1]][neighbor.arr[2]][neighbor.arr[3]] = 1;
                    q.push(neighbor);
                }
            }
        }
    }

    // cout << "GOT THROUGH WHILE " << endl;


    // Print result;
    // M:
    cout << T.dist << endl;
    // M + 1 prints:

    // go through parent and put on stack
    four_tuple stack[T.dist + 1];
    four_tuple cur = T;
    for (int i = 0; i < T.dist + 1; ++i) {
        stack[i] = cur;
        cur = parent[cur.arr[0]][cur.arr[1]][cur.arr[2]][cur.arr[3]];
    }

    // print from stack
    for (int i = T.dist; i >= 0; --i) {
        stack[i].print();
    }



    

    // for (int i = 0; i < n - 2; ++i) {
    //     cout << "i:" << i << endl;
    //     for (int j = 0; j < n - 1; ++j) {
    //         cout << "j:" << j << endl;
    //         for (int k = 0; k < n; ++k) {
    //             cout << "k:" << k << endl;
    //             for (int l = 0; l < n + 1; ++l) {
    //                 cout << "(l = " << l << ") " << visited[i][j][k][l] << " | ";
    //             }
    //             cout << endl;
    //         }
    //     }
    // }
}