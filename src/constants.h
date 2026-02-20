#ifndef ROBUSTSCALE_CONSTANTS_H
#define ROBUSTSCALE_CONSTANTS_H

#include <Rcpp.h>
#include <cmath>

namespace robustscale {

/**
 * c4(n) consistency constant for Unbiased Standard Deviation
 * Formula: sqrt(2/(n-1)) * Gamma(n/2) / Gamma((n-1)/2)
 */
inline double c4(int n) {
    if (n < 2) return 1.0;
    double log_c4 = 0.5 * std::log(2.0 / (n - 1.0)) + 
                    std::lgamma(n / 2.0) - 
                    std::lgamma((n - 1.0) / 2.0);
    return std::exp(log_c4);
}

/**
 * GMD consistency constant for Gaussian data: sqrt(pi)/2
 */
const double K_GMD = 0.886226925452758;

/**
 * MAD consistency constant for Gaussian data: 1 / Phi^-1(0.75)
 */
const double K_MAD = 1.482602218505602;

/**
 * IQR consistency constant for Gaussian data: 1 / (Phi^-1(0.75) - Phi^-1(0.25))
 */
const double K_IQR = 0.741301109252801;

} // namespace robustscale

#endif
