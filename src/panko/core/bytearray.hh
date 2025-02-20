// SPDX-License-Identifier: BSD-3-Clause
/* bytearray.hh - Flexble and subdividable `bytearray_t`  */

#pragma once
#if !defined(PANKO_CORE_BYTEARRAY_HH)
#define PANKO_CORE_BYTEARRAY_HH

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <expected>
#include <limits>
#include <memory>
#include <optional>
#include <span>
#include <stdexcept>
#include <stdfloat>
#include <string_view>
#include <vector>

#include "panko/config.hh"
#include "panko/internal/defs.hh"
#include "panko/core/types.hh"
#include "panko/core/errcodes.hh"
#include "panko/core/integers.hh"

namespace Panko::core {
	using Panko::core::error_codes::strdec_error_t;
	using Panko::core::error_codes::decomp_error_t;

	/*! \struct Panko::core::bytearray_t
		\brief Flexible raw byte buffer

		All of the packet data within Panko is represented as a `bytearray_t`,
		you can think of it a lot like Wiresharks TVBs and C++'s std::spans.


	*/
	struct bytearray_t final {
		using byte_t    = std::byte;
		using storage_t = std::optional<std::shared_ptr<byte_t>>;
	private:
		storage_t _backing_storage{std::nullopt};
		std::span<byte_t> _backing_span;
		std::size_t _backing_offset{};
		std::size_t _index_offset{};

		template<typename T>
		[[nodiscard]]
		std::enable_if_t<
			std::is_standard_layout_v<T> && std::is_trivial_v<T> && !std::is_same_v<T, void*>, T*
		>
		index(const std::size_t idx) const {
			if (idx <= _backing_span.size()) {
				auto* const addr{_backing_span.data()};
				return new (addr + (idx * sizeof(T))) T{};
			}
			throw std::out_of_range("bytearray access out of range (1)");
		}

		/* TODO(aki): Replace array with span */
		template<std::size_t N>
		void index(const std::size_t idx, std::array<byte_t, N>& data) {
			if ((idx + N) <= _backing_span.size()) {
				for (std::size_t off{}; off < N; ++off) {
					data[0] = *index<byte_t>(idx + off);
				}
			}
			throw std::out_of_range("bytearray access out of range (0)");
		}

		template<typename T, std::size_t len>
		[[nodiscard]]
		T from_bytes_be(std::array<byte_t, len>& data) {
			using ptype = std::make_unsigned_t<Panko::core::types::promoted_type_t<T>>;
			static_assert(len != 0zu && len <= 8zu, "Length of T must be between 1 and 8 bytes");

			if constexpr (len == 1zu) {
				return static_cast<T>(data[0]);
			}

			if constexpr (len == 2zu) {
				return static_cast<T>(
					(ptype(data[0]) << 8U) | ptype(data[1])
				);
			}

			if constexpr (len == 3zu) {
				return static_cast<T>(
					(ptype(data[0]) << 16U) | (ptype(data[1]) << 8U) |
					ptype(data[2])
				);
			}

			if constexpr (len == 4zu) {
				return static_cast<T>(
					(ptype(data[0]) << 24U) | (ptype(data[1]) << 16U) |
					(ptype(data[2]) <<  8U) | ptype(data[3])
				);
			}

			if constexpr (len == 5zu) {
				return static_cast<T>(
					(ptype(data[0]) << 32U) | (ptype(data[1]) << 24U) |
					(ptype(data[2]) << 16U) | (ptype(data[3]) <<  8U) |
					ptype(data[4])
				);
			}

			if constexpr (len == 6zu) {
				return static_cast<T>(
					(ptype(data[0]) << 40U) | (ptype(data[1]) << 32U) |
					(ptype(data[2]) << 24U) | (ptype(data[3]) << 16U) |
					(ptype(data[4]) <<  8U) | ptype(data[5])
				);
			}

			if constexpr (len == 7zu) {
				return static_cast<T>(
					(ptype(data[0]) << 48U) | (ptype(data[1]) << 40U) |
					(ptype(data[2]) << 32U) | (ptype(data[3]) << 24U) |
					(ptype(data[4]) << 16U) | (ptype(data[5]) <<  8U) |
					ptype(data[6])
				);
			}

			if constexpr (len == 8zu) {
				return static_cast<T>(
					(ptype(data[0]) << 56U) | (ptype(data[1]) << 48U) |
					(ptype(data[2]) << 40U) | (ptype(data[3]) << 32U) |
					(ptype(data[4]) << 24U) | (ptype(data[5]) << 16U) |
					(ptype(data[6]) <<  8U) | ptype(data[7])
				);
			}
		}
		template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
		[[nodiscard]]
		T from_bytes_be(std::array<byte_t, sizeof(T)>& data) {
			const auto raw{[&](){
				if constexpr (std::is_same_v<T, std::float16_t>) {
					return from_byte_be<std::uint16_t>(data);
				}

				if constexpr (std::is_same_v<T, std::float32_t>) {
					return from_byte_be<std::uint32_t>(data);

				}
				if constexpr (std::is_same_v<T, std::float64_t>) {
					return from_byte_be<std::uint64_t>(data);
				}
			}()};
			T _data{};
			std::memcpy(&_data, &raw, sizeof(raw));
			return _data;
		}

		template<typename T, std::size_t len>
		[[nodiscard]]
		T from_bytes_le(std::array<byte_t, len>& data) {
			using ptype = std::make_unsigned_t<Panko::core::types::promoted_type_t<T>>;

			static_assert(len != 0zu && len <= 8zu, "Length of T must be between 1 and 8 bytes");

			if constexpr (len == 1zu) {
				return static_cast<T>(data[0]);
			}

			if constexpr (len == 2zu) {
				return static_cast<T>(
					(ptype(data[1]) << 8U) | ptype(data[0])
				);
			}

			if constexpr (len == 3zu) {
				return static_cast<T>(
					(ptype(data[2]) << 16U) | (ptype(data[1]) << 8U) |
					ptype(data[0])
				);
			}

			if constexpr (len == 4zu) {
				return static_cast<T>(
					(ptype(data[3]) << 24U) | (ptype(data[2]) << 16U) |
					(ptype(data[1]) <<  8U) | ptype(data[0])
				);
			}

			if constexpr (len == 5zu) {
				return static_cast<T>(
					(ptype(data[4]) << 32U) | (ptype(data[3]) << 24U) |
					(ptype(data[2]) << 16U) | (ptype(data[1]) <<  8U) |
					ptype(data[0])
				);
			}

			if constexpr (len == 6zu) {
				return static_cast<T>(
					(ptype(data[5]) << 40U) | (ptype(data[4]) << 32U) |
					(ptype(data[3]) << 24U) | (ptype(data[2]) << 16U) |
					(ptype(data[1]) <<  8U) | ptype(data[0])
				);
			}

			if constexpr (len == 7zu) {
				return static_cast<T>(
					(ptype(data[6]) << 48U) | (ptype(data[5]) << 40U) |
					(ptype(data[4]) << 32U) | (ptype(data[3]) << 24U) |
					(ptype(data[2]) << 16U) | (ptype(data[1]) <<  8U) |
					ptype(data[0])
				);
			}

			if constexpr (len == 8zu) {
				return static_cast<T>(
					(ptype(data[7]) << 56U) | (ptype(data[6]) << 48U) |
					(ptype(data[5]) << 40U) | (ptype(data[4]) << 32U) |
					(ptype(data[3]) << 24U) | (ptype(data[2]) << 16U) |
					(ptype(data[1]) <<  8U) | ptype(data[0])
				);
			}
		}

		template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
		[[nodiscard]]
		T from_bytes_le(std::array<byte_t, sizeof(T)>& data) {
			const auto raw{[&](){
				if constexpr (std::is_same_v<T, std::float16_t>) {
					return from_byte_le<std::uint16_t>(data);
				}

				if constexpr (std::is_same_v<T, std::float32_t>) {
					return from_byte_le<std::uint32_t>(data);

				}
				if constexpr (std::is_same_v<T, std::float64_t>) {
					return from_byte_le<std::uint64_t>(data);
				}
			}()};
			T _data{};
			std::memcpy(&_data, &raw, sizeof(raw));
			return _data;
		}

					/*! \brief Construct a sub-slice `bytearray_t`

			This constructor is only used with `operator[](idx, len)` and `slice(idx, len)` to get
			a sub-view into the `bytearray_t`.

			\param storage The backing storage for the parent `bytearray_t`.
			\param subspan The subspan slice into the backing storage.
			\param offset The offset into the parents backing storage this subslice is.
		*/
		bytearray_t(storage_t storage, std::span<byte_t> subspan, const std::size_t offset, std::nullptr_t) :
			_backing_storage{[&]() -> storage_t {
				if (storage.has_value()) {
					return std::make_optional(std::shared_ptr<byte_t>(*storage));
				}
				return std::nullopt;
			}()},
			_backing_span{subspan},
			_backing_offset{offset}
		{ }

	public:
		constexpr bytearray_t() noexcept = default;

		bytearray_t(const bytearray_t&) = delete;
		bytearray_t& operator=(const bytearray_t&) = delete;

		bytearray_t(bytearray_t&&) = delete;
		bytearray_t& operator=(bytearray_t&&) = delete;

		/*! \brief Construct a new, empty `bytearray_t`.

			\param size The size to allocate for this array.
		*/
		bytearray_t(const std::size_t size) :
			_backing_storage{{reinterpret_cast<byte_t*>(std::malloc(size)), std::free}},
			_backing_span{(*_backing_storage).get(), size}
		{
			/* we need to be good and initialize the memory */
			std::memset(_backing_storage->get(), 0, size);
		}

		/*! \brief Construct a `bytearray_t` view over a std::uint8_t buffer.

			\note This does **not** take ownership of `buff`!

			\param buff The buffer to overlay onto.
			\param len The length of the buffer.
		*/
		bytearray_t(std::uint8_t* const buff, const std::size_t len) :
			_backing_span{reinterpret_cast<byte_t*>(buff), len}
		{ }

		/*! \brief Construct a `bytearray_t` view over a std::byte buffer.

			\note This does **not** take ownership of `buff`!

			\param buff The buffer to overlay onto.
			\param len The length of the buffer.
		*/
		bytearray_t(byte_t* const buff, const std::size_t len) :
			_backing_span{buff, len}
		{ }

		/*! \brief Construct a `bytearray_t` view over a std::byte buffer from a std::shared_ptr.

			\note This method adds to the buffers reference count.

			\param buff The buffer to overlay onto.
			\param size The length of the buffer.
		*/
		bytearray_t(std::shared_ptr<byte_t>& buff, const std::size_t size) :
			_backing_storage{std::shared_ptr<byte_t>(buff)},
			_backing_span{(*_backing_storage).get(), size}
		{ }

		/*! \brief Construct a `bytearray_t` view over a std::byte buffer from a std::unique_ptr.

			\note This method takes ownership over the buffer.

			\param buff The buffer to overlay onto.
			\param size The length of the buffer.
		*/
		bytearray_t(std::unique_ptr<byte_t>& buff, const std::size_t size) :
			_backing_storage{std::shared_ptr<byte_t>(std::move(buff))},
			_backing_span{(*_backing_storage).get(), size}
		{ }

		/*! \brief Construct a `bytearray_t` view over an array of std::uint8_t.

			\note This will tie the valid lifetime of this `bytearray_t` to that of `array`!

			\param array The array to overlay this `bytearray_t` onto.
		*/
		template<std::size_t len>
		bytearray_t(std::array<std::uint8_t, len>& array) :
			_backing_span{reinterpret_cast<byte_t*>(array.data()), len}
		{ }

		/*! \brief Construct a `bytearray_t` view over an array of std::byte.

			\note This will tie the valid lifetime of this `bytearray_t` to that of `array`!

			\param array The array to overlay this `bytearray_t` onto.
		*/
		template<std::size_t len>
		bytearray_t(std::array<byte_t, len>& array) :
			_backing_span{array.data(), len}
		{ }

		/*! \brief Construct a `bytearray_t` view over a vector of std::uint8_t.

			\note This will tie the valid lifetime of this `bytearray_t` to that of `vec`!

			\param vec The vector to overlay this `bytearray_t` onto.
		*/
		bytearray_t(std::vector<std::uint8_t>& vec) :
			_backing_span{reinterpret_cast<byte_t*>(vec.data()), vec.size()}
		{ }

		/*! \brief Construct a `bytearray_t` view over a vector of std::byte.

			\note This will tie the valid lifetime of this `bytearray_t` to that of `vec`!

			\param vec The vector to overlay this `bytearray_t` onto.
		*/
		bytearray_t(std::vector<byte_t>& vec) :
			_backing_span{vec.data(), vec.size()}
		{ }

		/* == Forward/Reverse Iterators == */

		/*! \brief Returns an iterator to the first byte of the `bytearray_t`

			If the `bytearray_t` is empty this will be equivelent to `end()`.
		*/
		[[nodiscard]]
		constexpr auto begin() noexcept {
			return _backing_span.begin();
		}

		/*! \brief Returns an iterator to the first byte of the `bytearray_t`

			If the `bytearray_t` is empty this will be equivelent to `end()`.
		*/
		[[nodiscard]]
		constexpr auto begin() const noexcept {
			return _backing_span.begin();
		}

		/*! \brief Returns an iterator to the byte beyond the last byte of the `bytearray_t`

			This element is a sentinel value used for iteration, as such it is not valid.

			\warning Accessing this element as a real value will result in undefined behaviour!
		*/
		[[nodiscard]]
		constexpr auto end() noexcept {
			return _backing_span.end();
		}

		/*! \brief Returns an iterator to the byte beyond the last byte of the `bytearray_t`

			This element is a sentinel value used for iteration, as such it is not valid.

			\warning Accessing this element as a real value will result in undefined behaviour!
		*/
		[[nodiscard]]
		constexpr auto end() const noexcept {
			return _backing_span.end();
		}

		/*! \brief Returns a reverse iterator to the first byte of the reversed `bytearray_t`

			This iterator enables reverse iteration, the value of `rbegin()` is the last element
			of the `bytearray_t` unlike `end()` which is one past it.

			If the `bytearray_t` is empty this will be equivelent to `rend()`.
		*/
		[[nodiscard]]
		constexpr auto rbegin() noexcept {
			return _backing_span.rbegin();
		}

		/*! \brief Returns a reverse iterator to the first byte of the reversed `bytearray_t`

			This iterator enables reverse iteration, the value of `rbegin()` is the last element
			of the `bytearray_t` unlike `end()` which is one past it.

			If the `bytearray_t` is empty this will be equivelent to `rend()`.
		*/
		[[nodiscard]]
		constexpr auto rbegin() const noexcept {
			return _backing_span.rbegin();
		}

		/*! \brief Returns a reverse iterator to the byte beyond the last byte of the reversed `bytearray_t`

			This element is a sentinel value used for iteration, as such it is not valid, it is the element
			**before** the first valid byte of the `bytearray_t`.

			\warning Accessing this element as a real value will result in undefined behaviour!
		*/
		[[nodiscard]]
		constexpr auto rend() noexcept {
			return _backing_span.rend();
		}

		/*! \brief Returns a reverse iterator to the byte beyond the last byte of the reversed `bytearray_t`

			This element is a sentinel value used for iteration, as such it is not valid, it is the element
			**before** the first valid byte of the `bytearray_t`.

			\warning Accessing this element as a real value will result in undefined behaviour!
		*/
		[[nodiscard]]
		constexpr auto rend() const noexcept {
			return _backing_span.rend();
		}

		/* == Buffer Properties == */

		/*! \brief Returns the length of the `bytearray_t` in bytes.

		*/
		[[nodiscard]]
		constexpr auto length() const noexcept {
			return _backing_span.size_bytes();
		}

		/*! \brief Returns the offset of the `bytearray_t` into it's parent `bytearray_t`

		*/
		[[nodiscard]]
		constexpr auto offset() const noexcept {
			return _backing_offset;
		}

		[[nodiscard]]
		constexpr bool valid() const noexcept {
			return !_backing_span.empty();
		}

// 			template<typename T>
// 			[[nodiscard]]
// 			T& operator[](const std::size_t idx) {
// 				return *index<T>(idx);
// 			}
//
// 			template<typename T>
// 			[[nodiscard]]
// 			const T& operator[](const std::size_t idx) {
// 				return *index<const T>(idx);
// 			}

		/* == Buffer Slicing == */

		/*! \brief Obtain a sub-slice bytearray_t from this `bytearray_t`.

			This method constructs a sub-slice of this `bytearray_t`.

			\param start The index into the `bytearray_t` to start the slice.
			\param end The index into the `bytearray_t` to end the slice.
		*/
		[[nodiscard]]
		bytearray_t operator[](const std::size_t start, const std::size_t end) {
			return slice(start, end);
		}

		/*! \brief Obtain a sub-slice bytearray_t from this `bytearray_t`.

			This method constructs a sub-slice of this `bytearray_t`.

			\param start The index into the `bytearray_t` to start the slice.
			\param end The index into the `bytearray_t` to end the slice.
		*/
		[[nodiscard]]
		bytearray_t slice(const std::size_t start, const std::size_t end) {
			if (start > _backing_span.size() || end > _backing_span.size() || end < start) {
				throw std::out_of_range("bytearray access out of range");
			}
			const auto len{(end - start) + 1};
			return {_backing_storage, _backing_span.subspan(start, len), start, nullptr};
		}

		/* == Buffer conjoining == */
		/* TODO(aki): Join buffers */

		/* == Raw integer type access == */

		/*! \brief Extract an element at the given offset into the buffer with the specified endian.

			This method takes an absolute byte offset into the `bytearray_t` to start extracting the wanted
			type from the buffer.

			\code{.cc}
			auto val{arr.at<std::uint32_t, std::endian::big>(8uz)};
			\endcode

			\tparam T The type to extract.
			\tparam endian The endian of the type to extract.
			\param idx The offset into the `bytearray_t` to begin the extraction.
		*/
		template<typename T, std::endian endian>
		[[nodiscard]]
		T at(const std::size_t idx) {
			constexpr static auto type_size{[&](){
				using Panko::core::integers::is_intn_v;
				if constexpr (is_intn_v<T>) {
					return (T::bits + 7zu) / 8zu;
				} else {
					return sizeof(T);
				}
			}()};

			std::array<byte_t, type_size> data{};
			index(idx, data);

			if constexpr (endian == std::endian::little) {
				return from_bytes_le<T>(data);
			} else {
				return from_bytes_be<T>(data);
			}
		}

		/*! \brief Extract the next element of the specified type from the buffer.

			This method extracts the next N-byte wide type from the buffer and moves it's internal
			offset pointer for the next call to be directly after the extracted element.

			\code{.cc}
			// On first call to a buffer or subslice there of the offset is 0
			auto val{arr.next<std::uint32_t, std::endian::big>()};
			// On the next call, the internal offset is now sizeof(T) farther along,
			auto val2{arr.next<std::uint16_t, std::endian::big>()};
			\endcode

			\tparam T The type to extract.
			\tparam endian The endian of the type to extract.
		*/
		template<typename T, std::endian endian>
		[[nodiscard]]
		T next() {
			constexpr static auto type_size{[&](){
				using Panko::core::integers::is_intn_v;
				if constexpr (is_intn_v<T>) {
					return (T::bits + 7zu) / 8zu;
				} else {
					return sizeof(T);
				}
			}()};

			auto res{at<T, endian>(_index_offset)};

			_index_offset += type_size;

			return res;
		}

		/* == String Helpers == */

		/* 8-bit ASCII */

		/*! \brief Read a fixed length 8-bit ASCII encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as an 8-bit ASCII string.

			It will ingest control characters and null terminators up until the first byte out of the ASCII range
			is encountered or the target length is reached, whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii(const std::size_t idx, const std::size_t len);

		/*! \brief Read a fixed length null padded 8-bit ASCII encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as an 8-bit ASCII string.

			It will ingest control characters and null terminators up until the first byte out of the ASCII range
			is encountered or the target length is reached, whichever occurs first.

			This method is effectively the same as `string_ascii`.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii_zp(const std::size_t idx, const std::size_t len);

		/*! \brief Read a null-terminated 8-bit ASCII encoded string from the `bytearray_t`.

			This method will read up until a null terminator or `max_len` and decode it as an 8-bit ASCII string.

			It will ingest all control characters up until the first byte out of the ASCII range, if it's a
			null terminator, or we reach `max_len` if it non-zero, whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param max_len The maximum length of the string if the null terminator is not found, 0 for no limit.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii_zt(const std::size_t idx, const std::size_t max_len = 0zu);

		/*! \brief Read an 8-bit length prefixed 8-bit ASCII encoded string from the `bytearray_t`.

			This method will read the first 8 bits at the offset `idx` as an unsigned integer and use that as the
			number of ASCII characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			ASCII range or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭─────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN │ String Data ┊
			├─────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0     8            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii_lp8(const std::size_t idx);

		/*! \brief Read a 16-bit length prefixed 8-bit ASCII encoded string from the `bytearray_t`.

			This method will read the first 16 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number ASCII characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			ASCII range or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ String Data ┊
			├──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii_lp16(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 24-bit length prefixed 8-bit ASCII encoded string from the `bytearray_t`.

			This method will read the first 24 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number ASCII characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			ASCII range or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii_lp24(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 32-bit length prefixed 8-bit ASCII encoded string from the `bytearray_t`.

			This method will read the first 32 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number ASCII characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			ASCII range or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii_lp32(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 64-bit length prefixed 8-bit ASCII encoded string from the `bytearray_t`.

			This method will read the first 64 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number ASCII characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			ASCII range or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ LEN3 │ LEN4 │ LEN5 │ LEN6 │ String Data ┊
			├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32     40     48     56     64            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii_lp64(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a LEB128 length prefixed 8-bit ASCII encoded string from the `bytearray_t`.

			This method will read up to 5 bytes at the offset `idx` to construct an LEB128 byte stream, decode that
			as an unsigned integer and use that as the number of ASCII characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			ASCII range or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬┈┈┈┈┈┈┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ .... │ LENN │ String Data ┊
			├──────┼┈┈┈┈┈┈┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8      N     N+8           LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii_leb128(const std::size_t idx);

		/* 7-bit ASCII */

		/*! \brief Read a fixed length 7-bit ASCII encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as 7-bit ASCII string.

			It will ingest control characters and null terminators up until the first byte out of the ASCII
			range (0x00-0x7F) is encountered or the target length is reached, whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii7(const std::size_t idx, const std::size_t len);

		/*! \brief Read a fixed length null padded 7-bit ASCII encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as 7-bit ASCII string.

			It will ingest control characters and null terminators up until the first byte out of the ASCII
			range (0x00-0x7F) is encountered or the target length is reached, whichever occurs first.

			This method is effectively the same as `string_ascii7`.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii7_zp(const std::size_t idx, const std::size_t len);

		/*! \brief Read a null-terminated 7-bit ASCII encoded string from the `bytearray_t`.

			This method will read up until a null terminator (0x00)	or `max_len` and decode it as a 7-bit ASCII string.

			It will ingest all control characters up until the first byte out of the ASCII range (0x00-0x7F),
			it it's a null terminator, or we reach `max_len` if it non-zero, whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param max_len The maximum length of the string if the null terminator is not found, 0 for no limit.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii7_zt(const std::size_t idx, const std::size_t max_len = 0zu);

		/*! \brief Read an 8-bit length prefixed 7-bit ASCII encoded string from the `bytearray_t`.

			This method will read the first 8 bits at the offset `idx` as an unsigned integer and use that as the
			number of 7-bit ASCII characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			ASCII range (0x00-0x7F) or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭─────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN │ String Data ┊
			├─────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0     8            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii7_lp8(const std::size_t idx);

		/*! \brief Read a 16-bit length prefixed 7-bit ASCII encoded string from the `bytearray_t`.

			This method will read the first 16 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number 7-bit ASCII characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			ASCII range (0x00-0x7F) or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ String Data ┊
			├──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii7_lp16(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 24-bit length prefixed 7-bit ASCII encoded string from the `bytearray_t`.

			This method will read the first 24 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number 7-bit ASCII characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			ASCII range (0x00-0x7F) or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii7_lp24(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 32-bit length prefixed 7-bit ASCII encoded string from the `bytearray_t`.

			This method will read the first 32 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number 7-bit ASCII characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			ASCII range (0x00-0x7F) or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii7_lp32(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 64-bit length prefixed 7-bit ASCII encoded string from the `bytearray_t`.

			This method will read the first 64 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number 7-bit ASCII characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			ASCII range (0x00-0x7F) or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ LEN3 │ LEN4 │ LEN5 │ LEN6 │ String Data ┊
			├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32     40     48     56     64            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii7_lp64(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a LEB128 length prefixed 7-bit ASCII encoded string from the `bytearray_t`.

			This method will read up to 5 bytes at the offset `idx` to construct an LEB128 byte stream, decode that
			as an unsigned integer and use that as the number of 7-bit ASCII characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			ASCII range (0x00-0x7F) or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬┈┈┈┈┈┈┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ .... │ LENN │ String Data ┊
			├──────┼┈┈┈┈┈┈┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8      N     N+8           LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ascii7_leb128(const std::size_t idx);

		/* EBCDIC */

		/*! \brief Read a fixed length EBCDIC encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them an as EBCDIC string.

			It will ingest control characters and null terminators up until the first byte out of the EBCDIC range
			is encountered or the target length is reached, whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ebcdic(const std::size_t idx, const std::size_t len);

		/*! \brief Read a fixed length null padded EBCDIC encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them an as EBCDIC string.

			It will ingest control characters and null terminators up until the first byte out of the EBCDIC range
			is encountered or the target length is reached, whichever occurs first.

			This method is effectively the same as 'string_ebcdic`.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ebcdic_zp(const std::size_t idx, const std::size_t len);

		/*! \brief Read a null-terminated EBCDIC encoded string from the `bytearray_t`.

			This method will read up until a null terminator or `max_len` and decode it as an EBCDIC string.

			It will ingest all control characters up until the first byte out of the EBCDIC range, it it's a
			null terminator, or we reach `max_len` if it non-zero, whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param max_len The maximum length of the string if the null terminator is not found, 0 for no limit.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ebcdic_zt(const std::size_t idx, const std::size_t max_len = 0zu);

		/*! \brief Read an 8-bit length prefixed EBCDIC encoded string from the `bytearray_t`.

			This method will read the first 8 bits at the offset `idx` as an unsigned integer and use that as the
			number of EBCDIC characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			EBCDIC range or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭─────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN │ String Data ┊
			├─────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0     8            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ebcdic_lp8(const std::size_t idx);

		/*! \brief Read a 16-bit length prefixed EBCDIC encoded string from the `bytearray_t`.

			This method will read the first 16 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number EBCDIC characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			EBCDIC range or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ String Data ┊
			├──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ebcdic_lp16(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 24-bit length prefixed EBCDIC encoded string from the `bytearray_t`.

			This method will read the first 24 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number EBCDIC characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			EBCDIC range or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ebcdic_lp24(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 32-bit length prefixed EBCDIC encoded string from the `bytearray_t`.

			This method will read the first 32 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number EBCDIC characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			EBCDIC range or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ebcdic_lp32(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 64-bit length prefixed EBCDIC encoded string from the `bytearray_t`.

			This method will read the first 64 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number EBCDIC characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			EBCDIC range or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ LEN3 │ LEN4 │ LEN5 │ LEN6 │ String Data ┊
			├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32     40     48     56     64            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ebcdic_lp64(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a LEB128 length prefixed EBCDIC encoded string from the `bytearray_t`.

			This method will read up to 5 bytes at the offset `idx` to construct an LEB128 byte stream, decode that
			as an unsigned integer and use that as the number of EBCDIC characters to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is outside of the
			EBCDIC range or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬┈┈┈┈┈┈┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ .... │ LENN │ String Data ┊
			├──────┼┈┈┈┈┈┈┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8      N     N+8           LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::string_view, strdec_error_t> string_ebcdic_leb128(const std::size_t idx);

		/* UTF-8 */

		/*! \brief Read a fixed length UTF-8 encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as UTF-8.

			It will ingest up until the first invalid UTF-8 codepoint encountered or the target length is reached,
			whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u8string_view, strdec_error_t> string_utf8(const std::size_t idx, const std::size_t len);

		/*! \brief Read a fixed length null padded UTF-8 encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as UTF-8.

			It will ingest up until the first invalid UTF-8 codepoint encountered or the target length is reached,
			whichever occurs first.

			This method is effectively the same as `string_utf8`.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u8string_view, strdec_error_t> string_utf8_zp(const std::size_t idx, const std::size_t len);

		/*! \brief Read a null-terminated UTF-8 encoded string from the `bytearray_t`.

			This method will read up until a null terminator or `max_len` and decode it as a UTF-8 string.

			It will ingest all control characters up until the first byte out of the UTF-8 codepoint,
			it it's a null terminator, or we reach `max_len` if it non-zero, whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param max_len The maximum length of the string if the null terminator is not found, 0 for no limit.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u8string_view, strdec_error_t> string_utf8_zt(const std::size_t idx, const std::size_t max_len = 0zu);

		/*! \brief Read an 8-bit length prefixed UTF-8 encoded string from the `bytearray_t`.

			This method will read the first 8 bits at the offset `idx` as an unsigned integer and use that as the
			number of UTF-8 codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-8
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭─────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN │ String Data ┊
			├─────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0     8            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u8string_view, strdec_error_t> string_utf8_lp8(const std::size_t idx);

		/*! \brief Read a 16-bit length prefixed UTF-8 encoded string from the `bytearray_t`.

			This method will read the first 16 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-8 codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-8
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ String Data ┊
			├──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u8string_view, strdec_error_t> string_utf8_lp16(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 24-bit length prefixed UTF-8 encoded string from the `bytearray_t`.

			This method will read the first 24 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-8 codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-8
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u8string_view, strdec_error_t> string_utf8_lp24(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 32-bit length prefixed UTF-8 encoded string from the `bytearray_t`.

			This method will read the first 32 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-8 codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-8
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u8string_view, strdec_error_t> string_utf8_lp32(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 64-bit length prefixed UTF-8 encoded string from the `bytearray_t`.

			This method will read the first 64 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-8 codepoint to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-8
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ LEN3 │ LEN4 │ LEN5 │ LEN6 │ String Data ┊
			├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32     40     48     56     64            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u8string_view, strdec_error_t> string_utf8_lp64(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a LEB128 length prefixed UTF-8 encoded string from the `bytearray_t`.

			This method will read up to 5 bytes at the offset `idx` to construct an LEB128 byte stream, decode that
			as an unsigned integer and use that as the number of UTF-8 codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-8
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬┈┈┈┈┈┈┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ .... │ LENN │ String Data ┊
			├──────┼┈┈┈┈┈┈┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8      N     N+8           LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u8string_view, strdec_error_t> string_utf8_leb128(const std::size_t idx);

		/* UTF-16 w/ BOM */

		/*! \brief Read a fixed length UTF-16 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as UTF-16 according to
			the byte-order-mark found at the first byte.

			It will ingest up until the first invalid UTF-16 codepoint encountered or the target length is reached,
			whichever occurs first, but only if the first codepoint is a valid byte-order-mark.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16_bom(const std::size_t idx, const std::size_t len);

		/*! \brief Read a fixed length null padded UTF-16 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as UTF-16 according to
			the byte-order-mark found at the first byte.

			It will ingest up until the first invalid UTF-16 codepoint encountered or the target length is reached,
			whichever occurs first, but only if the first codepoint is a valid byte-order-mark.

			This method is effectively the same as `string_utf16_bom`.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16_bom_zp(const std::size_t idx, const std::size_t len);

		/*! \brief Read a null-terminated UTF-16 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method will read up until a null terminator or `max_len` and decode it as UTF-16 according to
			the byte-order-mark found at the first byte.

			It will ingest all control characters up until the first byte out of the UTF-16 codepoint,
			it it's a null terminator, or we reach `max_len` if it non-zero, whichever occurs first,
			but only if the first codepoint is a valid byte-order-mark.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param max_len The maximum length of the string if the null terminator is not found, 0 for no limit.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16_bom_zt(const std::size_t idx, const std::size_t max_len = 0zu);

		/*! \brief Read an 8-bit length prefixed UTF-16 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method will read the first 8 bits at the offset `idx` as an unsigned integer and use that as the
			number of UTF-16 codepoints to read directly after it.

			The UTF-16 codepoints are decoded according to the Byte-Order-Mark found as the first codepoint in the
			string directly preceding the end of the length prefix bytes.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte, but only if the first
			codepoint is a valid byte-order-mark.

			\code{.unparsed}
			╭─────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN │ String Data ┊
			├─────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0     8            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16_bom_lp8(const std::size_t idx);

		/*! \brief Read a 16-bit length prefixed UTF-16 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method will read the first 16 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-16 codepoints to read directly after it.

			The UTF-16 codepoints are decoded according to the Byte-Order-Mark found as the first codepoint in the
			string directly preceding the end of the length prefix bytes.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte, but only if the first
			codepoint is a valid byte-order-mark.

			\code{.unparsed}
			╭──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ String Data ┊
			├──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16_bom_lp16(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 24-bit length prefixed UTF-16 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method will read the first 24 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-16 codepoints to read directly after it.

			The UTF-16 codepoints are decoded according to the Byte-Order-Mark found as the first codepoint in the
			string directly preceding the end of the length prefix bytes.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte, but only if the first
			codepoint is a valid byte-order-mark.

			\code{.unparsed}
			╭──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16_bom_lp24(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 32-bit length prefixed UTF-16 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method will read the first 32 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-16 codepoints to read directly after it.

			The UTF-16 codepoints are decoded according to the Byte-Order-Mark found as the first codepoint in the
			string directly preceding the end of the length prefix bytes.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte, but only if the first
			codepoint is a valid byte-order-mark.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16_bom_lp32(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 64-bit length prefixed UTF-16 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method will read the first 64 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-16 codepoints to read directly after it.

			The UTF-16 codepoints are decoded according to the Byte-Order-Mark found as the first codepoint in the
			string directly preceding the end of the length prefix bytes.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte, but only if the first
			codepoint is a valid byte-order-mark.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ LEN3 │ LEN4 │ LEN5 │ LEN6 │ String Data ┊
			├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32     40     48     56     64            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16_bom_lp64(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a LEB128 length prefixed UTF-16 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method will read up to 5 bytes at the offset `idx` to construct an LEB128 byte stream, decode that
			as an unsigned integer and use that as the number of UTF-16 codepoints to read directly after it.

			The UTF-16 codepoints are decoded according to the Byte-Order-Mark found as the first codepoint in the
			string directly preceding the end of the length prefix bytes.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte, but only if the first
			codepoint is a valid byte-order-mark.

			\code{.unparsed}
			╭──────┬┈┈┈┈┈┈┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ .... │ LENN │ String Data ┊
			├──────┼┈┈┈┈┈┈┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8      N     N+8           LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16_bom_leb128(const std::size_t idx);

		/* UTF-16 Little Endian */

		/*! \brief Read a fixed length little endian UTF-16 encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as UTF-16 little-endian.

			It will ingest up until the first invalid UTF-16 codepoint encountered or the target length is reached,
			whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16le(const std::size_t idx, const std::size_t len);

		/*! \brief Read a fixed length null padded little endian UTF-16 encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as UTF-16 little-endian.

			It will ingest up until the first invalid UTF-16 codepoint encountered or the target length is reached,
			whichever occurs first.

			This method is effectively the same as `string_utf16le`.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16le_zp(const std::size_t idx, const std::size_t len);

		/*! \brief Read a null-terminated little endian UTF-16 encoded string from the `bytearray_t`.

			This method will read up until a null terminator or `max_len` and decode it as a UTF-16 little-endian
			string.

			It will ingest all control characters up until the first byte out of the UTF-16 codepoint,
			it it's a null terminator, or we reach `max_len` if it non-zero, whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param max_len The maximum length of the string if the null terminator is not found, 0 for no limit.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16le_zt(const std::size_t idx, const std::size_t max_len = 0zu);

		/*! \brief Read an 8-bit length prefixed little endian UTF-16 encoded string from the `bytearray_t`.

			This method will read the first 8 bits at the offset `idx` as an unsigned integer and use that as the
			number of UTF-16 little endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭─────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN │ String Data ┊
			├─────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0     8            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16le_lp8(const std::size_t idx);

		/*! \brief Read a 16-bit length prefixed little endian UTF-16 encoded string from the `bytearray_t`.

			This method will read the first 16 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-16 little endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ String Data ┊
			├──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16le_lp16(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 24-bit length prefixed little endian UTF-16 encoded string from the `bytearray_t`.

			This method will read the first 24 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-16 little endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16le_lp24(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 32-bit length prefixed little endian UTF-16 encoded string from the `bytearray_t`.

			This method will read the first 32 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-16 little endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16le_lp32(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 64-bit length prefixed little endian UTF-16 encoded string from the `bytearray_t`.

			This method will read the first 64 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-16 little endian codepoint to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ LEN3 │ LEN4 │ LEN5 │ LEN6 │ String Data ┊
			├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32     40     48     56     64            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16le_lp64(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a LEB128 length prefixed little endian UTF-16 encoded string from the `bytearray_t`.

			This method will read up to 5 bytes at the offset `idx` to construct an LEB128 byte stream, decode that
			as an unsigned integer and use that as the number of UTF-16 little endian codepoints to read directly
			after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬┈┈┈┈┈┈┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ .... │ LENN │ String Data ┊
			├──────┼┈┈┈┈┈┈┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8      N     N+8           LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16le_leb128(const std::size_t idx);

		/* UTF-16 Big Endian */

		/*! \brief Read a fixed length big endian UTF-16 encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as UTF-16 big-endian.

			It will ingest up until the first invalid UTF-16 codepoint encountered or the target length is reached,
			whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16be(const std::size_t idx, const std::size_t len);

		/*! \brief Read a fixed length null padded big endian UTF-16 encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as UTF-16 big-endian.

			It will ingest up until the first invalid UTF-16 codepoint encountered or the target length is reached,
			whichever occurs first.

			This method is effectively the same as `string_utf16be`.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16be_zp(const std::size_t idx, const std::size_t len);

		/*! \brief Read a null-terminated big endian UTF-16 encoded string from the `bytearray_t`.

			This method will read up until a null terminator or `max_len` and decode it as a UTF-16 big-endian
			string.

			It will ingest all control characters up until the first byte out of the UTF-16 codepoint,
			it it's a null terminator, or we reach `max_len` if it non-zero, whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param max_len The maximum length of the string if the null terminator is not found, 0 for no limit.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16be_zt(const std::size_t idx, const std::size_t max_len = 0zu);

		/*! \brief Read an 8-bit length prefixed big endian UTF-16 encoded string from the `bytearray_t`.

			This method will read the first 8 bits at the offset `idx` as an unsigned integer and use that as the
			number of UTF-8 big endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭─────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN │ String Data ┊
			├─────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0     8            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16be_lp8(const std::size_t idx);

		/*! \brief Read a 16-bit length prefixed big endian UTF-16 encoded string from the `bytearray_t`.

			This method will read the first 16 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-16 big endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ String Data ┊
			├──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16be_lp16(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 24-bit length prefixed big endian UTF-16 encoded string from the `bytearray_t`.

			This method will read the first 24 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-16 big endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16be_lp24(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 32-bit length prefixed big endian UTF-16 encoded string from the `bytearray_t`.

			This method will read the first 32 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-16 big endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16be_lp32(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 64-bit length prefixed big endian UTF-16 encoded string from the `bytearray_t`.

			This method will read the first 64 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-16 big endian codepoint to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ LEN3 │ LEN4 │ LEN5 │ LEN6 │ String Data ┊
			├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32     40     48     56     64            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16be_lp64(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a LEB128 length prefixed big endian UTF-16 encoded string from the `bytearray_t`.

			This method will read up to 5 bytes at the offset `idx` to construct an LEB128 byte stream, decode that
			as an unsigned integer and use that as the number of UTF-16 big endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-16
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬┈┈┈┈┈┈┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ .... │ LENN │ String Data ┊
			├──────┼┈┈┈┈┈┈┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8      N     N+8           LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u16string_view, strdec_error_t> string_utf16be_leb128(const std::size_t idx);

		/* UTF-32 w/ BOM */

		/*! \brief Read a fixed length UTF-32 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as UTF-32 according to
			the byte-order-mark found at the first byte.

			It will ingest up until the first invalid UTF-32 codepoint encountered or the target length is reached,
			whichever occurs first, but only if the first codepoint is a valid byte-order-mark.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32_bom(const std::size_t idx, const std::size_t len);

		/*! \brief Read a fixed length null padded UTF-32 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as UTF-32 according to
			the byte-order-mark found at the first byte.

			It will ingest up until the first invalid UTF-32 codepoint encountered or the target length is reached,
			whichever occurs first, but only if the first codepoint is a valid byte-order-mark.

			This method is effectively the same as `string_utf32_bom`.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32_bom_zp(const std::size_t idx, const std::size_t len);

		/*! \brief Read a null-terminated UTF-32 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method will read up until a null terminator or `max_len` and decode it as UTF-32 according to
			the byte-order-mark found at the first byte.

			It will ingest all control characters up until the first byte out of the UTF-32 codepoint,
			it it's a null terminator, or we reach `max_len` if it non-zero, whichever occurs first,
			but only if the first codepoint is a valid byte-order-mark.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param max_len The maximum length of the string if the null terminator is not found, 0 for no limit.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32_bom_zt(const std::size_t idx, const std::size_t max_len = 0zu);

		/*! \brief Read an 8-bit length prefixed UTF-32 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method will read the first 8 bits at the offset `idx` as an unsigned integer and use that as the
			number of UTF-32 codepoints to read directly after it.

			The UTF-32 codepoints are decoded according to the Byte-Order-Mark found as the first codepoint in the
			string directly preceding the end of the length prefix bytes.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte, but only if the first
			codepoint is a valid byte-order-mark.

			\code{.unparsed}
			╭─────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN │ String Data ┊
			├─────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0     8            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32_bom_lp8(const std::size_t idx);

		/*! \brief Read a 16-bit length prefixed UTF-32 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method will read the first 16 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-32 codepoints to read directly after it.

			The UTF-32 codepoints are decoded according to the Byte-Order-Mark found as the first codepoint in the
			string directly preceding the end of the length prefix bytes.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte, but only if the first
			codepoint is a valid byte-order-mark.

			\code{.unparsed}
			╭──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ String Data ┊
			├──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32_bom_lp16(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 24-bit length prefixed UTF-32 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method will read the first 24 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-32 codepoints to read directly after it.

			The UTF-32 codepoints are decoded according to the Byte-Order-Mark found as the first codepoint in the
			string directly preceding the end of the length prefix bytes.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte, but only if the first
			codepoint is a valid byte-order-mark.

			\code{.unparsed}
			╭──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32_bom_lp24(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 32-bit length prefixed UTF-32 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method will read the first 32 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-32 codepoints to read directly after it.

			The UTF-32 codepoints are decoded according to the Byte-Order-Mark found as the first codepoint in the
			string directly preceding the end of the length prefix bytes.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte, but only if the first
			codepoint is a valid byte-order-mark.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32_bom_lp32(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 64-bit length prefixed UTF-32 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method will read the first 64 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-32 codepoints to read directly after it.

			The UTF-32 codepoints are decoded according to the Byte-Order-Mark found as the first codepoint in the
			string directly preceding the end of the length prefix bytes.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte, but only if the first
			codepoint is a valid byte-order-mark.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ LEN3 │ LEN4 │ LEN5 │ LEN6 │ String Data ┊
			├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32     40     48     56     64            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32_bom_lp64(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a LEB128 length prefixed UTF-32 encoded string with a Byte-Order-Mark from the `bytearray_t`.

			This method will read up to 5 bytes at the offset `idx` to construct an LEB128 byte stream, decode that
			as an unsigned integer and use that as the number of UTF-32 codepoints to read directly after it.

			The UTF-32 codepoints are decoded according to the Byte-Order-Mark found as the first codepoint in the
			string directly preceding the end of the length prefix bytes.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte, but only if the first
			codepoint is a valid byte-order-mark.

			\code{.unparsed}
			╭──────┬┈┈┈┈┈┈┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ .... │ LENN │ String Data ┊
			├──────┼┈┈┈┈┈┈┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8      N     N+8           LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32_bom_leb128(const std::size_t idx);

		/* UTF-32 Little Endian */

		/*! \brief Read a fixed length little endian UTF-32 encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as UTF-32 little-endian.

			It will ingest up until the first invalid UTF-32 codepoint encountered or the target length is reached,
			whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32le(const std::size_t idx, const std::size_t len);

		/*! \brief Read a fixed length null padded little endian UTF-32 encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as UTF-32 little-endian.

			It will ingest up until the first invalid UTF-32 codepoint encountered or the target length is reached,
			whichever occurs first.

			This method is effectively the same as `string_utf32le`.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32le_zp(const std::size_t idx, const std::size_t len);

		/*! \brief Read a null-terminated little endian UTF-32 encoded string from the `bytearray_t`.

			This method will read up until a null terminator or `max_len` and decode it as a UTF-32 little-endian
			string.

			It will ingest all control characters up until the first byte out of the UTF-32 codepoint,
			it it's a null terminator, or we reach `max_len` if it non-zero, whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param max_len The maximum length of the string if the null terminator is not found, 0 for no limit.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32le_zt(const std::size_t idx, const std::size_t max_len = 0zu);

		/*! \brief Read an 8-bit length prefixed little endian UTF-32 encoded string from the `bytearray_t`.

			This method will read the first 8 bits at the offset `idx` as an unsigned integer and use that as the
			number of UTF-32 little endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭─────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN │ String Data ┊
			├─────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0     8            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32le_lp8(const std::size_t idx);

		/*! \brief Read a 16-bit length prefixed little endian UTF-32 encoded string from the `bytearray_t`.

			This method will read the first 16 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-32 little endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ String Data ┊
			├──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32le_lp16(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 24-bit length prefixed little endian UTF-32 encoded string from the `bytearray_t`.

			This method will read the first 24 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-32 little endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32le_lp24(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 32-bit length prefixed little endian UTF-32 encoded string from the `bytearray_t`.

			This method will read the first 32 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-32 little endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32le_lp32(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 64-bit length prefixed little endian UTF-32 encoded string from the `bytearray_t`.

			This method will read the first 64 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-32 little endian codepoint to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ LEN3 │ LEN4 │ LEN5 │ LEN6 │ String Data ┊
			├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32     40     48     56     64            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32le_lp64(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a LEB128 length prefixed little endian UTF-32 encoded string from the `bytearray_t`.

			This method will read up to 5 bytes at the offset `idx` to construct an LEB128 byte stream, decode that
			as an unsigned integer and use that as the number of UTF-32 little endian codepoints to read directly
			after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬┈┈┈┈┈┈┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ .... │ LENN │ String Data ┊
			├──────┼┈┈┈┈┈┈┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8      N     N+8           LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32le_leb128(const std::size_t idx);

		/* UTF-32 Big Endian */

		/*! \brief Read a fixed width big endian UTF-32 encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as UTF-32 big-endian.

			It will ingest up until the first invalid UTF-32 codepoint encountered or the target length is reached,
			whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32be(const std::size_t idx, const std::size_t len);

		/*! \brief Read a fixed length null padded big endian UTF-32 encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as UTF-32 big-endian.

			It will ingest up until the first invalid UTF-32 codepoint encountered or the target length is reached,
			whichever occurs first.

			This method is effectively the same as `string_utf32be`.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32be_zp(const std::size_t idx, const std::size_t len);

		/*! \brief Read a null-terminated big endian UTF-32 encoded string from the `bytearray_t`.

			This method will read up until a null terminator or `max_len` and decode it as a UTF-32 big-endian
			string.

			It will ingest all control characters up until the first byte out of the UTF-32 codepoint,
			it it's a null terminator, or we reach `max_len` if it non-zero, whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param max_len The maximum length of the string if the null terminator is not found, 0 for no limit.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32be_zt(const std::size_t idx, const std::size_t max_len = 0zu);

		/*! \brief Read an 8-bit length prefixed big endian UTF-32 encoded string from the `bytearray_t`.

			This method will read the first 8 bits at the offset `idx` as an unsigned integer and use that as the
			number of UTF-32 big endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭─────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN │ String Data ┊
			├─────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0     8            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32be_lp8(const std::size_t idx);

		/*! \brief Read a 16-bit length prefixed big endian UTF-32 encoded string from the `bytearray_t`.

			This method will read the first 16 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-32 big endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ String Data ┊
			├──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32be_lp16(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 24-bit length prefixed big endian UTF-32 encoded string from the `bytearray_t`.

			This method will read the first 24 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-32 big endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32be_lp24(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 32-bit length prefixed big endian UTF-32 encoded string from the `bytearray_t`.

			This method will read the first 32 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-32 big endian codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32be_lp32(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 64-bit length prefixed big endian UTF-32 encoded string from the `bytearray_t`.

			This method will read the first 64 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number UTF-32 big endian codepoint to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ LEN3 │ LEN4 │ LEN5 │ LEN6 │ String Data ┊
			├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32     40     48     56     64            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32be_lp64(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief  Read a LEB128 length prefixed big endian UTF-32 encoded string from the `bytearray_t`.

			This method will read up to 5 bytes at the offset `idx` to construct an LEB128 byte stream, decode that
			as an unsigned integer and use that as the number of UTF-32 big endian codepoints to read directly after
			it.

			It will ingest all control characters and null terminators up until the first byte is an invalid UTF-32
			codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬┈┈┈┈┈┈┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ .... │ LENN │ String Data ┊
			├──────┼┈┈┈┈┈┈┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8      N     N+8           LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::u32string_view, strdec_error_t> string_utf32be_leb128(const std::size_t idx);

		/* SHIFT-JIS */

		/*! \brief Read a fixed width SHIFT-JIS encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as SHIFT-JIS.

			It will ingest up until the first invalid SHIFT-JIS codepoint encountered or the target length is
			reached, whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::wstring_view, strdec_error_t> string_shiftjis(const std::size_t idx, const std::size_t len);

		/*! \brief Read a fixed length null padded SHIFT-JIS encoded string from the `bytearray_t`.

			This method reads a fixed number of bytes from the `bytearray_t` and decodes them as SHIFT-JIS.

			It will ingest up until the first invalid SHIFT-JIS codepoint encountered or the target length is
			reached, whichever occurs first.

			This method is effectively the same as `string_shiftjis`.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param len The length of the string.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::wstring_view, strdec_error_t> string_shiftjis_zp(const std::size_t idx, const std::size_t len);

		/*! \brief Read a null-terminated SHIFT-JIS encoded string from the `bytearray_t`.

			This method will read up until a null terminator or `max_len` and decode it as a SHIFT-JIS string.

			It will ingest all control characters up until the first byte out of the SHIFT-JIS codepoint,
			it it's a null terminator, or we reach `max_len` if it non-zero, whichever occurs first.

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param max_len The maximum length of the string if the null terminator is not found, 0 for no limit.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::wstring_view, strdec_error_t> string_shiftjis_zt(const std::size_t idx, const std::size_t max_len = 0zu);

		/*! \brief Read an 8-bit length prefixed SHIFT-JIS encoded string from the `bytearray_t`.

			This method will read the first 8 bits at the offset `idx` as an unsigned integer and use that as the
			number of SHIFT-JSI codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid
			SHIFT-JIS codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭─────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN │ String Data ┊
			├─────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0     8            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::wstring_view, strdec_error_t> string_shiftjis_lp8(const std::size_t idx);

		/*! \brief Read a 16-bit length prefixed SHIFT-JIS encoded string from the `bytearray_t`.

			This method will read the first 16 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number SHIFT-JIS codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid
			SHIFT-JIS codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ String Data ┊
			├──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::wstring_view, strdec_error_t> string_shiftjis_lp16(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 24-bit length prefixed SHIFT-JIS encoded string from the `bytearray_t`.

			This method will read the first 24 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number SHIFT-JIS codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid
			SHIFT-JIS codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::wstring_view, strdec_error_t> string_shiftjis_lp24(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 32-bit length prefixed SHIFT-JIS encoded string from the `bytearray_t`.

			This method will read the first 32 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number SHIFT-JIS codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid
			SHIFT-JIS codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ String Data ┊
			├──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::wstring_view, strdec_error_t> string_shiftjis_lp32(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief Read a 64-bit length prefixed SHIFT-JIS encoded string from the `bytearray_t`.

			This method will read the first 64 bits at the offset `idx`, decode them as an unsigned integer according
			to `endian` and use that as the number SHIFT-JIS codepoint to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid
			SHIFT-JIS codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ LEN1 │ LEN2 │ LEN2 │ LEN3 │ LEN4 │ LEN5 │ LEN6 │ String Data ┊
			├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8     16     24     32     40     48     56     64            LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
			\param endian The endian of the integer representing the string length prefix.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::wstring_view, strdec_error_t> string_shiftjis_lp64(const std::size_t idx, const std::endian endian = std::endian::native);

		/*! \brief  Read a LEB128 length prefixed SHIFT-JIS encoded string from the `bytearray_t`.

			This method will read up to 5 bytes at the offset `idx` to construct an LEB128 byte stream, decode that
			as an unsigned integer and use that as the number of SHIFT-JIS codepoints to read directly after it.

			It will ingest all control characters and null terminators up until the first byte is an invalid
			SHIFT-JIS codepoint, or we hit the number of characters contained in the length prefix byte.

			\code{.unparsed}
			╭──────┬┈┈┈┈┈┈┬──────┬┈┈┈┈┈┈┈┈┈┈┈┈┈╮
			│ LEN0 │ .... │ LENN │ String Data ┊
			├──────┼┈┈┈┈┈┈┼──────┼┈┈┈┈┈┈┈┈┈┈┈┈┈┤
			0      8      N     N+8           LEN
			\endcode

			\param idx The offset into the `bytearray_t` to start the extraction from.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<std::wstring_view, strdec_error_t> string_shiftjis_leb128(const std::size_t idx);

		/* == Decompression Helpers == */

		#if defined(PANKO_WITH_BROTLI)
		/*! \brief Decompress brotli compressed data into a new `bytearray_t`

			This method decompresses a block of data starting at `idx` that is `len` bytes long with the brotli
			decompression method.

			\note
			The returned `bytearray_t` owns it's buffer, as such the memory will be automatically reclaimed
			when the last reference to it dies unless an explicit call to `bytearray_t::disown` is called to get
			the raw buffer and take over ownership.

			\param idx The offset into the `bytearray_t` the start of the compressed data is located.
			\param len The length of the compressed data.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<bytearray_t, decomp_error_t> decompress_brotli(const std::size_t idx, const std::size_t len);
		#endif

		/*! \brief Decompress bz2 compressed data into a new `bytearray_t`

			This method decompresses a block of data starting at `idx` that is `len` bytes long with the bzip2
			decompression method.

			\note
			The returned `bytearray_t` owns it's buffer, as such the memory will be automatically reclaimed
			when the last reference to it dies unless an explicit call to `bytearray_t::disown` is called to get
			the raw buffer and take over ownership.

			\param idx The offset into the `bytearray_t` the start of the compressed data is located.
			\param len The length of the compressed data.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<bytearray_t, decomp_error_t> decompress_bz2(const std::size_t idx, const std::size_t len);

		/*! \brief Decompress zlib DEFLATE compressed data into a new `bytearray_t`

			This method decompresses a block of data starting at `idx` that is `len` bytes long with the zlib DEFLATE
			decompression method.

			\note
			The returned `bytearray_t` owns it's buffer, as such the memory will be automatically reclaimed
			when the last reference to it dies unless an explicit call to `bytearray_t::disown` is called to get
			the raw buffer and take over ownership.

			\param idx The offset into the `bytearray_t` the start of the compressed data is located.
			\param len The length of the compressed data.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<bytearray_t, decomp_error_t> decompress_deflate(const std::size_t idx, const std::size_t len);

		/*! \brief Decompress LZ4 compressed data into a new `bytearray_t`

			This method decompresses a block of data starting at `idx` that is `len` bytes long with the LZ4
			decompression method.

			\note
			The returned `bytearray_t` owns it's buffer, as such the memory will be automatically reclaimed
			when the last reference to it dies unless an explicit call to `bytearray_t::disown` is called to get
			the raw buffer and take over ownership.

			\param idx The offset into the `bytearray_t` the start of the compressed data is located.
			\param len The length of the compressed data.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<bytearray_t, decomp_error_t> decompress_lz4(const std::size_t idx, const std::size_t len);

		/*! \brief Decompress LZ77 compressed data into a new `bytearray_t`

			This method decompresses a block of data starting at `idx` that is `len` bytes long with the LZ77
			decompression method.

			\note
			The returned `bytearray_t` owns it's buffer, as such the memory will be automatically reclaimed
			when the last reference to it dies unless an explicit call to `bytearray_t::disown` is called to get
			the raw buffer and take over ownership.

			\param idx The offset into the `bytearray_t` the start of the compressed data is located.
			\param len The length of the compressed data.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<bytearray_t, decomp_error_t> decompress_lz77(const std::size_t idx, const std::size_t len);

		/*! \brief Decompress LZ77+Huffman compressed data into a new `bytearray_t`

			This method decompresses a block of data starting at `idx` that is `len` bytes long with the LZ77 with
			Huffman tables decompression method.

			\note
			The returned `bytearray_t` owns it's buffer, as such the memory will be automatically reclaimed
			when the last reference to it dies unless an explicit call to `bytearray_t::disown` is called to get
			the raw buffer and take over ownership.

			\param idx The offset into the `bytearray_t` the start of the compressed data is located.
			\param len The length of the compressed data.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<bytearray_t, decomp_error_t> decompress_lz77huff(const std::size_t idx, const std::size_t len);

		/*! \brief Decompress LZMA compressed data into a new `bytearray_t`

			This method decompresses a block of data starting at `idx` that is `len` bytes long with the LZMA
			decompression method.

			\note
			The returned `bytearray_t` owns it's buffer, as such the memory will be automatically reclaimed
			when the last reference to it dies unless an explicit call to `bytearray_t::disown` is called to get
			the raw buffer and take over ownership.

			\param idx The offset into the `bytearray_t` the start of the compressed data is located.
			\param len The length of the compressed data.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<bytearray_t, decomp_error_t> decompress_lzma(const std::size_t idx, const std::size_t len);

		/*! \brief Decompress LZNT1 compressed data into a new `bytearray_t`

			This method decompresses a block of data starting at `idx` that is `len` bytes long with the LZNT1
			decompression method.

			\note
			The returned `bytearray_t` owns it's buffer, as such the memory will be automatically reclaimed
			when the last reference to it dies unless an explicit call to `bytearray_t::disown` is called to get
			the raw buffer and take over ownership.

			\param idx The offset into the `bytearray_t` the start of the compressed data is located.
			\param len The length of the compressed data.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<bytearray_t, decomp_error_t> decompress_lznt1(const std::size_t idx, const std::size_t len);

		#if defined(PANKO_WITH_SNAPPY)
		/*! \brief Decompress snappy compressed data into a new `bytearray_t`

			This method decompresses a block of data starting at `idx` that is `len` bytes long with the snappy
			decompression method.

			\note
			The returned `bytearray_t` owns it's buffer, as such the memory will be automatically reclaimed
			when the last reference to it dies unless an explicit call to `bytearray_t::disown` is called to get
			the raw buffer and take over ownership.

			\param idx The offset into the `bytearray_t` the start of the compressed data is located.
			\param len The length of the compressed data.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<bytearray_t, decomp_error_t> decompress_snappy(const std::size_t idx, const std::size_t len);
		#endif

		/*! \brief Decompress XZ compressed data into a new `bytearray_t`

			This method decompresses a block of data starting at `idx` that is `len` bytes long with the XZ
			decompression method.

			\note
			The returned `bytearray_t` owns it's buffer, as such the memory will be automatically reclaimed
			when the last reference to it dies unless an explicit call to `bytearray_t::disown` is called to get
			the raw buffer and take over ownership.

			\param idx The offset into the `bytearray_t` the start of the compressed data is located.
			\param len The length of the compressed data.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<bytearray_t, decomp_error_t> decompress_xz(const std::size_t idx, const std::size_t len);

		/*! \brief Decompress ZSTD compressed data into a new `bytearray_t`

			This method decompresses a block of data starting at `idx` that is `len` bytes long with the ZSTD
			decompression method.

			\note
			The returned `bytearray_t` owns it's buffer, as such the memory will be automatically reclaimed
			when the last reference to it dies unless an explicit call to `bytearray_t::disown` is called to get
			the raw buffer and take over ownership.

			\param idx The offset into the `bytearray_t` the start of the compressed data is located.
			\param len The length of the compressed data.
		*/
		[[nodiscard]]
		PANKO_CLS_API std::expected<bytearray_t, decomp_error_t> decompress_zstd(const std::size_t idx, const std::size_t len);
	};
}

#endif /* PANKO_CORE_BYTEARRAY_HH */
