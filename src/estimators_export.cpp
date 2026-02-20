#include <Rcpp.h>
#include "estimators.h"
#include "sn_qn.h"

using namespace Rcpp;

// [[Rcpp::export]]
double cpp_scale_gmd(NumericVector x) {
    std::vector<double> xv = as<std::vector<double>>(x);
    return robustscale::gmd(xv);
}

// [[Rcpp::export]]
double cpp_scale_mad(NumericVector x) {
    std::vector<double> xv = as<std::vector<double>>(x);
    return robustscale::mad(xv);
}

// [[Rcpp::export]]
double cpp_scale_iqr(NumericVector x) {
    std::vector<double> xv = as<std::vector<double>>(x);
    return robustscale::iqr(xv);
}

// [[Rcpp::export]]
double cpp_scale_sn(NumericVector x) {
    std::vector<double> xv = as<std::vector<double>>(x);
    return robustscale::sn(xv);
}

// [[Rcpp::export]]
double cpp_scale_qn(NumericVector x) {
    std::vector<double> xv = as<std::vector<double>>(x);
    return robustscale::qn(xv);
}

// [[Rcpp::export]]
double cpp_scale_sd_c4(NumericVector x) {
    std::vector<double> xv = as<std::vector<double>>(x);
    return robustscale::sd_unbiased(xv);
}
