#include <string>

#include "OmegaTool.h"
#include "PackTool.h"

using namespace OmegaTool;

int main(int argc, char* args[])
{
	argc--;
	args++;

	if (argc > 0)
	{
		if (!std::strcmp((const char*)args[0], CMD_PACK))
		{
			return PackTool::Pack(argc, args);
		} 
	
		if (!std::strcmp((const char*)args[0], CMD_UNPACK))
		{
			return PackTool::UnPack(argc, args);
		} 
	
		if (!std::strcmp((const char*)args[0], CMD_GENCOLORS))
		{
			// TODO Call color generation tool
		}
	}
	
	// TODO Print usage info

	return -1;
}