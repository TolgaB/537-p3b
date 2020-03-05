
#include "rand.h"

int currSeed;
int firstRun;


int xv6_rand(void) {
	int randNum = currSeed;
	if (firstRun == 0) {
		//we use 1 then
		randNum = 1;
		firstRun = 1;
	}
	randNum ^= (randNum << 13);
	randNum ^= (randNum >> 7);
	randNum ^= (randNum << 17);
	//idk about this
	currSeed = randNum;
	if ((randNum < 0) || (randNum > XV6_RAND_MAX)) {
		randNum = xv6_rand();
	}
	return randNum;
}

void xv6_srand(unsigned int seed) {
	firstRun = 1;
	currSeed = seed;
}


/*
#include "rand.h"

unsigned int seq[100] = {0x1, 0x2, 0x3, 0x4,
                        0x74b0dc51, 0x19495cff, 0x2ae8944a, 0x625558ec,
                        0x238e1f29, 0x46e87ccd, 0x3d1b58ba, 0x507ed7ab,
                        0x2eb141f2, 0x41b71efb, 0x79e2a9e3, 0x7545e146,
                        0x515f007c, 0x5bd062c2, 0x12200854, 0x4db127f8,
                        0x216231b, 0x1f16e9e8, 0x1190cde7, 0x66ef438d,
                        0x140e0f76, 0x3352255a, 0x109cf92e, 0xded7263,
                        0x7fdcc233, 0x1befd79f, 0x41a7c4c9, 0x6b68079a,
                        0x4e6afb66, 0x25e45d32, 0x519b500d, 0x431bd7b7,
                        0x3f2dba31, 0x7c83e458, 0x257130a3, 0x62bbd95a,
                        0x436c6125, 0x628c895d, 0x333ab105, 0x721da317,
                        0x2443a858, 0x2d1d5ae9, 0x6763845e, 0x75a2a8d4,
                        0x8edbdab, 0x79838cb2, 0x4353d0cd, 0xb03e0c6,
                        0x189a769b, 0x54e49eb4, 0x71f32454, 0x2ca88611,
                        0x836c40e, 0x2901d82, 0x3a95f874, 0x8138641,
                        0x1e7ff521, 0x7c3dbd3d, 0x737b8ddc, 0x6ceaf087,
                        0x22221a70, 0x4516dde9, 0x3006c83e, 0x614fd4a1,
                        0x419ac241, 0x5577f8e1, 0x440badfc, 0x5072367,
                        0x3804823e, 0x77465f01, 0x7724c67e, 0x5c482a97,
                        0x2463b9ea, 0x5e884adc, 0x51ead36b, 0x2d517796,
                        0x580bd78f, 0x153ea438, 0x3855585c, 0x70a64e2a,
                        0x6a2342ec, 0x2a487cb0, 0x1d4ed43b, 0x725a06fb,
                        0x2cd89a32, 0x57e4ccaf, 0x7a6d8d3c, 0x4b588f54,
                        0x542289ec, 0x6de91b18, 0x38437fdb, 0x7644a45c,
                        0x32fff902, 0x684a481a, 0x579478fe, 0x749abb43};
unsigned int pos = 0;
void xv6_srand(unsigned int seed) {
    return;
}


int xv6_rand() {
    if (pos >= 100) return 0;
    else return seq[pos ++] % XV6_RAND_MAX;
}
*/
