#include "stdafx.h"


class Sln{
public:
	Sln(CString _filename);
	~Sln();
	BOOL changeProject(CString oldProjectName,CString newProjectName);
	BOOL addProject(CString projectName,CString newProjectFile,WCHAR *nguid);
	CString getContent();
private :
	CString _filename;
	CString _content;
	CFile *_file;

};