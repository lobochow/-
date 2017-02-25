#include <vector>

#ifndef BLOCK
#define BLOCK


using namespace std;

class block{
public:
	block(int,int,int,int);
	vector<int> locs;
};

block::block(int a, int b, int c, int d)
{
	locs.push_back(a);
	locs.push_back(b);
	locs.push_back(c);
	locs.push_back(d);
}

#endif
