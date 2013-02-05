#include "PackTool.h"

extern "C" {
    int pack(int argc, char* args[]);
    int unpack(int argc, char* args[]);
}

namespace OmegaTool
{
int PackTool::Pack(int argc, char* args[])
{
    return pack(argc, args);
}

int PackTool::UnPack(int argc, char* args[])
{
    return unpack(argc, args);
}
}