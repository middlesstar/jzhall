#ifndef _PathManager_H_
#define _PathManager_H_	

#include "cocos2d.h"
#include "GameUtils_Shark.h"
#include "GameDef.h"
#include "MathAide.h"


class PathManager : public Singleton<PathManager>
{
public:
	bool initPath(const std::string filePath);

	const MovePoints& getPaths(FISH_PATH_TYPE type, int index) const;

	bool get_string_line(std::string& path, const std::string& all, int& start_pos);

	void ReadPath(const std::string& szPath, int nCount, std::vector<MovePoints>& pathVector);

	void clear();
private:
	std::vector<MovePoints> smallPaths;

};


#endif