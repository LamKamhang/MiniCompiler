#include <ast.h>
#include <memory>

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
    res["children"].resize(0);
    for (auto i = node->children.begin(); i != node->children.end(); ++i)
    {
        res["children"].append(exports(*i));
    }
    return std::move(res);
}