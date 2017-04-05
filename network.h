#include<cstdio>
#include<vector>
#include<algorithm>

typedef long long ll;

typedef std :: pair<ll,ll> PII;
typedef std :: vector<PII> VP;

typedef std :: vector<ll> VI;
typedef std :: vector<VI> VII;

template<ll maxn>
struct network {
    ll n, m;
    ll deg[maxn];
    std :: vector<ll> nbr[maxn];
    std :: vector<ll> hubs;

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

    network(ll n_ = 0, VP edges = {}, VI hubs_ = {}): n(n_), hubs(hubs_) {
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

    void print() {
        ll v = 0, e = 0;
        puts("degree distribution:");
        for (ll i = 1; i <= n; ++ i) if (fa[i] == i) {
            ++ v;
            e += deg[i];
            printf("%lld\n",deg[i]);
            //for (ll j : nbr[i]) printf("%lld %lld\n",i,getf(j));
        }
        printf("|V| = %lld, 2|E| = %lld\n",v,e);
        puts("");
    }
};
