#include "ll1_parser.hpp"
#include "semantic_nodes.hpp"
#include "../ast.hpp"
#include <iostream>
#include <stack>

namespace LL1 {

// Funciones auxiliares para crear nodos del AST
namespace SemanticActionsV4 {

// Helper para obtener operador binario del token
BinaryExpr::Op stringToOp(const std::string& op) {
    if (op == "+" || op == "PLUS") return BinaryExpr::OP_ADD;
    if (op == "-" || op == "MINUS") return BinaryExpr::OP_SUB;
    if (op == "*" || op == "MULT") return BinaryExpr::OP_MUL;
    if (op == "/" || op == "DIV") return BinaryExpr::OP_DIV;
    if (op == "%" || op == "MOD") return BinaryExpr::OP_MOD;
    if (op == "<" || op == "LESS_THAN") return BinaryExpr::OP_LT;
    if (op == ">" || op == "GREATER_THAN") return BinaryExpr::OP_GT;
    if (op == "<=" || op == "LE") return BinaryExpr::OP_LE;
    if (op == ">=" || op == "GE") return BinaryExpr::OP_GE;
    if (op == "==" || op == "EQ") return BinaryExpr::OP_EQ;
    if (op == "!=" || op == "NEQ") return BinaryExpr::OP_NEQ;
    if (op == "&&" || op == "&" || op == "AND") return BinaryExpr::OP_AND;
    if (op == "||" || op == "|" || op == "OR") return BinaryExpr::OP_OR;
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

// Helper para crear marcador de epsilon
void pushEpsilonMarker(std::stack<std::unique_ptr<SemanticNode>>& stack) {
    auto marker = std::make_unique<NumberExpr>(-999); // marcador especial
    stack.push(std::make_unique<ExprSemanticNode>(std::move(marker)));
}

// Helper para verificar si es marcador de epsilon
bool isEpsilonMarker(const std::unique_ptr<SemanticNode>& node) {
    if (auto exprNode = dynamic_cast<ExprSemanticNode*>(node.get())) {
        if (auto numberExpr = dynamic_cast<NumberExpr*>(exprNode->expr.get())) {
            return numberExpr->value == -999;
        }
    }
    return false;
}

} // namespace SemanticActionsV4

// Configurar acciones semánticas completas para la gramática V3
void ParserFactory::setupCompleteSemanticActionsV4(LL1Parser& parser) {
    std::cout << "Setting up complete semantic actions V4 for Full HULK Grammar V3..." << std::endl;
    
    using namespace SemanticActionsV4;
    
    // ==== MAPPING PRECISO DE IDs DE PRODUCCIÓN ====
    
    // ID 0: program -> stmt_list
    parser.setSemanticAction(0, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
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
    
    // ID 1: stmt_list -> stmt stmt_list
    parser.setSemanticAction(1, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        printStackInfo(stack, "stmt_list->stmt stmt_list");
        
        if (stack.size() >= 2) {
            auto restProgram = popProgram(stack, "stmt_list (rest)");
            auto stmtProgram = popProgram(stack, "stmt (first)");
            
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
    
    // ID 2: stmt_list -> ε
    parser.setSemanticAction(2, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        auto program = std::make_unique<Program>();
        stack.push(std::make_unique<ProgramSemanticNode>(std::move(program)));
        std::cout << "DEBUG: stmt_list epsilon - created empty program" << std::endl;
    });
    
    // ID 3: stmt -> decl
    parser.setSemanticAction(3, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        printStackInfo(stack, "stmt->decl");
        // Pass through - el decl ya debería haber puesto algo útil en el stack
        if (stack.empty()) {
            auto program = std::make_unique<Program>();
            stack.push(std::make_unique<ProgramSemanticNode>(std::move(program)));
            std::cout << "DEBUG: stmt->decl - fallback empty program" << std::endl;
        } else {
            std::cout << "DEBUG: stmt->decl - passed through declaration" << std::endl;
        }
    });
    
    // IDs 4-9: stmt -> expr_type SEMICOLON
    for (int i = 4; i <= 9; ++i) {
        parser.setSemanticAction(i, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
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
    
    // ID 10: decl -> function_decl
    parser.setSemanticAction(10, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        printStackInfo(stack, "decl->function_decl");
        // Pass through - function_decl ya debería haber puesto algo útil en el stack
        if (stack.empty()) {
            auto program = std::make_unique<Program>();
            stack.push(std::make_unique<ProgramSemanticNode>(std::move(program)));
            std::cout << "DEBUG: decl->function_decl - fallback empty program" << std::endl;
        } else {
            std::cout << "DEBUG: decl->function_decl - passed through function declaration" << std::endl;
        }
    });
    
    // ID 11: or_expr -> and_expr or_expr_prime
    parser.setSemanticAction(11, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        printStackInfo(stack, "or_expr->and_expr or_expr_prime");
        
        if (stack.size() >= 2) {
            auto rightExpr = popExpr(stack, "or_expr_prime");
            auto leftExpr = popExpr(stack, "and_expr");
            
            // Si rightExpr es un marcador de epsilon, solo usar leftExpr
            if (auto numberExpr = dynamic_cast<NumberExpr*>(rightExpr.get())) {
                if (numberExpr->value == -999) {
                    stack.push(std::make_unique<ExprSemanticNode>(std::move(leftExpr)));
                    std::cout << "DEBUG: or_expr - used only left expression" << std::endl;
                    return;
                }
            }
            
            // Crear expresión OR binaria
            auto orExpr = std::make_unique<BinaryExpr>(BinaryExpr::OP_OR, std::move(leftExpr), std::move(rightExpr));
            stack.push(std::make_unique<ExprSemanticNode>(std::move(orExpr)));
            std::cout << "DEBUG: or_expr - created OR expression" << std::endl;
        } else if (stack.size() == 1) {
            std::cout << "DEBUG: or_expr - passed through single expression" << std::endl;
        } else {
            auto expr = std::make_unique<NumberExpr>(0);
            stack.push(std::make_unique<ExprSemanticNode>(std::move(expr)));
            std::cout << "DEBUG: or_expr - fallback expression" << std::endl;
        }
    });
    
    // ID 12: or_expr_prime -> OR and_expr or_expr_prime
    parser.setSemanticAction(12, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        printStackInfo(stack, "or_expr_prime->OR and_expr or_expr_prime");
        
        if (stack.size() >= 2) {
            auto rightExpr = popExpr(stack, "or_expr_prime (rest)");
            auto leftExpr = popExpr(stack, "and_expr");
            
            // Si rightExpr es epsilon, solo usar leftExpr
            if (auto numberExpr = dynamic_cast<NumberExpr*>(rightExpr.get())) {
                if (numberExpr->value == -999) {
                    stack.push(std::make_unique<ExprSemanticNode>(std::move(leftExpr)));
                    std::cout << "DEBUG: or_expr_prime - used only left expression" << std::endl;
                    return;
                }
            }
            
            auto orExpr = std::make_unique<BinaryExpr>(BinaryExpr::OP_OR, std::move(leftExpr), std::move(rightExpr));
            stack.push(std::make_unique<ExprSemanticNode>(std::move(orExpr)));
            std::cout << "DEBUG: or_expr_prime - created OR chain" << std::endl;
        } else {
            pushEpsilonMarker(stack);
            std::cout << "DEBUG: or_expr_prime - fallback epsilon marker" << std::endl;
        }
    });
    
    // ID 13: or_expr_prime -> ε
    parser.setSemanticAction(13, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        pushEpsilonMarker(stack);
        std::cout << "DEBUG: or_expr_prime epsilon - empty marker" << std::endl;
    });
    
    // Similar para and_expr - ID 14: and_expr -> eq_expr and_expr_prime
    parser.setSemanticAction(14, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        printStackInfo(stack, "and_expr->eq_expr and_expr_prime");
        
        if (stack.size() >= 2) {
            auto rightExpr = popExpr(stack, "and_expr_prime");
            auto leftExpr = popExpr(stack, "eq_expr");
            
            // Si rightExpr es epsilon, solo usar leftExpr
            if (auto numberExpr = dynamic_cast<NumberExpr*>(rightExpr.get())) {
                if (numberExpr->value == -999) {
                    stack.push(std::make_unique<ExprSemanticNode>(std::move(leftExpr)));
                    std::cout << "DEBUG: and_expr - used only left expression" << std::endl;
                    return;
                }
            }
            
            auto andExpr = std::make_unique<BinaryExpr>(BinaryExpr::OP_AND, std::move(leftExpr), std::move(rightExpr));
            stack.push(std::make_unique<ExprSemanticNode>(std::move(andExpr)));
            std::cout << "DEBUG: and_expr - created AND expression" << std::endl;
        } else {
            pushEpsilonMarker(stack);
            std::cout << "DEBUG: and_expr - fallback epsilon marker" << std::endl;
        }
    });
    
    // ID 15: and_expr_prime -> AND eq_expr and_expr_prime
    parser.setSemanticAction(15, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        printStackInfo(stack, "and_expr_prime->AND eq_expr and_expr_prime");
        
        if (stack.size() >= 2) {
            auto rightExpr = popExpr(stack, "and_expr_prime (rest)");
            auto leftExpr = popExpr(stack, "eq_expr");
            
            // Si rightExpr es epsilon, solo usar leftExpr
            if (auto numberExpr = dynamic_cast<NumberExpr*>(rightExpr.get())) {
                if (numberExpr->value == -999) {
                    stack.push(std::make_unique<ExprSemanticNode>(std::move(leftExpr)));
                    std::cout << "DEBUG: and_expr_prime - used only left expression" << std::endl;
                    return;
                }
            }
            
            auto andExpr = std::make_unique<BinaryExpr>(BinaryExpr::OP_AND, std::move(leftExpr), std::move(rightExpr));
            stack.push(std::make_unique<ExprSemanticNode>(std::move(andExpr)));
            std::cout << "DEBUG: and_expr_prime - created AND chain" << std::endl;
        } else {
            pushEpsilonMarker(stack);
            std::cout << "DEBUG: and_expr_prime - fallback epsilon marker" << std::endl;
        }
    });
    
    // ID 16: and_expr_prime -> ε
    parser.setSemanticAction(16, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        pushEpsilonMarker(stack);
        std::cout << "DEBUG: and_expr_prime epsilon - empty marker" << std::endl;
    });
    
    // Continuar con más producciones...
    
    // ID 27: arith_expr -> add_expr (pass through)
    parser.setSemanticAction(27, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        printStackInfo(stack, "arith_expr->add_expr");
        // Pass through - solo pasar la expresión
        if (stack.empty()) {
            auto expr = std::make_unique<NumberExpr>(0);
            stack.push(std::make_unique<ExprSemanticNode>(std::move(expr)));
            std::cout << "DEBUG: arith_expr - fallback" << std::endl;
        } else {
            std::cout << "DEBUG: arith_expr - passed through add_expr" << std::endl;
        }
    });
    
    // ID 28: add_expr -> mult_expr add_expr_prime
    parser.setSemanticAction(28, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        printStackInfo(stack, "add_expr->mult_expr add_expr_prime");
        
        if (stack.size() >= 2) {
            auto rightExpr = popExpr(stack, "add_expr_prime");
            auto leftExpr = popExpr(stack, "mult_expr");
            
            // Si rightExpr es epsilon, solo usar leftExpr
            if (auto numberExpr = dynamic_cast<NumberExpr*>(rightExpr.get())) {
                if (numberExpr->value == -999) {
                    stack.push(std::make_unique<ExprSemanticNode>(std::move(leftExpr)));
                    std::cout << "DEBUG: add_expr - used only left expression" << std::endl;
                    return;
                }
            }
            
            // Aquí necesitamos determinar si es suma o resta - asumimos que el operador está en rightExpr
            // Para simplificar, asumimos suma por defecto
            auto addExpr = std::make_unique<BinaryExpr>(BinaryExpr::OP_ADD, std::move(leftExpr), std::move(rightExpr));
            stack.push(std::make_unique<ExprSemanticNode>(std::move(addExpr)));
            std::cout << "DEBUG: add_expr - created ADD expression" << std::endl;
        } else {
            pushEpsilonMarker(stack);
            std::cout << "DEBUG: add_expr - fallback epsilon marker" << std::endl;
        }
    });
    
    // ID 29: add_expr_prime -> PLUS mult_expr add_expr_prime
    parser.setSemanticAction(29, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        printStackInfo(stack, "add_expr_prime->PLUS mult_expr add_expr_prime");
        
        if (stack.size() >= 2) {
            auto rightExpr = popExpr(stack, "add_expr_prime (rest)");
            auto leftExpr = popExpr(stack, "mult_expr");
            
            // Si rightExpr es epsilon, solo usar leftExpr
            if (auto numberExpr = dynamic_cast<NumberExpr*>(rightExpr.get())) {
                if (numberExpr->value == -999) {
                    stack.push(std::make_unique<ExprSemanticNode>(std::move(leftExpr)));
                    std::cout << "DEBUG: add_expr_prime PLUS - used only left expression" << std::endl;
                    return;
                }
            }
            
            auto addExpr = std::make_unique<BinaryExpr>(BinaryExpr::OP_ADD, std::move(leftExpr), std::move(rightExpr));
            stack.push(std::make_unique<ExprSemanticNode>(std::move(addExpr)));
            std::cout << "DEBUG: add_expr_prime PLUS - created ADD chain" << std::endl;
        } else {
            pushEpsilonMarker(stack);
            std::cout << "DEBUG: add_expr_prime PLUS - fallback epsilon marker" << std::endl;
        }
    });
    
    // ID 30: add_expr_prime -> MINUS mult_expr add_expr_prime
    parser.setSemanticAction(30, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        printStackInfo(stack, "add_expr_prime->MINUS mult_expr add_expr_prime");
        
        if (stack.size() >= 2) {
            auto rightExpr = popExpr(stack, "add_expr_prime (rest)");
            auto leftExpr = popExpr(stack, "mult_expr");
            
            // Si rightExpr es epsilon, solo usar leftExpr
            if (auto numberExpr = dynamic_cast<NumberExpr*>(rightExpr.get())) {
                if (numberExpr->value == -999) {
                    stack.push(std::make_unique<ExprSemanticNode>(std::move(leftExpr)));
                    std::cout << "DEBUG: add_expr_prime MINUS - used only left expression" << std::endl;
                    return;
                }
            }
            
            auto subExpr = std::make_unique<BinaryExpr>(BinaryExpr::OP_SUB, std::move(leftExpr), std::move(rightExpr));
            stack.push(std::make_unique<ExprSemanticNode>(std::move(subExpr)));
            std::cout << "DEBUG: add_expr_prime MINUS - created SUB chain" << std::endl;
        } else {
            pushEpsilonMarker(stack);
            std::cout << "DEBUG: add_expr_prime MINUS - fallback epsilon marker" << std::endl;
        }
    });
    
    // ID 31: add_expr_prime -> ε
    parser.setSemanticAction(31, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        pushEpsilonMarker(stack);
        std::cout << "DEBUG: add_expr_prime epsilon - empty marker" << std::endl;
    });
    
    // ID 32: mult_expr -> primary_expr mult_expr_prime
    parser.setSemanticAction(32, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        printStackInfo(stack, "mult_expr->primary_expr mult_expr_prime");
        
        if (stack.size() >= 2) {
            auto rightExpr = popExpr(stack, "mult_expr_prime");
            auto leftExpr = popExpr(stack, "primary_expr");
            
            // Si rightExpr es epsilon, solo usar leftExpr
            if (auto numberExpr = dynamic_cast<NumberExpr*>(rightExpr.get())) {
                if (numberExpr->value == -999) {
                    stack.push(std::make_unique<ExprSemanticNode>(std::move(leftExpr)));
                    std::cout << "DEBUG: mult_expr - used only left expression" << std::endl;
                    return;
                }
            }
            
            // Asumimos multiplicación por defecto
            auto multExpr = std::make_unique<BinaryExpr>(BinaryExpr::OP_MUL, std::move(leftExpr), std::move(rightExpr));
            stack.push(std::make_unique<ExprSemanticNode>(std::move(multExpr)));
            std::cout << "DEBUG: mult_expr - created MUL expression" << std::endl;
        } else {
            pushEpsilonMarker(stack);
            std::cout << "DEBUG: mult_expr - fallback epsilon marker" << std::endl;
        }
    });
    
    // ID 36: mult_expr_prime -> ε
    parser.setSemanticAction(36, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        pushEpsilonMarker(stack);
        std::cout << "DEBUG: mult_expr_prime epsilon - empty marker" << std::endl;
    });
    
    // === PRODUCCIONES CLAVE PARA LITERALES ===
    
    // ID 37: primary_expr -> NUMBER
    parser.setSemanticAction(37, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        if (!tokens.empty()) {
            double value = std::stod(tokens[0].lexeme);
            auto numberExpr = std::make_unique<NumberExpr>(value);
            stack.push(std::make_unique<ExprSemanticNode>(std::move(numberExpr)));
            std::cout << "DEBUG: Created NUMBER expression: " << value << std::endl;
        } else {
            auto expr = std::make_unique<NumberExpr>(0);
            stack.push(std::make_unique<ExprSemanticNode>(std::move(expr)));
            std::cout << "DEBUG: NUMBER fallback" << std::endl;
        }
    });
    
    // ID 38: primary_expr -> STRING
    parser.setSemanticAction(38, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        if (!tokens.empty()) {
            std::string value = tokens[0].lexeme;
            if (value.length() >= 2 && value[0] == '"' && value.back() == '"') {
                value = value.substr(1, value.length() - 2);
            }
            auto stringExpr = std::make_unique<StringExpr>(value);
            stack.push(std::make_unique<ExprSemanticNode>(std::move(stringExpr)));
            std::cout << "DEBUG: Created STRING expression: " << value << std::endl;
        } else {
            auto expr = std::make_unique<StringExpr>("");
            stack.push(std::make_unique<ExprSemanticNode>(std::move(expr)));
            std::cout << "DEBUG: STRING fallback" << std::endl;
        }
    });
    
    // ID 39: primary_expr -> TRUE
    parser.setSemanticAction(39, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        auto boolExpr = std::make_unique<BooleanExpr>(true);
        stack.push(std::make_unique<ExprSemanticNode>(std::move(boolExpr)));
        std::cout << "DEBUG: Created TRUE expression" << std::endl;
    });
    
    // ID 40: primary_expr -> FALSE
    parser.setSemanticAction(40, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        auto boolExpr = std::make_unique<BooleanExpr>(false);
        stack.push(std::make_unique<ExprSemanticNode>(std::move(boolExpr)));
        std::cout << "DEBUG: Created FALSE expression" << std::endl;
    });
    
    // ID 41: primary_expr -> IDENT ident_suffix
    parser.setSemanticAction(41, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        if (!tokens.empty()) {
            std::string varName = tokens[0].lexeme;
            auto varExpr = std::make_unique<VariableExpr>(varName);
            stack.push(std::make_unique<ExprSemanticNode>(std::move(varExpr)));
            std::cout << "DEBUG: Created VARIABLE expression: " << varName << std::endl;
        } else {
            auto expr = std::make_unique<VariableExpr>("unknown");
            stack.push(std::make_unique<ExprSemanticNode>(std::move(expr)));
            std::cout << "DEBUG: VARIABLE fallback" << std::endl;
        }
    });
    
    // ID 42: primary_expr -> LPAREN or_expr RPAREN
    parser.setSemanticAction(42, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        printStackInfo(stack, "primary_expr->LPAREN or_expr RPAREN");
        
        if (!stack.empty()) {
            // La expresión en paréntesis ya debería estar en el stack
            std::cout << "DEBUG: Parenthesized expression - passed through" << std::endl;
        } else {
            auto expr = std::make_unique<NumberExpr>(0);
            stack.push(std::make_unique<ExprSemanticNode>(std::move(expr)));
            std::cout << "DEBUG: Parenthesized expression - fallback" << std::endl;
        }
    });
    
    // ID 45: ident_suffix -> ε
    parser.setSemanticAction(45, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        pushEpsilonMarker(stack);
        std::cout << "DEBUG: ident_suffix epsilon - empty marker" << std::endl;
    });
    
    // Acciones por defecto para producciones no implementadas
    for (int i = 0; i < 100; ++i) {
        // Solo configurar si no está ya configurado
        if (parser.getSemanticAction(i) == nullptr) {
            parser.setSemanticAction(i, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
                std::cout << "DEBUG: Default action for production" << std::endl;
                // Pass through por defecto
                if (stack.empty()) {
                    pushEpsilonMarker(stack);
                }
            });
        }
    }
    
    std::cout << "Complete semantic actions V4 setup completed." << std::endl;
}

} // namespace LL1
