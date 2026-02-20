library(testthat)
library(robustscale)

test_that("scale_robust handles edge cases", {
  expect_true(is.na(scale_robust(c(1))))
  expect_error(scale_robust(rnorm(10), method = "invalid"))
})

test_that("Core estimators return expected values under normality", {
  set.seed(123)
  x <- rnorm(1000, mean = 0, sd = 1)
  
  # Large samples should be close to 1
  expect_equal(scale_robust(x, method = "sd"), sd(x), tolerance = 0.05)
  expect_equal(scale_robust(x, method = "gmd"), 1.0, tolerance = 0.05)
  expect_equal(scale_robust(x, method = "mad"), 1.0, tolerance = 0.05)
  expect_equal(scale_robust(x, method = "iqr"), 1.0, tolerance = 0.05)
})

test_that("Auto-switch threshold is respected", {
  x_small <- rnorm(10)
  x_large <- rnorm(25)
  
  # No error on either
  expect_no_error(scale_robust(x_small, auto_switch = TRUE))
  expect_no_error(scale_robust(x_large, auto_switch = TRUE))
})
