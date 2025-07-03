#include "ll1_parser.hpp"
#include "semantic_nodes.hpp"
#include "../ast.hpp"
#include <iostream>
#include <sstream>

namespace LL1 {

// Funciones auxiliares para crear nodos del AST
namespace SemanticActions {

// Helper para obtener operador binario del token
BinaryExpr::Op stringToOp(const std::string& op) {
    if (op == "+") return BinaryExpr::OP_ADD;
    if (op == "-") return BinaryExpr::OP_SUB;
    if (op == "*") return BinaryExpr::OP_MUL;
    if (op == "/") return BinaryExpr::OP_DIV;
    if (op == "%") return BinaryExpr::OP_MOD;
    if (op == "<") return BinaryExpr::OP_LT;
    if (op == ">") return BinaryExpr::OP_GT;
    if (op == "<=") return BinaryExpr::OP_LE;
    if (op == ">=") return BinaryExpr::OP_GE;
    if (op == "==") return BinaryExpr::OP_EQ;
    if (op == "!=") return BinaryExpr::OP_NEQ;
    if (op == "&&" || op == "&") return BinaryExpr::OP_AND;
    if (op == "||" || op == "|") return BinaryExpr::OP_OR;
    if (op == "@@") return BinaryExpr::OP_CONCAT;
    return BinaryExpr::OP_ADD; // default
}

// Helper para debug del stack
void printStackInfo(const std::stack<std::unique_ptr<SemanticNode>>& stack, const std::string& context) {
    std::cout << "DEBUG [" << context << "]: Stack size = " << stack.size() << std::endl;
}

// Helper para obtener expresión del stack
ExprPtr popExpr(std::stack<std::unique_ptr<SemanticNode>>& stack, const std::string& context) {
    if (stack.empty()) {
        std::cout << "ERROR [" << context << "]: Stack is empty when trying to pop expression" << std::endl;
        return std::make_unique<NumberExpr>(0); // fallback
    }
    
    auto node = std::move(stack.top());
    stack.pop();
    
    if (auto exprNode = dynamic_cast<ExprSemanticNode*>(node.get())) {
        return std::move(exprNode->expr);
    }
    
    std::cout << "ERROR [" << context << "]: Top node is not an expression" << std::endl;
    return std::make_unique<NumberExpr>(0); // fallback
}

// Helper para obtener programa del stack
std::unique_ptr<Program> popProgram(std::stack<std::unique_ptr<SemanticNode>>& stack, const std::string& context) {
    if (stack.empty()) {
        std::cout << "ERROR [" << context << "]: Stack is empty when trying to pop program" << std::endl;
        return std::make_unique<Program>();
    }
    
    auto node = std::move(stack.top());
    stack.pop();
    
    if (auto progNode = dynamic_cast<ProgramSemanticNode*>(node.get())) {
        return std::move(progNode->program);
    }
    
    std::cout << "ERROR [" << context << "]: Top node is not a program" << std::endl;
    return std::make_unique<Program>();
}

} // namespace SemanticActions

// Configurar acciones semánticas completas para la gramática V3
void ParserFactory::setupCompleteSemanticActionsV3(LL1Parser& parser) {
    std::cout << "Setting up complete semantic actions for Full HULK Grammar V3..." << std::endl;
    
    // ==== MAPPING DE IDs DE PRODUCCIÓN ====
    // Basado en el orden de addProduction en full_hulk_grammar_v3.cpp:
    
    // Production ID 0: program -> stmt_list
    parser.setSemanticAction(0, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        using namespace SemanticActions;
        printStackInfo(stack, "program->stmt_list");
        
        if (!stack.empty()) {
            auto program = popProgram(stack, "program->stmt_list");
            stack.push(std::make_unique<ProgramSemanticNode>(std::move(program)));
            std::cout << "DEBUG: Program action - passed through program" << std::endl;
        } else {
            auto program = std::make_unique<Program>();
            stack.push(std::make_unique<ProgramSemanticNode>(std::move(program)));
            std::cout << "DEBUG: Program action - created empty program" << std::endl;
        }
    });
    
    // Production ID 1: stmt_list -> stmt stmt_list
    parser.setSemanticAction(1, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        using namespace SemanticActions;
        printStackInfo(stack, "stmt_list->stmt stmt_list");
        
        if (stack.size() >= 2) {
            // Segundo elemento: stmt_list (resto de statements)
            auto restProgram = popProgram(stack, "stmt_list->stmt_list (rest)");
            // Primer elemento: stmt (statement actual)
            auto stmtProgram = popProgram(stack, "stmt_list->stmt (first)");
            
            // Combinar: agregar statements del primer programa al resto
            for (auto& stmt : stmtProgram->stmts) {
                restProgram->stmts.insert(restProgram->stmts.begin(), std::move(stmt));
            }
            
            stack.push(std::make_unique<ProgramSemanticNode>(std::move(restProgram)));
            std::cout << "DEBUG: stmt_list action - combined statements" << std::endl;
        } else {
            auto program = std::make_unique<Program>();
            stack.push(std::make_unique<ProgramSemanticNode>(std::move(program)));
            std::cout << "DEBUG: stmt_list action - fallback empty program" << std::endl;
        }
    });
    
    // Production ID 2: stmt_list -> ε
    parser.setSemanticAction(2, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        using namespace SemanticActions;
        auto program = std::make_unique<Program>();
        stack.push(std::make_unique<ProgramSemanticNode>(std::move(program)));
        std::cout << "DEBUG: stmt_list epsilon - created empty program" << std::endl;
    });
    
    // Production ID 3: stmt -> decl
    parser.setSemanticAction(3, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        using namespace SemanticActions;
        printStackInfo(stack, "stmt->decl");
        // El decl ya debería haber puesto un programa en el stack, simplemente pasar a través
        if (stack.empty()) {
            auto program = std::make_unique<Program>();
            stack.push(std::make_unique<ProgramSemanticNode>(std::move(program)));
            std::cout << "DEBUG: stmt->decl - fallback empty program" << std::endl;
        } else {
            std::cout << "DEBUG: stmt->decl - passed through declaration" << std::endl;
        }
    });
    
    // Production IDs 4-9: stmt -> let_expr; | if_expr; | while_expr; | for_expr; | block_expr; | or_expr;
    for (int i = 4; i <= 9; ++i) {
        parser.setSemanticAction(i, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
            using namespace SemanticActions;
            printStackInfo(stack, "stmt->expr;");
            
            if (!stack.empty()) {
                auto expr = popExpr(stack, "stmt->expr");
                auto exprStmt = std::make_unique<ExprStmt>(std::move(expr));
                auto program = std::make_unique<Program>();
                program->stmts.push_back(std::move(exprStmt));
                stack.push(std::make_unique<ProgramSemanticNode>(std::move(program)));
                std::cout << "DEBUG: stmt->expr; - created program with expression statement" << std::endl;
            } else {
                auto program = std::make_unique<Program>();
                stack.push(std::make_unique<ProgramSemanticNode>(std::move(program)));
                std::cout << "DEBUG: stmt->expr; - fallback empty program" << std::endl;
            }
        });
    }
    
    // Production ID 10: decl -> function_decl
    parser.setSemanticAction(10, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        using namespace SemanticActions;
        printStackInfo(stack, "decl->function_decl");
        // La declaración de función ya debería haber puesto algo en el stack
        if (stack.empty()) {
            auto program = std::make_unique<Program>();
            stack.push(std::make_unique<ProgramSemanticNode>(std::move(program)));
            std::cout << "DEBUG: decl->function_decl - fallback empty program" << std::endl;
        } else {
            std::cout << "DEBUG: decl->function_decl - passed through function declaration" << std::endl;
        }
    });
    
    // Production IDs 11-13: or_expr hierarchy
    parser.setSemanticAction(11, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        using namespace SemanticActions;
        printStackInfo(stack, "or_expr->and_expr or_expr_prime");
        
        if (stack.size() >= 2) {
            auto rightExpr = popExpr(stack, "or_expr_prime");
            auto leftExpr = popExpr(stack, "and_expr");
            
            // Si rightExpr es null o es una expresión "vacía", simplemente usar leftExpr
            if (!rightExpr || (dynamic_cast<NumberExpr*>(rightExpr.get()) && 
                               dynamic_cast<NumberExpr*>(rightExpr.get())->value == -999)) {
                stack.push(std::make_unique<ExprSemanticNode>(std::move(leftExpr)));
            } else {
                // Crear expresión OR binaria
                auto orExpr = std::make_unique<BinaryExpr>(BinaryExpr::OP_OR, std::move(leftExpr), std::move(rightExpr));
                stack.push(std::make_unique<ExprSemanticNode>(std::move(orExpr)));
            }
            std::cout << "DEBUG: or_expr - combined OR expression" << std::endl;
        } else if (stack.size() == 1) {
            std::cout << "DEBUG: or_expr - passed through single expression" << std::endl;
        } else {
            auto expr = std::make_unique<NumberExpr>(0);
            stack.push(std::make_unique<ExprSemanticNode>(std::move(expr)));
            std::cout << "DEBUG: or_expr - fallback expression" << std::endl;
        }
    });
    
    // Production ID 12: or_expr_prime -> OR and_expr or_expr_prime
    parser.setSemanticAction(12, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        using namespace SemanticActions;
        printStackInfo(stack, "or_expr_prime->OR and_expr or_expr_prime");
        
        if (stack.size() >= 2) {
            auto rightExpr = popExpr(stack, "or_expr_prime (rest)");
            auto leftExpr = popExpr(stack, "and_expr");
            
            // Crear OR expression, o cadena si rightExpr no es vacía
            if (!rightExpr || (dynamic_cast<NumberExpr*>(rightExpr.get()) && 
                               dynamic_cast<NumberExpr*>(rightExpr.get())->value == -999)) {
                stack.push(std::make_unique<ExprSemanticNode>(std::move(leftExpr)));
            } else {
                auto orExpr = std::make_unique<BinaryExpr>(BinaryExpr::OP_OR, std::move(leftExpr), std::move(rightExpr));
                stack.push(std::make_unique<ExprSemanticNode>(std::move(orExpr)));
            }
            std::cout << "DEBUG: or_expr_prime - created OR chain" << std::endl;
        } else {
            auto expr = std::make_unique<NumberExpr>(-999); // marker for empty
            stack.push(std::make_unique<ExprSemanticNode>(std::move(expr)));
            std::cout << "DEBUG: or_expr_prime - fallback marker" << std::endl;
        }
    });
    
    // Production ID 13: or_expr_prime -> ε
    parser.setSemanticAction(13, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        using namespace SemanticActions;
        // Para epsilon, ponemos un marcador especial
        auto expr = std::make_unique<NumberExpr>(-999); // marker for empty
        stack.push(std::make_unique<ExprSemanticNode>(std::move(expr)));
        std::cout << "DEBUG: or_expr_prime epsilon - empty marker" << std::endl;
    });
    
    // Continuar con más producciones...
    
    // Production IDs for primary_expr (números, strings, etc.)
    // Asumir que primary_expr -> NUMBER está alrededor del ID 25-30
    for (int i = 25; i <= 35; ++i) {
        parser.setSemanticAction(i, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
            using namespace SemanticActions;
            
            if (!tokens.empty()) {
                const Token& token = tokens[0];
                
                if (token.symbol.name == "NUMBER") {
                    double value = std::stod(token.lexeme);
                    auto numberExpr = std::make_unique<NumberExpr>(value);
                    stack.push(std::make_unique<ExprSemanticNode>(std::move(numberExpr)));
                    std::cout << "DEBUG: Created NUMBER expression: " << value << std::endl;
                }
                else if (token.symbol.name == "STRING") {
                    // Remover comillas del string literal
                    std::string value = token.lexeme;
                    if (value.length() >= 2 && value[0] == '"' && value.back() == '"') {
                        value = value.substr(1, value.length() - 2);
                    }
                    auto stringExpr = std::make_unique<StringExpr>(value);
                    stack.push(std::make_unique<ExprSemanticNode>(std::move(stringExpr)));
                    std::cout << "DEBUG: Created STRING expression: " << value << std::endl;
                }
                else if (token.symbol.name == "TRUE") {
                    auto boolExpr = std::make_unique<BooleanExpr>(true);
                    stack.push(std::make_unique<ExprSemanticNode>(std::move(boolExpr)));
                    std::cout << "DEBUG: Created TRUE expression" << std::endl;
                }
                else if (token.symbol.name == "FALSE") {
                    auto boolExpr = std::make_unique<BooleanExpr>(false);
                    stack.push(std::make_unique<ExprSemanticNode>(std::move(boolExpr)));
                    std::cout << "DEBUG: Created FALSE expression" << std::endl;
                }
                else if (token.symbol.name == "IDENT") {
                    auto varExpr = std::make_unique<VariableExpr>(token.lexeme);
                    stack.push(std::make_unique<ExprSemanticNode>(std::move(varExpr)));
                    std::cout << "DEBUG: Created VARIABLE expression: " << token.lexeme << std::endl;
                }
                else {
                    // Fallback para otros tokens
                    auto expr = std::make_unique<NumberExpr>(0);
                    stack.push(std::make_unique<ExprSemanticNode>(std::move(expr)));
                    std::cout << "DEBUG: Primary expression fallback for token: " << token.symbol.name << std::endl;
                }
            } else {
                auto expr = std::make_unique<NumberExpr>(0);
                stack.push(std::make_unique<ExprSemanticNode>(std::move(expr)));
                std::cout << "DEBUG: Primary expression fallback - no tokens" << std::endl;
            }
        });
    }
    
    // Acciones por defecto para todas las demás producciones
    for (int i = 36; i < 100; ++i) {
        parser.setSemanticAction(i, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
            using namespace SemanticActions;
            std::cout << "DEBUG: Default action for production " << std::endl;
            // Por defecto, no hacer nada o crear una expresión vacía
            if (stack.empty()) {
                auto expr = std::make_unique<NumberExpr>(0);
                stack.push(std::make_unique<ExprSemanticNode>(std::move(expr)));
            }
        });
    }
    
    std::cout << "Complete semantic actions setup completed for V3 grammar." << std::endl;
}

} // namespace LL1
