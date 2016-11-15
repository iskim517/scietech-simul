#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

const int sz = 50000; // # of nodes
const int width = 1000; // width of map
const int height = 500; // height of map
int map8 [height][width], map7[height][width];
int h, w; // 직사각형 꼴의 map이 입력된다고 가정하고 map의 실제 h와 w를 readTextMap에서 계산
vector<pair<int,int> > g[sz]; // adjacency list

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
                    map_[h][w] = 1;
                else if (c=='.')
                    map_[h][w] = 2;
                else if (c=='#')
                    map_[h][w] = 0;
                else if (c=='X')
                    map_[h][w] = 3;
                else
                    cout << "unexpected character at (" << h << "," << w << ")" << endl;
            }
        }
        file.close();
    }
}
void showMap (int map_[height][width]) {
    if (h<1 || w<1) {
        cout << "call readTextMap to set map_";
        return;
    }
     for (int i=1; i<=h; i++) {
        for (int j=1; j<=w; j++) {
            cout << map8[i][j];
        }
        cout << endl;
    }
}
int main () {
    readTextMap(map8, "8th.txt");
    readTextMap(map7, "7th.txt");
    showMap(map8);
}
