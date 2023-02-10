#include <iostream>
#include <queue>
#include <cstring> // for memset
#include <map>
#include <sstream>

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

    int get_index() {
        // return arr[0] + l1 * arr[1] + l2 * arr[2] + l3 * arr[3];
        return arr[0] * 1000000 +  arr[1] * 10000 + arr[2] * 100 + arr[3];
    }

    string to_str() {
        stringstream ss;
        for (int i = 0; i < 4; ++i) {
            ss << arr[i];
        }
        return ss.str();
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
    int *visited = new int[80808080];
    map<string, four_tuple> parent;
    queue<four_tuple> q;
    

    S.dist = 0;
   
    visited[S.get_index()] = 1; // visited[s] = true
    q.push(S); 

    // cout << "starting while" << endl;

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

                // cout << "left shift | visited = " << visited[neighbor.get_index()] << endl;

                if (!visited[neighbor.get_index()]) {
                    // cout << "SHIFT LEFT stack push (i = " << i << "), neighbor: ";
                    // neighbor.print();
                    parent[neighbor.to_str()] = cur;

                    if (is_T) {
                        T.dist = cur.dist + 1;
                        done = true;
                        break;
                    }

                    visited[neighbor.get_index()] = 1;
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

                // cout << "right shift | visited = " << visited[neighbor.get_index()] << endl;

                if (!visited[neighbor.get_index()]) {
                    // cout << "SHIFT RIGHT stack push (i = " << i << "), neighbor: ";
                    // neighbor.print();
                    parent[neighbor.to_str()] = cur;

                    if (is_T) {
                        T.dist = cur.dist + 1;
                        done = true;
                        break;
                    }

                    visited[neighbor.get_index()] = 1;
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

                // cout << "left reflect | visited = " << visited[neighbor.get_index()] << endl;

                if (!visited[neighbor.get_index()]) {
                    // cout << "REFLECT LEFT stack push (i = " << i << "), neighbor: ";
                    // neighbor.print();
                    parent[neighbor.to_str()] = cur;

                    if (is_T) {
                        T.dist = cur.dist + 1;
                        done = true;
                        break;
                    }

                    visited[neighbor.get_index()] = 1;
                    q.push(neighbor);
                }
            }
            //4
            if (i < 3 and 2 * cur.arr[i+1] - cur.arr[i] <= n and (i == 2 or cur.arr[i+2] > 2 * cur.arr[i+1] - cur.arr[i])) {
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

                // cout << "right reflect | visited = " << visited[neighbor.get_index()] << endl;

                if (!visited[neighbor.get_index()]) {
                    // cout << "REFLECT RIGHT stack push (i = " << i << "), neighbor: ";
                    // neighbor.print();
                    parent[neighbor.to_str()] = cur;

                    if (is_T) {
                        T.dist = cur.dist + 1;
                        done = true;
                        break;
                    }

                    visited[neighbor.get_index()] = 1;
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
        cur = parent[cur.to_str()];
    }

    // print from stack
    for (int i = T.dist; i >= 0; --i) {
        stack[i].print();
    }
}