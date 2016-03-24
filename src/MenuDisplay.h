#pragma once

#include "stdafx.h"
#include <vector>

class MenuDisplay
{
public:
	char* szMenuTitle;
	DWORD dwOptions;
	std::vector<char*> vOptions;

	MenuDisplay::MenuDisplay(char* title)
	{
		szMenuTitle = title;
		dwOptions = 0;
		vOptions = std::vector<char*>();
	}
	void MenuDisplay::AddOption(char* option)
	{
		dwOptions++;
		vOptions.push_back(option);
	}
	void MenuDisplay::Clear()
	{
		dwOptions = 0;
		vOptions.clear();
	}
	void MenuDisplay::Copy(MenuDisplay display)
	{
		szMenuTitle = display.szMenuTitle;
		dwOptions = display.dwOptions;
		vOptions = display.vOptions;
	}
	bool MenuDisplay::Equals(MenuDisplay display)
	{
		return (strcmp(szMenuTitle, display.szMenuTitle) == 0 && dwOptions == display.dwOptions && vOptions == display.vOptions);
	}
	void MenuDisplay::SetTitle(char* title)
	{
		strcpy(szMenuTitle, title);
	}
};