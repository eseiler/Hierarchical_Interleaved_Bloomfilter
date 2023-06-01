// --------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2023, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2023, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/raptor/blob/main/LICENSE.md
// --------------------------------------------------------------------------------------------------

/*!\file
 * \brief Implements hibf::initialise_build_tree.
 * \author Svenja Mehringer <svenja.mehringer AT fu-berlin.de>
 */

#pragma once

#include <hibf/detail/build/node_data.hpp>
#include <hibf/detail/layout/layout.hpp>

namespace hibf
{

void initialise_build_tree(layout::layout & hibf_layout,
                           lemon::ListDigraph & ibf_graph,
                           lemon::ListDigraph::NodeMap<node_data> & node_map);

} // namespace hibf
