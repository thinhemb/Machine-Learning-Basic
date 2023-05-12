
/******************************************************************************** 
 Copyright (C) 2020, LE MANH THANG. All rights reserved.
 Module: SnpeRuntime.cpp
 Author: LE MANH THANG
 Created: 21/12/2020
 Description: 
********************************************************************************/
#ifndef LOADLABEL_H
#define LOADLABEL_H

#include <vector>
#include <stdio.h>
#include <regex>
#include <fstream>
#include <string>
#include <iostream>

static int loadLabel(std::string nameFile, std::vector<std::string>& labels)
{
    int countLine = 0;
	std::ifstream fileLabel(nameFile);
	if(!fileLabel)
    {
        printf("Reading file label error \n");
        return -1;
    }
    while(fileLabel.is_open())
    {
        std::string line;
        while (std::getline(fileLabel, line)) 
        {
            labels.push_back(line);
            countLine++;
        }
        fileLabel.close();
    }
    if( countLine < 1)
    {
        printf("There is no class in file label\n");
        return -1;
    }
	return 0;
}

std::vector<std::string> loadObjectNames(const std::string &filename) {
    std::ifstream file(filename);
    std::vector<std::string> fileLines;
    if (!file.is_open()) {
        return fileLines;
    }
    std::string line;
    while (getline(file, line)) {
        fileLines.push_back(line);
        //std::cout << line << "\n";
    }
    std::cout << "[INFO] Object names were loaded successfully! \n";
    return fileLines;
}

#endif