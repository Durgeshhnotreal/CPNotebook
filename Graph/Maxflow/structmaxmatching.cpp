struct MaxMatching {
    int m, n, cur; // m: số đỉnh bên trái, n: bên phải
    vector<int> used, matchX;
    vector<vector<int>> g;

    MaxMatching (int _m = 0, int _n = 0) {
        n = _n;
        m = _m;
        used.assign(m + 1, 0);
        matchX.assign(n + 1, 0);
        g.assign(m + 1, {});
    }

    void add(int u, int v) {
        g[u].push_back(v);
    }

    bool dfs(int u) {
        if (used[u] == cur) return 0;
        used[u] = cur;
        for(int v : g[u]) if (!matchX[v] || dfs(matchX[v])) {
            matchX[v] = u;
            return 1;
        }
        return 0;
    }

    int matching() {
        int ans = 0;
        
        vector<int> id(m);
        iota(id.begin(), id.end(), 1);
        random_device rd; mt19937 gen(rd());
        shuffle(id.begin(), id.end(), gen);
        for(int x : id) {
            cur = x;
            ans += dfs(cur);
        }

        return ans;
    }
};