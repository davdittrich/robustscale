#include <Rcpp.h>
#include "constants.h"

using namespace Rcpp;

// [[Rcpp::export]]
double get_consistency_constant(int n, std::string method) {
    if (method == "c4") {
        return robustscale::c4(n);
    } else if (method == "gmd") {
        return robustscale::K_GMD;
    } else if (method == "mad") {
        return robustscale::K_MAD;
    } else if (method == "iqr") {
        return robustscale::K_IQR;
    }
    
    Rcpp::stop("Unknown method for consistency constant");
    return 1.0;
}
