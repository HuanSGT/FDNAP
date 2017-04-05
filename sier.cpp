#include<cstdio>
#include<vector>
#include<algorithm>

#include"network.h"

network<1000> dual(3,{{1,2},{1,3},{2,3}}, {1,2,3});

int main() {
    puts("[Dual of Appollonian]");

    PII size = dual.get_size();

    printf("Generation %lld:\n", 1ll);
    printf("|V| = %lld, |E| = %lld, 2|E| = %lld\n",size.first,size.second/2,size.second);

    PII ans  = dual.get_mds(22);

    printf("|MDS| = %lld, # of MDSes = %lld\n\n", ans.first, ans.second);

    for (ll g=2; g<=4; ++g) {
        VII hubs = dual.triples();
        for (ll k = 0; k <= 2; ++ k) {
            //printf("hubs: %lld %lld %lld\n",hubs[k][0],hubs[k][1],hubs[k][2]);
        }
        dual.merge_2_nodes( hubs[0][0], hubs[1][0] );
        dual.merge_2_nodes( hubs[0][1], hubs[2][1] );
        dual.merge_2_nodes( hubs[1][1], hubs[2][0] );
        dual.set_hubs( {hubs[0][2], hubs[1][2], hubs[2][2]} );

        PII size = dual.get_size();

        printf("Generation %lld:\n", g);
        printf("|V| = %lld, |E| = %lld, 2|E| = %lld\n",size.first,size.second/2,size.second);

        PII ans  = dual.get_mds(9);

        printf("|MDS| = %lld, # of MDSes = %lld\n\n", ans.first, ans.second);
    }
    return 0;
}


