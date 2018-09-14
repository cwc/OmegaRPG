
#include "Object.h"

Object::Object() 
{
    id = -1;
    used = 0;
}

Object::Object(int i)
{
    id = i;
}

Object::Object( int tid, int tweight, int tplus, int tcharge, int tdmg, int thit, int taux, int tnumber, 
               int tfragility, long tbasevalue, unsigned char tknown, unsigned char tused, int tblessing, 
               unsigned char ttype, unsigned char tuniqueness, int tusef, unsigned char tlevel, unsigned long tobjchar, 
               const char* tobjstr, const char* ttruename, const char* tcursestr) : id ( tid ), weight ( tweight ), plus ( tplus ),
               charge ( tcharge ), dmg ( tdmg ), hit ( thit ), aux ( taux ), number ( tnumber ), 
               fragility ( tfragility ), basevalue ( tbasevalue ), known ( tknown ), used ( tused ), 
               blessing ( tblessing ), type ( ttype ), uniqueness ( tuniqueness ), usef( tusef ), level ( tlevel ),
               objchar ( tobjchar ), objstr ( tobjstr ), truename ( ttruename ), cursestr ( tcursestr )               
{

}

bool Object::isCursed() {
    return (id == -1 ?
        false : 
        blessing < 0);
}

bool Object::isUsed() {
    return used == 1;
}

bool Object::isEmpty() {
    return id == -1;
}

bool Object::operator == ( const Object& obj) const {
    return obj.id == id;
}

bool Object::operator == ( const int i) const {
    return i == id;
}

void Object::initialize() 
{

}

void Object::use() 
{

}


