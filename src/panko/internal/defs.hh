// SPDX-License-Identifier: BSD-3-Clause
/* defs.hh - Internal definitions for API visibility and warning silencing */
#pragma once
#if !defined(PANKO_INTERNAL_DEFS_HH)
#define PANKO_INTERNAL_DEFS_HH

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

#if defined(_MSC_VER) || defined(__MINGW64__) || defined(__MINGW32__)
#	if !defined(_WIN32)
#		define _WIN32 1
#	endif
#endif

#if defined(_MSC_VER)
#	if defined(PANKO_BUILD_INTERNAL)
#		define PANKO_CLS_API __declspec(dllexport)
#	else
#		define PANKO_CLS_API __declspec(dllimport)
#	endif
#	define PANKO_API extern PANKO_CLS_API
#	define PANKO_CLS_MAYBE_API
#else
#	define PANKO_CLS_API __attribute__((visibility("default")))
#	define PANKO_CLS_MAYBE_API PANKO_CLS_API
#	define PANKO_API extern PANKO_CLS_API
#endif

/* Allows us to defined pragmas via a macro */
#if defined(__GNUC__) || defined(__clang__)
#	define PANKO_PRAGMA_(p) _Pragma(#p)
#	define PANKO_PRAGMA(p) PANKO_PRAGMA_(p)
#else
# 	define PANKO_PRAGMA(p)
#endif

/* This is here we can squash warnings from external libraries */
/* as our warning policy is quite verbose, for a good reason too. */
#if defined(__GNUG__) && !defined(__clang__)
#	define PANKO_DIAGNOSTICS_PUSH() PANKO_PRAGMA(GCC diagnostic push)
#	define PANKO_DIAGNOSTICS_POP()  PANKO_PRAGMA(GCC diagnostic pop)
#	define PANKO_DIAGNOSTICS_IGNORE(DIAG_NAME) PANKO_PRAGMA(GCC diagnostic ignored DIAG_NAME)
#	define PANKO_POISON(IDENT) PANKO_PRAGMA(GCC poison IDENT)
#elif defined(__clang__)
#	define PANKO_DIAGNOSTICS_PUSH()     \
	PANKO_PRAGMA(clang diagnostic push) \
	PANKO_DIAGNOSTICS_IGNORE("-Wunknown-warning-option")
#	define PANKO_DIAGNOSTICS_POP()  PANKO_PRAGMA(clang diagnostic pop)
#	define PANKO_DIAGNOSTICS_IGNORE(DIAG_NAME) PANKO_PRAGMA(clang diagnostic ignored DIAG_NAME)
#	define PANKO_POISON(IDENT) PANKO_PRAGMA(clang poison IDENT)
#else
#	define PANKO_DIAGNOSTICS_PUSH()
#	define PANKO_DIAGNOSTICS_POP()
#	define PANKO_DIAGNOSTICS_IGNORE(DIAG_NAME)
#	define PANKO_POISON(IDENT)
#endif

// NOLINTEND(cppcoreguidelines-macro-usage)

#endif /* PANKO_INTERNAL_DEFS_HH */
