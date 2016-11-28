#include "ViewPort.h"

#include <Debugging/Debug.h>
#include <3rd Party/TinyXml/tinyxml.h>
#include <Utilities/XmlHelper.h>

TiXmlElement* ViewPort::ToXml() const
{
	auto element = DBG_NEW TiXmlElement("ViewPort");

	auto xElement = XmlHelper::ToXml("X", _X);
	auto yElement = XmlHelper::ToXml("Y", _Y);
	auto widthElement = XmlHelper::ToXml("Width", _Width);
	auto heightElement = XmlHelper::ToXml("Height", _Height);

	element->LinkEndChild(xElement);
	element->LinkEndChild(yElement);
	element->LinkEndChild(widthElement);
	element->LinkEndChild(heightElement);

	return element;
}

void ViewPort::Initialize(TiXmlElement* root)
{
	if (root->ValueStr() != "ViewPort")
		LOG_E("view port xml element error");

	XmlHelper::FromXml(root, "X", _X);
	XmlHelper::FromXml(root, "Y", _Y);
	XmlHelper::FromXml(root, "Width", _Width);
	XmlHelper::FromXml(root, "Height", _Height);
}
