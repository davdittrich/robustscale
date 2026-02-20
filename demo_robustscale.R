# Comprehensive Demonstration of `robustscale`

library(robustscale)
library(tidyverse)
library(microbenchmark)

# 1. Performance vs Sample Size (Distribution: Contaminated Normal)
# This simulates the "Fit Outlier" scenario discussed in the paper
set.seed(2026)
n <- 10
true_sigma <- sqrt(0.95*1^2 + 0.05*10^2) # ~2.34

simulate_performance <- function(reps = 1000) {
  results <- map_dfr(1:reps, ~{
    # Generate contaminated sample (95% clean, 5% extreme outliers)
    x <- c(rnorm(round(n*0.95), 0, 1), rnorm(n - round(n*0.95), 0, 10))
    
    tibble(
      Standard_SD = sd(x), # Uncorrected SD
      Unbiased_SD = scale_robust(x, method = "sd"),
      GMD = scale_robust(x, method = "gmd"),
      Robust_Ensemble = scale_robust(x, method = "ensemble")
    )
  })
  
  results %>%
    pivot_longer(everything(), names_to = "Method", values_to = "Estimate") %>%
    group_by(Method) %>%
    summarise(
      Mean_Estimate = mean(Estimate),
      Bias = mean(Estimate - true_sigma),
      MSE = mean((Estimate - true_sigma)^2),
      Rel_Improvement_vs_SD = (mean((results$Standard_SD - true_sigma)^2) - MSE) / mean((results$Standard_SD - true_sigma)^2) * 100
    )
}

cat("
--- Simulation Results (n=10, Contaminated Normal) ---
")
print(simulate_performance())

# 2. Speed Benchmarking
cat("
--- Speed Benchmark (n=10, 200 internal bootstrap replicates) ---
")
x_bench <- rnorm(10)
print(microbenchmark(
  base_sd = sd(x_bench),
  robust_ensemble = scale_robust(x_bench, method = "ensemble"),
  times = 100
))

# 3. Automatic Switch Demonstration
cat("
--- Automatic Switch Logic ---
")
x_small <- rnorm(10)
x_large <- rnorm(25)

# This should call ensemble (multithreaded)
cat("n=10: Using ensemble... ")
tic <- Sys.time()
scale_robust(x_small, auto_switch = TRUE)
cat("Done. Time:", Sys.time() - tic, "
")

# This should call GMD (instant)
cat("n=25: Using auto-switch to GMD... ")
tic <- Sys.time()
scale_robust(x_large, auto_switch = TRUE)
cat("Done. Time:", Sys.time() - tic, "
")
