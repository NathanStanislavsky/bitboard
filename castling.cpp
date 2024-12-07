#include "castling.h"

Castling::Castling(bool wk, bool wq, bool bk, bool bq)
{
    wkc = wk;
    wqc = wq;
    bkc = bk;
    bqc = bq;
}

Castling::Castling()
{
    wkc = false;
    wqc = false;
    bkc = false;
    bqc = false;
}