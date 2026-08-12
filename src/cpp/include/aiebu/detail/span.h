// SPDX-License-Identifier: MIT
// Copyright (C) 2025-2026, Advanced Micro Devices, Inc. All rights reserved.
#ifndef AIEBU_DETAIL_SPAN_H
#define AIEBU_DETAIL_SPAN_H

// C++17-compatible span replacement.
//
// std::span is C++20.  This header provides a minimal substitute with the
// same interface for use in C++17 translation units.  When AIEBU moves to
// C++20 this file can be replaced by a thin alias:
//
//   #include <span>
//   namespace aiebu::detail { template<typename T> using span = std::span<T>; }

#include <array>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace aiebu::detail {

template <typename T>
class span
{
  T* m_data = nullptr;
  std::size_t m_size = 0;

public:
  using element_type    = T;
  using value_type      = std::remove_cv_t<T>;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;
  using pointer         = T*;
  using const_pointer   = const T*;
  using reference       = T&;
  using const_reference = const T&;
  using iterator        = pointer;
  using const_iterator  = const_pointer;
  using reverse_iterator       = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  constexpr span() = default;

  constexpr span(T* data, std::size_t size)
    : m_data(data)
    , m_size(size)
  {}

  template <class U, std::size_t N>
  constexpr explicit span(std::array<U, N>& arr) noexcept
    : span(arr.data(), N)
  {}

  template <class U, std::size_t N>
  constexpr explicit span(const std::array<U, N>& arr) noexcept
    : span(const_cast<U*>(arr.data()), N)
  {}

  constexpr iterator begin() const noexcept { return m_data; }
  constexpr iterator end()   const noexcept { return m_data + m_size; }
  constexpr const_iterator cbegin() const noexcept { return begin(); }
  constexpr const_iterator cend()   const noexcept { return end(); }

  constexpr reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
  constexpr reverse_iterator rend()   const noexcept { return reverse_iterator(begin()); }
  constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
  constexpr const_reverse_iterator crend()   const noexcept { return const_reverse_iterator(begin()); }

  constexpr reference front() const { return *begin(); }
  constexpr reference back()  const { return *(end() - 1); }

  constexpr reference
  at(size_type idx) const
  {
    if (idx < m_size)
      return m_data[idx];

    throw std::out_of_range("aiebu::detail::span::at: pos (" +
                            std::to_string(idx) + ") >= size() (" +
                            std::to_string(m_size) + ")");
  }

  constexpr reference operator[](size_type idx) const { return m_data[idx]; }
  constexpr pointer   data()       const noexcept { return m_data; }

  constexpr size_type size()       const noexcept { return m_size; }
  constexpr size_type size_bytes() const noexcept { return m_size * sizeof(T); }
  constexpr bool      empty()      const noexcept { return m_size == 0; }

  constexpr span first(size_type count) const { return {m_data, count}; }
  constexpr span last(size_type count)  const { return {m_data + (m_size - count), count}; }

  constexpr span
  subspan(size_type offset, size_type count) const
  {
    return {m_data + offset, count};
  }
};

} // namespace aiebu::detail

#endif // AIEBU_DETAIL_SPAN_H
