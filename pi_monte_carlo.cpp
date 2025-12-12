#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <numeric>
#include <vector>
using namespace std;

// RNG wrapper
// I think this works faster than std::mt19937 and fine for small runs. 
// but it has too low quality for large simulations
struct RNG {
    uint32_t s;
    RNG(uint32_t seed=123456789u){
        if(seed==0) seed=2463534242u;
        s = seed;
    }
    uint32_t next_u32(){
        uint32_t x = s;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        return s = x;
    }
    double next_double(){
        uint64_t hi = next_u32() >> 6;
        uint64_t lo = next_u32() >> 5;
        uint64_t val = (hi << 27) | lo;
        return (double)val / (double)((uint64_t)1 << 53);
    }
};


// For Method 1. Quarter-circle inside unit square using integer arithmetic.
// It uses integers for speed and to minimize floating point operations.
// It simulates throwing random points into a unit square, checks if they fall
// inside the unit circle, and counts how many are inside.
uint64_t circle_count_uint(uint32_t trials, RNG &rng){
    const uint32_t MAX = 0xFFFFFFFFu; // 2^32-1
    uint64_t hits = 0;
    for(uint32_t i=0; i<trials; i++){
        uint32_t x = rng.next_u32();
        uint32_t y = rng.next_u32();

        //circle membership test
        // x² + y² ≤ r²
        uint64_t xx = (uint64_t)x * (uint64_t)x;
        uint64_t yy = (uint64_t)y * (uint64_t)y;
        uint64_t max2 = (uint64_t)MAX * (uint64_t)MAX;
        if (xx + yy <= max2) ++hits;
    }
    return hits;
}

// For Method 2. coprime test using integer gcd
// It counts how many pairs of random integers (a, b) satisfy gcd(a,b) == 1
uint32_t coprime_count(uint32_t trials, RNG &rng){
    uint32_t hits = 0;
    for(uint32_t i=0; i < trials; i++){
        uint32_t a = rng.next_u32() | 1u; 
        uint32_t b = rng.next_u32() | 1u;
        if (gcd(a,b) == 1) ++hits;
    }
    return hits;
}

// For Method 3. Buffon's needle. Lines spaced t=1, needle length l=1 (l<=t)
// estimates the probability that a needle crosses a line
uint32_t buffon_count(uint32_t trials, RNG &rng){
    uint32_t crosses = 0;
    for(uint32_t i=0; i<trials; i++){
        // random needle center
        double y = rng.next_double() * 0.5;
        // random needle angle
        double theta = rng.next_double() * (M_PI / 2.0);
        // projection of needle
        double half_proj = 0.5 * sin(theta); // (l/2)*sin(theta) with l=1
        if (y <= half_proj) ++crosses;
    }
    return crosses;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<uint32_t> Ns = {100u, 1000u, 10000u, 100000u};
    RNG rng(123456789u);

    // for(auto n: Ns) cout << n << " ";
    // cout << "\n";

    cout << "Method 1: Quarter-circle inside unit square (integer arithmetic, no floats)\n";
    for(auto n: Ns){
        uint64_t hits = circle_count_uint(n, rng);

        double p = (double)hits / (double)n;

        // area of Quarter-circle is pi/4
        double pi_est = 4.0 * p;
        cout << "  N=" << setw(6) << n << "  hits=" << setw(8) << hits << "  pi_est=" << pi_est << "\n";
    }
    cout << "\n";

    cout << "Method 2: Probability that two integers are coprime (gcd==1)\n";
    for(auto n: Ns){
        uint32_t hits = coprime_count(n, rng);
        double p = (double)hits / (double)n;

        // p= 6/pi^2 => pi = sqrt(6 / p)
        double pi_est = sqrt(6.0 / p);
        cout << "  N=" << setw(6) << n << "  coprime=" << setw(6) << hits << "  p=" << p << "  pi_est=" << pi_est << "\n";
    }
    cout << "\n";

    cout << "Method 3: Buffon's needle (l=1, t=1)\n";
    for(auto n: Ns){
        uint32_t crosses = buffon_count(n, rng);
        double p = (double)crosses / (double)n;

        // P = 2*L / (pi * T) = 2/pi  => pi = 2 / P
        double pi_est = (p > 0.0) ? (2.0 / p) : 0.0;
        cout << "  N=" << setw(6) << n << "  crosses=" << setw(6) << crosses << "  p=" << p << "  pi_est=" << pi_est << "\n";
    }
    cout << "\n";

    return 0;
}
