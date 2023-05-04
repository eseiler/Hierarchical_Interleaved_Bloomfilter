// --------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/raptor/blob/main/LICENSE.md
// --------------------------------------------------------------------------------------------------

#pragma once

#include <atomic>
#include <hibf/std/new>

#include <hibf/detail/build/hibf/node_data.hpp>
#include <hibf/interleaved_bloom_filter.hpp>

namespace hibf
{

// forward
class hierarchical_interleaved_bloom_filter;

} // namespace hibf

namespace hibf
{

template <typename config_type>
struct build_data
{
    alignas(std::hardware_destructive_interference_size) std::atomic<size_t> ibf_number{};
    alignas(std::hardware_destructive_interference_size) std::atomic<size_t> user_bin_number{};

    size_t number_of_user_bins{};
    size_t number_of_ibfs{};

    lemon::ListDigraph ibf_graph{};
    lemon::ListDigraph::NodeMap<node_data> node_map{ibf_graph};

    config_type hibf_config;

    hierarchical_interleaved_bloom_filter * hibf{};
    std::vector<double> fp_correction{};

    size_t request_ibf_idx()
    {
        return std::atomic_fetch_add(&ibf_number, 1u);
    }

    size_t request_user_bin_idx()
    {
        return std::atomic_fetch_add(&user_bin_number, 1u);
    }

    /*!\brief Precompute f_h factors that adjust the split bin size to prevent FPR inflation due to multiple testing.
     * \sa https://godbolt.org/z/zTj1v9W94
     */
    void compute_fp_correction(size_t const requested_max_tb, size_t const num_hash_functions, double const desired_fpr)
    {
        fp_correction.resize(requested_max_tb + 1, 0.0);
        fp_correction[1] = 1.0;

        // std::log1p(arg) = std::log(1 + arg). More precise than std::log(1 + arg) if arg is close to zero.
        double const numerator = std::log1p(-std::exp(std::log(desired_fpr) / num_hash_functions));

        for (size_t split = 2u; split <= requested_max_tb; ++split)
        {
            double const log_target_fpr = std::log1p(-std::exp(std::log1p(-desired_fpr) / split));
            fp_correction[split] = numerator / std::log1p(-std::exp(log_target_fpr / num_hash_functions));
            assert(fp_correction[split] >= 1.0);
        }
    }
};

} // namespace hibf
