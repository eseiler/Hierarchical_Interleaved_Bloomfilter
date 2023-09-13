#include <vector>

#include <hibf/config.hpp>

struct dna
{
    int rank{0}; // 0 = A, C = 1, G = 2, T = 3
};

int main()
{
    // user_bins stores one dna sequence per user bin
    // You can look at https://docs.seqan.de/seqan3/3-master-user/group__alphabet__nucleotide.html for dna alphabets
    std::vector<std::vector<dna>> user_bins{{{0}, {0}, {0} /*AAA*/}, {{1}, {1}, {1} /*CCC*/}};

    auto my_input = [&](size_t const user_bin_id, seqan::hibf::insert_iterator it)
    {
        auto const & seq = user_bins[user_bin_id];
        for (size_t i = 0; i < seq.size() - 1; ++i)
        {
            // compute 2-mer hashes
            uint64_t hash = 4 * seq[i].rank + seq[i + 1].rank;
            // You can also look at the seqan3::kmer_hash view for hashing
            // https://docs.seqan.de/seqan3/3-master-user/group__search__views.html#ga6e598d6a021868f704d39df73252974f
            it = hash;
        }
    };

    seqan::hibf::config config{.input_fn = my_input};
}
