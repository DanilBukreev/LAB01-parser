#include <any>
#include <string>
#include <map>
#include <vector>
#include <regex>

class Json {

	enum json_type
	{
		typeJsonObject,
		typeJsonArray,
	};
	int type;

	std::map<std::string, std::any> obj;
	std::vector<std::any> arr;
	void parse_json(std::smatch const & m);
	void parse_array(std::smatch const & m);

public:
	// ����������� �� ������, ���������� Json-������.
	Json(const std::string& s);

	// ����� ���������� true, ���� ������ ��������� �������� � ���� JSON-������. ����� false.
	bool is_array() const;

	// ����� ���������� true, ���� ������ ��������� �������� � ���� JSON-������. ����� false.
	bool is_object() const ;

	// ����� ���������� true, ���� ������ ��������� �� �������� ������, �.�. null. ����� false.
	bool is_null() const;

	// ����� ���������� �������� �� ����� key, ���� ��������� �������� JSON-��������.
	// �������� ����� ����� ���� �� ��������� �����: Json, std::string, double, bool ��� ���� ������.
	// ���� ��������� �������� JSON-��������, ������������ ����������.
	std::any& operator[](const std::string& key);

	// ����� ���������� �������� �� ������� index, ���� ��������� �������� JSON-��������.
	// �������� ����� ����� ���� �� ��������� �����: Json, std::string, double, bool ��� ���� ������.
	// ���� ��������� �������� JSON-��������, ������������ ����������.
	std::any& operator[](int index);

	// ����� ���������� �������� �� ����� key, ���� ��������� �������� JSON-��������.
	// �������� ����� ����� ���� �� ��������� �����: Json, std::string, double, bool ��� ���� ������.
	// ���� ��������� �������� JSON-��������, ������������ ����������.
	const std::any& operator[](const std::string& key) const;


	// ����� ���������� �������� �� ������� index, ���� ��������� �������� JSON-��������.
	// �������� ����� ����� ���� �� ��������� �����: Json, std::string, double, bool ��� ���� ������.
	// ���� ��������� �������� JSON-��������, ������������ ����������.
	const std::any& operator[](int index) const;

	// ����� ���������� ������ ������ Json �� ������, ���������� Json-������.
	static Json parse(const std::string& s);

	// ����� ���������� ������� ������ Json �� �����, ����������� Json-������ � ��������� �������.
	static Json parseFile(const std::string& path_to_file);
};