#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

class CsvConnect {
public:
	CsvConnect() {}
	~CsvConnect() {}
public:
	static bool CsvRead(const std::string& filename, std::vector<std::vector<std::string>>& table, const char delimiter = ',');
	static bool CsvWrite(const std::string& filename, std::vector<std::vector<std::string>>& table, const char delimiter = ',');
};