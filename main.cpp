#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <time.h>
#include <stack>
#include <queue>
#include "mnPoint.h"
#include "Cell.h"

using namespace std;

vector<vector<Cell>> grid;
list<mnPoint> L,L1;
vector<list<mnPoint>> pathTiers;
int rows(10), cols(10), numBlocks(14), curBid(0);
bool targetFound(false);

// some forward declarations
mnPoint getRandomPt();
mnPoint getOpenPt();
void setSource(mnPoint P);
void setTarget(mnPoint P);
void setObstacle(mnPoint P);
void gridInit();
void obstacleInit();
void printCoordinates();
void printCellTypes();
bool onGrid(mnPoint temp);
bool exploreCell (mnPoint P);
void exploreL (mnPoint P);
void iterateL ();
bool termCriteriaMet();
void pathTiersInit(int maxLength = cols*rows);
void addToPathTier(mnPoint P);
int manhattanDist(mnPoint P1, mnPoint P2);
void updateL();
void printExplored();
void printL();
void printL1();
void printST(mnPoint source, mnPoint target);
bool Ligible(mnPoint P);
bool possiblePrev (mnPoint P,Cell cur);
mnPoint findPrev(mnPoint P);
void printRoute();
void backTrace(mnPoint source, mnPoint target);

//begin functions
mnPoint getRandomPt() {
    return mnPoint(rand()%rows, rand()%cols);
}

mnPoint getOpenPt() {
    mnPoint temp = getRandomPt();
    if (grid[temp.m][temp.n].contents == open) {
        return temp;
    } else {
        return getOpenPt();
    }
}

void setSource(mnPoint P) {
    grid[P.m][P.n].contents=source;
    grid[P.m][P.n].viewStatus=explored;
}

void setTarget(mnPoint P) {
    grid[P.m][P.n].contents=target;
}

void setObstacle(mnPoint P) {
    grid[P.m][P.n].contents=obstacle;
}

void gridInit() {
    grid.resize(rows);
    for (int i = 0; i < cols; ++i) {
        grid[i].resize(cols);
    }
    // now open cells
    for (int m = 0; m < rows; ++m) {
        for (int n = 0; n < cols; ++n) {
            grid[m][m] = Cell();
        }
    }
}

void obstacleInit() {
    mnPoint P = getRandomPt();
    for (int i = 0; i < numBlocks;) {
        P = getRandomPt();
        if (grid[P.m][P.n].contents != obstacle) {
            setObstacle(P); ++i;
        }
    }

}

void printCoordinates() {
    std::cout << "Begin printing cell coordinates" << std::endl;
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            cout << "(" << i << ", " << j << ")   ";
        }
        cout << "\n";
    }

    std::cout << "Done printing cell coordinates\n\n" << std::endl;
}

//print cell coordinates X(blocked),S(ource),T(arget),w(ired),_(open)
void printCellTypes() {
    std::cout << "Begin printing cell content" << std::endl;
    for (int m = 0; m < cols; ++m) {
        for (int n = 0; n < rows; ++n) {
            cellType x = grid[m][n].contents;
            if (x==obstacle){
                cout << "X  ";
            } else if (x==source) {
                cout << "S  ";
            } else if (x==target) {
                cout << "T  ";
            } else{ // x == open
                cout << "_  ";
            }

        }
        cout << "\n";
    }

    std::cout << "Done printing cell content\n" << std::endl;
}

bool onGrid(mnPoint temp) {
    return !(temp.m < 0 || temp.n < 0 || temp.m >= rows || temp.n >= cols);
}

// also adds to L1
bool exploreCell(mnPoint P) {
    // false if off grid or occupied
    if (onGrid(P) && grid[P.m][P.n].contents != obstacle) {
     // false if already looked at this cell
     if (grid[P.m][P.n].viewStatus == newCell) {
         // explore the cell...
         // false if we use the cell now
         if (grid[P.m][P.n].cost == 1) { // add if cellCost = current bid
             grid[P.m][P.n].leeWt = curBid;
             L1.push_back(P);
             addToPathTier(P);
             grid[P.m][P.n].viewStatus = explored;
             if (grid[P.m][P.n].contents==target) {
                 targetFound = true;
                 cout << "\nTarget found with path cost = " << curBid;
             }
         }
         // true if we still need to consider this cell
         else { // this cell cost extra, but next round it is possible to add
             --grid[P.m][P.n].cost;
             return true;
         }
     }
   }
   return false;
}

// for a cell in L, decide what to put in L1;
void exploreL(mnPoint P) {
    mnPoint up = {P.m-1,P.n}; exploreCell(up);
    mnPoint down = {P.m+1,P.n}; exploreCell(down);
    mnPoint right = {P.m, P.n+1}; exploreCell(right);
    mnPoint left = {P.m, P.n-1}; exploreCell(left);
}

void iterateL() {
    for (auto it: L) {
        exploreL(it);
    }
}

bool termCriteriaMet() {
    return (targetFound || L.empty());
}

void pathTiersInit(int maxLength) {
    pathTiers.resize(maxLength); // longest path possible ** could do Col + rows, or other
}

void addToPathTier(mnPoint P) {
    pathTiers[curBid].push_back(P);
}

int manhattanDist(mnPoint P1, mnPoint P2){
    return (abs(P1.m - P2.m) + abs(P1.n - P2.n));
}

// move all elements of L1 into L, and then clear L1
void updateL() {
    list<mnPoint> temp;
    for (auto it: L) {
        if (Ligible(it))
            temp.push_back(it);
    }

    std::swap(temp,L);

    for (auto it: L1) {
        L.push_back(it);
    }
    cout << "\n";

    printL1();
    L1.clear();
}

void printExplored() {
    std::cout << "Begin printing cell content" << std::endl;
    for (int m = 0; m < cols; ++m) {
        for (int n = 0; n < rows; ++n) {
            auto x = grid[m][n].contents;
            if (x==obstacle){
                cout << "X  ";
            } else if (x==source) {
                cout << "S  ";
            } else if (x==target) {
                cout << "T  ";
            } else{ // x == open
                if (grid[m][n].viewStatus == explored) {
//                    cout << "v  ";
                    cout << grid[m][n].leeWt%10 << "  ";
                } else {
                    cout << "-  ";
                }
            }
        }
        cout << "\n";
    }

    std::cout << "Done printing cell content\n" << std::endl;
}

void printL() {
    cout << "L contains: ";
    for (auto it: L) {
        cout << "   " << it;
    }
}

void printL1() {
    cout << "L1 contains: ";
    for (auto it: L1) {
        cout << "   " << it;
    }
    cout << endl;
}

void printST(mnPoint source, mnPoint target) {
    cout << "Source: " << source << endl;
    cout << "Target: " << target << endl;
}

bool Ligible(mnPoint P) {
    bool keepup(true),keepdown(true),keepright(true),keepleft(true);
    mnPoint upP(P.m-1,P.n),downP(P.m+1,P.n),rightP(P.m,P.n+1),leftP(P.m,P.n-1);

    if (!onGrid(upP) || grid[upP.m][upP.n].contents == obstacle || grid[upP.m][upP.n].viewStatus == explored) {
        keepup = false;
    }

    if (!onGrid(downP) || grid[downP.m][downP.n].contents == obstacle || grid[downP.m][downP.n].viewStatus == explored) {
        keepdown = false;
    }

    if (!onGrid(leftP) || grid[leftP.m][leftP.n].contents == obstacle || grid[leftP.m][leftP.n].viewStatus == explored) {
        keepleft = false;
    }

    if (!onGrid(rightP) || grid[rightP.m][rightP.n].contents == obstacle || grid[rightP.m][rightP.n].viewStatus == explored) {
        keepright = false;
    }

    return (keepup || keepdown || keepleft || keepright);

}

bool possiblePrev (mnPoint P, Cell cur) {
    int m(P.m),n(P.n);
    if (!onGrid(P)) {
        return false;
    }

    Cell c = grid[m][n];
    if (c.contents == obstacle) {
        return false;
    }

    if (c.viewStatus == newCell) {
        return false;
    }

    if(c.leeWt >= cur.leeWt) {
        return false;
    }

    return true;
}

mnPoint findPrev(mnPoint P) {
/*
    mnPoint upP(P.m-1,P.n),downP(P.m+1,P.n),rightP(P.m,P.n+1),leftP(P.m,P.n-1);

    // preferred directions: up, down, left, right
    vector<mnPoint> revOrder;
    revOrder.push_back(rightP); revOrder.push_back(leftP);revOrder.push_back(downP);revOrder.push_back(upP);

    vector<mnPoint> order;

    // consider only explored elements
    for (mnPoint neighborIt: revOrder) {
        Cell c = grid[neighborIt.m][neighborIt.n];
        if (onGrid(neighborIt)) {
            if (c.viewStatus == explored && c.leeWt != -1) {
                order.push_back(neighborIt);
            }
        }
    }

    // find minimal element
    int x = order.size();
    mnPoint prevPt = order[0];
    for (int i = 1; i < x; ++i) {
        mnPoint tempPt = order[i];
        if (grid[prevPt.m][prevPt.n].leeWt < grid[tempPt.m][tempPt.n].leeWt){
            prevPt = order[i];
        }
    }
    return prevPt;
*/
    Cell cur = grid[P.m][P.n];
    mnPoint up(P.m -1,P.n);
    if (possiblePrev(up,cur)) {return up;}

    mnPoint down(P.m +1,P.n);
    if (possiblePrev(down,cur)) {return down;}

    mnPoint right(P.m,P.n+1);
    if (possiblePrev(right,cur)) {return right;}

    mnPoint left(P.m, P.n-1);
    return left;

}

void printRoute() {
    std::cout << "Begin route" << std::endl;
    for (int m = 0; m < cols; ++m) {
        for (int n = 0; n < rows; ++n) {
            cellType x = grid[m][n].contents;
            if (x==obstacle){
                cout << "X  ";
            } else if (x==source) {
                cout << "S  ";
            } else if (x==target) {
                cout << "T  ";
            } else{ // x == open
                bool included = grid[m][n].onRoute;
                if (included) {
                    cout << grid[m][n].leeWt%10 << "  ";
                } else {
                    cout << "_  ";
                }
            }

        }
        cout << "\n";
    }

    std::cout << "Done printing route\n" << std::endl;
}

void backTrace(mnPoint source, mnPoint target) {
    stack<mnPoint> route;
    route.push(target);
    while (route.top() != source) {
        route.push(findPrev(route.top()));
    }

    while (!route.empty()) {
        mnPoint rtPt = route.top();
        route.pop();
        grid[rtPt.m][rtPt.n].onRoute = true;
    }

    printRoute();
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    srand(time(0));

    // set up our grid, get a start & a finish
    gridInit();
    obstacleInit();
    pathTiersInit();
    mnPoint mySource = getOpenPt(); setSource(mySource);
    mnPoint myTarget = getOpenPt(); setTarget(myTarget);

    // printables
    //printCoordinates();
    printCellTypes();
    printST(mySource,myTarget);

    // find a path thru
    L.clear();L1.clear();
    L.push_back(mySource);
    printL(); printL1();

    while (!termCriteriaMet()){
        cout << "-------------------------------------------" << endl;
        ++curBid;
        cout << "Begin iteration for cost = " << curBid << endl;
        printL();
        iterateL();
        updateL();
        printExplored();
        cout << "-------------------------------------------" << endl;
    }

    backTrace(mySource,myTarget);
    return 0;
}
