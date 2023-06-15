template <const ull MOD, const ull prim_root> struct NTT {
    constexpr static ull modmul(ull a, ull b) {
        return (a * b) % MOD;
        // ll ret = a * b - MOD * ull(1.L / MOD * a * b);
        // return ret + MOD * (ret < 0) - MOD * (ret >= (ll)MOD);
    }
    constexpr static ull modpow(ull b, ull e) {
        ull ans = 1;
        for (; e; b = modmul(b, b), e /= 2)
            if (e & 1) ans = modmul(ans, b);
        return ans;
    }
    constexpr static ull modinv(ull x) { return modpow(x, MOD - 2); }

    static_assert(modpow(prim_root, MOD - 1) == 1);

    using Polynomial = vector<ull>;
    using PointForm = Polynomial;

    static void ntt_iter(Polynomial &a) {
        const auto n = a.size();
        debug_assert(has_single_bit(n));
        const auto log_n = bit_width(n) - 1;
        for (uint i = 0, j = 0; i < n - 1; ++i) {
            if (i < j) swap(a[i], a[j]);
            const uint x = countr_one(i) + 1;
            j ^= ((1 << x) - 1) << (log_n - x);
        }
        for (uint k = 1; 2 * k <= n; k *= 2) {
            debug_assert((MOD - 1) % (2 * k) == 0);
            const auto zn = modpow(prim_root, (MOD - 1) / (2 * k));
            for (uint i = 0; i < n; i += 2 * k) {
                ull z = 1;
                urep(j, k) {
                    const auto u = a[i + j], v = a[i + j + k];
                    const auto vv = modmul(v, z);
                    a[i + j] = (u + vv) % MOD;
                    a[i + j + k] = (u + (MOD - vv)) % MOD;
                    z = modmul(z, zn);
                }
            }
        }
    }

    // Repeated application
    static void ntt_q(Polynomial &a, ull q) {
        const auto factor = modpow(a.size(), ((q / 4) * 2));
        for (auto &x : a) x = modmul(x, factor);
        urep(_, q % 4) ntt_iter(a);
    }
};
