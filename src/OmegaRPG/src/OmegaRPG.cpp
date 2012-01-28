#include "OmegaRPG.h"

extern "C" {
    char* g_saveFileName = 0;
}

namespace Omega
{
OmegaRPG::OmegaRPG(void)
{
    Initialize();

    _saveFileName = "Omega";
}

OmegaRPG::OmegaRPG(char* filename)
{
    Initialize();

    _saveFileName = filename;
    g_saveFileName = _saveFileName; // TODO Refactor out this global
}

OmegaRPG::~OmegaRPG(void)
{
}

void OmegaRPG::Initialize()
{
    _continuing = 0;
    _scoresOnly = 0;
}

char* OmegaRPG::SaveFileName()
{
    return _saveFileName;
}

bool OmegaRPG::Continuing()
{
    return _continuing;
}

void OmegaRPG::Continuing(bool value)
{
    _continuing = value;
}

bool OmegaRPG::ScoresOnly()
{
    return _scoresOnly;
}

void OmegaRPG::ScoresOnly(bool value)
{
    _scoresOnly = value;
}
}
