#pragma once

#include "ll1_parser.hpp"
#include "../ast.hpp"

namespace LL1 {

// Nodos semánticos específicos para el parser LL(1)
struct ExprSemanticNode : SemanticNode {
    ExprPtr expr;
    ExprSemanticNode(ExprPtr e) : expr(std::move(e)) {}
};

struct StmtSemanticNode : SemanticNode {
    StmtPtr stmt;
    StmtSemanticNode(StmtPtr s) : stmt(std::move(s)) {}
};

struct ProgramSemanticNode : SemanticNode {
    std::unique_ptr<Program> program;
    ProgramSemanticNode(std::unique_ptr<Program> p) : program(std::move(p)) {}
};

} // namespace LL1
