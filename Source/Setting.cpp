#include "Setting.h"

using namespace tinyxml2;

void jkSetting::ReadLevelSetting(const std::string& setting_file)
{
    XMLDocument docXml;
    XMLError errXml = docXml.LoadFile(setting_file.c_str());
    if (XML_SUCCESS == errXml)
    {
        XMLElement* elmtRoot = docXml.RootElement();
        XMLElement* elmtUser = elmtRoot->FirstChildElement("User");
        XMLElement* elmtName = elmtUser->FirstChildElement("Name");
        if (elmtName)
        {
            const char* pContent = elmtName->GetText();
            printf("%s", pContent);
        }
        XMLElement* elmtAge = elmtName->NextSiblingElement();
        if (elmtAge)
        {
            const char* pContent = elmtAge->GetText();
            printf("%s", pContent);
        }
    }
}
