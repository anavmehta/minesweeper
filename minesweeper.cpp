#include <iostream>
#include <bits/stdc++.h>
using namespace std;

#define CLOSED 1
#define OPENED 2
#define MARKED 3
int N, M, K;
int a[30][30];
int status[30][30];
bool visited[30][30];


int dx[8] = {-1, 1, -1, 0, 1, -1, 0, 1};
int dy[8] = {0, 0, -1, -1, -1, 1, 1, 1};
int dxi[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
int dyi[9] = {0, 0, 0, -1, -1, -1, 1, 1, 1};
//https://dash.harvard.edu/bitstream/handle/1/14398552/BECERRA-SENIORTHESIS-2015.pdf

bool isValid(int x, int y)
{
    if(x >= 0 && y >= 0 && x < N && y < M) return true;
    return false;
}
void printMatrix(){
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
            if(status[i][j] == OPENED) cout << "  ";
            else if(status[i][j] == MARKED) cout << "x ";
            else cout << a[i][j] << " ";
        }
        cout << endl;
    }
}

int numMarkedNeighbors(int x, int y){
    int countMark = 0;
    for(int i=0;i<8;i++){
        if(isValid(x+dx[i], y+dy[i]) && (status[x+dx[i]][y+dy[i]] == MARKED)) countMark++;
    }
    return countMark;
}

int numOpenedNeighbors(int x, int y){
    int countOpened = 0;
    for(int i=0;i<8;i++){
        if(isValid(x+dx[i], y+dy[i]) && (status[x+dx[i]][y+dy[i]] == OPENED)) countOpened++;
    }
    return countOpened;
}

int numClosedNeighbors(int x, int y){
    int countClosed = 0;
    for(int i=0;i<8;i++){
        if(isValid(x+dx[i], y+dy[i]) && (status[x+dx[i]][y+dy[i]] == CLOSED)) countClosed++;
    }
    return countClosed;
}


int numMarked=0;

int count_or_assign(int x, int y, int from_state, int to_state) {
    int count = 0;
    for (int i = 0; i < 9; i++) {
        if (isValid(x + dxi[i], y + dyi[i])) {
            if (status[x+dxi[i]][y+dyi[i]] == from_state) {
                count++;
                if (to_state) {
                    //cout << "Switching " << x+dxi[i] << " " << y+dyi[i] << " from state " << from_state << " to state " << to_state << endl;
                    status[x+dxi[i]][y + dyi[i]] = to_state;
                    if(to_state == MARKED) numMarked++;
                }
            }
        }
    }
    return count;
}

int deduce(int x, int y)
{
    int n = a[x][y];


    int unknown = count_or_assign(x, y, CLOSED, 0);

    int bombs = count_or_assign(x, y, MARKED, 0);

    //cout << x << " " << y << " " << n << " " << unknown << " " << bombs << endl;
    if (unknown == n - bombs)
        return count_or_assign(x, y, CLOSED, MARKED);
    if (bombs == n)
        return count_or_assign(x, y, CLOSED, OPENED);
    int count=0;
    if(status[x][y] == CLOSED) {
        for(int i=0;i<8;i++) {
            if(isValid(x+dx[i], y+dy[i]) && (status[x+dx[i]][y+dy[i]] == OPENED) && (numMarkedNeighbors(x+dx[i], y+dy[i]) == a[x+dx[i]][y+dy[i]])) {
                //cout << "Opening cell " << x << " " << y << endl;
                status[x][y] = OPENED;
                count++;
                break;
            }
        }
    }
    if(status[x][y] == CLOSED) {
        for(int i=0;i<8;i++) {
            if(isValid(x+dx[i], y+dy[i]) && (status[x+dx[i]][y+dy[i]] == OPENED) && 
            (numClosedNeighbors(x+dx[i], y+dy[i]) == 1) &&
            (a[x+dx[i]][y+dy[i]]-numMarkedNeighbors(x+dx[i], y+dy[i]) == 1)) {
                //cout << "Marking cell " << x << " " << y << endl;
                status[x][y] = MARKED;
                numMarked++;
                count++;
                break;
            }
        }
    }
/*    
    if(status[x][y] == CLOSED && (count_or_assign(x,y,CLOSED,0) == 3) && (n==2)) {
        for(int i=0;i<8;i++){
            if(isValid(x+dx[i], y+dy[i]) && (status[x+dx[i]][y+dy[i]] == CLOSED) && 
                (count_or_assign(x+dx[i],y+dy[i],CLOSED, 0) == n) && (a[x+dx[i]][y+dy[i] == 1])) {
                    status[x+dx[i]][y+dy[i]] = MARKED;
                    cout << "Marked " << x + dx[i]<< " " << y+dy[i] << endl;
                    count++;
                }
        }
    }*/
    return count;
}







bool allDone(){
    bool done = true;
    for(int i=0;i<N;i++) {
        for(int j=0;j<M;j++) {
            done = done && (status[i][j] != CLOSED);
        }
    }
    return done;
}



bool minesweep(int x, int y, int numMarked){
    //printMatrix();
    if(numMarked == K && allDone()) {
        for(int i=0;i<N;i++) {
            for(int j=0;j<M;j++){
                if(status[i][j] == MARKED) cout << i << " " << j << endl;
            }
        }
        return true;
    }
    //cout << "Processing " << x << " " << y << endl;
    if(status[x][y] == CLOSED) {
        for(int i=0;i<8;i++) {
            if(isValid(x+dx[i], y+dy[i]) && (status[x+dx[i]][y+dy[i]] == OPENED) && (numMarkedNeighbors(x+dx[i], y+dy[i]) == a[x+dx[i]][y+dy[i]])) {
                cout << "Opening cell " << x << " " << y << endl;
                status[x][y] = OPENED;
                break;
            }
        }
    }
    if(status[x][y] == CLOSED) {
        for(int i=0;i<8;i++) {
            if(isValid(x+dx[i], y+dy[i]) && (status[x+dx[i]][y+dy[i]] == OPENED) && 
            (numClosedNeighbors(x+dx[i], y+dy[i]) == 1) &&
            (a[x+dx[i]][y+dy[i]]-numMarkedNeighbors(x+dx[i], y+dy[i]) == 1)) {
                cout << "Marking cell " << x << " " << y << endl;
                status[x][y] = MARKED;
                numMarked++;
                break;
            }
        }
    }
    if(a[x][y] == 0 && status[x][y] == CLOSED) {
        //cout << "Opening cell " << x << " " << y << endl;
        
        status[x][y] = OPENED;
        /*for(int i=0;i<8;i++) {
            if(isValid(x+dx[i], y+dy[i])) { 
                cout << "Opening cell " << x+dx[i] << " " << y+dy[i] << endl;
                status[x+dx[i]][y+dy[i]] = OPENED;
            }
        }*/
    }
/*
    if(status[x][y] == OPENED){
        for(int i=0;i<8;i++) {
            if(isValid(x+dx[i], y+dx[i]) && status[x][y] == CLOSED) {
                mark[{x+dx[i],y+dy[i]}] = true;
            }
        }
    }
    if(status[x][y] == CLOSED) {
        if(mark[{x,y]}){}
    }*/
    for(int i=0;i<8;i++) {
        if(isValid(x+dx[i], y+dy[i]) && !visited[x+dx[i]][y+dy[i]] && status[x+dx[i]][y+dy[i]] == CLOSED) {
            visited[x+dx[i]][y+dy[i]] = true;
            if(minesweep(x+dx[i], y+dy[i], numMarked)) return true;;
            visited[x+dx[i]][y+dy[i]] = false;
        }
    }
    /*
    //if neighbours dont work then go out of the region
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
            if(!visited[i][j] && status[i][j] == CLOSED) {
                visited[i][j] = true;
                if(minesweep(i, j, numMarked)) return true;;
                visited[i][j] = false;
            }            
        }
    }*/
    return false;

    
}

void findZero(int &x, int &y){
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
            if(a[i][j] == 0) {
                x = i;
                y = j;
                return;
            }
        }
    }
}

int main(){
    int x, y;
    cin >> N >> M >> K;
    

    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
            cin >> a[i][j];
            status[i][j] = CLOSED;
            visited[i][j] = false;
        }
    }
   
   
    int changes;
    do {
        changes = 0;
        for (int x= 0; x<N; x++)
            for (int y=0; y <M; y++)
                changes += deduce(x, y);
    } while (changes); 
    if(numMarked != K) {
        cout << "NO SOLUTION" << endl;
        return 0;
    }
    
    /*
    findZero(x, y);
    minesweep(x,y,0);*/
    for(int i=0;i<N;i++) {
        for(int j=0;j<M;j++){
            if(status[i][j] == MARKED) cout << i+1 << " " << j+1 << endl;
        }
    }


}
