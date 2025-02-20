// SPDX-License-Identifier: BSD-3-Clause
/* mmap.hh - Memory-mapped region */
#pragma once
#if !defined(PANKO_CORE_MMAP_HH)
#define PANKO_CORE_MMAP_HH

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string_view>
#include <string>
#include <stdexcept>

#include "panko/internal/defs.hh"

#if !defined(_WIN32)
#	include <unistd.h>
#	include <sys/mman.h>
#else
#	if !defined(NOMINMAX)
#		define NOMINMAX
#	endif
#	if !defined(WIN32_LEAN_AND_MEAN)
#		define WIN32_LEAN_AND_MEAN
#	endif
#	include <io.h>
#	include <windows.h>
#endif

#include "panko/core/types.hh"
namespace Panko::core {
	using Panko::core::types::ssize_t;
	using Panko::core::types::mode_t;
	using Panko::core::types::off_t;
	using Panko::core::types::has_nullable_ctor_v;

	namespace _compat {
		#if !defined(_WIN32)
		[[nodiscard]]
		inline bool lock(void* addr, const std::size_t len) noexcept {
			return ::mlock(addr, len) == 0;
		}

		[[nodiscard]]
		inline bool unlock(void* addr, const std::size_t len) noexcept {
			return ::munlock(addr, len) == 0;
		}

		[[nodiscard]]
		inline bool unmap(void* addr, const std::size_t len) noexcept {
			return ::munmap(addr, len) == 0;
		}

		[[nodiscard]]
		inline void* map_addr(
			void* addr, const std::size_t len, const std::int32_t prot, const std::int32_t flags, std::int32_t fd
		) noexcept {
			const auto ptr{::mmap(addr, len, prot, flags, fd, 0)};
			return ptr == MAP_FAILED ? nullptr : ptr;
		}

		[[nodiscard]]
		inline bool protect(void* addr, const std::size_t len, const std::int32_t prot) noexcept {
			return ::mprotect(addr, len, prot) == 0;
		}

		[[nodiscard]]
		inline void* remap(
			void* old_addr, const std::size_t old_len, const std::size_t new_len, const std::int32_t flags
		) noexcept {
			const auto ptr{::mremap(old_addr, old_len, new_len, flags)};
			return ptr == MAP_FAILED ? nullptr : ptr;
		}

		[[nodiscard]]
		inline void* remap_want_addr(
			void* old_addr, const std::size_t old_len, const void* new_addr, const std::size_t new_len,
			const std::int32_t flags
		) noexcept {
			const auto ptr{::mremap(old_addr, old_len, new_len, flags, new_addr)};
			return ptr == MAP_FAILED ? nullptr : ptr;
		}

		[[nodiscard]]
		inline bool sync(void* addr, const std::size_t len, const std::int32_t flags) noexcept {
			return ::msync(addr, len, flags) == 0;
		}

		[[nodiscard]]
		inline bool advise(void* addr, const std::size_t len, const std::int32_t advice) noexcept {
			return ::madvise(addr, len, advice) == 0;
		}
		#else /* !_WIN32 */
		[[nodiscard]]
		inline bool lock(void* addr, const std::size_t len) noexcept {
			return VirtualLock(addr, len) == 0;
		}

		[[nodiscard]]
		inline bool unlock(void* addr, const std::size_t len) noexcept {
			return VirtualUnlock(addr, len);
		}

		[[nodiscard]]
		inline bool unmap(void* addr, const std::size_t len) noexcept {
			(void)len;
			return UnmapViewOfFile(addr);
		}

		[[nodiscard]]
		constexpr inline DWORD clean_prot(const DWORD prot) noexcept {
			return (prot & PROT_WRITE) ? prot & ~PROT_READ : prot;
		}

		[[nodiscard]]
		constexpr inline DWORD prot_to_access(const DWORD prot) noexcept {
			if ((prot & PAGE_READWRITE) || (prot & PAGE_WRITECOPY)) {
				return FILE_MAP_WRITE;
			} else if (prot & PAGE_READONLY) {
				return FILE_MAP_READ;
			}
			return {};
		}


		[[nodiscard]]
		inline HANDLE make_mapping(const std::int32_t fd, const std::size_t len, const std::int32_t prot) noexcept {
			auto* const file{reinterpret_cast<HANDLE>(_get_osfhandle(fd))};
			return CreateFileMappingA(
				file, nullptr, clean_prot(static_cast<DWORD>(prot)), DWORD(len >> 32U), DWORD(len), nullptr
			);
		}

		[[nodiscard]]
		inline LPVOID map_addr(HANDLE mapping, const std::int32_t prot) noexcept {
			if !(mapping) {
				return nullptr;
			}
			return MapViewOfFile(mapping, prot_to_access(static_cast<DWORD>(prot)), 0, 0, 0);
		}

		[[nodiscard]]
		inline bool protect(void* addr, const std::size_t len, const std::int32_t prot) noexcept {
			return VirtualProtect(addr, len, static_cast<DWORD>(prot), nullptr) == 0;
		}


		[[nodiscard]]
		inline void* remap(
			void* old_addr, const std::size_t old_len, const std::size_t new_len, const std::int32_t flags
		) noexcept {
			(void)old_addr;
			(void)old_len;
			(void)new_len;
			(void)flags;
			return nullptr;
		}

		[[nodiscard]]
		inline void* remap_want_addr(
			void* old_addr, const std::size_t old_len, const void* new_addr, const std::size_t new_len,
			const std::int32_t flags
		) noexcept {
			(void)old_addr;
			(void)old_len;
			(void)new_addr;
			(void)new_len;
			(void)flags;
			return nullptr;
		}

		[[nodiscard]]
		inline bool sync(void* addr, const std::size_t len, const std::int32_t flags) noexcept {
			(void)flags;
			return FlushViewOfFile(addr, len);
		}

		[[nodiscard]]
		inline bool advise(void* addr, const std::size_t len, const std::int32_t advice) noexcept {
			return true;
		}
		#endif
	}

	/* Because we are a Linux-first API, we need to add some compat constants in for Windows */
	#if defined(_WIN32)
	/* ::mmap `flags` */
	[[maybe_unused]]
	static constexpr std::int32_t MAP_SHARED{0};
	[[maybe_unused]]
	static constexpr std::int32_t MAP_SHARED_VALIDATE{0};
	[[maybe_unused]]
	static constexpr std::int32_t MAP_PRIVATE{0};
	[[maybe_unused]]
	static constexpr std::int32_t MAP_ANONYMOUS{0};
	[[maybe_unused]]
	static constexpr std::int32_t MAP_FIXED{0};
	[[maybe_unused]]
	static constexpr std::int32_t MAP_FIXED_NOREPLACE{0};
	[[maybe_unused]]
	static constexpr std::int32_t MAP_HUGETLB{0};
	[[maybe_unused]]
	static constexpr std::int32_t MAP_HUGE_2MB{0};
	[[maybe_unused]]
	static constexpr std::int32_t MAP_HUGE_1GB{0};
	[[maybe_unused]]
	static constexpr std::int32_t MAP_LOCKED{0};
	[[maybe_unused]]
	static constexpr std::int32_t MAP_NONBLOCK{0};
	[[maybe_unused]]
	static constexpr std::int32_t MAP_NORESERVE{0};
	[[maybe_unused]]
	static constexpr std::int32_t MAP_POPULATE{0};
	[[maybe_unused]]
	static constexpr std::int32_t MAP_STACK{0};
	[[maybe_unused]]
	static constexpr std::int32_t MAP_SYNC{0};
	[[maybe_unused]]
	static constexpr std::int32_t MAP_UNINITIALIZED{0};
	[[maybe_unused]]
	static constexpr std::int32_t MAP_FAILED{-1};

	/* ::mprotect() */
	[[maybe_unused]]
	static constexpr DWORD PROT_NONE{PAGE_NOACCESS};
	[[maybe_unused]]
	static constexpr DWORD PROT_READ{PAGE_READONLY};
	[[maybe_unused]]
	static constexpr DWORD PROT_WRITE{PAGE_READWRITE};
	[[maybe_unused]]
	static constexpr DWORD PROT_EXEC{PAGE_EXECUTE};
	[[maybe_unused]]
	static constexpr DWORD PROT_SEM{0};
	[[maybe_unused]]
	static constexpr DWORD PROT_SAO{0};
	[[maybe_unused]]
	static constexpr DWORD PROT_GROWSUP{0};
	[[maybe_unused]]
	static constexpr DWORD PROT_GROWSDOWN{0};

	/* ::madvise() */
	[[maybe_unused]]
	static constexpr std::int32_t MADV_NORMAL{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_RANDOM{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_SEQUENTIAL{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_WILLNEED{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_DONTDUMP{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_DONTFORK{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_DOFORK{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_HWPOISON{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_MERGEABLE{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_UNMERGEABLE{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_SOFT_OFFLINE{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_HUGEPAGE{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_NOHUGEPAGE{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_COLLAPSE{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_DONTDUMP{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_DODUMP{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_FREE{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_WIPEONFORK{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_KEEPONFORK{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_COLD{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_PAGEOUT{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_POPULATE_READ{0};
	[[maybe_unused]]
	static constexpr std::int32_t MADV_POPULATE_WRITE{0};

	/* ::msync() */
	[[maybe_unused]]
	static constexpr std::int32_t MS_ASYNC{0};
	[[maybe_unused]]
	static constexpr std::int32_t MS_SYNC{0};
	[[maybe_unused]]
	static constexpr std::int32_t MS_INVALIDATE{0};

	/* ::mlock2() */
	[[maybe_unused]]
	static constexpr std::int32_t MLOCK_ONFAULT{0};

	/* ::mlockall()/::munlockall() */
	[[maybe_unused]]
	static constexpr std::int32_t MCL_CURRENT{0};
	[[maybe_unused]]
	static constexpr std::int32_t MCL_FUTURE{0};
	[[maybe_unused]]
	static constexpr std::int32_t MCL_ONFAULT{0};

	/* ::mremap() */
	[[maybe_unused]]
	static constexpr std::int32_t MREMAP_MAYMOVE{0};
	[[maybe_unused]]
	static constexpr std::int32_t MREMAP_FIXED{0};
	[[maybe_unused]]
	static constexpr std::int32_t MREMAP_DONTUNMAP{0};

	#endif

	struct mmap_t final {
	private:
		using off_t = Panko::core::types::off_t;
		using ptr_t = std::conditional_t<
			sizeof(std::uint32_t*) == sizeof(void*),
			std::uint32_t*, std::uint64_t*
		>;

		std::size_t _len{0z};
		#if defined(_WIN32)
			HANDLE _mapping{INVALID_HANDLE_VALUE};
		#endif
		void* _addr{nullptr};
		std::int32_t _fd{-1};

		mmap_t(
			const mmap_t& map, const std::size_t len, const std::int32_t prot,
			const std::int32_t flags = MAP_SHARED, void* addr = nullptr
		) noexcept : _len{len},
			#if !defined(_WIN32)
				_addr{_compat::map_addr(addr, len, prot, flags, map._fd)}
			#else
				_mapping{_compat::make_mapping(map._fd, len, prot)}, _addr{_compat::map_addr(_mapping, prot)}
			#endif
		{ }

		template<typename T>
		[[nodiscard]]
		std::enable_if_t<
			std::is_standard_layout_v<T> && std::is_trivial_v<T> && !has_nullable_ctor_v<T> && !std::is_same_v<T, void*>, T*
		>
		index(const std::size_t idx) const {
			if (idx <= _len) {
				auto* const addr{reinterpret_cast<std::uint8_t*>(_addr)};
				return new (addr + (idx * sizeof(T))) T{};
			}
			throw std::out_of_range("mmap_t index out of range");
		}

		template<typename T>
		[[nodiscard]]
		std::enable_if_t<has_nullable_ctor_v<T> && !std::is_same_v<T, void*>, T*>
		index(const std::size_t idx) const {
			if (idx <= _len) {
				auto* const addr{reinterpret_cast<std::uint8_t*>(_addr)};
				return new (addr + (idx * sizeof(T))) T{nullptr};
			}
			throw std::out_of_range("mmap_t index out of range");
		}

		template<typename T>
		[[nodiscard]]
		std::enable_if_t<std::is_same_v<T, void*>, void*>
		index(const std::size_t idx) const {
			if (idx <= _len) {
				auto* const addr{reinterpret_cast<std::uint8_t*>(_addr)};
				return (addr + idx);
			}
			throw std::out_of_range("mmap_t index out of range");
		}

	public:
		constexpr mmap_t() noexcept = default;

		mmap_t(
			const std::int32_t fd, const std::size_t len, const std::int32_t prot,
			const std::int32_t flags = MAP_SHARED, void* addr = nullptr
		) noexcept :
			#if !defined(_WIN32)
				_addr{_compat::map_addr(addr, len, prot, flags, fd)}
			#else
				_mapping{_compat::make_mapping(fd, len, prot)}, _addr{_compat::map_addr(_mapping, prot)}
			#endif
		{ }

		mmap_t(const mmap_t&) = delete;
		mmap_t& operator=(const mmap_t&) = delete;

		mmap_t(mmap_t&& other) noexcept : mmap_t{} {
			*this = std::move(other);
		}
		mmap_t& operator=(mmap_t&& other) noexcept {
			std::swap(_fd, other._fd);
			std::swap(_addr, other._addr);
			#if defined(_WIN32)
				std::swap(_mapping, other._mapping);
			#endif
			std::swap(_len, other._len);

			return *this;
		}

		~mmap_t() noexcept {
			/* Unmap the memory if it's mapped */
			if (_addr) {
				[[maybe_unused]]
				auto _{_compat::unmap(_addr, _len)};
			}

			/* Cleanup file descriptors */
			#if !defined(_WIN32)
				if (_fd != -1) {
					::close(_fd);
				}
			#else
				if (_mapping) {
					CloseHandle(_mapping);
				}

				if (_fd != -1) {
					_close(_fd);
				}
			#endif
		}

		[[nodiscard]]
		bool operator==(const mmap_t& rhs) const noexcept {
			return _fd == rhs._fd && _addr == rhs._addr  && _len == rhs._len;
		}
		[[nodiscard]]
		bool operator!=(const mmap_t& rhs) const noexcept {
			return !(*this == rhs);
		}

		[[nodiscard]]
		operator std::int32_t() const noexcept {
			return _fd;
		}

		[[nodiscard]]
		std::int32_t fd() const noexcept {
			return _fd;
		}

		[[nodiscard]]
		bool valid() const noexcept {
			#if !defined(_WIN32)
				return _addr != nullptr;
			#else
				return _mapping && _addr != nullptr;
			#endif
		}

		[[nodiscard]]
		mmap_t	dup(const std::int32_t prot, const std::size_t len, const std::int32_t flags, void* addr) const noexcept {
			if (!valid()) {
				return {};
			}
			return {*this, len, prot, flags, addr};
		}

		template<typename T>
		[[nodiscard]]
		T* address() noexcept {
			return static_cast<T*>(_addr);
		}

		template<typename T>
		[[nodiscard]]
		const T* address() const noexcept {
			return static_cast<const T*>(_addr);
		}

		template<typename T>
		[[nodiscard]]
		T *operator [](const std::size_t idx) {
			return index<T>(idx);
		}

		template<typename T>
		[[nodiscard]]
		const T *operator [](const off_t idx) const {
			return index<const T>(idx);
		}

		template<typename T>
		[[nodiscard]]
		T *at(const std::size_t idx) {
			return index<T>(idx);
		}

		template<typename T>
		[[nodiscard]]
		const T *at(const std::size_t idx) const {
			return index<const T>(idx);
		}

		[[nodiscard]]
		std::size_t length() const noexcept {
			return _len;
		}

		[[nodiscard]]
		void* address(const std::size_t offset) noexcept {
			return index<void*>(offset);
		}

		[[nodiscard]]
		const void* address(const std::size_t offset) const noexcept {
			return index<const void*>(offset);
		}

		[[nodiscard]]
		std::uintptr_t numeric_address() const noexcept {
			return reinterpret_cast<std::uintptr_t>(_addr);
		}

		[[nodiscard]]
		bool protect(const std::int32_t prot) noexcept {
			return _compat::protect(_addr, _len, prot);
		}

		[[nodiscard]]
		bool lock() const noexcept {
			return lock(_len);
		}

		[[nodiscard]]
		bool lock(const std::size_t len) const noexcept {
			return _compat::lock(_addr, len);
		}

		[[nodiscard]]
		bool lock_at(const std::size_t idx, const std::size_t len) const noexcept {
			auto* const addr{reinterpret_cast<ptr_t>(_addr)};
			return _compat::lock(addr + idx, len);
		}

		[[nodiscard]]
		bool unlock() const noexcept {
			return unlock(_len);
		}

		[[nodiscard]]
		bool unlock(const std::size_t len) const noexcept {
			return _compat::unlock(_addr, len);
		}

		[[nodiscard]]
		bool unlock_at(const std::size_t idx, const std::size_t len) const noexcept {
			auto* const addr{reinterpret_cast<ptr_t>(_addr)};
			return _compat::unlock(addr + idx, len);
		}

		[[nodiscard]]
		bool remap(const std::size_t new_len, const std::int32_t flags) noexcept {
			const auto old_len{_len};
			_len = new_len;

			return (_addr = _compat::remap(_addr, old_len, _len, flags)) != nullptr;
		}

		[[nodiscard]]
		bool remap(const std::uintptr_t new_addr, const std::size_t new_len, const std::int32_t flags) noexcept {
			const auto old_len{_len};
			_len = new_len;

			const void* wanted_addr{reinterpret_cast<void*>(new_addr)};

			return (_addr = _compat::remap_want_addr(_addr, old_len, wanted_addr, _len, flags)) != nullptr;
		}

		[[nodiscard]]
		bool sync(const std::int32_t flags = MS_SYNC | MS_INVALIDATE) const noexcept {
			return sync(_len, flags);
		}

		[[nodiscard]]
		bool sync(const std::size_t len, const std::int32_t flags = MS_SYNC | MS_INVALIDATE) const noexcept {
			return _compat::sync(_addr, len, flags);
		}

		[[nodiscard]]
		bool advise(const std::int32_t advice) const noexcept {
			return advise(advice, _len);
		}

		[[nodiscard]]
		bool advise(const std::int32_t advice, const std::size_t len) const noexcept {
			return _compat::advise(_addr, len, advice);
		}


		[[nodiscard]]
		bool advise_at(const std::int32_t advice, const std::size_t len, const std::size_t idx) const noexcept {
			const auto addr{reinterpret_cast<std::uintptr_t>(_addr)};
			auto ptr{reinterpret_cast<void*>(addr + idx)};
			return _compat::advise(ptr, len, advice);
		}

		template<typename T>
		void copy_to(const std::size_t idx, T& value) const {
			const auto* const src{index<const void*>(idx)};
			/* TODO(aki): We should assert sizeof(T) <= _len - idx */
			std::memcpy(&value, src, sizeof(T));
		}

		template<typename T, std::size_t N>
		void copy_to(const std::size_t idx, std::array<T, N>& value) const {
			constexpr auto len{sizeof(T) * N};
			const auto* const src{index<const void*>(idx)};
			/* TODO(aki): We should assert len <= _len - idx */
			std::memcpy(value.data(), src, len);
		}

		void copy_to(const std::size_t idx, std::string& value) const {
			const auto* const src{index<const void*>(idx)};
			/* TODO(aki): We should assert value.size() <= _len - idx */
			std::memcpy(const_cast<char *>(value.data()), src, value.size());
		}

		void copy_to(const std::size_t idx, std::string_view& value) const {
			const auto* const src{index<const void*>(idx)};
			/* TODO(aki): We should assert value.size() <= _len - idx */
			std::memcpy(const_cast<char *>(value.data()), src, value.size());
		}


		template<typename T>
		void copy_from(const std::size_t idx, T& value) const {
			auto* const dest{index<void*>(idx)};
			/* TODO(aki): We should assert sizeof(T) <= _len - idx */
			std::memcpy(dest, &value, sizeof(T));
		}

		template<typename T, std::size_t N>
		void copy_from(const std::size_t idx, std::array<T, N>& value) const {
			constexpr auto len{sizeof(T) * N};
			auto* const dest{index<void*>(idx)};
			/* TODO(aki): We should assert len <= _len - idx */
			std::memcpy(dest, value.data(), len);
		}

		void copy_from(const std::size_t idx, std::string& value) const {
			auto* const dest{index<void*>(idx)};
			/* TODO(aki): We should assert value.size() <= _len - idx */
			std::memcpy(dest, value.data(), value.size());
		}

		void copy_from(const std::size_t idx, std::string_view& value) const {
			auto* const dest{index<void*>(idx)};
			/* TODO(aki): We should assert value.size() <= _len - idx */
			std::memcpy(dest, value.data(), value.size());
		}
	};

}

#endif /* PANKO_CORE_MMAP_HH */
