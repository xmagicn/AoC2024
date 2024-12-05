#include "HelperFunctions.h"

#include <sstream>

std::vector<int32_t> AoCHelpers::SplitStringToIntArray(const std::string& SrcStr)
{
	std::stringstream ss(SrcStr);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);

	std::vector<int32_t> Output;
	for (const std::string& str : vstrings)
	{
		Output.push_back(std::atoi(str.c_str()));
	}

	return Output;
}

void AoCHelpers::PrintIntArray(const std::vector<int32_t>& IntVec)
{
	if (IntVec.empty())
	{
		return;
	}

	for (size_t Idx = 0; Idx < IntVec.size() - 1; ++Idx)
	{
		std::cout << IntVec[Idx] << ", ";
	}

	std::cout << IntVec[IntVec.size() - 1];
}