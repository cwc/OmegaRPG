#pragma once

#include "glob.h"

namespace Omega
{
class OmegaRPG
{
public:
    static void printUsage();

    OmegaRPG(void);
    ~OmegaRPG(void);

    const char* SaveFilename();
    void SaveFilename(const char* filename);

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
    const char* _saveFilename;
    bool _continuing;
    bool _scoresOnly;
    bool _showScores;
    bool _showTitle;
};
}
