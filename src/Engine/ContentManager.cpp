#include "ContentManager.h"

ContentManager::ContentManager()
{
	m_sRootDirectory = CONTENTMANAGER_DEFAULT_ROOTDIRECTORY;
}

ContentManager::~ContentManager()
{
}

void ContentManager::LoadContent()
{
	
}

void ContentManager::UnloadContent()
{
}

ContentManager*	ContentManager::GetInstance()
{
	if(!m_pInstance)
	{
		m_pInstance = new ContentManager();
	}

	return m_pInstance;
}

const String ContentManager::GetRootDirectory() const
{
	return m_sRootDirectory;
}

const 
std::string ContentManager::GetFileContents(const char *filename) const
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::ostringstream contents;
		contents << in.rdbuf();
		in.close();
		return(contents.str());
	}
	throw(errno);
}

ContentManager* ContentManager::m_pInstance = NULL;