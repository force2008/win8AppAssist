#include "stdafx.h"


class Sln{
public:
	Sln(CString _filename);
	~Sln();
	BOOL changeProject(CString oldProjectName,CString newProjectName);
	BOOL addProject(CString projectName,CString newProjectFile,CString nguid);
	CString getContent();
	CString getProjectCfg(CString nguid);
private :
	CString _filename;
	CString _content;
	CFile *_file;

};