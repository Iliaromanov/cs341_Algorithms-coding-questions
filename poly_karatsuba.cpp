#include <iostream>
#include <vector>
#include <utility>

using namespace std;


class Poly {
    public:
        // ctors
        Poly() : coeffs{0} {}
        Poly(int a_0) : coeffs{a_0} {}


        // The Rule of 5
        // copy ctor
        Poly(Poly &other) {
            cout << "COPY CTOR" << endl;

            coeffs.clear();
            coeffs = other.coeffs;
        }
        // copy assignment
        Poly &operator=(const Poly &rhs) {
            cout << "COPY ASSIGN" << endl;
            if (this == &rhs) return *this;
            coeffs.clear();
            coeffs = rhs.coeffs;
            return *this;
        }
        // move ctor
        Poly(Poly &&other) {
            cout << "MOVE CTOR" << endl;
            coeffs.clear();
            coeffs = move(other.coeffs);
        }
        // move assignment
        Poly &operator=(Poly &&rhs) {
            cout << "MOVE ASSIGN" << endl;
            coeffs.clear();
            coeffs = move(rhs.coeffs);
            return *this;
        }
        // dtor
        ~Poly() {}

        // coefficients a0, ..., an; for a_0 + a_1*x + ... + a_n*x
        vector<long long int> coeffs;

        // Shift coeffs to start at index i
        Poly shift(int i) const {
            int cur_size = coeffs.size();
            Poly shifted;
            for (int j = 0; j < cur_size + i; ++j) {
                int push_val = j < i ? 0 : coeffs[j-i];

                if (j == 0) {
                    shifted.coeffs[0] = push_val;
                } else {
                    shifted.coeffs.push_back(push_val);
                }
            }
            return shifted;
        }

        // splits polynomial into x1 = a_0 + ... + a_floor(n/2)x^floor(n/2)
        //  and x2 = a_{floor(n/2) + 1} + ... + a_n
        // so a = x1 + x2.shift(floor(n/2) + 1)
        // if n is even, use floor(n/2) - 1, and floor(n/2) as splits
        // if n is odd, use floor(n/2), and floor(n/2) + 1 as splits
        // returns the amount by which x2 needs to be shifted
        // requires coeffs.size() > 1
        int split(Poly &x1, Poly &x2) const {
            int n = coeffs.size();
            int mid = n % 2 ? n/2 : n/2 - 1;

            for (int i = 0; i < n; ++i) {
                if (i <= mid) {
                    // Poly are initialized as deg 0 so need to
                    //  reset coeffs[0]
                    if (i == 0) {
                        x1.coeffs[0] = coeffs[0];
                    } else {
                        x1.coeffs.push_back(coeffs[i]);
                    }
                } else {
                    if (i == mid + 1) {
                        x2.coeffs[0] = coeffs[i];
                    } else {
                        x2.coeffs.push_back(coeffs[i]);
                    }
                }
            }
            return mid + 1;
        }

        // print out coefficients
        void print() const {
            for (int i = 0; i < coeffs.size() - 1; ++i) {
                cout << coeffs[i] << " ";
            }
            cout << coeffs[coeffs.size() - 1] << endl; // avoid trailing space
        }

        // requires coeffs.size() == rhs.coeffs.size()
        Poly operator+(const Poly &rhs) const {
            Poly result{coeffs[0] + rhs.coeffs[0]};
            int l = coeffs.size();
            for (int i = 1; i < l; ++i) {
                result.coeffs.push_back(coeffs[i] + rhs.coeffs[i]);
            }
            return result;
        }

        // requires coeffs.size() == rhs.coeffs.size()
        Poly operator-(const Poly &rhs) const {
            Poly result{coeffs[0] - rhs.coeffs[0]};
            int l = coeffs.size();
            for (int i = 1; i < l; ++i) {
                result.coeffs.push_back(coeffs[i] - rhs.coeffs[i]);
            }
            return result;
        }

        // Karatsuba
        //  requires coeffs.size() == rhs.coeffs.size()
        Poly operator*(const Poly &rhs) const {
            // base case: degree 0
            if (coeffs.size() == 1) {
                Poly result{coeffs[0] * rhs.coeffs[0]};
                return result;
            }

            Poly a1, a2, b1, b2;
            int shift_amount = split(a1, a2);
            rhs.split(b1, b2); // rhs should give the same shift_amount

            cout << "a1: " << endl;
            a1.print();
            cout << "a2: " << endl;
            a2.print();
            cout << "b1: " << endl;
            b1.print();
            cout << "b2: " << endl;
            b2.print();

            Poly a1_b1 = a1 * b1;
            cout << "a1_b1: " << endl;
            a1_b1.print();
            Poly a2_b2 = a2 * b2;
            cout << "a2_b2: " << endl;
            a2_b2.print();
            Poly a12_b12 = (a1 + a2) * (b1 + b2);
            cout << "a12_b12: " << endl;
            a12_b12.print();

            return a1_b1 + 
                   (a12_b12 - a1_b1 - a2_b2).shift(shift_amount) + 
                   a2_b2.shift(2 * shift_amount);
        }
};


int main() {
    int deg;
    cin >> deg;
    Poly a{};
    Poly b{};
    
    // read in coeffs for a
    int coeff;
    for (int i = 0; i <= deg; ++i) {
        cin >> coeff;
        if (i == 0) {
            a.coeffs[0] = coeff;
        } else {
            a.coeffs.push_back(coeff);
        }
    }

    // read in coeffs for b
    for (int i = 0; i <= deg; ++i) {
        cin >> coeff;
        if (i == 0) {
            b.coeffs[0] = coeff;
        } else {
            b.coeffs.push_back(coeff);
        }
    }

    cout << "a:" << endl;
    a.print();
    Poly x1;
    Poly x2;
    a.split(x1, x2);
    cout << "x1:" << endl;
    x1.print();
    cout << "x2:" << endl;
    x2.print();
    cout << "a*x^2:" << endl;
    a.shift(2).print();
    cout << "b:" << endl;
    b.print(); 
    cout << "a + b: " << endl;
    (a + b).print();
    cout << "a - b: " << endl;
    (a - b).print();

    cout << "a * b: " << endl;
    (a * b).print();



}