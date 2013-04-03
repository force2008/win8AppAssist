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
BOOL Sln::addProject(CString projectName,CString newProjectFile,CString guid){
	CString s;
	if(newProjectFile.Find(L".jsproj")!=-1)
		s.Format(_T("\r\nProject(\"{262852C6-CD72-467D-83FE-5EEB1973A190}\") = \"%s\", \"%s\", \"%s\"\r\nEndProject"),projectName,newProjectFile,guid);
	else if(newProjectFile.Find(L".vcxproj")!=-1)
		s.Format(_T("\r\nProject(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"%s\", \"%s\", \"%s\"\r\nEndProject"),projectName,newProjectFile,guid);
	else if(newProjectFile.Find(L".csproj")!=-1)
		s.Format(_T("\r\nProject(\"{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}\") = \"%s\", \"%s\", \"%s\"\r\nEndProject"),projectName,newProjectFile,guid);
	int length = strlen("EndProject");
	int index = _content.Find(_T("EndProject"));
	ASSERT(index!=-1);
	_content.Insert(index+length,s);
	CString cfg = getProjectCfg(guid);
	length = strlen("postSolution");
	index = _content.Find(_T("postSolution"));
	ASSERT(index!=-1);
	_content.Insert(index+length+2,cfg);
	return true;
}
CString Sln::getProjectCfg(CString nguid){
	CString projectCfg("\t\t%s.%s|Any CPU.ActiveCfg = %s|Any CPU\r\n\
		%s.%s|Any CPU.Build.0 = %s|Any CPU\r\n\
		%s.%s|ARM.ActiveCfg = %s|ARM\r\n\
		%s.%s|ARM.Build.0 = %s|ARM\r\n\
		%s.%s|Mixed Platforms.ActiveCfg = %s|x86\r\n\
		%s.%s|Mixed Platforms.Build.0 = %s|x86\r\n\
		%s.%s|Win32.ActiveCfg = %s|x86\r\n\
		%s.%s|Win32.Build.0 = %s|x86\r\n\
		%s.%s|x64.ActiveCfg = %s|x64\r\n\
		%s.%s|x64.Build.0 = %s|x64\r\n\
		%s.%s|x86.ActiveCfg = %s|x86\r\n\
		%s.%s|x86.Build.0 = %s|x86\r\n");
	CString debugcfg;
	debugcfg.Format(projectCfg,nguid,_T("Debug"),_T("Debug"),//1
		nguid,_T("Debug"),_T("Debug"),//2
		nguid,_T("Debug"),_T("Debug"),//3
		nguid,_T("Debug"),_T("Debug"),//4
		nguid,_T("Debug"),_T("Debug"),//5
		nguid,_T("Debug"),_T("Debug"),//6
		nguid,_T("Debug"),_T("Debug"),//7
		nguid,_T("Debug"),_T("Debug"),//8
		nguid,_T("Debug"),_T("Debug"),//9
		nguid,_T("Debug"),_T("Debug"),//10
		nguid,_T("Debug"),_T("Debug"),//11
		nguid,_T("Debug"),_T("Debug"),//12
		nguid,_T("Debug"),_T("Debug"));
	CString releasecfg;
	releasecfg.Format(projectCfg,nguid,_T("Release"),_T("Release"),//1
		nguid,_T("Release"),_T("Release"),//2
		nguid,_T("Release"),_T("Release"),//3
		nguid,_T("Release"),_T("Release"),//4
		nguid,_T("Release"),_T("Release"),//5
		nguid,_T("Release"),_T("Release"),//6
		nguid,_T("Release"),_T("Release"),//7
		nguid,_T("Release"),_T("Release"),//8
		nguid,_T("Release"),_T("Release"),//9
		nguid,_T("Release"),_T("Release"),//10
		nguid,_T("Release"),_T("Release"),//11
		nguid,_T("Release"),_T("Release"),//12
		nguid,_T("Release"),_T("Release"));
	return (debugcfg+releasecfg);
}
CString Sln::getContent(){
	return _content;
}