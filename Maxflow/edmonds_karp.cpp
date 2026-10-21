#include <bits/stdc++.h>
using namespace std;

struct edmon_karp {
    using ll = long long;
    struct Edge {
        int to;
        ll c, f;
 
        Edge (int _to = 0, ll _c = 0, ll _f = 0) :
            to(_to), c(_c), f(_f) {}
    };
 
    int n;
    vector<vector<int>> g;
    vector<Edge> e;
    vector<int> pre;
 
    edmon_karp (int _n = 0) {
        n = _n;
        g.assign(n + 1, {});
        pre.assign(n + 1, 0); // pre[v]: index của cạnh bắt đầu từ v và có thể truy ngược lại đỉnh trước đó
    }
 
    void add_edge(int u, int v, int c) {
        g[u].push_back(e.size());
        e.push_back(Edge(v, c, 0));
        g[v].push_back(e.size());
        e.push_back(Edge(u, 0, 0));
    }
 
    bool find_path(int s, int t) {
        fill(pre.begin(), pre.end(), 0);
        pre[s] = -1;
        queue<int> q;
        q.push(s);
 
        while (!q.empty()) {
            int u = q.front(); q.pop();
 
            for(int &i : g[u]) {
                auto [v, c, f] = e[i];
                if (!pre[v] && c > f) {
                    pre[v] = i ^ 1;
                    q.push(v);
 
                    if (v == t) return true;
                }
            }
        }
 
        return false;
    }
 
    ll inc(int s, int t) {
        ll d = 1e15;
        int v = t;
 
        while (v != s) {
            int u = e[pre[v]].to;
            int id = pre[v] ^ 1;
            
            minimize(d, e[id].c - e[id].f);
            v = u;
        }
 
        v = t;
        while (v != s) {
            int u = e[pre[v]].to;
            int id = pre[v] ^ 1;
 
            e[id].f += d;
            e[id ^ 1].f -= d;
            v = u;
        }
 
        return d;
    }
 
    ll maxflow(int s, int t) {
        ll flow = 0;
        while (find_path(s, t)) 
            flow += inc(s, t);
 
        return flow;
    }
};