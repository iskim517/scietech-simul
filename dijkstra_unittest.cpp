#include <cstdio>
#include <algorithm>
#include <functional>
#include <cmath>
#include "dijkstra.h"
using namespace std;
vector<pair<int,int>> vt =
{
	{0,0},
	{1,2},
	{3,5},
	{5,0},
	{4,7},
	{9,9}
};

int main()
{
	auto ret = dijkstra<pair<int,int>, double>
		(vt, [](const pair<int,int> &p1, const pair<int,int> &p2)->pair<bool,double>
			{ return {true, pow(hypot(p1.first-p2.first,p1.second-p2.second),2)}; }
		, 0);
	for (double t : ret) printf("%f\n", t);
}
