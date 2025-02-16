// SPDX-License-Identifier: BSD-3-Clause
/* types.cc - core types test harness */

#include <type_traits>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

namespace test {
	struct AK {
	private:
		[[maybe_unused]]
		int foo;
		[[maybe_unused]]
		int bar;
	public:
		constexpr AK(std::nullptr_t) noexcept : foo{}, bar{} { }
	};
}

#include "panko/core/types.hh"

using Panko::core::types::make_array;

TEST_CASE("make_array") {
	const auto test{make_array({
		0x00, 0x01, 0x02, 0x03
	})};

	CHECK(test.size() == 4);
}

using Panko::core::types::has_nullable_ctor_v;

TEST_CASE("has_nullable_ctor") {
	constexpr auto is_pod{std::is_standard_layout_v<test::AK> && std::is_trivial_v<test::AK>};
	CHECK_FALSE(is_pod);
	CHECK(has_nullable_ctor_v<test::AK>);
}
