#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "include/json/include/json.h"
#include "include/json/include/json-forwards.h"

namespace ast
{
class Node
{
public:
	Node() = default;
	Node(const std::string &type) : type(type){};
	Node(const std::string &type, const std::string &value, int x1, int y1, int x2, int y2)
		: type(type)
		//, pos({x1, y1, x2, y2})
		, value(value)
	{pos[0] = x1; pos[1] = y1; pos[2] = x2; pos[3] = y2;}

public:
	std::string type;
	int pos[4] = {0};
	std::vector<std::shared_ptr<Node>> children;
	std::string value; // only used for a few non-terminals
};

std::shared_ptr<ast::Node> imports(Json::Value &json);
Json::Value exports(std::shared_ptr<ast::Node> node);
} // namespace ast