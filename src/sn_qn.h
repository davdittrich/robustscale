#ifndef ROBUSTSCALE_SN_QN_H
#define ROBUSTSCALE_SN_QN_H

#include <vector>
#include <algorithm>
#include <cmath>

namespace robustscale {

/**
 * Consistency factors for Sn from robustbase / Rousseeuw & Croux (1993)
 */
inline double sn_consistency(int n) {
    if (n <= 1) return 1.0;
    static const double sn_table[] = {
        1.0, 1.196, 1.495, 1.191, 1.333, 1.129, 1.231, 1.106, 1.171, 1.092
    };
    if (n <= 10) return sn_table[n-1];
    return 1.1926;
}

/**
 * Consistency factors for Qn from robustbase / Rousseeuw & Croux (1993)
 */
inline double qn_consistency(int n) {
    if (n <= 1) return 1.0;
    static const double qn_table[] = {
        1.0, 0.399, 0.994, 0.512, 0.844, 0.611, 0.857, 0.669, 0.872, 0.708
    };
    if (n <= 10) return qn_table[n-1];
    return 2.2191;
}

/**
 * Sn Estimator: O(n^2) implementation
 */
inline double sn(const std::vector<double>& x) {
    size_t n = x.size();
    if (n < 2) return 0.0;
    
    std::vector<double> outer_meds(n);
    for (size_t i = 0; i < n; ++i) {
        std::vector<double> diffs(n);
        for (size_t j = 0; j < n; ++j) {
            diffs[j] = std::abs(x[i] - x[j]);
        }
        std::sort(diffs.begin(), diffs.end());
        outer_meds[i] = diffs[n/2];
    }
    
    std::sort(outer_meds.begin(), outer_meds.end());
    double res;
    if (n % 2 == 0) {
        res = (outer_meds[n/2 - 1] + outer_meds[n/2]) / 2.0;
    } else {
        res = outer_meds[n/2];
    }
    
    return res * sn_consistency(n);
}

/**
 * Qn Estimator: O(n^2) implementation
 */
inline double qn(const std::vector<double>& x) {
    size_t n = x.size();
    if (n < 2) return 0.0;
    
    size_t n_pairs = n * (n - 1) / 2;
    std::vector<double> diffs;
    diffs.reserve(n_pairs);
    
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            diffs.push_back(std::abs(x[i] - x[j]));
        }
    }
    
    size_t k = std::floor(n_pairs / 4.0);
    std::nth_element(diffs.begin(), diffs.begin() + k, diffs.end());
    
    return diffs[k] * qn_consistency(n);
}

} // namespace robustscale

#endif
