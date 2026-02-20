#include <Rcpp.h>
#include <vector>
#include <numeric>
#include "estimators.h"
#include "sn_qn.h"

using namespace Rcpp;

/**
 * Thread-safe simple PRNG for resampling
 */
struct XorShift {
    uint32_t state;
    XorShift(uint32_t seed) : state(seed) {}
    uint32_t next() {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return state;
    }
};

// [[Rcpp::export]]
double cpp_scale_ensemble(NumericVector x, int n_boot) {
    size_t n = x.size();
    if (n < 2) return NA_REAL;
    
    NumericMatrix boot_results(n_boot, 6);
    
    // Fallback to single-threaded if RcppParallel has issues
    for (int r = 0; r < n_boot; ++r) {
        std::vector<double> resample(n);
        XorShift rng(static_cast<uint32_t>(r + 12345));
        for (size_t i = 0; i < n; ++i) {
            resample[i] = x[rng.next() % n];
        }
        
        boot_results(r, 0) = robustscale::sd_unbiased(resample);
        boot_results(r, 1) = robustscale::gmd(resample);
        boot_results(r, 2) = robustscale::mad(resample);
        boot_results(r, 3) = robustscale::iqr(resample);
        boot_results(r, 4) = robustscale::sn(resample);
        boot_results(r, 5) = robustscale::qn(resample);
    }
    
    std::vector<double> vars(6, 0.0);
    std::vector<double> means(6, 0.0);
    
    for (int j = 0; j < 6; ++j) {
        double sum = 0.0;
        for (int i = 0; i < n_boot; ++i) sum += boot_results(i, j);
        means[j] = sum / n_boot;
        
        double sum_sq = 0.0;
        for (int i = 0; i < n_boot; ++i) {
            double diff = boot_results(i, j) - means[j];
            sum_sq += diff * diff;
        }
        vars[j] = sum_sq / (n_boot - 1.0);
    }
    
    double weight_sum = 0.0;
    std::vector<double> inv_vars(6);
    for (int j = 0; j < 6; ++j) {
        inv_vars[j] = 1.0 / std::max(1e-10, vars[j]);
        weight_sum += inv_vars[j];
    }
    
    std::vector<double> xv = as<std::vector<double>>(x);
    std::vector<double> estimates(6);
    estimates[0] = robustscale::sd_unbiased(xv);
    estimates[1] = robustscale::gmd(xv);
    estimates[2] = robustscale::mad(xv);
    estimates[3] = robustscale::iqr(xv);
    estimates[4] = robustscale::sn(xv);
    estimates[5] = robustscale::qn(xv);
    
    double final_est = 0.0;
    for (int j = 0; j < 6; ++j) {
        final_est += (inv_vars[j] / weight_sum) * estimates[j];
    }
    
    return final_est;
}
