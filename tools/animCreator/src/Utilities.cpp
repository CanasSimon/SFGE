/*
MIT License

Copyright (c) 2018 SAE Institute Switzerland AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

----------------------------
Author : Duncan Bourquard (bourquardduncan@gmail.com)
Date : November 2018
Project : AnimationTool for SFGE
*/

#include <json.hpp>

#include <iostream>
#include <fstream>
#include <windows.h>

#include <AnimationManager.h>
#include <TextureManager.h>
#include <Utilities.h>

using json = nlohmann::json;

char* Utilities::ConvertStringToArrayChar(std::string string, size_t size)
{
	char * result = new char[size];
	strcpy(result, string.c_str());
	return result;
}

LogSaveError Utilities::ExportToJson(AnimationManager* anim, std::vector<TextureInfos*>* textures, bool confirmedReplacement)
{
	json value;
	std::string animName = anim->GetName();

	//Creating base folder
	if(!CreateDirectory(DATA_FOLDER.c_str(), NULL) && !CreateDirectory(SAVE_FOLDER.c_str(), NULL))
	{
		if (ERROR_ALREADY_EXISTS != GetLastError())
		{
			//std::cout << "COULDN'T CREATE BASE FOLDER : error(" << GetLastError() << ")\n";
			return FAILURE;
		}
	}

	if (!CreateDirectory((SAVE_FOLDER + animName + "/").c_str(), NULL))
	{
		if(ERROR_ALREADY_EXISTS == GetLastError())
		{
			//std::cout << "FOLDER '"+ SAVE_FOLDER + animName +"/' ALREADY EXISTS. \n";
		}
		else
		{
			//std::cout << "COULDN'T CREATE FOLDER '" + SAVE_FOLDER + animName + "/' : error(" << GetLastError()  << ")\n";
			return FAILURE;
		}
	}

	//Check if file already exists. If so, we ask if the user wants to replace it
	if (confirmedReplacement)
		RemoveDirectory((SAVE_FOLDER + animName + "/").c_str());
	
	std::ifstream doAnimExists(SAVE_FOLDER + animName + ".json");
	confirmedReplacement = !confirmedReplacement && doAnimExists;
	doAnimExists.close();

	if(confirmedReplacement)
		return DO_REPLACE;

	// Json construction
	value["frameFps"] = anim->GetSpeed();
	auto frames = anim->GetAnim();
	short index = 0;

	for (auto frame : frames)
	{
		TextureInfos* textToApply = nullptr;
		for(auto textu : *textures)
		{
			if (textu->id == frame.second)
			{
				textToApply = textu;
				break;
			}
		}

		if (textToApply == nullptr)
			continue;

		//Get the file name from the path
		std::string fileName = textToApply->path;
		std::size_t found = fileName.find("/");
		while (found < fileName.length())
		{
			fileName = fileName.substr(found + 1);
			found = fileName.find("/");
		}

		//Copy file into save folder
		std::ifstream myImage(SAVE_FOLDER + animName + "/" + fileName);
		if (!myImage.good())
		{
			//std::cout << "saving image \n";
			std::ifstream  src(textToApply->path, std::ios::binary);
			std::ofstream  dst(SAVE_FOLDER + animName + "/" + fileName, std::ios::binary);

			dst << src.rdbuf();
			dst.close();
		}
		else
		{
			//std::cout << "image already copied\n";
		}
		myImage.close();

		//Registering information of the frame
		value["frames"][index]["key"] = frame.first;		
		value["frames"][index]["path"] = fileName;
		value["frames"][index]["position"]["x"] = textToApply->position.x;
		value["frames"][index]["position"]["y"] = textToApply->position.y;
		value["frames"][index]["size"]["x"] = textToApply->size.x;
		value["frames"][index]["size"]["y"] = textToApply->size.y;
		
		index++;
	}

	// File write
	std::ofstream myfile;
	myfile.open(SAVE_FOLDER + animName + ".json");
	myfile << std::setw(4) << value << std::endl;
	myfile.close();

	std::cout << "Animation saved in "<< SAVE_FOLDER << animName << "/ \n";

	return SUCCESS;
}

LogSaveError Utilities::ExportToGif(AnimationManager* anim, std::vector<TextureInfos*>* textures, bool confirmedReplacement)
{
	return SUCCESS;
}


std::string Utilities::GetClipboardText()
{
	// Try opening the clipboard
	if (!OpenClipboard(nullptr))
		return "";

	// Get handle of clipboard object for ANSI text
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr)
		return "";

	// Lock the handle to get the actual text pointer
	char * pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == nullptr)
		return "";

	// Save text in a string class instance
	std::string text(pszText);

	// Release the lock
	GlobalUnlock(hData);

	// Release the clipboard
	CloseClipboard();

	return text;
}