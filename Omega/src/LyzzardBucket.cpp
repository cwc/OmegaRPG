#include "defs.h"
#include "LyzzardBucket.h"

LyzzardBucket::LyzzardBucket(void)
{
    id = FOODID + 0;
    objstr = "red and white striped bucket";
    truename = "Lyzzard Bucket";
    cursestr = "Lyzzard Bucket";
}

LyzzardBucket::~LyzzardBucket(void)
{
}

LyzzardBucket::LyzzardBucket(Object *o) : LyzzardBucket() {
    weight = o->weight;
    plus = o->plus;
    charge = o->charge;
    dmg = o->dmg;
    hit = o->hit;
    aux = o->aux;
    number = o->number;
    fragility = o->fragility;
    basevalue = o->basevalue;
    known = o->known;
    used = o->used;
    blessing = o->blessing;
    type = o->type;
    uniqueness = o->uniqueness;
    usef = o->usef;
    level = o->level;
    objchar = o->objchar;
    if(o->objstr != NULL) objstr = o->objstr;
    if(o->truename != NULL) truename = o->truename;
    if(o->cursestr != NULL) cursestr = o->cursestr;
}
