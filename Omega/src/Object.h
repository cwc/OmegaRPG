#pragma once

#ifndef OBJECT_H
#define OBJECT_H

class Object
{
public:

    /* Data members */
    int id,weight,plus,charge,dmg,hit,aux,number,fragility;
    long basevalue;
    unsigned char known,used;
    int blessing;
    unsigned char type,uniqueness;
    int usef;
    unsigned char level;
    unsigned long objchar;
    const char *objstr,*truename,*cursestr;

    /* Constructors */
    Object();
    Object(int);
    Object(int, int, int, int, int, int, int, int, int, long, 
        unsigned char, unsigned char, int, unsigned char, 
        unsigned char, int, unsigned char, unsigned long, const char*, const char*, const char*);

    /* Methods */
    bool isCursed();
    bool isUsed();
    bool isEmpty();

    /* Virtual functions */
    virtual void use();
    virtual void initialize();

    /* Operator overloads */
    bool operator == ( const Object& ) const;
    bool operator == ( const int ) const;
};


#endif
