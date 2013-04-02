#include "stdafx.h"


Sln::Sln(CString filename):_filename(filename){
	_file = new CFile(filename, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	int nCount = _file->GetLength();
	char* ptchBuffer = NULL;
	ptchBuffer = new char[nCount + 1];  
	
	ptchBuffer[nCount] = '\0';  
	_file->Read(ptchBuffer,nCount);
	_content = ptchBuffer;
	if(NULL!=ptchBuffer){
		delete[] ptchBuffer;
		ptchBuffer = NULL;
	}
}
Sln::~Sln(){
	_file->SetLength(0);
	CT2CA outputString(_content, CP_UTF8);
	_file->Write(outputString,::strlen(outputString));
	_file->Close();
	delete _file;
	_file = NULL;
}
BOOL Sln::changeProject(CString oldProjectName,CString newProjectName){
	_content.Replace(_T("webapp\\webapp.jsproj"),newProjectName);
	_content.Replace(_T("webapp"),newProjectName);
	return true;
}
BOOL Sln::addProject(CString projectName,CString newProjectFile,WCHAR *nguid){
	CString s;
	s.Format(_T("\r\nProject(\"{262852C6-CD72-467D-83FE-5EEB1973A190}\") = \"%s\", \"%s\", \"%s\"\r\nEndProject"),projectName,newProjectFile,nguid);
	int length = strlen("EndProject");
	int index = _content.Find(_T("EndProject"));
	ASSERT(index!=-1);
	_content.Insert(index+length,s);
	return true;
}
CString Sln::getContent(){
	return _content;
}