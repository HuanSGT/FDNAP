#include<cstdio>
#include<vector>
#include<algorithm>

#include"network.h"

network<1000> dual(3,{{1,2},{1,3},{2,3}}, {1,2,3});

int main() {
    freopen("dual.ans","w",stdout);
    dual.push_node( dual.hubs );
    dual.print();
    dual.pop_node();
    for (ll g=2; g<=3; ++g) {
        VII hubs = dual.triples();
        for (ll k = 0; k <= 2; ++ k) {
            //printf("hubs: %lld %lld %lld\n",hubs[k][0],hubs[k][1],hubs[k][2]);
        }
        dual.add_edge( hubs[0][0], hubs[1][0] );
        dual.add_edge( hubs[0][1], hubs[2][1] );
        dual.add_edge( hubs[1][1], hubs[2][0] );
        dual.set_hubs( {hubs[0][2], hubs[1][2], hubs[2][2]} );

        dual.push_node( dual.hubs );
        dual.print();
        dual.pop_node();
    }
    return 0;
}

