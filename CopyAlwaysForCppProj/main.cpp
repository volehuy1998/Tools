#include "tinyxml2.h"

using namespace tinyxml2;

constexpr const char* FILE_PATH = "<name>.vcxproj";

XMLElement* FindElemByTagName(XMLDocument& xXmlDocument, const char* tagName)
{
	tinyxml2::XMLNode* xElem = xXmlDocument.FirstChildElement();
	while (xElem)
	{
		if (xElem->Value() && 0 == strcmp(xElem->Value(), tagName))
		{
			return xElem->ToElement();
		}
		/*
		*   We move through the XML tree following these rules (basically in-order tree walk):
		*
		*   (1) if there is one or more child element(s) visit the first one
		*       else
		*   (2)     if there is one or more next sibling element(s) visit the first one
		*               else
		*   (3)             move to the parent until there is one or more next sibling elements
		*   (4)             if we reach the end break the loop
		*/
		if (xElem->FirstChildElement()) //(1)
		{
			xElem = xElem->FirstChildElement();
		}
		else if (xElem->NextSiblingElement())  //(2)
		{
			xElem = xElem->NextSiblingElement();
		}
		else
		{
			while (xElem->Parent() && !xElem->Parent()->NextSiblingElement()) //(3)
			{
				xElem = xElem->Parent();
			}
			if (xElem->Parent() && xElem->Parent()->NextSiblingElement())
			{
				xElem = xElem->Parent()->NextSiblingElement();
			}
			else //(4)
			{
				return NULL;
			}
		}
	}
}

int main(int, const char**)
{
	XMLDocument doc;
	XMLError error;
	if (XMLError::XML_SUCCESS == (error = doc.LoadFile(FILE_PATH)))
	{
		// load xml success
		XMLElement* projectElement = FindElemByTagName(doc, "Project");
		if (NULL != projectElement)
		{
			// comment for custom vcxproj
			XMLComment* comment = doc.NewComment("VLH's custom. Copy necessary files to output directory");
			// create item group
			XMLElement* itempGrpElement = doc.NewElement("ItemGroup");
			// create content from item group
			XMLElement* contentFromItempGrpElement = doc.NewElement("Content");
			// set attribute from content element
			contentFromItempGrpElement->SetAttribute("Include", "intro.wav");
			// create this element from content
			XMLElement* outputDirFromContentElement = doc.NewElement("CopyToOutputDirectory");
			outputDirFromContentElement->SetText("PreserveNewest");

			projectElement->InsertEndChild(comment);
			projectElement->InsertEndChild(itempGrpElement);
			itempGrpElement->InsertFirstChild(contentFromItempGrpElement);
			contentFromItempGrpElement->InsertFirstChild(outputDirFromContentElement);

			doc.SaveFile(FILE_PATH);
		}
	}
	else
	{
		// load xml fail
		printf("Load file fail");
	}
	return doc.ErrorID();
}