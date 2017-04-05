#include<cstdio>
#include<vector>
#include<algorithm>

#include"network.h"

network<1000> apln(3,{{1,2},{1,3},{2,3}}, {1,2,3});

int main() {
    puts("[Appollonian]");
    for (ll g=1; g<=4; ++g) {
        VII hubs = apln.triples();
        for (ll k = 0; k <= 2; ++ k) {
            //printf("hubs: %lld %lld %lld\n",hubs[k][0],hubs[k][1],hubs[k][2]);
        }
        apln.merge_2_nodes( hubs[0][0], hubs[1][0] );
        apln.merge_2_nodes( hubs[0][1], hubs[2][1] );
        apln.merge_2_nodes( hubs[1][1], hubs[2][0] );
        apln.merge_3_nodes( hubs[0][2], hubs[1][2], hubs[2][2]);
        apln.set_hubs( {hubs[0][0], hubs[0][1], hubs[1][1]} );

        PII size = apln.get_size();

        printf("Generation %lld:\n", g);
        printf("|V| = %lld, |E| = %lld, 2|E| = %lld\n",size.first,size.second/2,size.second);

        PII ans  = apln.get_mds(9);

        printf("|MDS| = %lld, # of MDSes = %lld\n\n", ans.first, ans.second);
    }
    return 0;
}
