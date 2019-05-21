#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "include/json/jsoncpp.cc"

namespace ast
{
class Node
{
public:
    std::string type;
    int pos[4] = {0};
    std::vector<std::shared_ptr<Node>> children;
    Node() = default;
    Node(const std::string &type) : type(type){};
};
std::shared_ptr<ast::Node> imports(Json::Value &json);
Json::Value exports(std::shared_ptr<ast::Node> node);
} // namespace ast