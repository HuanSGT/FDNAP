#include<cstdio>
#include<vector>
#include<algorithm>

#include"network.h"

network<1000> sier(3,{{1,2},{1,3},{2,3}}, {1,2,3}, {0,0,0});

int main() {
    puts("[Sier]\n");

    PII size = sier.get_size();

    printf("Generation %lld:\n\n", 1ll);
    printf("|V| = %lld, |E| = %lld, 2|E| = %lld\n\n",size.first,size.second/2,size.second);

    //PII ans  = sier.get_mds(22);
    
    //printf("|MDS| = %lld, # of MDSes = %lld\n\n", ans.first, ans.second);
    
    sier.print_mlst();

    puts("");

    for (ll g=2; g<=3; ++g) {
        VII hubs = sier.triples();
        for (ll k = 0; k <= 2; ++ k) {
            //printf("hubs: %lld %lld %lld\n",hubs[k][0],hubs[k][1],hubs[k][2]);
        }
        sier.merge_2_nodes( hubs[0][0], hubs[1][0] );
        sier.merge_2_nodes( hubs[0][1], hubs[2][1] );
        sier.merge_2_nodes( hubs[1][1], hubs[2][0] );
        sier.set_hubs( {hubs[0][2], hubs[1][2], hubs[2][2]} );
        sier.set_waist( {hubs[0][0], hubs[0][1], hubs[1][1]} );

        PII size = sier.get_size();

        printf("Generation %lld:\n\n", g);
        printf("|V| = %lld, |E| = %lld, 2|E| = %lld\n\n",size.first,size.second/2,size.second);

        sier.print_mlst();
        
        //PII ans  = sier.get_mds(9);

        //printf("|MDS| = %lld, # of MDSes = %lld\n\n", ans.first, ans.second);
        
        puts("");
    }
    return 0;
}
