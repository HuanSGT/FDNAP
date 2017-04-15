#include<cstdio>
#include<vector>
#include<algorithm>
#include<cstring>

typedef long long ll;

typedef std :: pair<ll,ll> PII;
typedef std :: vector<PII> VP;

typedef std :: vector<ll> VI;
typedef std :: vector<VI> VII;

typedef std :: vector<bool> VB;

typedef VI :: iterator it;
typedef VP :: iterator it_p;

bool occurs(ll x,VI& v,ll len) {
    return std :: find(v.begin(),v.begin() + len,x) != v.begin() + len;
}

bool is_last(ll x,VI& v,ll len) {
    return std :: find(v.end() - len,v.end(),x) != v.end();
}

template<ll maxn>
struct network {
    ll n, m;
    ll deg[maxn];
    std :: vector<ll> nbr[maxn];
    std :: vector<ll> hubs,waist;
    std :: vector<ll> reds;

    ll fa[maxn]; //Disjoint-set; for merging two nodes
    ll getf(ll i) {
        return fa[i] == i ? i : (fa[i] = getf(fa[i]));
    }

    void get_nbr(ll i) {
        for (ll& j : nbr[i]) j = getf(j);
        sort( nbr[i].begin(), nbr[i].end() );
        nbr[i].erase( unique( nbr[i].begin(), nbr[i].end() ), nbr[i].end() );
        deg[i] = nbr[i].size();
    }

    network(ll n_ = 0, VP edges = {}, VI hubs_ = {}, VI waist_ = {}): n(n_), hubs(hubs_),waist(waist_) {
        for (ll i = 1; i <= n; ++i ) {
            fa[i] = i; deg[i] = 0;
        }
        for (PII e : edges) {
            ll i = e.first, j = e.second;
            nbr[i].push_back(j);
            nbr[j].push_back(i);
        }
        for (ll i = 1; i <= n; ++ i) {
            get_nbr(i);
        }
    }

    void set_hubs(VI hubs_) {
        hubs = hubs_;
        for (ll& i : hubs) i = getf(i);
    }

    void set_waist(VI waist_) {
        waist = waist_;
        for (ll& i : waist) i = getf(i);
    }

    void add_edge(ll i,ll j) { // sort the whold nbr[i],nbr[j] every time
        nbr[i].push_back(j); nbr[j].push_back(i);

        get_nbr(i); get_nbr(j);
    }

    void push_node(VI nbrs) {
        ++ n;
        fa[n] = n; deg[n] = 0;
        for (ll j : nbrs) {
            j = getf(j);
            nbr[n].push_back(j);
            nbr[j].push_back(n); ++ deg[j];
        }
        get_nbr(n);
    }
    void pop_node() {
        for (ll j : nbr[n]) {
            j = getf(j);
            nbr[j].pop_back(); --deg[j];
        }
        -- n;
    }

    void merge_2_nodes(ll i,ll j) { // i = i `meg` j; j is removed; sort the whold nbr[i],nbr[k]-s every time
        i = getf(i); j = getf(j);
        fa[j] = i;
        nbr[i].insert( nbr[i].end(), nbr[j].begin(), nbr[j].end() );
        get_nbr(i);
        for (ll k : nbr[j]) get_nbr(k);
    }

    void merge_3_nodes(ll i,ll j,ll k) { // i = i `meg` j `meg` k; j and k is removed
        merge_2_nodes(j,k);
        merge_2_nodes(i,j);
    }

    VII triples() {
        if (n * 3 > maxn) {
            puts("Number of nodes exceeds");
            return {};
        }
        for (ll i = 1; i <= n; ++i) {
            nbr[i + n] = nbr[i + n*2] = nbr[i];
            fa[i + n]   = getf(i) + n;
            fa[i + 2*n] = getf(i) + 2*n;
            deg[i + n]   = deg[i];
            deg[i + 2*n] = deg[i];
            for (ll& j : nbr[i+n])   j = getf(j) + n;
            for (ll& j : nbr[i+n*2]) j = getf(j) + n*2;
        }
        VII ret = {hubs};
        ret.push_back( {hubs[0] + n,   hubs[1] + n,   hubs[2] + n  } );
        ret.push_back( {hubs[0] + 2*n, hubs[1] + 2*n, hubs[2] + 2*n} );

        n = n * 3;

        return ret;
    }

    PII get_size() {
        ll v = 0, e = 0;
        for (ll i = 1; i <= n; ++ i) if (fa[i] == i) {
            ++ v;
            e += deg[i];
        }
        return {v, e};
    }

    void print() {
        ll v = 0, e = 0;
        puts("degree distribution:");
        for (ll i = 1; i <= n; ++ i) if (fa[i] == i) {
            ++ v;
            e += deg[i];
            printf("%lld\n",deg[i]);
            for (ll j : nbr[i]) printf("%lld %lld\n",i,getf(j));
        }
        printf("|V| = %lld, 2|E| = %lld\n",v,e);
        puts("");
    }

    ll match, way_match, del_match;
    bool matched[maxn];
    ll in_match[maxn];

    PII get_match(ll upper_bound = 0, ll im = -1) {

        //VI tmp_fa = {fa[hubs[0]], fa[hubs[1]], fa[hubs[2]]};
        //for (ll i = 0; i < del; ++i) fa[hubs[i]] = 0;

        match = upper_bound; way_match = 0;
        for (ll i = 1; i <= n; ++i) matched[i] = false, in_match[i] = 0;

        if (im >= 0) {
            for (ll i = 0; i < im; ++i) in_match[hubs[i]] = 1;
            for (ll i = im; i < 3; ++i) in_match[hubs[i]] = -1;
        }

        dfs_match(1,0);

        for (ll i = 0; i < 3; ++i) in_match[hubs[i]] = 0;

        ///for (ll i = 0; i < del; ++i) fa[hubs[i]] = tmp_fa[i];
        
        return {match, way_match};
    }

    void dfs_match(ll i, ll tot) {
        //if (n - i + 1 + tot < match) return;
        if (i > n) {
            for (ll k = 1; k <= n; ++k) if ((in_match[k] == 1 && !matched[k]) || (in_match[k] == -1 && matched[k])) return;
            if (tot > match) { match = tot; way_match = 1; }
            else if (tot == match) { ++ way_match; }
            return;
        }
        if (fa[i] != i || matched[i]) {
            dfs_match(i + 1, tot);
            return;
        }
        //if (in_match[i] >= 0) {
            for (ll j : nbr[i]) if (i < j && fa[j] == j && !matched[j] && in_match[j] >= 0) {
                matched[i] = matched[j] = true;
                dfs_match(i + 1, tot + 1);
                matched[i] = matched[j] = false;
            }
        //}
        //if (in_match[i] <= 0)
        dfs_match(i + 1, tot);
    }

    ll mlst, way_mlst, way_hubs[4], way_waist[4],dst[maxn];
    ll xmark, black;
    VP edges;
    VI degn;

    ll nn,pa[maxn];
    ll getp(ll i) {
        return i == pa[i] ? i : getp(pa[i]);
    }

    void dfs_mlst(it_p e, ll en) {
        if (e == edges.end()) {
            if (en != nn - 1) return;
            ll f0 = getp(reds[0]), f1 = getp(reds[1]);
            //if (f0 == f1) return;
            ll ln = 0, wn = 0, hn = 0;
            VI dn(20,0);
            for (ll i = 1; i <= n; ++i) if (fa[i] == i) {
                if (dst[i] == 1 && !occurs(i,hubs,black)) {
                    ++dn[deg[i]];
                    ++ ln;
                    if (i == waist[0] || i == waist[1] || i == waist[2]) ++ wn;
                    if (i == hubs[0] || i == hubs[1] || i == hubs[2]) ++ hn;
                }
            }
            //for (ll k = 0; k < black; ++ k) if (dst[hubs[k]] == 1) -- ln;
            if (ln > mlst) {
                mlst = ln; way_mlst = 1;
                way_waist[0] = way_waist[1] = way_waist[2] = way_waist[3] = 0;
                way_hubs[0] = way_hubs[1] = way_hubs[2] = way_hubs[3] = 0;
                ++way_waist[wn];
                ++way_hubs[hn];
                for (ll k=0; k<20; ++k) degn[k] = dn[k];
            }
            else if (ln == mlst) {
                ++ way_mlst;
                ++way_waist[wn];
                ++way_hubs[hn];
                for (ll k=0; k<20; ++k) degn[k] += dn[k];
            }
            return;
        }
        ll i = e->first, j = e->second, pi = getp(i), pj = getp(j), tmp;
        if (edges.end() - e - 1 + en >= nn - 1) dfs_mlst(e+1,en);
        if (en < nn - 1 && pi != pj) {
            tmp = pa[pj]; pa[pj] = pi;
            ++dst[i]; ++dst[j];
            dfs_mlst(e + 1, en + 1);
            --dst[i]; --dst[j];
            pa[pj] = tmp;
        }
    }

    void print_mlst() {

        for (xmark = 0; xmark < 2; ++ xmark) { // 3; ++ xmark) {
            for (black = 0; black <= 0; ++black) { // black <= (xmark <= 1 ? 2 : 1); ++ black) {

                edges = {};
                nn = 0;
                memset(deg,0,sizeof(deg));
                for (ll i = 1; i <= n; ++i) if (fa[i] == i && !is_last(i,hubs,xmark)) {
                    dst[i] = 0;
                    ++ nn;
                    pa[i] = i;
                    for (ll j : nbr[i]) if (j == fa[j] && !is_last(j,hubs,xmark) && i < j) {
                        edges.push_back({i,j});
                        printf("(%lld,%lld)\n",i,j);
                        ++ deg[i]; ++ deg[j];
                    }
                }
                for (ll i = 1; i <= n; ++i) if (fa[i] == i) { printf("%lld ",deg[i]); } puts("");

                //for (PII e : edges) printf("(%lld,%lld)\n",e.first,e.second);
                //printf("nn=%lld\n",nn);
                nn;

                mlst = 0; way_mlst = 0;
                way_waist[0] = way_waist[1] = way_waist[2] = way_waist[3] = 0;
                way_hubs[0] = way_hubs[1] = way_hubs[2] = way_hubs[3] = 0;
                degn = VI(20,0);
                reds = {hubs[0],hubs[1]};
                if (black) reds = {hubs[1],hubs[2]};
                //reds = black == 0 ? {hubs[0],hubs[1]} : {hubs[1],hubs[2]};
                dfs_mlst(edges.begin(), 0);
                printf("[%lld %lld w(%lld,%lld,%lld,%lld) h(%lld,%lld,%lld,%lld)]] ",mlst,way_mlst,way_waist[0],way_waist[1],way_waist[2],way_waist[3],way_hubs[0],way_hubs[1],way_hubs[2],way_hubs[3]);
                for (ll k = 0; k < 20; ++ k) printf("%lld ",degn[k]); puts("");
            }
            puts("");
        }
    }

    ll mds = n, way;
    bool in_ds[maxn];
    ll nds;

    PII get_mds(ll lower_bound = maxn,ll del = 0, ll nds_ = -1) {
        nds = nds_;

        VI tmp_fa = {fa[hubs[0]], fa[hubs[1]], fa[hubs[2]]};
        for (ll i = 0; i < del; ++i) fa[hubs[i]] = 0;


        mds = lower_bound; way = 0;
        for (ll i = 1; i <= n; ++i) in_ds[i] = false;

        dfs_mds(1,0);

        for (ll i = 0; i < del; ++i) fa[hubs[i]] = tmp_fa[i];

        return {mds, way};
    }

    void dfs_mds(ll i,ll tot) {
        if (tot > mds) return;
        if (i > n) {
            bool yes = true;
            for (ll i = 1; i <= n; ++i) if (fa[i] == i && !in_ds[i]) {
                bool flag = false;
                for (ll j : nbr[i]) if (in_ds[j]) {
                    flag = true;
                    break;
                }
                if (!flag) { yes = false; break; }
            }

            if (!yes) return;
            if (tot < mds) { mds = tot; way = 1; }
            else if (tot == mds) { ++ way; }
            return;
        }
        if (fa[i] != i) {
            dfs_mds(i + 1, tot);
            return;
        }

        if (nds >= 0 && std :: find(hubs.begin(),hubs.end(),i) != hubs.end()) {
            if (std :: find(hubs.begin(),hubs.begin() + nds, i) != hubs.begin() + nds) {
                in_ds[i] = true;
                dfs_mds(i + 1, tot + 1);
                in_ds[i] = false;
            }
            else dfs_mds(i + 1, tot);
            return;
        }

        dfs_mds(i + 1, tot);

        in_ds[i] = true;
        dfs_mds(i + 1, tot + 1);
        in_ds[i] = false;
    }
};
