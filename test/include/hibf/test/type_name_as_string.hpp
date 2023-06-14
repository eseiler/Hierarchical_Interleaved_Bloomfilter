// ------------------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2023, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2023, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/Hierarchical_Interleaved_Bloomfilter/blob/main/LICENSE.md
// ------------------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides traits to inspect some information of a type, for example its name.
 * \author Enrico Seiler <enrico.seiler AT fu-berlin.de>
 */

#pragma once

#if defined(__GNUC__) || defined(__clang__)
#    include <cxxabi.h> // for __cxa_demangle
#endif                  // defined(__GNUC__) || defined(__clang__)

#include <cstdlib>     // for free
#include <functional>  // for function
#include <memory>      // for allocator, unique_ptr
#include <string>      // for operator+, char_traits, string, to_string
#include <type_traits> // for remove_reference_t
#include <typeinfo>    // for type_info
#include <utility>     // for addressof

#include <hibf/platform.hpp>

namespace hibf::detail
{

/*!\brief Defines the human-readable name of the given type using the
          [typeid](https://en.cppreference.com/w/cpp/language/typeid) operator.
 * \ingroup utility
 *
 * \tparam type The type to get the human-readable name for.
 *
 * \details
 *
 * On gcc and clang std::type_info only returns a mangled name.
 * The mangled name can be converted to human-readable form using implementation-specific API such as
 * abi::__cxa_demangle. In other implementations the name returned is already human-readable.
 *
 * \note The returned name is implementation defined and might change between different tool chains.
 */
template <typename type>
inline std::string const type_name_as_string = []()
{
    std::string demangled_name{};
#if defined(__GNUC__) || defined(__clang__) // clang and gcc only return a mangled name.
    using safe_ptr_t = std::unique_ptr<char, std::function<void(char *)>>;

    // https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
    int status{};
    safe_ptr_t demangled_name_ptr{abi::__cxa_demangle(typeid(type).name(), 0, 0, &status),
                                  [](char * name_ptr)
                                  {
                                      free(name_ptr);
                                  }};

    // We exclude status != 0, because this code can't be reached normally, only if there is a defect in the compiler
    // itself, since the type is directly given by the compiler.
    // LCOV_EXCL_START
    if (status != 0)
        return std::string{typeid(type).name()} + " (abi::__cxa_demangle error status (" + std::to_string(status)
             + "): "
             + (status == -1 ? "A memory allocation failure occurred."
                             : (status == -2 ? "mangled_name is not a valid name under the C++ ABI mangling rules."
                                             : (status == -3 ? "One of the arguments is invalid." : "Unknown Error")))
             + ")";
    // LCOV_EXCL_STOP

    demangled_name = std::string{std::addressof(*demangled_name_ptr)};
#else  // e.g. MSVC
    demangled_name = typeid(type).name();
#endif // defined(__GNUC__) || defined(__clang__)

    if constexpr (std::is_const_v<std::remove_reference_t<type>>)
        demangled_name += " const";
    if constexpr (std::is_lvalue_reference_v<type>)
        demangled_name += " &";
    if constexpr (std::is_rvalue_reference_v<type>)
        demangled_name += " &&";

    return demangled_name;
}();

} // namespace hibf::detail
