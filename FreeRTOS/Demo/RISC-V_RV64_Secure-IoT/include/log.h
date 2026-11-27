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
 * @file log.h
 * @brief  Contains the API for log statment.
 * @details Provides the API for log statement.
 * @version 1.1
 * @authors Kapil Shyam. M (kapil@mindgrovetech.in)
 *          Harini Sree.S (harini@mindgrovetech.in)
 * @date 11-05-2026
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description
 * -----------|---------|-----------------------|-------------------------------
 * 13-09-2024 | 1.0     | Kapil Shyam. M        | Initial release.
 * 11-05-2026 | 1.1     | Harini Sree. S        | Updated it to be
 *            |         |                       | MISRA-compliant.
 * -----------------------------------------------------------------------------
 */

#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @enum: LogLevel
 * @brief: Describes the Log Level Macros
 */
typedef enum {
	FATAL = 0, // Ordered by severity
	ERROR = 1,
	WARN = 2,
	INFO = 3,
	TRACE = 4,
	DEBUG = 5
} LogLevel;

/**
 * @defgroup LOG_LEVEL Configuration Macros
 *
 * @brief Macros used to configure the severity of the log statment.
 *
 * @{
 */
/** @brief Conditional Default level. */
#ifndef LOG_LEVEL
#define LOG_LEVEL WARN
#endif

/** @brief Conditional log emission macro.
 *  @details Evaluates the specified log level against the current global
 *  log_level and invokes log_emit_function() only if the message severity is
 *  enabled. This helps in filtering log output at runtime without additional
 *  function overhead.
 *  @param level Logging severity level of the message.
 *  @param fmt   Format string (similar to printf-style formatting).
 *  @param ...   Optional arguments corresponding to the format string.
 */
#define log_emit(level, fmt, ...)                         \
	do {                                                  \
		if ((level) <= log_level) {                       \
			log_emit_function(level, fmt, ##__VA_ARGS__); \
		}                                                 \
	} while (0)

/**
 * @brief Global log level control variable.
 * @details Determines the current logging verbosity used across the system.
 *          The value of this variable can be modified at runtime to adjust
 *          the level of log messages being generated.
 * @note The default value is set during initialization in log.c.
 *       It can be overridden at runtime (from main.c) by assigning
 *       a new value to log_level.
 */
extern volatile LogLevel log_level;

/**
 * @brief General function to emit logs based on log level.
 * @details This function emits logs based on the provided log level.
 *          It will only print logs if the provided level is equal to or
 *          more severe than the current `log_level`.
 * @param level Log level (TRACE, DEBUG, INFO, WARN, ERROR, FATAL)
 * @param fmt   Format string for printf-style arguments
 * @return none
 */
void log_emit_function(LogLevel level, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif // LOG_H
