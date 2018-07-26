#include "CommFun.h"

std::vector<std::string> CommFun::split(const std::string str, const std::string c)
{
	std::vector<std::string> v;
	std::string::size_type pos1, pos2;
	pos2 = str.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(str.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = str.find(c, pos1);
	}
	if (pos1 != str.length())
		v.push_back(str.substr(pos1));

	return std::move(v);
}
