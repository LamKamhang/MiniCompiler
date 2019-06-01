#include "ast.h"
#include <memory>
ast::Node::Node(const std::string &type, const std::string &value, int x1, int y1, int x2, int y2)
    : type(type), value(value)
{
    pos[0] = x1;
    pos[1] = y1;
    pos[2] = x2;
    pos[3] = y2;
}
ast::Node::Node(const std::string &type, std::pair<int, int> left, std::pair<int, int> right)
    : type(type)
{
    pos[0] = left.first;
    pos[1] = left.second;
    pos[2] = right.first;
    pos[3] = right.second;
}
void ast::Node::set_left(const std::pair<int, int> &xy)
{
    pos[0] = xy.first;
    pos[1] = xy.second;
}
void ast::Node::set_right(const std::pair<int, int> &xy)
{
    pos[2] = xy.first;
    pos[3] = xy.second;
}
inline ast::Node *ast::Node::getNameChild(const std::string &name)
{
    if (this->type == name)
        return this;
    for (auto child : this->children)
    {
        auto res = child->getNameChild(name);
        if (res)
            return res;
    }
    return nullptr;
}
inline std::vector<ast::Node *> ast::Node::getNameChildren(const std::string &name)
{
    std::vector<ast::Node *> res;
    if (this->type == name)
        res.push_back(this);
    for (auto child : this->children)
    {
        auto childRes = child->getNameChildren(name);
        res.insert(res.end(), childRes.begin(), childRes.end());
    }
    return res;
}
std::shared_ptr<ast::Node> ast::imports(Json::Value &json)
{
    std::shared_ptr<ast::Node> res = std::make_shared<ast::Node>(json["type"].asString());
    for (auto i = 0; i < 4; ++i)
    {
        res->pos[i] = json["pos"].asInt();
    }
    Json::Value &children = json["children"];
    for (auto i = children.begin(); i != children.end(); ++i)
    {
        res->children.push_back(imports(*i));
    }
    // if a node with value
    if (children.size() == 0)
    {
        res->value = json["value"].asString();
    }
    return std::move(res);
}
Json::Value ast::exports(std::shared_ptr<ast::Node> node)
{
    Json::Value res;
    res["type"] = Json::Value(node->type);
    res["pos"].resize(0);
    for (auto i = 0; i < 4; ++i)
    {
        res["pos"].append(Json::Value(node->pos[i]));
    }
    if (node->children.size() == 0)
    {
        res["value"] = Json::Value(node->value);
    }
    else
    {
        res["children"].resize(0);
        for (auto i = node->children.begin(); i != node->children.end(); ++i)
        {
            res["children"].append(exports(*i));
        }
    }
    return std::move(res);
}