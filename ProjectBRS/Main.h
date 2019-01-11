#pragma once
#include <fstream>
#include <iostream>
#include <string>

bool fileExists(const std::string& filename);

// Functions
void read_data(std::ofstream record);