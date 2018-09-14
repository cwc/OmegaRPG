/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* mon.c */
/* various functions to do with monsters */

#include "glob.h"

const char *mancorpse(void)
{
    switch(random_range(20)) {
    case 0:
        return "dead janitor";
    case 1:
        return "dead beggar";
    case 2:
        return "dead barbarian";
    case 3:
        return "dead hairdresser";
    case 4:
        return "dead accountant";
    case 5:
        return "dead lawyer";
    case 6:
        return "dead indian chief";
    case 7:
        return "dead tinker";
    case 8:
        return "dead tailor";
    case 9:
        return "dead soldier";
    case 10:
        return "dead spy";
    case 11:
        return "dead doctor";
    case 12:
        return "dead miner";
    case 13:
        return "dead noble";
    case 14:
        return "dead serf";
    case 15:
        return "dead neer-do-well";
    case 16:
        return "dead vendor";
    case 17:
        return "dead dilettante";
    case 18:
        return "dead surveyor";
    default:
    case 19:
        return "dead jongleur";
    }
}

const char *angeltype(int mid, int deity)
{
    if (ANGEL == mid )
    {
        switch (deity)
        {
        case ODIN:
            return "angel of Odin";
            break;
        case SET:
            return "angel of Set";
            break;
        case HECATE:
            return "angel of Hecate";
            break;
        case ATHENA:
            return "angel of Athena";
            break;
        case DESTINY:
            return "angel of Destiny";
            break;
        case DRUID:
            return "angel of the Balance";
            break;
        }
    }
    else if (HIGH_ANGEL == mid )
    {
        switch (deity)
        {
        case ODIN:
            return "high angel of Odin";
            break;
        case SET:
            return "high angel of Set";
            break;
        case HECATE:
            return "high angel of Hecate";
            break;
        case ATHENA:
            return "high angel of Athena";
            break;
        case DESTINY:
            return "high angel of Destiny";
            break;
        case DRUID:
            return "high angel of the Balance";
            break;
        }
    }
    else /* ARCHANGEL */
    {
        switch (deity)
        {
        case ODIN:
            return "archangel of Odin";
            break;
        case SET:
            return "archangel of Set";
            break;
        case HECATE:
            return "archangel of Hecate";
            break;
        case ATHENA:
            return "archangel of Athena";
            break;
        case DESTINY:
            return "archangel of Destiny";
            break;
        case DRUID:
            return "archangel of the Balance";
            break;
        }
    }
    /* And, if none of those work, this function's broken -- I'm gonna die. */
    return "angel of Death";
}

/* makes one of the highscore npcs */
void make_hiscore_npc(Monster* npc, int npcid)
{
    int st = -1;
    Object* ob;
    *npc = Monsters[HISCORE_NPC];
    npc->aux2 = npcid;
    /* each of the high score npcs can be created here */
    switch(npcid) {
    case 0:
        strcpy(Str2,Hiscorer);
        npc->determine_npc_behavior(Hilevel,Hibehavior);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        strcpy(Str2,Priest[npcid]);
        npc->determine_npc_behavior(Priestlevel[npcid],Priestbehavior[npcid]);
        st = ARTIFACTID+13+npcid;	/* appropriate holy symbol... */
        Objects[st].uniqueness = UNIQUE_MADE;
        if (npcid == DRUID)
            npc->talkf = M_TALK_DRUID;
        if (Player.patron == npcid)
            m_status_reset(npc, HOSTILE);
        break;
    case 7:
        strcpy(Str2,Shadowlord);
        npc->determine_npc_behavior(Shadowlordlevel,Shadowlordbehavior);
        break;
    case 8:
        strcpy(Str2,Commandant);
        npc->determine_npc_behavior(Commandantlevel,Commandantbehavior);
        if (Player.rank[LEGION])
            m_status_reset(npc, HOSTILE);
        break;
    case 9:
        strcpy(Str2,Archmage);
        npc->determine_npc_behavior(Archmagelevel,Archmagebehavior);
        st = OB_KOLWYNIA; /* kolwynia */
        npc->talkf = M_TALK_ARCHMAGE;
        m_status_reset(npc, WANDERING);
        m_status_reset(npc, HOSTILE);
        break;
    case 10:
        strcpy(Str2,Prime);
        npc->determine_npc_behavior(Primelevel,Primebehavior);
        npc->talkf = M_TALK_PRIME;
        npc->specialf = M_SP_PRIME;
        if (Player.alignment < 0)
            m_status_reset(npc, HOSTILE);
        break;
    case 11:
        strcpy(Str2,Champion);
        npc->determine_npc_behavior(Championlevel,Championbehavior);
        if (Player.rank[ARENA])
            m_status_reset(npc, HOSTILE);
        break;
    case 12:
        strcpy(Str2,Duke);
        npc->determine_npc_behavior(Dukelevel,Dukebehavior);
        break;
    case 13:
        strcpy(Str2,Chaoslord);
        npc->determine_npc_behavior(Chaoslordlevel,Chaoslordbehavior);
        if (Player.alignment < 0 && random_range(2))
            m_status_reset(npc, HOSTILE);
        break;
    case 14:
        strcpy(Str2,Lawlord);
        npc->determine_npc_behavior(Lawlordlevel,Lawlordbehavior);
        if (Player.alignment > 0)
            m_status_reset(npc, HOSTILE);
        break;
    case 15:
        strcpy(Str2,Justiciar);
        npc->determine_npc_behavior(Justiciarlevel,Justiciarbehavior);
        st = OB_JUSTICIAR_BADGE;	/* badge */
        npc->talkf = M_TALK_GUARD;
        npc->specialf = M_SP_WHISTLEBLOWER;
        m_status_reset(npc, WANDERING);
        m_status_reset(npc, HOSTILE);
        break;
    case 16:   /* PGM TODO */
        strcpy(Str2,Grandmaster);
        npc->determine_npc_behavior(Grandmasterlevel,Grandmasterbehavior);
        break;
    }
    if (st > -1 && Objects[st].uniqueness == UNIQUE_MADE) {
        ob = ((Object*) checkmalloc(sizeof(Object)));
        *ob = Objects[st];
        npc->m_pickup(ob);
    }
    npc->name = salloc(Str2);
    strcpy(Str1,"The body of ");
    strcat(Str1,Str2);
    npc->corpseString = salloc(Str1);
    m_status_set( npc, ALLOC );
}

/* makes an ordinary npc (maybe undead) */
void make_log_npc(Monster* npc)
{
    int i,n;
    int behavior,status,level;
    FILE *fd;

    /* in case the log file is null */
    behavior = 2718;
    level = 1;
    status = 2;
    strcpy(Str2,"Malaprop the Misnamed");

    strcpy(Str1,Omegalib);
    strcat(Str1,"omega.log");
    fd = checkfopen(Str1,"r");
    n = 1;
    while(fgets(Str1,STRING_LEN, fd)) {
        if (random_range(n) == 0) {	/* this algo. from Knuth 2 - cute, eh? */
            sscanf(Str1,"%d %d %d",&status,&level,&behavior);
            for (i = 0; (Str1[i] < 'a' || Str1[i] > 'z') &&
                    (Str1[i] < 'A' || Str1[i] > 'Z'); i++)
                ;
            strcpy(Str2, Str1 + i);
            Str2[strlen(Str2) - 1] = '\0';	/* 'cos fgets reads in the \n */
        }
        n++;
    }
    fclose(fd);
    npc->hp = level*20;
    if (status==1) {
        if (level < 3) {
            *npc = Monsters[GHOST];
            strcpy(Str1,"ghost named ");
        }
        else if (level < 7) {
            *npc = Monsters[HAUNT];
            strcpy(Str1,"haunt named ");
        }
        else if (level < 12) {
            *npc = Monsters[SPECTRE];
            strcpy(Str1,"spectre named ");
        }
        else {
            *npc = Monsters[LICHE];
            strcpy(Str1,"lich named ");
        }
        strcat(Str1,Str2);
        npc->name = salloc(Str1);
        strcpy(Str3,"the mortal remains of ");
        strcat(Str3,Str2);
        npc->corpseString = salloc(Str3);
        m_status_set( npc, ALLOC );
    }
    else {
        npc->name=salloc(Str2);
        strcpy(Str3,"the corpse of ");
        strcat(Str3,Str2);
        npc->corpseString = salloc(Str3);
        m_status_set( npc, ALLOC );
    }
    npc->determine_npc_behavior(level,behavior);
}
