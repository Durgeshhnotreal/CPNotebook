struct PST {
    vector<ll> st;
    vector<int> le, ri, root;
    int n, timer = 0, nnode = 0;
    const int maxlog = 20;

    PST (int _n = 0) {
        n = _n;
        st.assign(n * maxlog + 1, 0);
        le.assign(n * maxlog + 1, 0);
        ri.assign(n * maxlog + 1, 0);
        root.assign(n + 1, 0);
    }

    int upd(int id, int l, int r, int pos, ll v) {
        int new_id = ++nnode;
        st[new_id] = st[id];
        le[new_id] = le[id];
        ri[new_id] = ri[id];

        if (l == r) {
            st[new_id] += v;
            return new_id;
        }

        int mid = (l + r) >> 1;
        if (pos <= mid) 
            le[new_id] = upd(le[id], l, mid, pos, v);
        else 
            ri[new_id] = upd(ri[id], mid + 1, r, pos, v);

        st[new_id] = st[le[new_id]] + st[ri[new_id]];
        return new_id;
    }

    ll get(int id, int l, int r, int u, int v) {
        if (r < u || v < l) return 0;
        if (u <= l && r <= v) return st[id];

        int mid = (l + r) >> 1;
        return get(le[id], l, mid, u, v) +
                get(ri[id], mid + 1, r, u, v);
    }

    void update(int pos, int v) {
        timer++;
        root[timer] = upd(root[timer - 1], 1, n, pos, v);
    }

    ll get_time(int l, int r, int k) {
        return get(root[k], 1, n, l, r);
    }
};