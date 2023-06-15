auto prefix(string_view x) {
    // P[i] is the length of the longest proper prefix of x that is a suffix of
    // x[..i]
    vector<unsigned> P(x.length());
    P[0] = 0;
    for (unsigned i = 1; i < x.length(); i++) {
        auto j = P[i - 1];
        while (j > 0 && x[i] != x[j]) j = P[j - 1];
        if (x[i] == x[j]) j++;
        P[i] = j;
    }
    return P;
}

auto string_z(string_view x) {
    // Z[i] is the length of the longest common prefix of t and t[i..]
    vector<unsigned> Z(x.length(), 0);
    unsigned l = 0, r = 0;
    for (unsigned i = 1; i < x.length(); i++) {
        if (i <= r) Z[i] = min(r - i + 1, Z[i - l]);
        while (i + Z[i] < x.length() && x[Z[i]] == x[i + Z[i]]) Z[i]++;
        if (i + Z[i] - 1 > r) {
            l = i;
            r = i + Z[i] - 1;
        }
    }
    return Z;
}
