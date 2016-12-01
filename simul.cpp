#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <queue>
#include <tuple>
using namespace std;

vector<int> dijkstra(const vector<pair<int,int>> graph[], int size, int start)
{
	vector<int> weight(size, 1e9);
	weight[start] = 0;

	priority_queue<pair<int,int>> pq;
	pq.emplace(0, start);

	while (pq.empty() == false)
	{
		int v; int w;
		w = -pq.top().first;
		v = pq.top().second;
		pq.pop();

		if (weight[v] < w) continue;

		for (auto &&edge : graph[v])
		{
			int nv = edge.first;
			int nw = w + edge.second;

			if (weight[nv] > nw)
			{
				weight[nv] = nw;
				pq.emplace(-nw, nv);
			}
		}
	}

	return weight;
}

const int width = 1000; // width of map
const int height = 500; // height of map
int map8 [height][width], map7[height][width];
int h, w; // 직사각형 꼴의 map이 입력된다고 가정하고 map의 실제 h와 w를 readTextMap에서 계산
vector<pair<int,int> > g[width*height*2]; // graph
vector<int> exits, shortest, percentage, exitdist;
int peoplecnt;

const int WALL = 0;
const int BLANK = 1;
const int PERSON = 2;
const int STAIRS = 3;

// testfile에서 map을 읽어 2d-array에 저장
void readTextMap (int map_[height][width], string fileName) {
    string line;
    ifstream file (fileName);
    if (file.is_open()) {
        h=0;
        while ( getline (file,line) ) {
            h++;
            w=0;
            for(char& c : line) {
                w++;
                if (c==' ')
                    map_[h][w] = BLANK;
                else if (c=='.') {
                    map_[h][w] = PERSON; peoplecnt++;
                }
                else if (c=='#')
                    map_[h][w] = WALL;
                else if (c=='X') {
                    cout << "stair at (" << w << "," << h << ")" << endl; // print as (x,y) foramt
                    map_[h][w] = STAIRS;
                }
                else
                    cout << "unexpected character at (" << w << "," << h << ")" << endl;
            }
        }
        file.close();
    }
}
// print 2d-array map
void showMap (int map_[height][width]) {
    if (h<1 || w<1) {
        cout << "call readTextMap to set map_" <<endl;;
        return;
    }
    for (int i=1; i<=h; i++) {
        for (int j=1; j<=w; j++) {
            cout << map8[i][j];
        }
        cout << endl;
    }
}
// 각 층의 offset과 x,y좌표를 이용하여 그래프에서의 해당 노드 index 구하기
// i corresponds to y (not x)
int idx(int i, int j, int off) {
    if (h<1 || w<1) {
        cout << "call readTextMap to set map_" <<endl;;
        return -1;
    }
    return j-1 + (i-1)*w + off;
}
void setGraph (int map_[height][width], int off) {
    for (int i=1; i<=h; i++) {
        for (int j=1; j<=w; j++) {
            // if horizontally connected
            if (j<w && map_[i][j]!=WALL && map_[i][j+1]!=WALL) {
                g[idx(i,j,off)].push_back(make_pair(idx(i,j+1,off),1));
                g[idx(i,j+1,off)].push_back(make_pair(idx(i,j,off),1));
            }
            // if vertically connected
            if (i<h && map_[i][j]!=WALL && map_[i+1][j]!=WALL) {
                g[idx(i,j,off)].push_back(make_pair(idx(i+1,j,off),1));
                g[idx(i+1,j,off)].push_back(make_pair(idx(i,j,off),1));
            }
        }
    }
}
// set edges between stairs
// @param off means offset of index between 8th and 7th
void setStair (string fileName, int off) {
    string line;
    ifstream file (fileName);
    if (file.is_open()) {
        getline(file,line); // throw dummy line away
        int x1, y1, x2, y2, len;
        cout << " index of stairs : " << endl;
        while ( file >> x1 >> y1 >> x2 >> y2 >> len ) {
            int idx8 = idx(y1,x1,0), idx7 = idx(y2,x2,off); // index of stair in each floor
            printf("(%d,%d):%d in 8th\t(%d,%d):%d in 7th\n",x1,y1,idx8,x2,y2,idx7);
            // TODO: how to decide weight of edge between stairs using length?
            g[idx8].push_back(make_pair(idx7, len));
            g[idx7].push_back(make_pair(idx8, len));
        }
        file.close();
    }
}
// set exits
// @param filename
void setExit (string filename, int off)
{
    string line;
    ifstream file (filename);
    if (file.is_open()) {
        getline(file,line); // throw dummy line away
        int x, y, dist, percent;
        cout << " index of exits : " << endl;
        while ( file >> x >> y >> dist >> percent ) {
            int idx7 = idx(y,x,off);
            printf("(%d,%d)\n",x,y);
            exits.push_back(idx7);
            percentage.push_back(percent);
            exitdist.push_back(dist);
        }
    }
}

int main () {
    cout << " - read 8th.txt - " << endl;
    readTextMap(map8, "8th.txt");
    cout << " - read 7th.txt - " << endl;
    readTextMap(map7, "7th.txt");
    // showMap(map8);
    // showMap(map7);

    /** set graph **/
    int offset = 0;
    cout << " - build graph for 8th - " << endl;
    setGraph(map8, offset);
    offset = w * h;
    cout << "8th has " << offset << " nodes" << endl;
    cout << "Thus, index of node in 7th starts from " << offset+1 << endl;
    cout << " - build graph for 7th - " << endl;
    setGraph(map7, offset);
    cout << " - build edges of stairs - " << endl;
    setStair("stair.txt", offset);
    setExit("exit.txt", offset);

    if (exits.empty())
    {
        printf("fatal error: no exit.txt or parse failed\n");
        exit(0);
    }

    if (accumulate(percentage.begin(), percentage.end(), 0, [](int a,int b){return max(a,0) + max(b,0);}) > 100)
    {
        printf("fatal error: sum of percentage is greater than 100\n");
        exit(0);
    }

    shortest.resize(exits.size(), 2e9);

    printf("the number of people: %d\n", peoplecnt);

    for (size_t i = 0; i < exits.size(); i++)
    {
        auto res = dijkstra(g, w*h*2, exits[i]);
        for (int j = 1; j <= h; j++)
        {
            for (int k = 1; k <= w; k++)
            {
                if (map8[j][k] == PERSON)
                {
                    shortest[i] = min(shortest[i], res[idx(j,k,0)]) + exitdist[i];
                }

                if (map7[j][k] == PERSON)
                {
                    shortest[i] = min(shortest[i], res[idx(j,k,w*h)]) + exitdist[i];
                }
            }
        }

        printf("shortest dist of exit %d is %d\n", exits[i], shortest[i]);
    }

    int presum = 0;
    int pretime = 0;

    for (size_t i = 0; i < exits.size(); i++)
    {
        if (percentage[i] == -1) continue;

        presum += peoplecnt * percentage[i];
        pretime = max(pretime, peoplecnt * percentage[i] / 100 + shortest[i] - 1);

        while ((pretime - shortest[i] + 1) * 100 < peoplecnt * percentage[i])
            ++pretime;
    }

    // binary search for time
    // it is a little difficult to directly calculate

    int lo = 0, hi = w*h*2, ans = 2e9;

    while (lo <= hi)
    {
        long long total = 0;
        int mid = (lo + hi) / 2;

        // When summing, multiply by 100 so that percentage values can be calculated correctly
        for (size_t i = 0; i < exits.size(); i++)
        {
            int t = shortest[i];
            if (percentage[i] == -1) total += (mid - t + 1) * 100;
            else continue;
        }

        if (total >= peoplecnt * 100 - presum)
        {
            ans = mid;
            hi = mid - 1;
        }
        else
        {
            lo = mid + 1;
        }
    }

    printf("%d unit time(s) needed\n\n", max(ans, pretime));

    for (size_t i = 0; i < exits.size(); i++)
    {
        printf("people count of exit %d: ", exits[i]);
        if (percentage[i] != -1)
        {
            printf("%d\n", peoplecnt * percentage[i] / 100);
        }
        else
        {
            printf("%d\n", ans - shortest[i] + 1);
        }
    }

    return 0;
}
