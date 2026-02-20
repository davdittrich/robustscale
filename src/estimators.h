#ifndef ROBUSTSCALE_ESTIMATORS_H
#define ROBUSTSCALE_ESTIMATORS_H

#include <Rcpp.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include "constants.h"

namespace robustscale {

/**
 * Optimized Gini Mean Difference (GMD)
 * Time Complexity: O(n log n)
 */
inline double gmd(const std::vector<double>& x) {
    size_t n = x.size();
    if (n < 2) return 0.0;
    
    std::vector<double> sorted_x = x;
    std::sort(sorted_x.begin(), sorted_x.end());
    
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i) {
        // formula: 2 * sum over i of [ (2*i - n - 1) * x_(i) ] / (n * (n-1))
        // using 0-based index: i -> i+1
        sum += (2.0 * (i + 1.0) - n - 1.0) * sorted_x[i];
    }
    
    double raw_gmd = (2.0 / (n * (n - 1.0))) * sum;
    return raw_gmd * K_GMD;
}

/**
 * Median Absolute Deviation (MAD)
 */
inline double mad(const std::vector<double>& x) {
    size_t n = x.size();
    if (n < 2) return 0.0;
    
    std::vector<double> temp = x;
    std::sort(temp.begin(), temp.end());
    
    double median;
    if (n % 2 == 0) {
        median = (temp[n/2 - 1] + temp[n/2]) / 2.0;
    } else {
        median = temp[n/2];
    }
    
    std::vector<double> abs_dev(n);
    for (size_t i = 0; i < n; ++i) {
        abs_dev[i] = std::abs(x[i] - median);
    }
    
    std::sort(abs_dev.begin(), abs_dev.end());
    double mad_raw;
    if (n % 2 == 0) {
        mad_raw = (abs_dev[n/2 - 1] + abs_dev[n/2]) / 2.0;
    } else {
        mad_raw = abs_dev[n/2];
    }
    
    return mad_raw * K_MAD;
}

/**
 * Interquartile Range (IQR)
 */
inline double iqr(const std::vector<double>& x) {
    size_t n = x.size();
    if (n < 2) return 0.0;
    
    std::vector<double> temp = x;
    std::sort(temp.begin(), temp.end());
    
    // Type 7 quantile (R default)
    auto quantile = [&](double p) {
        double h = (n - 1.0) * p + 1.0;
        int lhs = std::floor(h);
        int rhs = std::ceil(h);
        if (lhs == rhs) return temp[lhs - 1];
        return temp[lhs - 1] + (h - lhs) * (temp[rhs - 1] - temp[lhs - 1]);
    };
    
    double q1 = quantile(0.25);
    double q3 = quantile(0.75);
    
    return (q3 - q1) * K_IQR;
}

/**
 * Unbiased Standard Deviation
 */
inline double sd_unbiased(const std::vector<double>& x) {
    size_t n = x.size();
    if (n < 2) return 0.0;
    
    double sum = 0.0;
    double sum_sq = 0.0;
    for (double val : x) {
        sum += val;
        sum_sq += val * val;
    }
    
    double mean = sum / n;
    double var = (sum_sq - n * mean * mean) / (n - 1.0);
    double sd = std::sqrt(std::max(0.0, var));
    
    return sd / c4(n);
}

} // namespace robustscale

#endif
