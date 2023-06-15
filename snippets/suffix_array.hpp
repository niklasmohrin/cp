pair<vector<vector<uint>>, vector<uint>> build_suffix_array(const auto &s) {
    debug_assert(has_single_bit(s.size()));

    vector<uint> order(s.size());
    iota(all(order), 0);
    ranges::sort(order, [&](auto i, auto j) { return s[i] < s[j]; });

    auto equiv = vector(ceil(log2(s.size())) + 1, vector(s.size(), 0u));
    urep(i, s.size() - 1) equiv[0][order[i + 1]] =
        equiv[0][order[i]] + (s[order[i]] != s[order[i + 1]]);

    for (uint k = 0; (1 << k) < s.size(); ++k) {
        const auto &cur_equiv = equiv[k];

        vector<uint> count(s.size(), 0);
        urep(i, s.size()) count[cur_equiv[i]]++;
        partial_sum(all(count), count.begin());

        vector<uint> by_second(s.size());
        urep(i, s.size()) by_second[i] =
            ((((int)order[i] - (1 << k)) % s.size()) + s.size()) % s.size();
        ranges::reverse(by_second);
        for (auto i : by_second) order[--count[cur_equiv[i]]] = i;

        const auto parts = [&](auto i) {
            return pair(cur_equiv[i], cur_equiv[(i + (1 << k)) % s.size()]);
        };

        // ranges::sort(order,
        //              [&](auto i, auto j) { return parts(i) < parts(j); });

        auto &next_equiv = equiv[k + 1];
        urep(i, s.size() - 1) next_equiv[order[i + 1]] =
            next_equiv[order[i]] + (parts(order[i]) != parts(order[i + 1]));
    }

    return pair(move(equiv), move(order));
}

string s;

s += '$';
while (!has_single_bit(s.size())) s += '$';

const auto [_equivs, _suf] = build_suffix_array(s);
const auto &equivs = _equivs;
const auto &suf = _suf;

const auto lcp = [&](uint i, uint j) {
    debug_assert(i != j);

    uint len = 0;
    for (int k = equivs.size() - 1; k >= 0; --k) {
        if (equivs[k][i] == equivs[k][j]) {
            len += 1 << k;
            i += 1 << k;
            j += 1 << k;
        }
    }
    return len;
};
