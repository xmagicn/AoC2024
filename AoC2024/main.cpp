#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <queue>
#include <map>
#include <unordered_map>
#include <stack>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <functional>

#include "HelperFunctions.h"

// Unused Param
#pragma warning( disable : 4100 )

int Year24Day1Part1( const std::string& Filename )
{
	std::ifstream myfile;
	myfile.open( Filename );

	int CurrentSum = 0;
	int CurrentMax = 0;

	std::vector<int32_t> LeftVec, RightVec;

	while ( myfile.good() )
	{
		char line[256];
		myfile.getline( line, 256, ' ');
		LeftVec.push_back(std::stoi(line));
		myfile.getline(line, 256);
		RightVec.push_back(std::stoi(line));
	}

	myfile.close();

	if (LeftVec.size() != RightVec.size())
	{
		// Shouldn't happen!
		return -1;
	}

	std::sort(LeftVec.begin(), LeftVec.end());
	std::sort(RightVec.begin(), RightVec.end());

	size_t ListSize = LeftVec.size();

	for (size_t Idx = 0; Idx < ListSize; ++Idx)
	{
		CurrentSum += std::abs(LeftVec[Idx] - RightVec[Idx]);
	}

	return CurrentSum;
}

int Year24Day1Part2( const std::string& Filename )
{
	std::ifstream myfile;
	myfile.open(Filename);

	int CurrentSum = 0;
	int CurrentMax = 0;

	std::vector<int32_t> LeftVec;
	std::map<int32_t, int32_t> RightMap;

	while (myfile.good())
	{
		char line[256];
		myfile.getline(line, 256, ' ');
		LeftVec.push_back(std::stoi(line));
		myfile.getline(line, 256);
		int32_t RightKey = std::stoi(line);
		auto RightIt = RightMap.find(RightKey);
		if (RightIt != RightMap.end())
		{
			RightIt->second++;
		}
		else
		{
			RightMap.insert(std::make_pair(RightKey, 1));
		}
	}

	myfile.close();

	for (int32_t Val : LeftVec)
	{
		auto RightIt = RightMap.find(Val);
		if (RightIt != RightMap.end())
		{
			CurrentSum += RightIt->second * Val;
		}
	}

	return CurrentSum;
}

bool AreEntriesValid(int32_t A, int32_t B, int32_t SignVal, int32_t MinDiff, int32_t MaxDiff)
{
	int32_t Diff = A - B;
	return (
		SignVal * Diff >= 0
		&& std::abs(Diff) >= MinDiff
		&& std::abs(Diff) <= MaxDiff
		);
}

// UnsafeIdx can be -1 to allow for a single error 
bool IsReportSafe(const std::vector<int32_t>& Report, int32_t MinDiff, int32_t MaxDiff, int32_t& UnsafeIdx)
{
	int32_t Prev = 0; // Always multiply against Prev to see if we change signs
	bool bSafe = true;

	for (size_t Idx = 1; Idx < Report.size(); ++Idx)
	{
		int32_t A = Report[Idx - 1];
		int32_t B = Report[Idx];

		int32_t Diff = A - B;
		if (AreEntriesValid(A, B, Prev, MinDiff, MaxDiff))
		{
			Prev = A - B;
			continue;
		}

		if (UnsafeIdx < 0)
		{
			// End of Report. Discard final entry
			if (Idx == Report.size() - 1)
			{
				UnsafeIdx = Idx;
				continue;
			}

			if (A == B)
			{
				UnsafeIdx = Idx - 1;
				continue;
			}

			bool bZA = Idx - 1 > 0 ? AreEntriesValid(Report[Idx - 2], A, Prev, MinDiff, MaxDiff) : true;
			bool bZB = Idx - 1 > 0 ? AreEntriesValid(Report[Idx - 2], B, Prev, MinDiff, MaxDiff) : true;
			bool bAC = Idx < Report.size() - 1 ? AreEntriesValid(A, Report[Idx + 1], Prev, MinDiff, MaxDiff) : true;
			bool bBC = Idx < Report.size() - 1 ? AreEntriesValid(B, Report[Idx + 1], Prev, MinDiff, MaxDiff) : true;

			//bool bBSafe = Idx - 1 > 0 ? AreEntriesValid(Report[Idx - 2], B, Prev, MinDiff, MaxDiff) : true;
			//bool bASafe = Idx < Report.size() - 1 ? AreEntriesValid(A, Report[Idx + 1], Prev, MinDiff, MaxDiff) : true;

			if (bZB && bBC)
			{
				UnsafeIdx = Idx - 1;
				if (Idx > 1) { ++Idx; }
				continue;
			}

			if (bZA && bAC)
			{
				UnsafeIdx = Idx;
				if (Idx > 1) { ++Idx; }
				continue;
			}
		}

		bSafe = false;
		break;
	}

	return bSafe;
}

int Year24Day2Part1(const std::string& Filename)
{
	std::ifstream myfile;
	myfile.open(Filename);

	int CurrentSum = 0;
	std::vector<std::vector<int32_t>> Reports;

	while (myfile.good())
	{
		char line[256];
		myfile.getline(line, 256);

		Reports.push_back(AoCHelpers::SplitStringToIntArray(line));
	}

	myfile.close();

	const int32_t MinDiff = 1;
	const int32_t MaxDiff = 3;
	for (const std::vector<int32_t>& Report : Reports)
	{
		int32_t Prev = 0; // Always multiply against Prev to see if we change signs
		bool bSafe = true;

		for (size_t Idx = 1; Idx < Report.size(); ++Idx)
		{
			int32_t A = Report[Idx - 1];
			int32_t B = Report[Idx];

			if (AreEntriesValid(A, B, Prev, MinDiff, MaxDiff))
			{
				Prev = A - B;
				continue;
			}
			bSafe = false;
			break;
		}

		if (bSafe)
		{
			++CurrentSum;
		}

		//AoCHelpers::PrintIntArray(Report);
		//std::cout << (bSafe ? " Safe" : "") << std::endl;

	}

	return CurrentSum;
}

int Year24Day2Part2(const std::string& Filename)
{
	std::ifstream myfile;
	myfile.open(Filename);

	int CurrentSum = 0;
	std::vector<std::vector<int32_t>> Reports;

	while (myfile.good())
	{
		char line[256];
		myfile.getline(line, 256);

		Reports.push_back(AoCHelpers::SplitStringToIntArray(line));
	}

	myfile.close();

	const int32_t MinDiff = 1;
	const int32_t MaxDiff = 3;
	int32_t Ct = 0;
	for (const std::vector<int32_t>& Report : Reports)
	//std::vector<int32_t> Report({ 50, 56, 58, 55, 52 });
	{
		int32_t UnsafeIdx = -1;
		bool bSafe = IsReportSafe(Report, MinDiff, MaxDiff, UnsafeIdx);

		if (!bSafe)
		{
			for (size_t Idx = 0; Idx < Report.size() - 1; ++Idx)
			{
				UnsafeIdx = Idx;
				std::vector<int32_t> EditedReport(Report);
				EditedReport.erase(EditedReport.begin() + UnsafeIdx, EditedReport.begin() + UnsafeIdx + 1);
				bSafe = IsReportSafe(EditedReport, MinDiff, MaxDiff, UnsafeIdx);
				if (bSafe)
				{
					break;
				}
			}

			if (!bSafe)
			{
				UnsafeIdx = Report.size() - 1;
				std::vector<int32_t> EditedReport(Report.begin(), Report.end() - 1);
				bSafe = IsReportSafe(EditedReport, MinDiff, MaxDiff, UnsafeIdx);
			}
		}

		if (bSafe)
		{
			CurrentSum++;
			//std::cout << Ct << std::endl;
		}
		Ct++;

		/*
		int32_t UnsafeIdx = -1;
		bool bSafe = IsReportSafe(Report, MinDiff, MaxDiff, UnsafeIdx);

		if (!bSafe)
		{
			UnsafeIdx = 0;
			bSafe = IsReportSafe(std::vector<int32_t>(Report.begin() + 1, Report.end()), MinDiff, MaxDiff, UnsafeIdx);
			if (!bSafe)
			{
				std::vector<int32_t> LastTry( Report );
				LastTry.erase(LastTry.begin() + 1, LastTry.begin() + 2);
				bSafe = IsReportSafe(LastTry, MinDiff, MaxDiff, UnsafeIdx);
			}
		}

		if (bSafe)
		{
			++CurrentSum;
		}

		if (!bSafe)
		//if (Report[0] == Report[1] && bSafe)
		{
			AoCHelpers::PrintIntArray(Report);
			std::cout << (bSafe ? " Safe, " : ", ") << "Unsafe Idx: " << UnsafeIdx << std::endl;
		}
		*/
	}

	return CurrentSum;
}

int main()
{
	/*
	std::string Day1Sample( "..\\Input\\Day1Sample.txt" );
	std::string Day1Input("..\\Input\\Day1Input.txt" );
	std::cout << "Day1Part1Sample: " << Year24Day1Part1( Day1Sample ) << std::endl;
	std::cout << "Day1Part1: " << Year24Day1Part1(Day1Input) << std::endl;
	std::cout << "Day1Part2Sample: " << Year24Day1Part2( Day1Sample ) << std::endl;
	std::cout << "Day1Part2: " << Year24Day1Part2( Day1Input ) << std::endl;
	*/


	std::string Day2Sample( "..\\Input\\Day2Sample.txt" );
	std::string Day2EdgeCases( "..\\Input\\Day2EdgeCases.txt" );
	std::string Day2Input( "..\\Input\\Day2Input.txt" );
	std::cout << "Day2Part1Sample: " << Year24Day2Part1( Day2Sample ) << std::endl;
	std::cout << "Day2Part1: " << Year24Day2Part1( Day2Input ) << std::endl;
	std::cout << "Day2Part2Sample: " << Year24Day2Part2( Day2Sample ) << std::endl;
	std::cout << "Day2Part2EdgeCases: " << Year24Day2Part2(Day2EdgeCases) << std::endl;
	std::cout << "Day2Part2: " << Year24Day2Part2( Day2Input ) << std::endl;


	/*
	std::string Day3Sample( "..\\Input\\Day3Sample.txt" );
	std::string Day3Input( "..\\Input\\Day3Input.txt" );
	std::cout << "Day3Part1Sample: " << Year23Day3Part1( Day3Sample ) << std::endl;
	std::cout << "Day3Part1: " << Year23Day3Part1( Day3Input ) << std::endl;
	std::cout << "Day3Part2Sample: " << Year23Day3Part2( Day3Sample ) << std::endl;
	std::cout << "Day3Part2: " << Year23Day3Part2( Day3Input ) << std::endl;

	std::string Day4Sample( "..\\Input\\Day4Sample.txt" );
	std::string Day4Input( "..\\Input\\Day4Input.txt" );
	std::cout << "Day4Part1Sample: " << Year23Day4Part1( Day4Sample ) << std::endl;
	std::cout << "Day4Part1: " << Year23Day4Part1( Day4Input ) << std::endl;
	std::cout << "Day4Part2Sample: " << Year23Day4Part2( Day4Sample ) << std::endl;
	std::cout << "Day4Part2: " << Year23Day4Part2( Day4Input ) << std::endl;

	std::string Day5Sample( "..\\Input\\Day5Sample.txt" );
	std::string Day5Input("..\\Input\\Day5Input.txt" );
	std::cout << "Day5Part1Sample: " << Year23Day5Part1( Day5Sample ) << std::endl;
	std::cout << "Day5Part1: " << Year23Day5Part1( Day5Input ) << std::endl;
	std::cout << "Day5Part2Sample: " << Year23Day5Part2( Day5Sample ) << std::endl;
	std::cout << "Day5Part2: " << Year23Day5Part2( Day5Input ) << std::endl;

	std::unordered_map<long, long> Day6Sample{ {7, 9}, {15, 40}, {30, 200} };
	std::unordered_map<long, long> Day6Input{ {48, 296}, {93, 1928}, {85, 1236}, {95, 1391} };
	std::cout << "Day6Part1Sample: " << Year23Day6Part1( Day6Sample ) << std::endl;
	std::cout << "Day6Part1: " << Year23Day6Part1( Day6Input ) << std::endl;
	std::cout << "Day6Part2Sample: " << CalcMaxRaceDistance(71530, 940200) << std::endl;
	std::cout << "Day6Part2: " << CalcMaxRaceDistance(48938595, 296192812361391) << std::endl;


	std::string Day7Sample( "..\\Input\\Day7Sample.txt" );
	std::string Day7Input("..\\Input\\Day7Input.txt" );
	std::cout << "Day7Part1Sample: " << Year23Day7Part1( Day7Sample ) << std::endl;
	std::cout << "Day7Part1: " << Year23Day7Part1( Day7Input ) << std::endl;
	std::cout << "Day7Part2Sample: " << Year23Day7Part2( Day7Sample ) << std::endl;
	std::cout << "Day7Part2: " << Year23Day7Part2( Day7Input ) << std::endl;

	std::string Day8Sample( "..\\Input\\Day8Sample.txt" );
	std::string Day8Sample2( "..\\Input\\Day8Sample2.txt" );
	std::string Day8Input("..\\Input\\Day8Input.txt" );
	std::cout << "Day8Part1Sample: " << Year23Day8Part1( Day8Sample ) << std::endl;
	std::cout << "Day8Part1: " << Year23Day8Part1( Day8Input ) << std::endl;
	std::cout << "Day8Part2Sample: " << Year23Day8Part2( Day8Sample2 ) << std::endl;
	std::cout << "Day8Part2: " << Year23Day8Part2( Day8Input ) << std::endl;

	std::string Day9Sample( "..\\Input\\Day9Sample.txt" );
	std::string Day9Input("..\\Input\\Day9Input.txt" );
	std::cout << "Day9Part1Sample: " << Year23Day9Part1( Day9Sample ) << std::endl;
	std::cout << "Day9Part1: " << Year23Day9Part1( Day9Input ) << std::endl;
	std::cout << "Day9Part2Sample: " << Year23Day9Part2( Day9Sample ) << std::endl;
	std::cout << "Day9Part2: " << Year23Day9Part2( Day9Input ) << std::endl;
	*/
	//std::string Day10Sample( "..\\Input\\Day10Sample.txt" );
	//std::string Day10Sample2( "..\\Input\\Day10Sample2.txt" );
	//std::string Day10Input("..\\Input\\Day10Input.txt" );
	//std::cout << "Day10Part1Sample: " << Year23Day10Part1( Day10Sample ) << std::endl;
	//std::cout << "Day10Part1Sample2: " << Year23Day10Part1( Day10Sample2 ) << std::endl;
	//std::cout << "Day10Part1: " << Year23Day10Part1( Day10Input ) << std::endl;
	//std::string Day10Part2Sample("..\\Input\\Day10Part2Sample.txt");
	//std::string Day10Part2Sample2("..\\Input\\Day10Part2Sample2.txt");
	//std::string Day10Part2Sample3("..\\Input\\Day10Part2Sample3.txt");
	//std::string Day10Part2Sample4("..\\Input\\Day10Part2Sample4.txt");
	//std::cout << "Day10Part2Sample: " << Year23Day10Part2(Day10Part2Sample) << std::endl;
	//std::cout << "Day10Part2Sample2: " << Year23Day10Part2(Day10Part2Sample2) << std::endl;
	//std::cout << "Day10Part2Sample3: " << Year23Day10Part2(Day10Part2Sample3) << std::endl;
	//std::cout << "Day10Part2Sample4: " << Year23Day10Part2(Day10Part2Sample4) << std::endl;
	//std::cout << "Day10Part2: " << Year23Day10Part2( Day10Input ) << std::endl;
	/*
	std::string Day11Sample( "..\\Input\\Day11Sample.txt" );
	std::string Day11Input("..\\Input\\Day11Input.txt" );
	std::cout << "Day11Part1Sample: " << Year23Day11Part1( Day11Sample ) << std::endl;
	std::cout << "Day11Part1: " << Year23Day11Part1( Day11Input ) << std::endl;
	std::cout << "Day11Part2Sample: " << Year23Day11Part2( Day11Sample ) << std::endl;
	std::cout << "Day11Part2: " << Year23Day11Part2( Day11Input ) << std::endl;

	std::string Day12Sample( "..\\Input\\Day12Sample.txt" );
	std::string Day12Input("..\\Input\\Day12Input.txt" );
	std::cout << "Day12Part1Sample: " << Year23Day12Part1( Day12Sample ) << std::endl;
	std::cout << "Day12Part1: " << Year23Day12Part1( Day12Input ) << std::endl;
	std::cout << "Day12Part2Sample: " << Year23Day12Part2( Day12Sample ) << std::endl;
	std::cout << "Day12Part2: " << Year23Day12Part2( Day12Input ) << std::endl;

	std::string Day13Sample( "..\\Input\\Day13Sample.txt" );
	std::string Day13Input("..\\Input\\Day13Input.txt" );
	std::cout << "Day13Part1Sample: " << Year23Day13Part1( Day13Sample ) << std::endl;
	std::cout << "Day13Part1: " << Year23Day13Part1( Day13Input) << std::endl;
	std::cout << "Day13Part2Sample: " << Year23Day13Part2( Day13Sample, true ) << std::endl;
	std::cout << "Day13Part2: " << Year23Day13Part2( Day13Input ) << std::endl;

	std::string Day14Sample( "..\\Input\\Day14Sample.txt" );
	std::string Day14Input("..\\Input\\Day14Input.txt" );
	std::cout << "Day14Part1Sample: " << Year23Day14Part1( Day14Sample ) << std::endl;
	std::cout << "Day14Part1: " << Year23Day14Part1( Day14Input ) << std::endl;
	std::cout << "Day14Part2Sample: " << Year23Day14Part2( Day14Sample, true ) << std::endl;
	std::cout << "Day14Part2: " << Year23Day14Part2( Day14Input ) << std::endl;

	std::string Day15Sample( "..\\Input\\Day15Sample.txt" );
	std::string Day15Input("..\\Input\\Day15Input.txt" );
	std::cout << "Day15Part1Sample: " << Year23Day15Part1( Day15Sample, 10, true ) << std::endl;
	std::cout << "Day15Part1: " << Year23Day15Part1( Day15Input, 2000000 ) << std::endl;
	std::cout << "Day15Part2Sample: " << Year23Day15Part2( Day15Sample, 20, true ) << std::endl;
	std::cout << "Day15Part2: " << Year23Day15Part2( Day15Input, 4000000 ) << std::endl;

	std::string Day16Sample( "..\\Input\\Day16Sample.txt" );
	std::string Day16Input("..\\Input\\Day16Input.txt" );
	std::cout << "Day16Part1Sample: " << Year23Day16Part1( Day16Sample ) << std::endl;
	std::cout << "Day16Part1: " << Year23Day16Part1( Day16Input ) << std::endl;
	std::cout << "Day16Part2Sample: " << Year23Day16Part2( Day16Sample, true ) << std::endl;
	std::cout << "Day16Part2: " << Year23Day16Part2( Day16Input ) << std::endl;

	std::string Day17Sample("..\\Input\\Day17Sample.txt");
	std::string Day17Input("..\\Input\\Day17Input.txt");
	std::cout << "Day17Part1Sample: " << Year23Day17Part1(Day17Sample) << std::endl;
	std::cout << "Day17Part1: " << Year23Day17Part1(Day17Input) << std::endl;
	std::cout << "Day17Part2Sample: " << Year23Day17Part2(Day17Sample, true) << std::endl;
	std::cout << "Day17Part2: " << Year23Day17Part2(Day17Input) << std::endl;

	std::string Day18Sample("..\\Input\\Day18Sample.txt");
	std::string Day18Input("..\\Input\\Day18Input.txt");
	std::cout << "Day18Part1Sample: " << Year23Day18Part1(Day18Sample) << std::endl;
	std::cout << "Day18Part1: " << Year23Day18Part1(Day18Input) << std::endl;
	std::cout << "Day18Part2Sample: " << Year23Day18Part2(Day18Sample, true) << std::endl;
	std::cout << "Day18Part2: " << Year23Day18Part2(Day18Input) << std::endl;

	std::string Day19Sample("..\\Input\\Day19Sample.txt");
	std::string Day19Input("..\\Input\\Day19Input.txt");
	std::cout << "Day19Part1Sample: " << Year23Day19Part1(Day19Sample) << std::endl;
	std::cout << "Day19Part1: " << Year23Day19Part1(Day19Input) << std::endl;
	std::cout << "Day19Part2Sample: " << Year23Day19Part2(Day19Sample, true) << std::endl;
	std::cout << "Day19Part2: " << Year23Day19Part2(Day19Input) << std::endl;

	std::string Day20Sample("..\\Input\\Day20Sample.txt");
	std::string Day20Input("..\\Input\\Day20Input.txt");
	std::cout << "Day20Part1Sample: " << Year23Day20Part1(Day20Sample) << std::endl;
	std::cout << "Day20Part1: " << Year23Day20Part1(Day20Input) << std::endl;
	std::cout << "Day20Part2Sample: " << Year23Day20Part2(Day20Sample, true) << std::endl;
	std::cout << "Day20Part2: " << Year23Day20Part2(Day20Input) << std::endl;

	std::string Day21Sample("..\\Input\\Day21Sample.txt");
	std::string Day21Input("..\\Input\\Day21Input.txt");
	std::cout << "Day21Part1Sample: " << Year23Day21Part1(Day21Sample) << std::endl;
	std::cout << "Day21Part1: " << Year23Day21Part1(Day21Input) << std::endl;
	std::cout << "Day21Part2Sample: " << Year23Day21Part2(Day21Sample, true) << std::endl;
	std::cout << "Day21Part2: " << Year23Day21Part2(Day21Input) << std::endl;

	std::string Day22Sample("..\\Input\\Day22Sample.txt");
	std::string Day22Input("..\\Input\\Day22Input.txt");
	std::cout << "Day22Part1Sample: " << Year23Day22Part1(Day22Sample) << std::endl;
	std::cout << "Day22Part1: " << Year23Day22Part1(Day22Input) << std::endl;
	std::cout << "Day22Part2Sample: " << Year23Day22Part2(Day22Sample, true) << std::endl;
	std::cout << "Day22Part2: " << Year23Day22Part2(Day22Input) << std::endl;

	std::string Day23Sample("..\\Input\\Day23Sample.txt");
	std::string Day23Input("..\\Input\\Day23Input.txt");
	std::cout << "Day23Part1Sample: " << Year23Day23Part1(Day23Sample) << std::endl;
	std::cout << "Day23Part1: " << Year23Day23Part1(Day23Input) << std::endl;
	std::cout << "Day23Part2Sample: " << Year23Day23Part2(Day23Sample, true) << std::endl;
	std::cout << "Day23Part2: " << Year23Day23Part2(Day23Input) << std::endl;

	std::string Day24Sample("..\\Input\\Day24Sample.txt");
	std::string Day24Input("..\\Input\\Day24Input.txt");
	std::cout << "Day24Part1Sample: " << Year23Day24Part1(Day24Sample) << std::endl;
	std::cout << "Day24Part1: " << Year23Day24Part1(Day24Input) << std::endl;
	std::cout << "Day24Part2Sample: " << Year23Day24Part2(Day24Sample, true) << std::endl;
	std::cout << "Day24Part2: " << Year23Day24Part2(Day24Input) << std::endl;

	std::string Day25Sample("..\\Input\\Day25Sample.txt");
	std::string Day25Input("..\\Input\\Day25Input.txt");
	std::cout << "Day25Part1Sample: " << Year23Day25Part1(Day25Sample) << std::endl;
	std::cout << "Day25Part1: " << Year23Day25Part1(Day25Input) << std::endl;
	std::cout << "Day25Part2Sample: " << Year23Day25Part2(Day25Sample, true) << std::endl;
	std::cout << "Day25Part2: " << Year23Day25Part2(Day25Input) << std::endl;
	//*/

	std::cin.get();

	return 0;
}