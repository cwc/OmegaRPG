#include <string>

#include "OmegaTool.h"
#include "PackTool.h"

namespace OmegaTool
{
	OmegaTool::OmegaTool(void)
	{
	}

	OmegaTool::~OmegaTool(void)
	{
	}
}

int main(int argc, char* args[])
{
	argc--;
	args++;

	if (argc > 0)
	{
		if (!std::strcmp((const char*)args[0], OmegaTool::OmegaTool::PACK))
		{
			return OmegaTool::PackTool::Pack(argc, args);
		} 
	
		if (!std::strcmp((const char*)args[0], OmegaTool::OmegaTool::UNPACK))
		{
			return OmegaTool::PackTool::UnPack(argc, args);
		} 
	
		if (!std::strcmp((const char*)args[0], OmegaTool::OmegaTool::GENCOLORS))
		{
			// TODO Call color generation tool
		}
	}
	
	// TODO Print usage info

	return -1;
}