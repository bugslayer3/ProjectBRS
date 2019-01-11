#pragma once
#include <fstream>
#include <iostream>

bool fileExists(const std::string& filename);

// Functions
void invalid_input();
void write_record();
void read_record();
void search_record();
void edit_record();
void update_record();
void delete_record();