bool miller_rabin(ull n) {
    assert(n > 1);
    constexpr static array<ll, 12> witnesses = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    if (n < 38) return find(all(witnesses), n) != witnesses.end();

    auto d = n - 1;
    auto s = countr_zero(d);
    d >>= s;

    return ranges::all_of(witnesses, [=](auto witness) {
        ll x = mod_pow(witness, d, n);
        if (x == 1 || x == n - 1) return true;
        rep(_, s - 1) {
            x = (x * x) % n;
            if (x == n - 1) return true;
        }
        return false;
    });
}

ll rho(ll n, ll x0 = 2, ll c = 1) {
    if (n % 2 == 0) return 2;
    auto f = [=](ll x) { return (((x * x) % n) + c) % n; };
    ll x = x0;
    ll y = f(x0);
    ll g = gcd((((x - y) % n) + n) % n, n);
    while (g == 1) {
        x = f(x);
        y = f(f(y));
        g = gcd((((x - y) % n) + n) % n, n);
    }
    return g;
}

void factorize(auto n, auto &callback) {
    static mt19937_64 gen;
    static uniform_int_distribution<> distrib(2, 1000);

    assert(n > 1);
    if (miller_rabin(n)) {
        callback(n);
        return;
    }

    auto d = rho(n, 2, distrib(gen));
    while (d == n) d = rho(n, 2, distrib(gen));

    factorize(d, callback);
    factorize(n / d, callback);
}
