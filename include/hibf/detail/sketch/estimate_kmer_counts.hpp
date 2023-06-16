#pragma once

#include <cstddef> // for size_t
#include <vector>  // for vector

#include <hibf/detail/sketch/hyperloglog.hpp> // for hyperloglog

namespace hibf::sketch
{

inline void estimate_kmer_counts(std::vector<hibf::sketch::hyperloglog> const & sketches,
                                 std::vector<size_t> & kmer_counts)
{
    kmer_counts.resize(sketches.size());

    for (size_t i = 0; i < sketches.size(); ++i)
        kmer_counts[i] = sketches[i].estimate();
}

} // namespace hibf::sketch
