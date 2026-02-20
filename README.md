# `robustscale`: Robust Ensemble Scale Estimation for Predictive Uncertainty

## Overview

The `robustscale` R package implements a high-performance, multithreaded variance-weighted ensemble of robust scale estimators. It is specifically designed for robust uncertainty quantification in small-sample regimes (n < 50), a common challenge in applied econometrics and causal machine learning, particularly when dealing with expensive or limited bootstrap replicates.

This package operationalizes the "Statistical Insurance" framework developed in recent research, providing an intelligent dispatcher (`scale_robust`) that automatically selects the most efficient estimator based on sample size:
- For ultra-small samples (n < 20), it leverages a multithreaded, C++ optimized Weighted Ensemble of various robust estimators.
- For samples of n >= 20, it defaults to the highly efficient Gini Mean Difference (GMD), which our research shows provides statistical parity or superior performance with significant computational savings.

All core statistical functions are implemented in optimized C++ (via Rcpp) to ensure maximum speed and minimal external dependencies.

## Installation

The `robustscale` package can be installed directly from GitHub using the `remotes` package.

### Prerequisites

*   **R**: Ensure you have R installed (version 4.0 or higher is recommended).
*   **Rtools (Windows)**: If you are on Windows, you need to install [Rtools](https://cran.r-project.org/bin/windows/Rtools/).
*   **Xcode Command Line Tools (macOS)**: If you are on macOS, you need to install [Xcode Command Line Tools](https://mac.install.guide/commandlinetools/index.html).

### Install `remotes` (if you don't have it)

```R
if (!requireNamespace("remotes", quietly = TRUE)) {
  install.packages("remotes", repos = "https://cloud.r-project.org")
}
```

### Install `robustscale`

```R
remotes::install_github("davdittrich/robustscale")
```

## Usage Example

```R
library(robustscale)

# Example data
set.seed(123)
x_small <- c(rnorm(8, 0, 1), 10) # Small sample with an outlier
x_large <- rnorm(25, 0, 1)      # Larger sample, clean

# Automatic selection based on sample size (default behavior)
scale_robust(x_small) # Should use the Weighted Ensemble (n < 20)
scale_robust(x_large) # Should use GMD (n >= 20)

# Force a specific method
scale_robust(x_small, method = "gmd", auto_switch = FALSE) # Force GMD
scale_robust(x_large, method = "ensemble", auto_switch = FALSE) # Force Ensemble
scale_robust(x_small, method = "sd") # Simple unbiased SD

# Access individual estimators directly (for advanced use)
# cpp_scale_gmd(x_small)
# cpp_scale_sn(x_small)
# ... and so on
```

## Authors

*   Dennis Alexis Valin Dittrich
       *   ORCID: [0000-0002-4438-8276](https://orcid.org/0000-0002-4438-8276)

---
For more details, refer to the package documentation or the accompanying research note.
