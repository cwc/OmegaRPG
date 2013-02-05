#pragma once

extern "C" {
#include "glob.h"
}

#include <stdio.h>

namespace Omega
{
class OmegaRPG
{
public:
    static void printUsage();

    OmegaRPG(void);
    ~OmegaRPG(void);

    char* SaveFilename();
    void SaveFilename(char* filename);

    bool Continuing();
    void Continuing(bool value);

    bool ScoresOnly();
    void ScoresOnly(bool value);

    bool ShowTitle();
    void ShowTitle(bool value);

    bool ShowScores();
    void ShowScores(bool value);

    void startGame();

private:
    char* _saveFilename;
    bool _continuing;
    bool _scoresOnly;
    bool _showScores;
    bool _showTitle;
};
}
