 /**
 * SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (c) 2021-2026 Mindgrove Technologies. All rights reserved.
 * 
 * @license Licensed under the Apache License, Version 2.0 (see LICENSE).
 * @licenseblock
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * @endlicenseblock
 * 
 * Project                   : MGS2401 SoC
 * @file math_functions.h
 * @brief Header file for mathematical functions
 * @details File contains funtion declarations to use reusable mathematical operations. 
 * @version 1.0
 * @authors Kapil Shyam. M (kapil@mindgrovetech.in)
 * @date 21-09-2024 
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 21-09-2024 | 1.0     | Kapil Shyam. M        | Initial release.              
 * -----------------------------------------------------------------------------
 */

#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Computes the exponential function of a given value
 * @details Calculates the base-e exponential function of x (ex), which is the mathematical constant e raised to the power of x.
 * @param double x - the exponent value
 * @return double - the calculated value of e raised to the power of x
*/
double exponents(double x);

/**
 * @brief Computes the natural logarithm of a given value
 * @details Calculates the base-e logarithm (ln(x)) of the passed argument x. The value of x must be greater than zero.
 * @param double x - the value whose natural logarithm is to be calculated
 * @return double - the natural logarithm of x, or a domain error if x is negative
*/
double logrithm(double x);

/**
 * @brief Breaks a floating-point number into fractional and integral parts
 * @details Splits the given double value into its constituent fractional and integer components. The integer component is stored as a double value in the memory location pointed to by intpart, while the fractional component is returned.
 * @param double value - the floating-point value to be decomposed
 * @param double *intpart - pointer to the destination where the integer portion will be stored
 * @return double - the signed fractional portion of the decomposed value
 *
*/
double modfloat(double value, double *intpart);

/**
 * @brief Computes a base value raised to a specified power
 * @details Calculates the result of raising the base parameter to the power of the exponent parameter (baseexponent).
 * @param double base - the base value to be multiplied
 * @param double exponent - the power value to which the base is raised
 * @return double - the calculated result of the base raised to the specified exponent
*/
double powers(double base, double exponent);

/**
 * @brief Computes the square root of a given value
 * @details Calculates the non-negative square root (x​) of the provided input value. The input argument must be non-negative.
 * @param double value - the floating-point value whose square root is to be calculated
 * @return double - the square root of the value, or a domain error if the value is negative
*/
double squareroot(double value);

/**
 * @brief Computes the cube root of a given value
 * @details Calculates the exact cube root (3x​) of the provided input value. Unlike square root, this function accepts both positive and negative values.
 * @param double value - the floating-point value whose cube root is to be calculated
 * @return double - the calculated cube root of the value
*/
double cuberoot(double value);

#ifdef __cplusplus
}
#endif
#endif