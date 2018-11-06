
#include "json.hpp"
#include <sstream>
#include <fstream>
std::string str_regex_key=R"(^\s*\"([[:alpha:]]+)\"\s*)";
std::string str_regex_string=R"(\s*\"(.*)\"\s*)";
std::string str_regex_int=R"(\s*(\d+)\s*)";
std::string str_regex_bool=R"(\s*(false|true)\s*)";
std::string str_regex_json_or_array= R"(\s*((?:\{|\[)(?:.|\s)*(?:\}|\]))\s*)";

std::string str_regex_json= R"(\s*\{((?:.|\s)*)\}\s*)";
std::string str_regex_array= R"(\s*\[((?:.|\s)*)\]\s*)";

std::regex regex_string{ str_regex_string };
std::regex regex_int{ str_regex_int };
std::regex regex_bool{ str_regex_bool };

std::regex regex_json{ str_regex_json };
std::regex regex_array{ str_regex_array };
std::regex regex_json_or_array{ str_regex_json_or_array };

std::regex regex_string_key_value{str_regex_key+":"+ str_regex_string };
std::regex    regex_int_key_value{str_regex_key+":"+ str_regex_int };
std::regex   regex_bool_key_value{str_regex_key+":"+ str_regex_bool };
std::regex   regex_json_or_array_key_value{str_regex_key+":"+ str_regex_json_or_array };

//class count_brackets
//{
//	int& cnt_of_brackets;
//	count_brackets(int& cnt_of_brackets_):cnt_of_brackets(cnt_of_brackets_){}
//
//	void operator()(const char& c)
//	{
//		static const std::string brackets_open("([{");
//		static const std::string brackets_close(")]}");
//		if (brackets_open.find(c, 0) != std::string::npos)++cnt_of_brackets;
//		if (brackets_close.find(c, 0) != std::string::npos)--cnt_of_brackets;
//	}
//};

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
	int cnt_of_brackets = 0;
	std::vector<std::string> tokens;
	size_t prev = 0,pos=0;
	do
	{
		size_t prev_tmp = prev;
		do {
			pos = str.find(delim, prev_tmp);
			if (pos == std::string::npos) pos = str.length();
			auto it= str.begin();
			
			std::for_each(str.begin() + prev_tmp, str.begin() + pos, [&cnt_of_brackets](const char& c)
			{
				static const std::string brackets_open("([{");
				static const std::string brackets_close(")]}");
				if (brackets_open.find(c, 0) != std::string::npos)++cnt_of_brackets;
				if (brackets_close.find(c, 0) != std::string::npos)--cnt_of_brackets;
			});
			prev_tmp = pos + delim.length();
		}
		while (cnt_of_brackets);

		std::string token = str.substr(prev,pos-prev);
		if (!token.empty()) tokens.push_back(token);
		prev = prev_tmp;
	} while (pos < str.length() && prev < str.length());
	return tokens;
}
bool to_bool(std::string const& str)
{
	std::istringstream is(str);
	bool res;
	is >> std::boolalpha >> res;
	return res;
}

void Json::parse_array(std::smatch const & m)
{
	auto key_values = split(m[1].str(), ",");
	std::smatch match;
	for (auto& key_value : key_values)
	{
		if (std::regex_match(key_value, match, regex_string))
		{
			arr.emplace_back( std::make_any<std::string>(match[1].str()));
		}
		else if (std::regex_match(key_value, match, regex_int))
		{
			arr.emplace_back(std::make_any<int>(std::stoi(match[1].str())));
		}
		else if (std::regex_match(key_value, match, regex_bool))
		{
			arr.emplace_back(std::make_any<bool>(to_bool(match[1].str())));
		}
		else if (std::regex_match(key_value, match, regex_json_or_array))
		{
			arr.emplace_back(std::make_any<Json>(match[1].str()));
		}
	}
}

void Json::parse_json(std::smatch const & m)
{
	auto key_values = split(m[1].str(), ",");
	std::smatch match;
	for (auto& key_value : key_values)
	{
		if (std::regex_match(key_value, match, regex_string_key_value))
		{
			obj.emplace(match[1].str(), std::make_any<std::string>(match[2].str()));
		} else if (std::regex_match(key_value, match, regex_int_key_value))
		{
			obj.emplace(match[1].str(), std::make_any<int>(std::stoi(match[2].str())));
		} else if (std::regex_match(key_value, match, regex_bool_key_value))
		{
			obj.emplace(match[1].str(), std::make_any<bool>(to_bool(match[2].str())));
		} else if(std::regex_match(key_value, match, regex_json_or_array_key_value))
		{
			obj.emplace(match[1].str(), std::make_any<Json>(match[2].str()));
		}
	}
}


std::any& Json::operator[](const std::string& key)
{
	if (is_array())
		throw;
	return obj[key];
}

std::any& Json::operator[](int index)
{
	if (is_object())
		throw;
	return arr[index];

}
const std::any& Json::operator[](int index) const
{
	if (is_object())
		throw;
	return arr[index];
}
const std::any& Json::operator[](const std::string& key) const
{
	if (is_array())
		throw;
	return obj.at(key);
}
bool Json::is_array() const {
	return type == typeJsonArray;
}

// Метод возвращает true, если данный экземпляр содержит в себе JSON-объект. Иначе false.
bool Json::is_object() const {
	return type == typeJsonObject;
}

// Метод возвращает true, если данный экземпляр не содержит ничего, т.е. null. Иначе false.
bool Json::is_null() const
{
	return obj.empty() && arr.empty();
}

Json Json::parse(const std::string & s)
{
	return Json(s);
}

Json Json::parseFile(const std::string & path_to_file)
{
	std::ifstream iFile(path_to_file);

	return Json("");
}

Json::Json(const std::string& s)
{
	std::smatch matched;
	if (std::regex_match(s, matched, regex_json))
	{
		type = typeJsonObject;
		parse_json(matched);
	} else if(std::regex_match(s, matched, regex_array))
	{
		type = typeJsonArray;
		parse_array(matched);
	}

}