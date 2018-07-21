#include "csvdata.h"


bool CsvConnect::CsvRead(const std::string & filename, std::vector<std::vector<std::string>>& table, const char delimiter)
{
	// ファイルを開く
	std::fstream filestream(filename);
	if (!filestream.is_open())
	{
		// ファイルが開けなかった場合は終了する
		return false;
	}

	// ファイルを読み込む
	while (!filestream.eof())
	{
		// １行読み込む
		std::string buffer;
		filestream >> buffer;

		// ファイルから読み込んだ１行の文字列を区切り文字で分けてリストに追加する
		std::vector<std::string> record;              // １行分の文字列のリスト
		std::istringstream streambuffer(buffer); // 文字列ストリーム
		std::string token;                       // １セル分の文字列
		while (getline(streambuffer, token, delimiter))
		{
			// １セル分の文字列をリストに追加する
			record.push_back(token);
		}

		// １行分の文字列を出力引数のリストに追加する
		table.push_back(record);
	}

	return true;
}

bool CsvConnect::CsvWrite(const std::string & filename, std::vector<std::vector<std::string>>& table, const char delimiter)
{
	return true;
}
