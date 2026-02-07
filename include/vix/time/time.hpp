/**
 * @file time.hpp
 * @author Gaspard Kirira
 *
 * Copyright 2026, Gaspard Kirira.
 * All rights reserved.
 * https://github.com/vixcpp/time
 *
 * Use of this source code is governed by a MIT license
 * that can be found in the LICENSE file.
 *
 * Vix.cpp
 */

#ifndef VIX_TIME_TIME_HPP
#define VIX_TIME_TIME_HPP

/**
 * @brief Umbrella header for the Vix time module.
 *
 * @details
 * This header is the public entry point for time-related utilities in Vix.cpp.
 *
 * It provides a simple and explicit API for working with:
 * - @ref vix::time::Duration   (time spans)
 * - @ref vix::time::Timestamp  (absolute, epoch-based time)
 * - @ref vix::time::SystemClock and @ref vix::time::SteadyClock
 * - @ref vix::time::Date       (calendar date facade)
 * - @ref vix::time::DateTime   (calendar date + time)
 *
 * Users are encouraged to include this header instead of individual
 * components when possible:
 *
 * @code
 * #include <vix/time/time.hpp>
 * @endcode
 *
 * This keeps includes stable while allowing the implementation
 * to evolve internally.
 */

// Fundamental time primitives
#include <vix/time/Duration.hpp>
#include <vix/time/Timestamp.hpp>
#include <vix/time/Clock.hpp>

// High-level date abstractions
#include <vix/time/Date.hpp>
#include <vix/time/DateTime.hpp>

// Lightweight parsing helpers
#include <vix/time/Parse.hpp>

#endif // VIX_TIME_TIME_HPP
