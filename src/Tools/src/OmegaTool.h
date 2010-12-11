#pragma once

namespace OmegaTool
{
	class OmegaTool
	{
	public:
		static const char* PACK;
		static const char* UNPACK;
		static const char* GENCOLORS;

		OmegaTool(void);
		~OmegaTool(void);
	};

	const char* OmegaTool::PACK = "pack";
	const char* OmegaTool::UNPACK = "unpack";
	const char* OmegaTool::GENCOLORS = "gencolors";
}