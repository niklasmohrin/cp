struct AhoCorasick {
    constexpr static uint A = 26;

    static uint to_num(char c) { return c - 'a'; }

    struct Node {
        array<uint, A> next = {};
        uint matches_here = 0;
    };

    constexpr static uint root = 0;
    vector<Node> trie;

    AhoCorasick() : trie(1) {}

    void add_string(string_view s) {
        uint v = root;
        for (auto c : s) {
            c = to_num(c);
            if (!trie[v].next[c]) {
                trie[v].next[c] = trie.size();
                trie.emplace_back();
            }
            v = trie[v].next[c];
        }
        ++trie[v].matches_here;
    }
    void finalize() {
        queue<array<uint, 4>> q;
        q.push({0, 0, 0, 0});
        while (!q.empty()) {
            auto [v, p, plink, pch] = q.front();
            q.pop();

            const auto link = p == 0 ? 0 : trie[plink].next[pch];
            urep(c, A) {
                if (trie[v].next[c]) q.push({trie[v].next[c], v, link, c});
                else trie[v].next[c] = trie[link].next[c];
            }

            trie[v].matches_here += trie[link].matches_here;
        }
    }
    uint count_matches(string_view text) {
        uint v = root;
        uint count = trie[v].matches_here;
        for (auto c : text) {
            c = to_num(c);
            v = trie[v].next[c];
            count += trie[v].matches_here;
        }
        return count;
    }
};
