#include<cstdio>
#include<vector>
#include<algorithm>

#include"network.h"

network<1000> apln(3,{{1,2},{1,3},{2,3}}, {1,2,3});

int main() {
    apln.print();
    puts("");
    puts("");
    for (ll g=1; g<=3; ++g) {
        VII hubs = apln.triples();
        for (ll k = 0; k <= 2; ++ k) {
            printf("hubs: %lld %lld %lld\n",hubs[k][0],hubs[k][1],hubs[k][2]);
        }
        apln.merge_2_nodes( hubs[0][0], hubs[1][0] );
        apln.merge_2_nodes( hubs[0][1], hubs[2][1] );
        apln.merge_2_nodes( hubs[1][1], hubs[2][0] );

        //apln.print();
        //break;

        apln.merge_3_nodes( hubs[0][2], hubs[1][2], hubs[2][2]);
        apln.print();
    }
    return 0;
}
