array<ll, 3> gcd_ext(ll a, ll b) {
    if (!a) return {b, 0, 1};
    auto [d, x1, y1] = gcd_ext(b % a, a);
    return {d, y1 - (b / a) * x1, x1};
}

pair<ll, ll> crt(ll a, ll m, ll b, ll n) {
    auto [g, x, y] = gcd_ext(m, n);
    ll l = m / g * n;
    ll ret = (a + ((x * (b - a) / g) % (l / m)) * m) % l;
    return {(b - a) % g ? -1 : (ret + l) % l, l};
}

pair<ll, ll> crt(span<ll const> a, span<ll const> m) {
    ll sol = a[0], l = m[0];
    for (uint i = 1; i < a.size(); i++) {
        tie(sol, l) = crt(sol, l, a[i], m[i]);
        if (sol == -1) break;
    }
    return {sol, l};
}
