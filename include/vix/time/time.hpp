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
 * =====================================================
 * Vix.cpp — Time Module (Umbrella Header)
 * =====================================================
 *
 * This header provides the public entry point for the
 * Vix time module.
 *
 * It exposes a simple, explicit API for working with:
 *   - Duration   (time spans)
 *   - Timestamp  (epoch-based time)
 *   - Clock      (system / steady clocks)
 *   - Date       (ergonomic facade, Node/Python-like)
 *   - DateTime   (date + time)
 *
 * Users are encouraged to include this header instead
 * of individual components when possible:
 *
 *   #include <vix/time/time.hpp>
 *
 */

#ifndef VIX_TIME_TIME_HPP
#define VIX_TIME_TIME_HPP

// Fundamental time primitives
#include <vix/time/Duration.hpp>
#include <vix/time/Timestamp.hpp>
#include <vix/time/Clock.hpp>

// High-level date abstractions
#include <vix/time/Date.hpp>
#include <vix/time/DateTime.hpp>

// Parsing helpers (ISO-8601, yyyy-mm-dd, etc.)
#include <vix/time/Parse.hpp>

#endif // VIX_TIME_TIME_HPP
