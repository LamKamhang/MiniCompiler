#include "global.h"
#include "../util/prettyPrint.h"
ir::Generator generator;
std::unordered_map<std::string, std::shared_ptr<ir::FunctionTy>> FunctionTable;
ast::Node *current_node;
void Warning(ast::Node *node, const std::string &info)
{
    ast::Node *_node = !node ? current_node : node;
    pretty::pretty_print("Warning", info, node->get_left(), node->get_right());
}
void Errors(ast::Node *node, const std::string &info) throw(const char *)
{
    ast::Node *_node = !node ? current_node : node;
    pretty::pretty_print("Errors", info, node->get_left(), node->get_right());
    throw "";
}