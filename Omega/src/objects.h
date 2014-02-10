/* omega, (C) 1987,1988,1989 by Laurence Raphael Brothers */
/* iinit.h */
/* This file defines and initializes the Objects Array */

/* the struct object from defs.h looks like:

struct object
{
  int id;
  int weight;
  int plus;
  int charge;
  int dmg;
  int hit;
  int aux;
  int number;
  int fragility;
  long basevalue;
  unsigned char known;
  unsigned char used;
  int blessing;
  unsigned char type;
  unsigned char uniqueness;
  int usef;
  unsigned char level;
  Symbol objchar;
  char *objstr;
  char *truename;
  char *cursestr;
};

*/

#pragma once

#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "LyzzardBucket.h"

Object Objects[TOTALITEMS] = {
    Object(THINGID+0,10,0,0,0,0,0,1,10,50,0,0,0,0,COMMON,I_RAISE_PORTCULLIS,2,THING,"small grey box with red button","disposeable garage door opener","disposeable garage door opener"),

    Object( THINGID+1,20,0,0,0,0,0,1,20,2000,0,0,0,0,COMMON,I_NO_OP,4,THING,"some rocks","uncut diamonds","uncut diamonds" ),

    Object( THINGID+2,15,0,0,0,0,0,1,10,50,0,0,0,0,COMMON,I_PICK,2,THING,"twisted piece of metal","thieve's pick","thieve's pick" ),

    Object( THINGID+3,10,0,0,0,0,0,1,15,1500,0,0,0,0,COMMON,I_NO_OP,1,THING,"large green gem","humongous emerald","humongous emerald" ),

    Object( THINGID+4,200,0,0,0,0,0,1,18,1750,0,0,0,0,COMMON,I_NO_OP,4,THING,"gold ingot","gold ingot","gold ingot" ),

    Object( THINGID+5,5,0,0,0,0,0,1,1,3000,0,0,0,0,COMMON,I_NO_OP,5,THING,"old illegible documents","bearer bonds","bearer bonds" ),

    Object( THINGID+6,100,0,0,0,0,0,1,1,1,0,0,0,0,COMMON,I_NO_OP,0,THING,"bucket of salt water","bucket of salt water","bucket of salt water" ),

    Object( THINGID+7,3,0,0,0,0,0,1,10,20,0,0,0,0,COMMON,I_KEY,2,THING,"key","magic key","magic key" ),

    Object( THINGID+8,30,0,0,0,0,6,1,8,10,0,0,0,0,COMMON,I_PERM_ILLUMINATE,0,THING,"torch","torch","torch" ),

    Object( THINGID+9,10,0,0,0,0,0,1,12,0,0,0,0,0,COMMON,I_NO_OP,0,THING,"grot","grot","grot" ),

    Object( THINGID+10,10,0,0,0,0,0,1,12,0,0,0,0,0,COMMON,I_NO_OP,0,THING,"grot","grot","grot" ),

    Object( THINGID+11,10,0,0,0,0,0,1,12,0,0,0,0,0,COMMON,I_NO_OP,0,THING,"grot","grot","grot" ),

    Object( THINGID+12,10,0,0,0,0,0,1,12,0,0,0,0,0,COMMON,I_NO_OP,0,THING,"grot","grot","grot" ),

    Object( THINGID+13,10,0,0,0,0,0,1,12,0,0,0,0,0,COMMON,I_NO_OP,0,THING,"grot","grot","grot" ),

    Object( THINGID+14,10,0,0,0,0,0,1,12,0,0,0,0,0,COMMON,I_NO_OP,0,THING,"grot","grot","grot" ),

    Object( THINGID+15,10,0,0,0,0,0,1,12,0,0,0,0,0,COMMON,I_NO_OP,0,THING,"grot","grot","grot" ),

    Object( THINGID+16,10,0,0,0,0,0,1,12,0,0,0,0,0,UNIQUE_MADE,I_NO_OP,0,THING,"Justiciar's Badge","Justiciar's Badge","Justiciar's Badge" ),

    Object( THINGID+17,10,0,0,0,0,89,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"spring-loaded tube","dart-trap component","dart-trap component" ),

    Object( THINGID+18,10,0,0,0,0,98,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"pressurized cannister","acid-trap component","acid-trap component" ),

    Object( THINGID+19,10,0,0,0,0,92,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"coil of fine wire","snare component","snare component" ),

    Object( THINGID+20,10,0,0,0,0,94,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"mound of powdered aluminum","fire-trap component","fire-trap component" ),

    Object( THINGID+21,10,0,0,0,0,95,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"powerful rune","teleport-trap component","teleport-trap component" ),

    Object( THINGID+22,10,0,0,0,0,97,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"pressurized cannister","sleepgas-trap component","sleepgas-trap component" ),

    Object( THINGID+23,10,0,0,0,0,96,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"powerful rune","disintegration-trap component","disintegration-trap component" ),

    Object( THINGID+24,10,0,0,0,0,100,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"dark fabric patch","abyss-trap component","abyss-trap component" ),

    Object( THINGID+25,10,0,0,0,0,99,1,3,30,0,0,0,0,COMMON,I_TRAP,0,THING,"powerful rune","manadrain-trap component","manadrain-trap component" ),

    Object( THINGID+26,1,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_PERM_KNOWLEDGE,0,THING,"small mirror","mirror of self-knowledge","mirror of delusion" ),

    Object( THINGID+27,2,0,0,0,0,0,1,10,1,0,0,0,0,COMMON,I_NO_OP,3,THING,"plastic card with black stripe","debit card","debit card"),

    Object( THINGID+28,2,0,0,0,0,0,1,10,1,0,0,0,0,COMMON,I_NO_OP,5,THING,"plastic card with black stripe","credit card","credit card"),

    Object( THINGID+29,2,0,0,0,0,0,1,10,1,0,0,0,0,COMMON,I_NO_OP,1,THING,"plastic card with square gold patch","pre-paid card","pre-paid card"),

    Object( THINGID+30,2,0,0,0,0,0,1,10,1,0,0,0,0,COMMON,I_NO_OP,10,THING,"plastic card with square gold patch","smart card","smart card"),

    //Object( FOODID+0,20,0,0,0,0,8,1,0,2,0,0,0,0,COMMON,I_FOOD,0,FOOD,"red and white striped bucket","food ration","food ration" ),
	LyzzardBucket(),

    Object( FOODID+1,10,0,0,0,0,48,1,0,200,0,0,0,0,COMMON,I_LEMBAS,2,FOOD,"wafer of lembas","wafer of lembas","wafer of lembas" ),

    Object( FOODID+2,1,0,0,0,0,1,1,0,50,0,0,0,0,COMMON,I_STIM,3,FOOD,"small gray tablet","stimtab","stimtab" ),

    Object( FOODID+3,1,0,0,0,0,1,1,0,100,0,0,0,0,COMMON,I_POW,3,FOOD,"small gray tablet","powtab","powtab" ),

    Object( FOODID+4,1,0,0,0,0,36,1,0,20,0,0,0,0,COMMON,I_FOOD,3,FOOD,"small gray tablet","nutritab","nutritab" ),

    Object( FOODID+5,1,0,0,0,0,1,1,0,500,0,0,0,0,COMMON,I_IMMUNE,3,FOOD,"small gray tablet","immunotab","immunotab" ),

    Object( FOODID+6,1,0,0,0,0,2,1,0,1,0,0,0,0,COMMON,I_FOOD,0,FOOD,"jelly baby","jelly baby","jelly baby" ),

    Object( FOODID+7,20,0,0,0,0,-1,1,0,25,0,0,0,0,COMMON,I_POISON_FOOD,1,FOOD,"food ration","poisoned ration","poisoned ration" ),

    Object( FOODID+8,2,0,0,0,0,1,1,0,100,0,0,0,0,COMMON,I_PEPPER_FOOD,2,FOOD,"withered reddish stringy vegetable","szechuan pepper","szechuan pepper" ),

    Object( FOODID+9,2,0,0,0,0,8,1,0,5,0,0,0,0,COMMON,I_FOOD,2,FOOD,"messy red and white disk","pepperoni pizza pie","pepperoni pizza pie" ),

    Object( FOODID+10,2,0,0,0,0,6,1,0,7,0,0,0,0,COMMON,I_FOOD,2,FOOD,"brown bag with strange ideograms","hunan take-out","hunan take-out" ),

    Object( FOODID+11,2,0,0,0,0,2,1,0,5,0,0,0,0,COMMON,I_FOOD,2,FOOD,"strange foamy cylinders","twinkies","twinkies" ),

    Object( FOODID+12,2,0,0,0,0,3,1,0,3,0,0,0,0,COMMON,I_FOOD,2,FOOD,"squiggly orange spirals","cheese-doodles","cheese-doodles" ),

    Object( FOODID+13,2,0,0,0,0,4,1,0,3,0,0,0,0,COMMON,I_FOOD,2,FOOD,"sweet nutty-smelling disks","pecan twirls","pecan_twirls" ),

    Object( FOODID+14,2,0,0,0,0,10,1,0,75,0,0,0,0,COMMON,I_FOOD,2,FOOD,"pale white strips with green dots","veal piccata with capers","veal piccata with capers" ),

    Object( FOODID+15,2,0,0,0,0,3,1,0,5,0,0,0,0,COMMON,I_FOOD,2,FOOD,"a bag of hard brown kernels","a bag of unmilled grain","a bag of unmilled grain" ),

    Object( SCROLLID+0,5,0,0,0,0,0,1,1,5,0,0,0,0,COMMON,I_NOTHING,0,SCROLL,"?","blank scroll","Antinomy!" ),

    Object( SCROLLID+1,5,0,0,0,0,0,1,1,250,0,0,0,0,COMMON,I_SPELLS,1,SCROLL,"?","scroll of spells","scroll of spell loss" ),

    Object( SCROLLID+2,5,0,0,0,0,0,1,1,400,0,0,0,0,COMMON,I_BLESS,2,SCROLL,"?","scroll of blessing","scroll of cursing" ),

    Object( SCROLLID+3,5,0,0,0,0,0,1,1,5000,0,0,0,0,COMMON,I_ACQUIRE,7,SCROLL,"?","scroll of acquirement","scroll of destruction" ),

    Object( SCROLLID+4,5,0,0,0,0,0,1,1,200,0,0,0,0,COMMON,I_ENCHANT,2,SCROLL,"?","scroll of enchantment","scroll of disenchantment" ),

    Object( SCROLLID+5,5,0,0,0,0,0,1,1,75,0,0,0,0,COMMON,I_TELEPORT,1,SCROLL,"?","scroll of teleportation","scroll of teleportation" ),

    Object( SCROLLID+6,5,0,0,0,0,0,1,1,10000,0,0,0,0,COMMON,I_WISH,10,SCROLL,"?","scroll of wishing","scroll of wishing" ),

    Object( SCROLLID+7,5,0,0,0,0,0,1,1,25,0,0,0,0,COMMON,I_CLAIRVOYANCE,1,SCROLL,"?","scroll of clairvoyance","scroll of amnesia" ),

    Object( SCROLLID+8,5,0,0,0,0,0,1,1,50,0,0,0,0,COMMON,I_DISPLACE,2,SCROLL,"?","scroll of displacement","scroll of vulnerability" ),

    Object( SCROLLID+9,5,0,0,0,0,0,1,1,20,0,0,0,0,COMMON,I_ID,0,SCROLL,"?","scroll of identification","scroll of forgetfulness" ),

    Object( SCROLLID+10,5,0,0,0,0,0,1,1,10,0,0,0,0,COMMON,I_HINT,0,SCROLL,"?","hint sheet","hint sheet" ),

    Object( SCROLLID+11,5,0,0,0,0,0,1,1,50,0,0,0,0,COMMON,I_JANE_T,2,SCROLL,"?","Jane's guide to treasure","Jane's guide to treasure" ),

    Object( SCROLLID+12,5,0,0,0,0,0,1,1,50,0,0,0,0,COMMON,I_ALERT,1,SCROLL,"?","scroll of alertness","scroll of drowsiness" ),

    Object( SCROLLID+13,5,0,0,0,0,0,1,1,300,0,0,0,0,COMMON,I_FLUX,5,SCROLL,"?","scroll of cosmic flux","scroll of cosmic flux" ),

    Object( SCROLLID+14,5,0,0,0,0,0,1,1,100,0,0,0,0,COMMON,I_CHARGE,2,SCROLL,"?","scroll of charging","scroll of draining" ),

    Object( SCROLLID+15,5,0,0,0,0,0,1,1,100,0,0,0,0,COMMON,I_WARP,3,SCROLL,"?","scroll of the warp","scroll of the warp" ),

    Object( SCROLLID+16,5,0,0,0,0,0,1,1,10,0,0,0,0,COMMON,I_KNOWLEDGE,1,SCROLL,"?","scroll of self-knowledge","scroll of delusion" ),

    Object( SCROLLID+17,5,0,0,0,0,0,1,1,100,0,0,0,0,COMMON,I_LAW,2,SCROLL,"?","scroll of law","scroll of law" ),

    Object( SCROLLID+18,5,0,0,0,0,0,1,1,200,0,0,0,0,COMMON,I_HERO,5,SCROLL,"?","scroll of heroism","scroll of cowardliness" ),

    Object( SCROLLID+19,5,0,0,0,0,0,1,1,50,0,0,0,0,COMMON,I_TRUESIGHT,2,SCROLL,"?","scroll of true sight","scroll of blindness" ),

    Object( SCROLLID+20,5,0,0,0,0,0,1,1,250,0,0,0,0,COMMON,I_DISPEL,3,SCROLL,"?","scroll of dispelling","scroll of self-dispelling" ),

    Object( SCROLLID+21,5,0,0,0,0,0,1,1,300,0,0,0,0,COMMON,I_SUMMON,2,SCROLL,"?","scroll of summoning","scroll of summoning" ),

    Object( SCROLLID+22,5,0,0,0,0,0,1,1,30,0,0,0,0,COMMON,I_ILLUMINATE,0,SCROLL,"?","scroll of light","scroll of darkness" ),

    Object( SCROLLID+23,5,0,0,0,0,0,1,1,250,0,0,0,0,COMMON,I_DEFLECT,3,SCROLL,"?","scroll of deflection","scroll of vulnerability" ),

    Object( POTIONID+0,20,0,0,0,0,0,1,5,0,0,0,0,0,COMMON,I_NO_OP,0,POTION,"?","potion of quenching thirst","Antinomy!" ),

    Object( POTIONID+1,20,0,0,0,0,0,1,5,40,0,0,0,0,COMMON,I_HEAL,2,POTION,"?","potion of healing","potion of disruption" ),

    Object( POTIONID+2,20,0,0,0,0,0,1,5,10,0,0,0,0,COMMON,I_OBJDET,1,POTION,"?","potion of object detection","potion of object detection" ),

    Object( POTIONID+3,20,0,0,0,0,0,1,5,10,0,0,0,0,COMMON,I_MONDET,1,POTION,"?","potion of monster detection","potion of monster detection" ),

    Object( POTIONID+4,20,0,0,0,0,0,1,5,20,0,0,0,0,COMMON,I_NEUTRALIZE_POISON,2,POTION,"?","potion of neutralize poison","potion of poison" ),

    Object( POTIONID+5,20,0,0,0,0,0,1,5,100,0,0,0,0,COMMON,I_ALERT,0,POTION,"?","potion of alertness","potion of sleep" ),

    Object( POTIONID+6,20,0,0,0,0,0,1,5,40,0,0,0,0,COMMON,I_SPEED,1,POTION,"?","potion of speed","potion of slowness" ),

    Object( POTIONID+7,20,0,0,0,0,0,1,5,50,0,0,0,0,COMMON,I_RESTORE,3,POTION,"?","potion of restoration","potion of debilitation" ),

    Object( POTIONID+8,20,-1,0,0,0,0,1,5,1500,0,0,0,0,COMMON,I_AZOTH,7,POTION,"mercury","Azoth","mercury" ),

    Object( POTIONID+9,20,0,0,0,0,0,1,5,100,0,0,0,0,COMMON,I_REGENERATE,4,POTION,"?","potion of regeneration","potion of great disruption" ),

    Object( POTIONID+10,20,0,0,0,0,0,1,5,100,0,0,0,0,COMMON,I_INVISIBLE,2,POTION,"?","potion of invisibility","potion of aggravation" ),

    Object( POTIONID+11,20,0,0,0,0,0,1,5,30,0,0,0,0,COMMON,I_BREATHING,2,POTION,"?","potion of breathing","potion of choking" ),

    Object( POTIONID+12,20,0,0,0,0,0,1,5,200,0,0,0,0,COMMON,I_AUGMENT,5,POTION,"?","potion of augmentation","potion of debilitation" ),

    Object( POTIONID+13,20,0,0,0,0,0,1,5,50,0,0,0,0,COMMON,I_FEAR_RESIST,2,POTION,"?","potion of banishing fear","potion of fear" ),

    Object( POTIONID+14,20,0,0,0,0,0,1,5,100,0,0,0,0,COMMON,I_CHAOS,3,POTION,"?","potion of chaos","Antinomy!" ),

    Object( POTIONID+15,20,0,0,0,0,0,1,5,200,0,0,0,0,COMMON,I_ACCURACY,3,POTION,"?","potion of accuracy","potion of fuzziness" ),

    Object( POTIONID+16,20,0,0,0,0,0,1,5,50,0,0,0,0,COMMON,I_LEVITATION,1,POTION,"?","potion of levitation","potion of levitation" ),

    Object( POTIONID+17,20,0,0,0,0,0,1,5,50,0,0,0,0,COMMON,I_CURE,1,POTION,"?","potion of curing","potion of disease" ),

    Object( WEAPONID+0,10,0,0,6,12,0,1,15,5,0,0,0,THRUSTING,COMMON,I_NORMAL_WEAPON,0,WEAPON,"dagger","dagger","dagger" ),

    Object( WEAPONID+1,25,0,0,8,11,0,1,15,40,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,1,WEAPON,"short sword","short sword","short sword" ),

    Object( WEAPONID+2,40,0,0,12,9,0,1,15,90,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,2,WEAPON,"broad sword","broad sword","broad sword" ),

    Object( WEAPONID+3,50,0,0,16,8,0,1,15,250,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,3,WEAPON,"bastard sword","bastard sword","bastard sword" ),

    Object( WEAPONID+4,50,0,0,16,10,0,1,15,500,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,4,WEAPON,"katana","katana","katana" ),

    Object( WEAPONID+5,75,0,0,20,7,0,1,15,400,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,5,WEAPON,"great-sword","great-sword","great-sword" ),

    Object( WEAPONID+6,15,0,0,6,15,0,1,15,50,0,0,0,THRUSTING,COMMON,I_NORMAL_WEAPON,2,WEAPON,"epee","epee","epee" ),

    Object( WEAPONID+7,20,0,0,8,13,0,1,15,250,0,0,0,THRUSTING,COMMON,I_NORMAL_WEAPON,3,WEAPON,"rapier","rapier","rapier" ),

    Object( WEAPONID+8,25,0,0,11,12,0,1,15,500,0,0,0,THRUSTING,COMMON,I_NORMAL_WEAPON,5,WEAPON,"estoc","estoc","estoc" ),

    Object( WEAPONID+9,35,0,0,8,8,0,1,15,40,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,1,WEAPON,"cutlass","cutlass","cutlass" ),

    Object( WEAPONID+10,25,0,0,8,10,0,1,15,50,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,1,WEAPON,"hand-axe","hand-axe","hand-axe" ),

    Object( WEAPONID+11,50,0,0,12,8,0,1,15,100,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,2,WEAPON,"battle-axe","battle-axe","battle-axe" ),

    Object( WEAPONID+12,100,0,0,24,7,0,1,15,200,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,5,WEAPON,"great-axe","great-axe","great-axe" ),

    Object( WEAPONID+13,50,0,0,12,8,0,1,15,50,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,2,WEAPON,"mace","mace","mace" ),

    Object( WEAPONID+14,50,0,0,14,6,0,1,15,60,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,2,WEAPON,"war-hammer","war-hammer","war-hammer" ),

    Object( WEAPONID+15,60,0,0,16,7,0,1,15,150,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,5,WEAPON,"morning-star","morning-star","morning-star" ),

    Object( WEAPONID+16,50,0,0,12,7,0,1,15,50,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,3,WEAPON,"flail","flail","flail" ),

    Object( WEAPONID+17,30,0,0,6,10,0,1,15,5,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,0,WEAPON,"club","club","club" ),

    Object( WEAPONID+18,80,0,0,9,12,0,1,15,30,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,1,WEAPON,"quarterstaff","quarterstaff","quarterstaff" ),

    Object( WEAPONID+19,50,0,0,10,10,0,1,15,50,0,0,0,THRUSTING,COMMON,I_NORMAL_WEAPON,2,WEAPON,"spear","spear","spear" ),

    Object( WEAPONID+20,100,0,0,16,6,0,1,15,100,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,3,WEAPON,"halberd","halberd","halberd" ),

    Object( WEAPONID+21,80,0,0,12,7,0,1,15,75,0,0,0,THRUSTING,COMMON,I_NORMAL_WEAPON,3,WEAPON,"trident","trident","trident" ),

    Object( WEAPONID+22,20,0,0,4,8,1005,1,15,100,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,2,WEAPON,"whip","whip","whip" ),

    Object( WEAPONID+23,20,0,0,30,20,1002,1,15,2000,0,0,0,THRUSTING,COMMON,I_LIGHTSABRE,9,WEAPON,"grey metal cylinder","lightsabre","lightsabre" ),

    Object( WEAPONID+24,500,8,0,16,8,1003,1,15,1000,0,0,-8,CUTTING,COMMON,I_DEMONBLADE,9,WEAPON,"bastard sword","Demonblade","Demonblade" ),

    Object( WEAPONID+25,250,7,0,17,10,1004,1,15,3000,0,0,7,STRIKING,COMMON,I_MACE_DISRUPT,7,WEAPON,"mace","mace of disruption","mace" ),

    Object( WEAPONID+26,100,0,0,12,15,0,1,15,300,0,0,0,MISSILE,COMMON,I_NORMAL_WEAPON,2,MISSILEWEAPON,"longbow","longbow","longbow" ),

    Object( WEAPONID+27,150,0,0,20,15,0,1,15,500,0,0,0,MISSILE,COMMON,I_NORMAL_WEAPON,3,MISSILEWEAPON,"crossbow","crossbow","crossbow" ),

    Object( WEAPONID+28,2,0,0,3,3,1006,1,15,2,0,0,0,MISSILE,COMMON,I_NORMAL_WEAPON,1,MISSILEWEAPON,"arrow","arrow","arrow" ),

    Object( WEAPONID+29,2,0,0,3,0,1007,1,15,5,0,0,0,MISSILE,COMMON,I_NORMAL_WEAPON,1,MISSILEWEAPON,"bolt","bolt","bolt" ),

    Object( WEAPONID+30,50,0,0,3,10,1005,1,15,50,0,0,0,MISSILE,COMMON,I_NORMAL_WEAPON,2,MISSILEWEAPON,"bola","bola","bola" ),

    Object( WEAPONID+31,40,5,0,12,9,1008,1,15,3000,0,0,0,CUTTING,COMMON,I_NORMAL_WEAPON,7,WEAPON,"broad sword","vorpal sword","sword of self-mutilation" ),

    Object( WEAPONID+32,100,7,0,20,7,1009,1,15,5000,0,0,-7,CUTTING,COMMON,I_DESECRATE,9,WEAPON,"great-sword","Desecrator","Desecrator" ),

    Object( WEAPONID+33,60,7,0,16,7,1010,1,15,2000,0,0,0,STRIKING,COMMON,I_PERM_FIRE_RESIST,6,WEAPON,"morning-star","firestar","firestar" ),

    Object( WEAPONID+34,50,7,0,10,12,1011,1,15,3000,0,0,7,THRUSTING,COMMON,I_DEFEND,7,WEAPON,"estoc","Defender","Defender" ),

    Object( WEAPONID+35,50,10,0,100,10,1012,1,15,5000,0,0,10,THRUSTING,UNIQUE_MADE,I_VICTRIX,10,WEAPON,"ivory spear","Victrix","Victrix" ),

    Object( WEAPONID+36,500,6,0,32,6,0,1,15,2000,0,0,0,CUTTING,UNIQUE_MADE,I_NORMAL_WEAPON,7,WEAPON,"great-axe","Goblins' Hewer","Goblins' Hewer" ),

    Object( WEAPONID+37,100,20,0,30,10,0,1,15,500,0,0,-10,CUTTING,COMMON,I_NORMAL_WEAPON,6,WEAPON,"scythe","scythe of slicing","scythe of slicing" ),

    Object( WEAPONID+38,250,0,0,50,-10,0,1,15,100,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,5,WEAPON,"giant club","giant club","giant club" ),

    Object( WEAPONID+39,500,0,0,10000,1000,1014,1,15,10000,0,0,0,CUTTING,UNIQUE_MADE,I_NORMAL_WEAPON,10,WEAPON,"Scythe of Death","Scythe of Death","Scythe of Death" ),

    Object( WEAPONID+40,30,0,0,16,16,1015,1,15,1000,0,0,0,STRIKING,COMMON,I_NORMAL_WEAPON,4,WEAPON,"whip","acid whip","acid whip" ),

    Object( ARMORID+0,100,0,0,1,0,0,1,20,5,0,0,0,0,COMMON,I_NORMAL_ARMOR,0,ARMOR,"flak jacket","flak jacket","flak jacket" ),

    Object( ARMORID+1,200,0,0,2,0,1,1,20,10,0,0,0,0,COMMON,I_NORMAL_ARMOR,1,ARMOR,"soft leather armor","soft leather armor","soft leather armor" ),

    Object( ARMORID+2,200,0,0,2,0,1,1,20,30,0,0,0,0,COMMON,I_NORMAL_ARMOR,1,ARMOR,"cuirbouilli","cuirbouilli","cuirbouilli" ),

    Object( ARMORID+3,300,0,0,3,0,2,1,20,100,0,0,0,0,COMMON,I_NORMAL_ARMOR,2,ARMOR,"ring mail","ring mail","ring mail" ),

    Object( ARMORID+4,400,0,0,4,0,3,1,20,200,0,0,0,0,COMMON,I_NORMAL_ARMOR,3,ARMOR,"chain mail","chain mail","chain mail" ),

    Object( ARMORID+5,400,0,0,4,0,2,1,20,300,0,0,0,0,COMMON,I_NORMAL_ARMOR,4,ARMOR,"scale mail","scale mail","scale mail" ),

    Object( ARMORID+6,400,0,0,5,0,3,1,20,450,0,0,0,0,COMMON,I_NORMAL_ARMOR,5,ARMOR,"partial plate mail","partial plate mail","partial plate mail" ),

    Object( ARMORID+7,600,0,0,6,0,4,1,20,750,0,0,0,0,COMMON,I_NORMAL_ARMOR,6,ARMOR,"full plate mail","full plate mail","full plate mail" ),

    Object( ARMORID+8,500,0,0,7,0,4,1,20,1500,0,0,0,0,COMMON,I_NORMAL_ARMOR,6,ARMOR,"plate armor","plate armor","plate armor" ),

    Object( ARMORID+9,200,0,0,5,0,2,1,20,1500,0,0,0,0,COMMON,I_NORMAL_ARMOR,6,ARMOR,"lamellar armor","lamellar armor","lamellar armor" ),

    Object( ARMORID+10,200,0,0,5,0,2,1,20,2000,0,0,0,0,COMMON,I_PERM_FEAR_RESIST,7,ARMOR,"mithril chain mail","mithril chain mail","mithril chain mail" ),

    Object( ARMORID+11,300,0,0,8,0,3,1,20,3000,0,0,0,0,COMMON,I_PERM_FEAR_RESIST,7,ARMOR,"mithril plate armor","mithril plate armor","mithril plate armor" ),

    Object( ARMORID+12,500,0,0,6,0,3,1,20,3000,0,0,0,0,COMMON,I_PERM_FIRE_RESIST,7,ARMOR,"dragonscale armor","dragonscale armor","dragonscale armor" ),

    Object( ARMORID+13,100,9,0,7,0,2,1,20,1000,0,0,0,0,COMMON,I_PERM_ENERGY_RESIST,8,ARMOR,"prismatrix armor","prismatrix armor","prismatrix armor" ),

    Object( ARMORID+14,0,0,0,20,0,0,1,20,5000,0,0,0,0,COMMON,I_PERM_BREATHING,9,ARMOR,"powered combat armor","powered combat armor","powered combat armor" ),

    Object( ARMORID+15,0,0,0,10,0,0,1,20,3000,0,0,0,0,COMMON,I_PERM_BREATHING,7,ARMOR,"powered scout armor","powered scout armor","powered scout armor" ),

    Object( ARMORID+16,100,5,0,5,0,0,1,20,2000,0,0,-9,0,COMMON,I_PERM_ENERGY_RESIST,3,ARMOR,"demonhide armor","demonhide armor","demonhide armor" ),

    Object( SHIELDID+0,20,0,0,0,0,1,1,15,10,0,0,0,0,COMMON,I_NORMAL_SHIELD,0,SHIELD,"buckler","buckler","buckler" ),

    Object( SHIELDID+1,40,0,0,0,0,2,1,15,20,0,0,0,0,COMMON,I_NORMAL_SHIELD,1,SHIELD,"small round shield","small round shield","small round shield" ),

    Object( SHIELDID+2,100,0,0,0,0,3,1,15,50,0,0,0,0,COMMON,I_NORMAL_SHIELD,2,SHIELD,"large round shield","large round shield","large round shield" ),

    Object( SHIELDID+3,200,0,0,0,0,4,1,15,75,0,0,0,0,COMMON,I_NORMAL_SHIELD,2,SHIELD,"heater shield","heater shield","heater shield" ),

    Object( SHIELDID+4,300,0,0,0,0,5,1,15,150,0,0,0,0,COMMON,I_NORMAL_SHIELD,3,SHIELD,"hoplon","hoplon","hoplon" ),

    Object( SHIELDID+5,500,0,0,0,0,6,1,15,200,0,0,0,0,COMMON,I_NORMAL_SHIELD,4,SHIELD,"tower shield","tower shield","tower shield" ),

    Object( SHIELDID+6,20,0,0,0,0,7,1,15,1000,0,0,0,0,COMMON,I_NORMAL_SHIELD,6,SHIELD,"plasteel shield","plasteel shield","plasteel shield" ),

    Object( SHIELDID+7,200,6,0,0,0,6,1,15,2000,0,0,0,0,COMMON,I_PERM_DEFLECT,7,SHIELD,"shield of deflection","shield of deflection","shield of deflection" ),

    Object( CLOAKID+0,30,0,0,0,0,0,1,4,15,0,0,0,0,COMMON,I_NO_OP,0,CLOAK,"?","cloak of wool","cloak of wool" ),

    Object( CLOAKID+1,30,0,0,0,0,0,1,4,500,0,0,0,0,COMMON,I_PERM_NEGIMMUNE,3,CLOAK,"?","cloak of negimmunity","cloak of level drain" ),

    Object( CLOAKID+2,30,0,0,0,0,0,1,4,500,0,0,0,0,COMMON,I_PERM_INVISIBLE,5,CLOAK,"?","cloak of invisibility","cloak of invisibility" ),

    Object( CLOAKID+3,30,0,0,0,0,0,1,4,1000,0,0,0,0,COMMON,I_PERM_ACCURACY,5,CLOAK,"?","cloak of skill","cloak of skill" ),

    Object( CLOAKID+4,30,0,0,0,0,0,1,4,500,0,0,0,0,COMMON,I_PERM_PROTECTION,3,CLOAK,"?","cloak of protection","cloak of damage" ),

    Object( CLOAKID+5,30,0,0,0,0,0,1,4,2000,0,0,0,0,COMMON,I_PERM_DISPLACE,6,CLOAK,"?","cloak of displacement","cloak of vulnerability" ),

    Object( CLOAKID+6,30,0,0,0,0,0,1,4,500,0,0,0,0,COMMON,I_PERM_TRUESIGHT,3,CLOAK,"?","cloak of true sight","cloak of blindness" ),

    Object( BOOTID+0,30,0,0,0,0,0,1,6,1000,0,0,0,0,COMMON,I_PERM_SPEED,4,BOOTS,"?","boots of speed","boots of slowness" ),

    Object( BOOTID+1,30,0,0,0,0,0,1,6,1000,0,0,0,0,COMMON,I_PERM_HERO,6,BOOTS,"?","boots of heroism","boots of cowardliness" ),

    Object( BOOTID+2,30,0,0,0,0,0,1,6,500,0,0,0,0,COMMON,I_PERM_LEVITATE,3,BOOTS,"?","boots of levitation","boots of levitation" ),

    Object( BOOTID+3,30,0,0,0,0,0,1,6,250,0,0,0,0,COMMON,I_PERM_AGILITY,3,BOOTS,"?","boots of agility","boots of clumsiness" ),

    Object( BOOTID+4,30,0,0,0,0,0,1,6,200,0,0,0,0,COMMON,I_BOOTS_JUMPING,2,BOOTS,"?","jumping boots","jumping boots" ),

    Object( BOOTID+5,30,0,0,0,0,0,1,6,7,0,0,0,0,COMMON,I_NO_OP,0,BOOTS,"?","boots of leather","boots of leather" ),

    Object( BOOTID+6,30,0,0,0,0,0,1,6,2700,0,0,0,0,COMMON,I_BOOTS_7LEAGUE,7,BOOTS,"?","seven league boots","umpteen league boots" ),

    Object( RINGID+0,1,0,0,0,0,0,1,10,400,0,0,0,0,COMMON,I_PERM_TRUESIGHT,3,RING,"?","ring of TRUESIGHT","ring of blindness" ),

    Object( RINGID+1,1,0,0,0,0,0,1,10,1,0,0,-1,0,COMMON,I_PERM_BURDEN,0,RING,"?","ring of burden","ring of burden" ),

    Object( RINGID+2,1,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_PERM_STRENGTH,1,RING,"?","ring of strength","ring of strength" ),

    Object( RINGID+3,1,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_PERM_GAZE_IMMUNE,2,RING,"?","ring of gaze immunity","ring of gaze immunity" ),

    Object( RINGID+4,1,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_PERM_FIRE_RESIST,3,RING,"?","ring of fire resistance","ring of fire resistance" ),

    Object( RINGID+5,1,0,0,0,0,0,1,10,10,0,0,0,0,COMMON,I_NO_OP,0,RING,"?","ring of brass and glass","ring of brass and glass" ),

    Object( RINGID+6,1,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_PERM_POISON_RESIST,4,RING,"?","ring of poison resistance","ring of poison" ),

    Object( RINGID+7,1,0,0,0,0,0,1,10,1000,0,0,0,0,COMMON,I_PERM_REGENERATE,5,RING,"?","ring of regeneration","ring of regeneration" ),

    /*Object( RINGID+172,1,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_PERM_KNOWLEDGE,0,RING,"?","ring of self-knowledge","ring of delusion" ),*/

    Object( RINGID+8,1,0,0,0,0,0,1,10,500,0,0,0,0,COMMON,I_PERM_PROTECTION,4,RING,"?","ring of protection","ring of vulnerability" ),

    Object( STICKID+0,80,0,0,0,0,0,1,10,500,0,0,0,0,COMMON,I_FIREBOLT,3,STICK,"?","staff of firebolts","staff of firebolts" ),

    Object( STICKID+1,80,0,0,0,0,0,1,10,10,0,0,0,0,COMMON,I_NOTHING,0,STICK,"?","walking stick","walking stick" ),

    Object( STICKID+2,80,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_SLEEP_OTHER,1,STICK,"?","staff of sleep","staff of sleep" ),

    Object( STICKID+3,80,0,0,0,0,0,1,10,500,0,0,0,0,COMMON,I_LBALL,4,STICK,"?","wand of ball lightning","wand of ball lightning" ),

    Object( STICKID+4,80,0,0,0,0,0,1,10,500,0,0,0,0,COMMON,I_SUMMON,2,STICK,"?","rod of summoning","rod of summoning" ),

    Object( STICKID+5,80,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_HIDE,1,STICK,"?","rod of hiding","rod of hiding" ),

    Object( STICKID+6,80,0,0,0,0,0,1,10,500,0,0,0,0,COMMON,I_LBOLT,3,STICK,"?","staff of lightning bolts","staff of lightning bolts" ),

    Object( STICKID+7,80,0,0,0,0,0,1,10,500,0,0,0,0,COMMON,I_FIREBALL,5,STICK,"?","wand of fireballs","wand of fireballs" ),

    Object( STICKID+8,80,0,0,0,0,0,1,10,2000,0,0,0,0,COMMON,I_DISINTEGRATE,7,STICK,"?","rod of disintegration","rod of disintegration" ),

    Object( STICKID+9,80,0,0,0,0,0,1,10,1000,0,0,0,0,COMMON,I_DISRUPT,6,STICK,"?","staff of disruption","staff of disruption" ),

    Object( STICKID+10,80,0,0,0,0,0,1,10,100,0,0,0,0,COMMON,I_SNOWBALL,2,STICK,"?","snowball stick","snowball stick" ),

    Object( STICKID+11,80,0,0,0,0,0,1,10,50,0,0,0,0,COMMON,I_MISSILE,1,STICK,"?","staff of missiles","staff of missiles" ),

    Object( STICKID+12,80,0,0,0,0,0,1,10,200,0,0,0,0,COMMON,I_APPORT,2,STICK,"?","rod of apportation","rod of lossage" ),

    Object( STICKID+13,80,0,0,0,0,0,1,10,750,0,0,0,0,COMMON,I_DISPEL,6,STICK,"?","staff of dispelling","staff of self-dispelling" ),

    Object( STICKID+14,80,0,0,0,0,0,1,10,500,0,0,0,0,COMMON,I_HEAL,3,STICK,"?","staff of healing","staff of harming" ),

    Object( STICKID+15,80,0,0,0,0,0,1,10,1000,0,0,0,0,COMMON,I_POLYMORPH,3,STICK,"wand of stupidity","wand of polymorph","wand of stupidity" ),

    Object( STICKID+16,80,0,0,0,0,0,1,10,500,0,0,0,0,COMMON,I_FEAR,2,STICK,"?","wand of fear","wand of fear" ),

    Object( ARTIFACTID+0,500,0,0,0,0,0,1,15,10000,0,0,0,0,UNIQUE_MADE,I_ORBMASTERY,10,ARTIFACT,"Mysterious Orb","Orb of Mastery","Orb of Mastery" ),

    Object( ARTIFACTID+1,50,0,0,0,0,0,1,15,2000,0,0,0,0,COMMON,I_ORBFIRE,9,ARTIFACT,"Mysterious Orb","Orb of Fire","Orb of Fire" ),

    Object( ARTIFACTID+2,50,0,0,0,0,0,1,15,2000,0,0,0,0,COMMON,I_ORBWATER,8,ARTIFACT,"Mysterious Orb","Orb of Water","Orb of Water" ),

    Object( ARTIFACTID+3,50,0,0,0,0,0,1,15,2000,0,0,0,0,COMMON,I_ORBEARTH,7,ARTIFACT,"Mysterious Orb","Orb of Earth","Orb of Earth" ),

    Object( ARTIFACTID+4,50,0,0,0,0,0,1,15,2000,0,0,0,0,COMMON,I_ORBAIR,6,ARTIFACT,"Mysterious Orb","Orb of Air","Orb of Air" ),

    Object( ARTIFACTID+5,25,0,0,0,0,0,1,15,100,0,0,0,0,COMMON,I_ORBDEAD,1,ARTIFACT,"Black Orb","Burnt-out Orb","Burnt-out Orb" ),

    Object( ARTIFACTID+6,75,0,0,0,0,0,1,15,500,0,0,0,0,COMMON,I_CRYSTAL,3,ARTIFACT,"Glass Orb","Crystal Ball","Ball of Mindlessness" ),

    Object( ARTIFACTID+7,25,0,0,0,0,0,1,15,1000,0,0,0,0,COMMON,I_ANTIOCH,5,ARTIFACT,"Metal Pineapple","Holy Hand-Grenade of Antioch","Holy Hand-Grenade of Antioch" ),

    Object( ARTIFACTID+8,500,0,0,0,0,0,1,15,1,0,0,0,0,UNIQUE_UNMADE,I_NOTHING,1,ARTIFACT,"Mystic Amulet","Amulet of Yendor","Amulet of Yendor" ),

    Object( ARTIFACTID+9,1,0,0,0,0,0,1,15,20000,0,0,0,0,UNIQUE_MADE,I_KOLWYNIA,7,ARTIFACT,"Key","Kolwynia, the Key That Was Lost","Kolwynia, the Key That was Lost" ),

    Object( ARTIFACTID+10,10,0,0,0,0,0,1,15,500,0,0,0,0,UNIQUE_UNMADE,I_DEATH,3,ARTIFACT,"?","Potion of Death","Potion of Death" ),

    Object( ARTIFACTID+11,100,0,0,0,0,0,1,15,2000,0,0,0,0,UNIQUE_UNMADE,I_ENCHANTMENT,5,ARTIFACT,"Scintillating Staff","Staff of Enchantment","Staff of Enchantment" ),

    Object( ARTIFACTID+12,0,0,0,0,0,0,1,15,1500,0,0,0,0,UNIQUE_UNMADE,I_HELM,2,ARTIFACT,"Strange Weightless Helm","Helm of Teleportation","Helm of Teleportation" ),

    Object( ARTIFACTID+13,10,0,0,0,0,0,1,15,500,0,0,0,0,UNIQUE_UNMADE,I_LIFE,6,ARTIFACT,"?","Potion of Life","Potion of Life" ),

    Object( ARTIFACTID+14,5,0,0,0,0,1,1,15,5000,0,0,0,0,UNIQUE_MADE,I_SYMBOL,10,ARTIFACT,"Silver Gallows","Holy Symbol of Odin","Holy Symbol of Odin" ),

    Object( ARTIFACTID+15,5,0,0,0,0,2,1,15,5000,0,0,0,0,UNIQUE_MADE,I_SYMBOL,10,ARTIFACT,"Jet Scarab","Holy Symbol of Set","Holy Symbol of Set" ),

    Object( ARTIFACTID+16,5,0,0,0,0,3,1,15,5000,0,0,0,0,UNIQUE_MADE,I_SYMBOL,10,ARTIFACT,"Gold Owl Pendant","Holy Symbol of Athena","Holy Symbol of Athena" ),

    Object( ARTIFACTID+17,5,0,0,0,0,4,1,15,5000,0,0,0,0,UNIQUE_MADE,I_SYMBOL,10,ARTIFACT,"Obsidian Crescent","Holy Symbol of Hecate","Holy Symbol of Hecate" ),

    Object( ARTIFACTID+18,5,0,0,0,0,5,1,15,5000,0,0,0,0,UNIQUE_MADE,I_SYMBOL,10,ARTIFACT,"Sprig of Mistletoe","Holy Symbol of Druidism","Holy Symbol of Druidism" ),

    Object( ARTIFACTID+19,5,0,0,0,0,6,1,15,5000,0,0,0,0,UNIQUE_MADE,I_SYMBOL,10,ARTIFACT,"Blue Crystal Pendant","Holy Symbol of Destiny","Holy Symbol of Destiny" ),

    Object( ARTIFACTID+20,1000,0,0,0,0,0,1,15,1000,0,0,0,0,UNIQUE_UNMADE,I_JUGGERNAUT,6,ARTIFACT,"Crudely Carved Monolith","Juggernaut of Karnak","Juggernaut of Karnak" ),

    Object( ARTIFACTID+21,10,0,0,0,0,0,1,0,10000,0,0,0,0,UNIQUE_MADE,I_STARGEM,7,ARTIFACT,"Strangely Glowing Crystal","Star Gem","Star Gem" ),

    Object( ARTIFACTID+22,1000,0,0,0,0,0,1,10,1000,0,0,0,0,UNIQUE_MADE,I_SCEPTRE,10,ARTIFACT,"Extremely Heavy Stick ","Sceptre of High Magic","Sceptre of High Magic" ),

    Object( ARTIFACTID+23,10,0,0,0,0,0,1,10,1000,0,0,0,0,UNIQUE_MADE,I_PLANES,10,ARTIFACT,"Octagonal Copper Amulet ","Amulet of the Planes","Amulet of the Planes" ),

    Object( ARTIFACTID+24,100,0,0,0,0,0,1,10,1000,0,0,0,0,UNIQUE_UNMADE,I_HOLDING,3,ARTIFACT,"brown bag","Bag of Holding","Bag of Devouring" ),

    Object( ARTIFACTID+25,10,0,0,0,0,0,1,10,1000,0,0,0,0,UNIQUE_MADE,I_SERENITY,10,ARTIFACT,"Small, simple amulet ","Amulet of Serenity","Amulet of Serenity" ),

    Object( CASHID,0,0,0,0,0,0,1,10,0,0,0,0,0,COMMON,I_NO_OP,0,CASH,"money","money","money (the root of all evil)" ),

    Object( CORPSEID,100,0,0,0,0,0,1,10,0,0,0,0,0,COMMON,I_CORPSE,0,CORPSE,"a mysterious corpse","a mysterious corpse","a mysterious corpse" )
};

#endif
