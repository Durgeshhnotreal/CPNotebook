#include <bits/stdc++.h>
using namespace std;
// ===================== DINIC MAX FLOW =====================
// Cách dùng:
//   Dinic g(n);                 // n = số đỉnh (đỉnh đánh số 0..n-1)
//   g.add(u, v, c);             // cạnh có hướng u->v, sức chứa c
//   g.add(u, v, c, c);          // cạnh vô hướng (hoặc truyền thêm cap ngược)
//   long long f = g.flow(s, t); // luồng cực đại từ s đến t
//   g.cut(s) -> vector<bool>    // sau khi chạy flow: true nếu đỉnh thuộc phía S của lát cắt nhỏ nhất
// Đỉnh đánh số từ 1: khai báo Dinic g(n+1).
// Độ phức tạp: O(V^2 E) lý thuyết, thực tế rất nhanh; đồ thị đơn vị: O(E sqrt(V)).
// Muốn xem luồng trên cạnh thứ i (theo thứ tự add): g.e[2*i].f  (luồng đã đẩy = cap ban đầu - e[2*i].c)
struct Dinic {
    using ll = long long;
    struct Edge { int to; ll c; };   // c = sức chứa còn lại
    int n;
    vector<Edge> e;                  // cạnh i và i^1 là cặp xuôi/ngược
    vector<vector<int>> g;           // g[u] = danh sách chỉ số cạnh ra từ u
    vector<int> lvl, it;

    Dinic(int n = 0) : n(n), g(n), lvl(n), it(n) {}

    // Thêm cạnh u->v sức chứa c, cạnh ngược có sức chứa rc (mặc định 0)
    void add(int u, int v, ll c, ll rc = 0) {
        g[u].push_back(e.size()); e.push_back({v, c});
        g[v].push_back(e.size()); e.push_back({u, rc});
    }

    bool bfs(int s, int t) {
        fill(lvl.begin(), lvl.end(), -1);
        lvl[s] = 0;
        queue<int> q; q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int id : g[u])
                if (e[id].c > 0 && lvl[e[id].to] < 0) {
                    lvl[e[id].to] = lvl[u] + 1;
                    q.push(e[id].to);
                }
        }
        return lvl[t] >= 0;
    }

    ll dfs(int u, int t, ll f) {
        if (u == t || !f) return f;
        for (int &i = it[u]; i < (int)g[u].size(); i++) {
            int id = g[u][i], v = e[id].to;
            if (e[id].c > 0 && lvl[v] == lvl[u] + 1) {
                ll d = dfs(v, t, min(f, e[id].c));
                if (d > 0) {
                    e[id].c -= d;
                    e[id ^ 1].c += d;
                    return d;
                }
            }
        }
        return 0;
    }

    ll flow(int s, int t) {
        ll res = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (ll f = dfs(s, t, LLONG_MAX)) res += f;
        }
        return res;
    }

    // Gọi SAU flow(): trả về mảng đánh dấu các đỉnh còn tới được từ s (phía S của min-cut)
    vector<bool> cut(int s) {
        vector<bool> vis(n, false);
        queue<int> q; q.push(s); vis[s] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int id : g[u])
                if (e[id].c > 0 && !vis[e[id].to]) vis[e[id].to] = true, q.push(e[id].to);
        }
        return vis;
    }
};