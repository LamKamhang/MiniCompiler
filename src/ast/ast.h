#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "../lib/json/json.h"

namespace ast
{
class Node
{
public:
    Node() = default;
    Node(const std::string &type) : type(type){};
    Node(const std::string &type, const std::string &value, int x1, int y1, int x2, int y2)
        : type(type), value(value)
    {
        pos[0] = x1;
        pos[1] = y1;
        pos[2] = x2;
        pos[3] = y2;
    }
    Node(const std::string &type, std::pair<int, int> left, std::pair<int, int> right)
        : type(type)
    {
        pos[0] = left.first;
        pos[1] = left.second;
        pos[2] = right.first;
        pos[3] = right.second;
    }

    std::pair<int, int> get_left() const { return {pos[0], pos[1]}; }
    std::pair<int, int> get_right() const { return {pos[2], pos[3]}; }

    void set_left(const std::pair<int, int> &xy)     {pos[0] = xy.first; pos[1] = xy.second;}
    void set_right(const std::pair<int, int> &xy)    {pos[2] = xy.first; pos[3] = xy.second;}
public:
    std::string type;
    int pos[4] = {0};
    std::vector<std::shared_ptr<Node>> children;
    std::string value; // only used for a few non-terminals
};

std::shared_ptr<ast::Node> imports(Json::Value &json);
Json::Value exports(std::shared_ptr<ast::Node> node);
} // namespace ast