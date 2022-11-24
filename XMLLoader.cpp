#include "XMLLoader.h"
#include "pugixml/pugixml.hpp"
#include <iostream>
#include <vector>


QPointF XMLLoader::point(const pugi::char_t * x, const pugi::char_t* y)
/**
*	Creates QPointF from raw XML parsed data. Throws std::out_of_range if value can't fit into a float.
*/
{	
	char *p_endx, *p_endy;

	float x_f = strtof(x, &p_endx);
	float y_f = strtof(y, &p_endy);

	
	if (errno == ERANGE) throw std::out_of_range("failed to parse value into float at Point::Point(): Out of range");
	else return QPoint(x_f, y_f);
}

XMLLoader::XMLLoader()
{}
XMLLoader::~XMLLoader()
{}
std::vector<QPointF> XMLLoader::readXML(std::string& filePath, std::string* status)
/**
*	Parses XML file at filePath and creates a list of QPointF objects representing markers coordinates. If error occures, details are saved into *status.
*/
{
	std::vector<QPointF> points;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filePath.c_str());
	if (result)
	{
		pugi::xml_node plot = doc.child("Plot");
		for (pugi::xml_node node = plot.child("Point"); node; node = node.next_sibling())
		{
			pugi::xml_attribute attrX = node.first_attribute();
			pugi::xml_attribute attrY = attrX.next_attribute();	
			try {
				QPointF p = point(attrX.value(), attrY.value());
				points.push_back(p);
				
			}
			catch (const std::out_of_range)
			{
				std::string failed_point = attrX.value();
				failed_point += " ";
				failed_point += attrY.value();
				* status = "Failed to parse: invalid point value" + failed_point;
				points.clear();
				break;
			}	
		}
	}
	else
	{
		*status = "failed to read file " + filePath;
		std::cout << "failed to load XML: " << result.description();
	}
	return points;
}