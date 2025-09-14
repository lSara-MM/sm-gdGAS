#include "include/TagManager.h"
#include <sstream>
#include "TagNode.h"

sm::TagManager::TagManager()
{}

sm::TagManager::~TagManager()
{}

void sm::TagManager::AddTag(const std::string& tag)
{
    std::istringstream iss(tag);
    std::string token;

    while (std::getline(iss, token, '.'))
    {
        m_TagTable->GetOrAddTag(token);
    }
}