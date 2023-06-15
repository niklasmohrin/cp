using C = complex<long double>;
constexpr auto pi = std::numbers::pi_v<C::value_type>;
using Polynomial = vector<C>;
using PointForm = Polynomial;

void fft_iter(Polynomial &a, const span<const C> z) {
    const auto n = a.size();
    const auto log_n = bit_width(n) - 1;
    for (uint i = 0, j = 0; i < n - 1; ++i) {
        if (i < j) swap(a[i], a[j]);
        const uint x = countr_one(i) + 1;
        j ^= ((1 << x) - 1) << (log_n - x);
    }
    for (uint k = 1; 2 * k <= n; k *= 2) {
        for (uint i = 0; i < n; i += 2 * k) {
            urep(j, k) {
                const auto u = a[i + j], v = a[i + j + k];
                a[i + j] = u + v * z[j * n / (2 * k)];
                a[i + j + k] = u - v * z[j * n / (2 * k)];
            }
        }
    }
}

Polynomial multiply(Polynomial a, Polynomial b) {
    const auto na = a.size(), nb = b.size();
    const auto n = bit_ceil(na + nb - 1);
    a.resize(n, 0);
    b.resize(n, 0);

    PointForm z(n);
    urep(i, n) z[i] = polar(C::value_type(1), i * 2 * pi / n);

    fft_iter(a, z);
    fft_iter(b, z);

    PointForm c(n);
    urep(i, n) c[i] = a[i] * b[i] / C(n);
    fft_iter(c, z);

    reverse(c.begin() + 1, c.end());
    c.resize(na + nb - 1);
    return c;
}
