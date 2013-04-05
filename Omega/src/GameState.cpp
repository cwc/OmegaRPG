
#include "GameState.h"
GameState State;

GameState::GameState ( void )
{
    _state = 0L;
}

GameState::GameState( long s )
{
    _state = s;
}

GameState::~GameState( void )
{

}

void GameState::setGameStatus( long l )
{
    _state |= l;
}

void GameState::setGameStatus( bool b, long l )
{
    if ( b == true ) 
        setGameStatus(l);
    else resetGameStatus(l);
}

void GameState::resetGameStatus( long l )
{
    _state &= ~l;
}

bool GameState::getGameStatus( long l )
{
    return (_state & l) > 0;
}

bool GameState::getSpokeToDruid()
{
    return(getGameStatus(SPOKE_TO_DRUID));
}

void GameState::setSpokeToDruid( bool b)
{
    setGameStatus(b, SPOKE_TO_DRUID);
}

bool GameState::toggleGameStatus( long l )
{
    if(getGameStatus(l) == true) {
        resetGameStatus(l);
        return false;
    }
    setGameStatus(l);
    return true;
}

bool GameState::getCompletedCaves()
{
    return(getGameStatus(COMPLETED_CAVES));
}

void GameState::setCompletedCaves( bool b )
{
    setGameStatus(b, COMPLETED_CAVES);
}


bool GameState::getCompletedSewers()
{
    return(getGameStatus(COMPLETED_SEWERS));
}

void GameState::setCompletedSewers( bool b )
{
    setGameStatus(b, COMPLETED_SEWERS);
}

bool GameState::getCompletedCastle()
{
    return(getGameStatus(COMPLETED_CASTLE));
}

void GameState::setCompletedCastle( bool b )
{
    setGameStatus(b, COMPLETED_CASTLE);
}


bool GameState::getCompletedAstral()
{
    return(getGameStatus(COMPLETED_ASTRAL));
}

void GameState::setCompletedAstral( bool b )
{
    setGameStatus(b, COMPLETED_ASTRAL);
}


bool GameState::getCompletedVolcano()
{
    return(getGameStatus(COMPLETED_VOLCANO));
}

void GameState::setCompletedVolcano( bool b )
{
    setGameStatus(b, COMPLETED_VOLCANO);
}


bool GameState::getKilledDragonlord()
{
    return(getGameStatus(KILLED_DRAGONLORD));
}

void GameState::setKilledDragonlord( bool b )
{
    setGameStatus(b, KILLED_DRAGONLORD);
}


bool GameState::getKilledEater()
{
    return(getGameStatus(KILLED_EATER));
}

void GameState::setKilledEater( bool b )
{
    setGameStatus(b, KILLED_EATER);
}


bool GameState::getKilledLawbringer()
{
    return(getGameStatus(KILLED_LAWBRINGER));
}

void GameState::setKilledLawbringer( bool b )
{
    setGameStatus(b, KILLED_LAWBRINGER);
}


bool GameState::getCompletedChallenge()
{
    return(getGameStatus(COMPLETED_CHALLENGE));
}

void GameState::setCompletedChallenge( bool b )
{
    setGameStatus(b, COMPLETED_CHALLENGE);
}


bool GameState::getPurchasedCondo()
{
    return(getGameStatus(SOLD_CONDO));
}

void GameState::setPurchasedCondo( bool b )
{
    setGameStatus(b, SOLD_CONDO);
}


bool GameState::getFastMove()
{
    return(getGameStatus(FAST_MOVE));
}

void GameState::setFastMove( bool b )
{
    setGameStatus(b, FAST_MOVE);
}


bool GameState::getSkipPlayer()
{
    return(getGameStatus(SKIP_PLAYER));
}

void GameState::setSkipPlayer( bool b )
{
    setGameStatus(b, SKIP_PLAYER);
}


bool GameState::getSkipMonsters()
{
    return(getGameStatus(SKIP_MONSTERS));
}

void GameState::setSkipMonsters( bool b )
{
    setGameStatus(b, SKIP_MONSTERS);
}


bool GameState::getMounted()
{
    return(getGameStatus(MOUNTED));
}

void GameState::setMounted( bool b )
{
    setGameStatus(b, MOUNTED);
}


bool GameState::getSuppressPrinting()
{
    return(getGameStatus(SUPPRESS_PRINTING));
}

void GameState::setSuppressPrinting( bool b )
{
    setGameStatus(b, SUPPRESS_PRINTING);
}


bool GameState::getLost()
{
    return(getGameStatus(LOST));
}

void GameState::setLost( bool b )
{
    setGameStatus(b, LOST);
}


bool GameState::getInArena()
{
    return(getGameStatus(ARENA_MODE));
}

void GameState::setInArena( bool b )
{
    setGameStatus(b, ARENA_MODE);
}


bool GameState::getCheater()
{
    return(getGameStatus(CHEATED));
}

void GameState::setCheater( bool b )
{
    // once a cheater, always a cheater
    setGameStatus(true, CHEATED);
}


bool GameState::getBankBroken()
{
    return(getGameStatus(BANK_BROKEN));
}

void GameState::setBankBroken( bool b )
{
    setGameStatus(b, BANK_BROKEN);
}


bool GameState::getExplorerClubMember()
{
    return(getGameStatus(CLUB_MEMBER));
}

void GameState::setExplorerClubMember( bool b )
{
    setGameStatus(b, CLUB_MEMBER);
}


bool GameState::getPreparedVoid()
{
    return(getGameStatus(PREPARED_VOID));
}

void GameState::setPreparedVoid( bool b )
{
    setGameStatus(b, PREPARED_VOID);
}


bool GameState::getDestroyedOrder()
{
    return(getGameStatus(DESTROYED_ORDER));
}

void GameState::setDestroyedOrder( bool b )
{
    setGameStatus(b, DESTROYED_ORDER);
}


bool GameState::getGivenStargem()
{
    return(getGameStatus(GAVE_STARGEM));
}

void GameState::setGivenStargem( bool b )
{
    setGameStatus(b, GAVE_STARGEM);
}


bool GameState::getAttackedOracle()
{
    return(getGameStatus(ATTACKED_ORACLE));
}

void GameState::setAttackedOracle( bool b )
{
    setGameStatus(b, ATTACKED_ORACLE);
}

bool GameState::getUndeadGuards()
{
    return(getGameStatus(UNDEAD_GUARDS));
}

void GameState::setUndeadGuards( bool b )
{
    setGameStatus(b, UNDEAD_GUARDS);
}


