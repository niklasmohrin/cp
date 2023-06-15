constexpr ull base = 28;
constexpr ull base_inv = 535714297;
constexpr ull mod = 1000000021;
constexpr array<ull, 32> a_powers = ([]() {
    array<ull, 32> res;
    res[0] = base;
    urep(i, 31) res[i + 1] = (res[i] * res[i]) % mod;
    return res;
})();
auto to_num = [](char c) { return 1ull + c - 'a'; };

auto pow_a = [&a_powers](auto exp) {
    ull res = 1;
    urep(i, 32) {
        if (exp & 1) res = (res * a_powers[i]) % mod;
        exp >>= 1;
    }
    return res;
};

string s;
ull hash = 0;
rep(i, len) hash = (((hash * base) % mod) + to_num(s[len - 1 - i])) % mod;
