
#include "PathManager.h"

USING_NS_CC;


const MovePoints& PathManager::getPaths(FISH_PATH_TYPE nType, int nID) const
{
	switch (nType % MAXTYPE_FISH_PATH)
	{
	case SMALL_FISH_PATH:
	{
			return smallPaths[nID % MAX_SMALL_PATH];
	}
	default:
		break;
	}
	static MovePoints mp;
	return mp;
}


void PathManager::clear()
{
	smallPaths.clear();

}


bool PathManager::get_string_line(std::string& path, const std::string& all, int& start_pos)
{
	path.clear();
	int end_pos = 0;
	unsigned int n = (unsigned int)all.length();

	if ((start_pos >= 0) && (start_pos < n))
	{
		end_pos = (int)all.find_first_of("\n ", start_pos);

		if ((end_pos < 0) && (end_pos > n))
			end_pos = n;

		path += all.substr(start_pos, end_pos - start_pos);
		start_pos = end_pos + 1;

	}
	return !path.empty();
}

void PathManager::ReadPath(const std::string& szPath, int nCount, std::vector<MovePoints>& pathVector)
{
	int x, y, staff;
	float angle;

	std::ostringstream ostr;

	pathVector.clear();
	std::string sData;
	std::string line;
	std::string fullpath;

	for (int i = 1; i < nCount; i++)
	{
		MovePoints move_points;

		ostr.str("");

		ostr << szPath << i << ".dat";

		std::string fullpath = FileUtils::getInstance()->fullPathForFilename(ostr.str().c_str());
		sData = FileUtils::getInstance()->getStringFromFile(fullpath);

		if (sData.empty())
		{
			log("%s failed", fullpath.c_str());
			return;
		}

		int start_pos = 0;
		get_string_line(line, sData, start_pos);

		while (get_string_line(line, sData, start_pos))
		{
			sscanf(line.c_str(), "(%d,%d,%f,%d)", &x, &y, &angle, &staff);
			x *= GameUtils_Shark::winSize.width / pathResolutionSize.width;
			y *= GameUtils_Shark::winSize.height / pathResolutionSize.height;

			MOVE_POINT point;
			point.position = Vec2(x, y);
			point.direction = angle;
			move_points.push_back(point);
		}
		pathVector.push_back(move_points);
	}
}

bool PathManager::initPath(const std::string filePath)
{
	clear();

	std::ostringstream ostr;

	ostr.str("");
	ostr << filePath << "path/";
	ReadPath(ostr.str().c_str(), MAX_SMALL_PATH, smallPaths);

	return true;
}


