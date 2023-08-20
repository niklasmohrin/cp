struct Vec {
    using K = double;
    K x;
    K y;

    Vec operator+(const Vec &other) const {
        return Vec{x + other.x, y + other.y};
    }
    Vec operator-() const { return Vec{-x, -y}; }
    Vec operator-(const Vec &other) const { return *this + (-other); }

    K dot(const Vec &other) const { return x * other.x + y * other.y; }
    K cross_val(const Vec &other) const { return x * other.y - y * other.x; }
    K len_sqr() const { return this->dot(*this); }
    K len() const { return sqrt(len_sqr()); }
};
