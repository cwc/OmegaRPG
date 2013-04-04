
#ifndef GAMESTATE_H
#define GAMESTATE_H

#pragma once


class GameState
{
private:    
    
    /* gamestate bit vector */
    long _state;

    /* static definitions */
    static const long SPOKE_TO_DRUID		= 0x1;
    static const long COMPLETED_CAVES		= 0x2;
    static const long COMPLETED_SEWERS	    = 0x4;
    static const long COMPLETED_CASTLE	    = 0x8;
    static const long COMPLETED_ASTRAL	    = 0x10;
    static const long COMPLETED_VOLCANO	    = 0x20;
    static const long KILLED_DRAGONLORD	    = 0x40;
    static const long KILLED_EATER		    = 0x80;
    static const long KILLED_LAWBRINGER	    = 0x100;
    static const long COMPLETED_CHALLENGE	= 0x200;
    static const long SOLD_CONDO		    = 0x400;
    static const long FAST_MOVE		        = 0x800;
    static const long SKIP_PLAYER		    = 0x1000;
    static const long SKIP_MONSTERS		    = 0x2000;
    static const long MOUNTED			    = 0x4000;
    static const long SUPPRESS_PRINTING	    = 0x8000;
    static const long LOST			        = 0x10000;
    static const long ARENA_MODE		    = 0x20000;
    static const long CHEATED			    = 0x40000;
    static const long BANK_BROKEN		    = 0x80000;
    static const long CLUB_MEMBER		    = 0x100000;
    static const long PREPARED_VOID		    = 0x200000;
    static const long DESTROYED_ORDER		= 0x400000;
    static const long GAVE_STARGEM		    = 0x800000;
    static const long ATTACKED_ORACLE		= 0x1000000;
    static const long UNDEAD_GUARDS		    = 0x2000000;

    /* internal methods for setting gamestate */
    void setGameStatus( long );
    void setGameStatus( bool, long );
    bool getGameStatus( long );
    void resetGameStatus( long );
    bool toggleGameStatus( long );

public:
    GameState( void );
    GameState( long );
    ~GameState( void );
        
    /* has the player spoken to the druid in the north forest */
    bool hasSpokeToDruid();
    void setSpokeToDruid( bool b = true );

    /* has the player defeated the goblin king in the caves */
    bool hasCompletedCaves();
    void setCompletedCaves( bool b = true );

    /* has the player defeated the great wyrm in the sewers under rampart */
    bool hasCompletedSewers();
    void setCompletedSewers( bool b = true );

    /* has the player entered the arch mages court */
    bool hasCompletedCastle();
    void setCompletedCastle( bool b = true );

    /* has the player acquired and used the star gem */
    bool hasCompletedAstral();
    void setCompletedAstral( bool b = true );

    /* has the player defeated the demon emperor */
    bool hasCompletedVolcano();
    void setCompletedVolcano( bool b = true );

    /* has the player defeated the Dragon Lord */
    bool hasKilledDragonlord();
    void setKilledDragonlord( bool b = true );

    /* has the player defeated the Eater of Magic */
    bool hasKilledEater();
    void setKilledEater( bool b = true );

    /* has the player defeated the lawbringer */
    bool hasKilledLawbringer();
    void setKilledLawbringer( bool b = true );

    /* has the player defeated the challenge of omega */
    bool hasCompletedChallenge();
    void setCompletedChallenge( bool b = true );

    /* has the player purchased the condo */
    bool hasPurchasedCondo();
    void setPurchasedCondo( bool b = true );

    /* is the game in fastmove mode */
    bool hasFastMove();
    void setFastMove( bool b = true );

    /* is the player losing a turn */
    bool hasSkipPlayer();
    void setSkipPlayer( bool b = true );

     /* are NPC's losing a turn */
    bool hasSkipMonsters();
    void setSkipMonsters( bool b = true );

    /* is the player mounted */
    bool isMounted();
    void setMounted( bool b = true );

    /* used when for whatever reason we do not want to print status messages */
    bool hasSuppressPrinting();
    void setSuppressPrinting( bool b = true );

    /* is the player lost in the wilderness */
    bool isLost();
    void setLost( bool b = true );

    /* toggled when the player is in the rampart arena */
    bool isInArena();
    void setInArena( bool b = true );

    /* if the player has become a wizard */
    bool isCheater();
    void setCheater( bool b = true );

    /* if the player has broken the ATM */
    bool isBankBroken();
    void setBankBroken( bool b = true );

    /* if the player is a member of the explorers club */
    bool isExplorerClubMember();
    void setExplorerClubMember( bool b = true );

    /* has the player satisified the void's hunger? */
    bool hasPreparedVoid();
    void setPreparedVoid( bool b = true );

    /* destroy the order if the justicar has been killed and all guards are dead */
    bool hasDestroyedOrder();
    void setDestroyedOrder( bool b = true );

    /* if the player has given the stargem to the lawbringer */
    bool hasGivenStargem();
    void setGivenStargem( bool b = true );

    /* set if the player chooses to attack the oracle after ringing the bell */ 
    bool hasAttackedOracle();
    void setAttackedOracle( bool b = true );

    /* entire city is undead? this feature does not appear to have been implemented yet */
    bool hasUndeadGuards();
    void setUndeadGuards( bool b = true );

};

#endif

