// ===================== SEGMENT TREE LAZY =====================
// Chức năng: cộng val vào đoạn [u, v], truy vấn tổng đoạn [u, v]. Chỉ số 0-based.
// Cách dùng:
//   SegTree st(n);          // n phần tử, ban đầu toàn 0
//   SegTree st(a);          // dựng từ vector<long long> a (0-based)
//   st.update(u, v, val);   // a[u..v] += val
//   st.query(u, v);         // trả về tổng a[u..v]
// Đỉnh 1-based thì khai báo SegTree st(n + 1) hoặc dịch chỉ số -1.
// Độ phức tạp: O(log n) mỗi thao tác.
//
// ĐỔI SANG BÀI KHÁC CHỈ SỬA 3 HÀM: pull, apply, push (+ query khi hợp)
//  - Min/Max + cộng đoạn:  t[x] = min/max(t[2x], t[2x+1]);  apply: t[x] += v (KHÔNG nhân độ dài)
//  - Gán đoạn (assign):    lz = giá trị gán + cờ "đã gán" (dùng LLONG_MIN làm "không có"),
//                          apply: t[x] = v * (r-l+1); lz[x] = v;  push kiểm tra lz != LLONG_MIN
//  - Query tổng/min/max:   phần kết hợp 2 con trong query đổi theo phép toán (+, min, max),
//                          giá trị trả về khi ngoài đoạn là phần tử đơn vị (0 / INF / -INF)
#include <bits/stdc++.h>
using namespace std;

struct SegTree {
    using ll = long long;
    int n;
    vector<ll> t, lz;   // t = giá trị nút (tổng), lz = lazy chưa đẩy xuống con

    SegTree(int n = 0) : n(n), t(4 * n + 4, 0), lz(4 * n + 4, 0) {}
    SegTree(const vector<ll>& a) : SegTree((int)a.size()) { build(1, 0, n - 1, a); }

    // Gộp 2 con -> cha
    void pull(int x) { t[x] = t[2 * x] + t[2 * x + 1]; }

    // Áp lazy v lên nút x quản lý [l, r]
    void apply(int x, int l, int r, ll v) { t[x] += v * (r - l + 1); lz[x] += v; }

    // Đẩy lazy của x xuống 2 con
    void push(int x, int l, int r) {
        if (!lz[x]) return;
        int m = (l + r) / 2;
        apply(2 * x, l, m, lz[x]);
        apply(2 * x + 1, m + 1, r, lz[x]);
        lz[x] = 0;
    }

    void build(int x, int l, int r, const vector<ll>& a) {
        if (l == r) { t[x] = a[l]; return; }
        int m = (l + r) / 2;
        build(2 * x, l, m, a); build(2 * x + 1, m + 1, r, a);
        pull(x);
    }

    void update(int x, int l, int r, int u, int v, ll val) {
        if (v < l || r < u) return;
        if (u <= l && r <= v) { apply(x, l, r, val); return; }
        push(x, l, r);
        int m = (l + r) / 2;
        update(2 * x, l, m, u, v, val); update(2 * x + 1, m + 1, r, u, v, val);
        pull(x);
    }

    ll query(int x, int l, int r, int u, int v) {
        if (v < l || r < u) return 0;          // phần tử đơn vị của phép tổng
        if (u <= l && r <= v) return t[x];
        push(x, l, r);
        int m = (l + r) / 2;
        return query(2 * x, l, m, u, v) + query(2 * x + 1, m + 1, r, u, v);
    }

    // Hàm gọi ngoài
    void update(int u, int v, ll val) { update(1, 0, n - 1, u, v, val); }
    ll query(int u, int v) { return query(1, 0, n - 1, u, v); }
};
// ============================================================

// Ví dụ:
// int main() {
//     int n, q; cin >> n >> q;
//     vector<long long> a(n);
//     for (auto& x : a) cin >> x;
//     SegTree st(a);
//     while (q--) {
//         int type, l, r; cin >> type >> l >> r; l--; r--;   // đề 1-based -> 0-based
//         if (type == 1) { long long v; cin >> v; st.update(l, r, v); }
//         else cout << st.query(l, r) << "\n";
//     }
// }