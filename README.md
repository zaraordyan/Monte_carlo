# Monte_carlo

## Methods

### 1. Monte Carlo Circle
- Converges reasonably quickly.
- Accuracy improves with the number of points.
- Easy to implement and intuitive.

### 2. Co-prime Integers
- Converges slightly slower for small sample sizes.
- Requires very large integers to reduce bias.
- Becomes accurate for large `N`.

### 3. Buffon's Needle
- Converges slower due to high variance in crossings.
- For small `N`, the estimate can be far from the true value of π.
- Conceptually interesting, but less efficient computationally.


## Compilation

```bash
clang++ -std=c++17 pi_monte_carlo.cpp -o pi_mc
./pi_mc
