#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
using namespace std;

const int sz = 50000; // # of nodes
const int width = 1000; // width of map
const int height = 500; // height of map
int map8 [height][width], map7[height][width];
int h, w; // 직사각형 꼴의 map이 입력된다고 가정하고 map의 실제 h와 w를 readTextMap에서 계산
vector<pair<int,int> > g[width*height*2]; // graph

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
                else if (c=='.')
                    map_[h][w] = PERSON;
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
    return j + (i-1)*w + off;
}
void setGraph (int map_[height][width], int off) {
    for (int i=1; i<h; i++) {
        for (int j=1; j<w; j++) {
            // if horizontally connected
            if (map_[i][j]!=WALL && map_[i][j+1]!=WALL) {
                g[idx(i,j,off)].push_back(make_pair(idx(i,j+1,off),1));
                g[idx(i,j+1,off)].push_back(make_pair(idx(i,j,off),1));
            }
            // if vertically connected
            if (map_[i][j]!=WALL && map_[i+1][j]!=WALL) {
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

    return 0;
}
