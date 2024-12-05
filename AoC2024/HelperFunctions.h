#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace AoCHelpers
{
	std::vector<int32_t> SplitStringToIntArray(const std::string& SrcStr);

	void PrintIntArray(const std::vector<int32_t>& IntVec);
}