//
// Created by matth on 4/21/2020.
//

#ifndef LEEBASETWO_CELL_H
#define LEEBASETWO_CELL_H

enum cellType {source,target,obstacle,open};
enum cellStatus {explored,newCell};
enum akerWt {a,b,c,d};


class Cell {
public:
    Cell();

    cellType contents; // what type of square
    cellStatus viewStatus; // have we seen it before
    int leeWt; // weight we'll assign here
    akerWt myAkersWt, prevAkersWt; // improved Lee's method w/ a b c a b c...
    int cost; // how much to use this cell (if irregular)
    bool onRoute; // for backtrace

};


#endif //LEEBASETWO_CELL_H
