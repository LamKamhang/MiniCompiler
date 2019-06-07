#pragma once
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
    Node(const std::string &type, const std::string &value);
    Node(const std::string &type, const std::string &value, int x1, int y1, int x2, int y2);
    Node(const std::string &type, std::pair<int, int> left, std::pair<int, int> right);

    std::pair<int, int> get_left() const { return {pos[0], pos[1]}; }
    std::pair<int, int> get_right() const { return {pos[2], pos[3]}; }

    void set_left(const std::pair<int, int> &xy);
    void set_right(const std::pair<int, int> &xy);

public:
    std::string type;
    int pos[4] = {0};
    std::vector<std::shared_ptr<Node>> children;
    std::string value; // only used for a few non-terminals
    ast::Node *getNameChild(const std::string &name);
    std::vector<ast::Node *> getNameChildren(const std::string &name);
};

std::shared_ptr<ast::Node> imports(Json::Value &json);
Json::Value exports(std::shared_ptr<ast::Node> node);
} // namespace ast