// SPDX-License-Identifier: BSD-3-Clause
/* frontend.cc - Some helpers for the CLI/GUI frontends, mostly for info printing */

#include <string_view>

#include <pybind11/embed.h>

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include "panko/internal/frontend.hh"
#include "panko/config.hh"


namespace Panko::internal {
	namespace cfg = Panko::config;

	std::string_view python_version() noexcept {
		return { Py_GetVersion() };
	}

	std::string_view lua_version() noexcept {
		return { LUA_VERSION_MAJOR "." LUA_VERSION_MINOR "." LUA_VERSION_RELEASE };
	}
}
