#include "OmegaRPG.h"

extern "C" {
    char* g_saveFileName = 0;
}

namespace Omega
{
OmegaRPG::OmegaRPG(void)
{
    Initialize();

    SaveFilename("Omega");
}

OmegaRPG::OmegaRPG(char* filename)
{
    Initialize();

    SaveFilename(filename);
}

OmegaRPG::~OmegaRPG(void)
{
}

void OmegaRPG::Initialize()
{
    _continuing = 0;
    _scoresOnly = 0;
}

char* OmegaRPG::SaveFilename()
{
    return _saveFilename;
}

void OmegaRPG::SaveFilename(char* filename)
{
    _saveFilename = filename;
    g_saveFileName = _saveFilename; // TODO Refactor out this global
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
