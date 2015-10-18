#pragma once

#include "Common.h"

#define CONTENTMANAGER_DEFAULT_ROOTDIRECTORY			"data"
#define CONTENTMANAGER_MAXIMUMFILELISTLINELENGTH		256
#define CONTENTMANAGER_CACHEFILE						"Cache.ini"

class ContentManager
{
private:
	ContentManager();
public:
	virtual ~ContentManager();
public:
	void LoadContent();
	void UnloadContent();
public:
	static ContentManager*	GetInstance();
public:
	const String GetRootDirectory() const;
	const std::string GetFileContents(const char *filename) const;
private:
	static ContentManager* m_pInstance;
private:
	String m_sRootDirectory;
};
