#pragma once
#include <string>
#include <vector>
#include "pugixml/pugixml.hpp"
#include <qpoint.h>


class XMLLoader
{
	public:
		XMLLoader();
		~XMLLoader();
		std::vector<QPointF> readXML(std::string& filePath, std::string* status);
		QPointF point(const pugi::char_t* x, const pugi::char_t* y);
};


