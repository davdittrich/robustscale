#' Robust Ensemble Scale Estimation
#'
#' Estimates the population standard deviation using an ensemble of robust 
#' estimators or the Gini Mean Difference, following the findings of 
#' research on "Statistical Insurance for the Cheap Bootstrap".
#'
#' @param x A numeric vector of observations.
#' @param method The fallback or primary method if auto_switch is FALSE. 
#'   Options: "ensemble", "gmd", "sd", "mad", "iqr", "sn", "qn".
#' @param auto_switch Logical. If TRUE, automatically uses GMD for n >= threshold.
#' @param threshold Integer. Sample size at which the switch to GMD occurs.
#' @param n_boot Integer. Number of bootstrap replicates for the ensemble weighting.
#'
#' @return A numeric scale estimate.
#' @export
scale_robust <- function(x, 
                         method = "ensemble", 
                         auto_switch = TRUE, 
                         threshold = 20, 
                         n_boot = 200) {
  
  n <- length(x)
  if (n < 2) return(NA_real_)
  
  # 1. Automatic Switch Logic (Research-backed default n=20)
  if (auto_switch && n >= threshold) {
    return(cpp_scale_gmd(x))
  }
  
  # 2. Method Dispatch
  method <- match.arg(method, c("ensemble", "gmd", "sd", "mad", "iqr", "sn", "qn"))
  
  switch(method,
    ensemble = cpp_scale_ensemble(x, n_boot),
    gmd      = cpp_scale_gmd(x),
    sd       = cpp_scale_sd_c4(x),
    mad      = cpp_scale_mad(x),
    iqr      = cpp_scale_iqr(x),
    sn       = cpp_scale_sn(x),
    qn       = cpp_scale_qn(x)
  )
}
