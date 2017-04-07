#include<cstdio>
#include<vector>
#include<algorithm>

#include"network.h"

network<1000> dual(1,{}, {1,1,1});

int main() {
    puts("[Dual of Appollonian]\n");
    for (ll g=1; g<=3; ++g) {
        VII hubs = dual.triples();
        for (ll k = 0; k <= 2; ++ k) {
            //printf("hubs: %lld %lld %lld\n",hubs[k][0],hubs[k][1],hubs[k][2]);
        }
        dual.add_edge( hubs[0][0], hubs[1][0] );
        dual.add_edge( hubs[0][1], hubs[2][1] );
        dual.add_edge( hubs[1][1], hubs[2][0] );
        dual.set_hubs( {hubs[0][2], hubs[1][2], hubs[2][2]} );

        dual.push_node( dual.hubs );

        PII size = dual.get_size();

        printf("Generation %lld:\n\n", g);
        printf("|V| = %lld, |E| = %lld, 2|E| = %lld\n\n",size.first,size.second/2,size.second);

        PII ans  = dual.get_mds(22);

        printf("|MDS| = %lld, # of MDSes = %lld\n\n", ans.first, ans.second);

        dual.pop_node();

        ans  = dual.get_mds(22);

        printf("|MDS with external node removed| = %lld, # of those MDSes = %lld\n\n", ans.first, ans.second);

        /*
        for (ll del = 1; del <= 3; ++ del) {
            PII ans  = dual.get_mds(22, del);
            printf("|MDS with %lld hubs and external node removed| = %lld, # of those MDSes = %lld\n\n", del, ans.first, ans.second);
        }
        */

        for (ll nds_ = 0; nds_ <= 3; ++ nds_) {
            PII ans  = dual.get_mds(25, 0, nds_);
            printf("|MDS containing exact %lld hubs, with external node removed | = %lld, # of those MDSes = %lld\n\n", nds_, ans.first, ans.second);
        }
        puts("");

    }
    return 0;
}
