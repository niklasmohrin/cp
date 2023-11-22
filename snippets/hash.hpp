// copied from kactl
struct chash { // large odd number for C
    const uint64_t C = ll(4e18 * acos(0)) | 71;
    ull operator()(ull x) const {
        return __builtin_bswap64(x * C);
    }
};
