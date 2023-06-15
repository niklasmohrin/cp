struct Treap {
    struct Lazy {
        bool reversed = false;

        bool operator==(const Lazy &) const = default;
    };
    struct Agg {
        ull sum;
        ll max;
    };

    static inline mt19937 gen;
    static inline uniform_int_distribution<uint> dist;

    struct Node {
        uint value;
        uint priority;

        Node *left = nullptr, *right = nullptr;
        Agg agg;
        Lazy lazy{};
        uint size = 1;

        Node(uint _value) : value(_value), priority(dist(gen)), agg({value, value}) {}

        void push_lazy() {
            if (lazy.reversed) {
                swap(left, right);
                if (left) left->lazy.reversed ^= 1;
                if (right) right->lazy.reversed ^= 1;
            }
            lazy = Lazy{};
        }
        void update_agg() {
            debug_assert(lazy == Lazy{});

            agg.sum = value;
            agg.max = value;
            size = 1;
            if (left) {
                agg.sum += left->agg.sum;
                agg.max = max(agg.max, left->agg.max);
                size += left->size;
            }
            if (right) {
                agg.sum += right->agg.sum;
                agg.max = max(agg.max, right->agg.max);
                size += right->size;
            }
        }

        const Node *get_kth(uint k) {
            debug_assert(1 <= k);
            debug_assert(k <= size);
            push_lazy();
            if (k <= sz(left)) return left->get_kth(k);
            else if (k == 1 + sz(left)) return this;
            else return right->get_kth(k - 1 - sz(left));
        }
        void update_kth(uint k, uint new_value) {
            debug_assert(1 <= k);
            debug_assert(k <= size);
            push_lazy();
            if (k <= sz(left)) left->update_kth(k, new_value);
            else if (k == 1 + sz(left)) value = new_value;
            else right->update_kth(k - 1 - sz(left), new_value);
            update_agg();
        }
    };

    static Node *merge(Node *a, Node *b) {
        if (!a) return b;
        if (!b) return a;

        if (a->priority < b->priority) {
            a->push_lazy();
            a->right = merge(a->right, b);
            a->update_agg();
            return a;
        } else {
            b->push_lazy();
            b->left = merge(a, b->left);
            b->update_agg();
            return b;
        }
    }

    static uint sz(Node *v) { return v ? v->size : 0; }
    static pair<Node *, Node *> split_size(Node *v, uint left_size) {
        if (!v) return {nullptr, nullptr};

        v->push_lazy();

        if (1 + sz(v->left) <= left_size) {
            auto [rl, rr] = split_size(v->right, left_size - 1 - sz(v->left));
            v->right = rl;
            v->update_agg();
            return {v, rr};
        } else {
            auto [ll, lr] = split_size(v->left, left_size);
            v->left = lr;
            v->update_agg();
            return {ll, v};
        }
    }

    Node *root = nullptr;

    Treap(span<uint> initial_vals) {
        for (auto v : initial_vals) root = merge(root, new Node(v));
    }

    static tuple<Node *, Node *, Node *> extract_inner_range(Node *v, Range range) {
        auto [lm, r] = split_size(v, range.end - 1);
        auto [l, m] = split_size(lm, range.start - 1);
        return {l, m, r};
    }

    void insert_after(uint pos, uint value) { root = insert_all_after(root, pos, new Node(value)); }
    static Node *insert_all_after(Node *v, uint pos, Node *vals) {
        auto [l, r] = split_size(v, pos);
        return merge(merge(l, vals), r);
    }
    void remove(uint pos) {
        auto [l, single, r] = extract_inner_range(root, {pos, pos + 1});
        debug_assert(sz(single) == 1);
        dbg("Removed node with value {}\n", single->value);
        root = merge(l, r);
    }
    const Node *at(uint pos) { return root->get_kth(pos); }
    void update(uint pos, uint value) { root->update_kth(pos, value); }

    Agg range_query(Range range) {
        auto [l, m, r] = extract_inner_range(root, range);
        Agg result = m ? m->agg : Agg{};
        root = merge(l, merge(m, r));
        return result;
    }

    void move_range(Range range, uint to_pos) {
        auto [l, m, r] = extract_inner_range(root, range);
        root = merge(l, r);
        root = insert_all_after(root, to_pos - 1, m);
    }
    void reverse_in_range(Range range) {
        auto [l, m, r] = extract_inner_range(root, range);
        m->lazy.reversed ^= 1;
        root = merge(l, merge(m, r));
    }

    template <typename F> static void visit_in_order(Node *v, F &f) {
        if (!v) return;
        v->push_lazy();
        visit_in_order(v->left, f);
        f(*v);
        visit_in_order(v->right, f);
    }
    void debug_print_all() {
#ifdef COMPPROG_LOCAL
        auto visit = [](Node &v) { dbg("{} ", v.value); };
        visit_in_order(root, visit);
#endif
    }
};
