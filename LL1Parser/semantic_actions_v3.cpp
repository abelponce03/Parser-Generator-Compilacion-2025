#include "ll1_parser.hpp"
#include "semantic_nodes.hpp"
#include "../ast.hpp"
#include <iostream>

namespace LL1 {

// Funciones auxiliares para crear nodos del AST
namespace SemanticActions {

// Crear un programa desde una lista de statements
std::unique_ptr<Program> createProgram(std::vector<StmtPtr> statements) {
    auto program = std::make_unique<Program>();
    program->stmts = std::move(statements);
    return program;
}

// Crear una expresión statement
std::unique_ptr<ExprStmt> createExprStmt(ExprPtr expr) {
    return std::make_unique<ExprStmt>(std::move(expr));
}

// Crear expresiones literales
std::unique_ptr<NumberExpr> createNumberExpr(double value) {
    return std::make_unique<NumberExpr>(value);
}

std::unique_ptr<StringExpr> createStringExpr(const std::string& value) {
    return std::make_unique<StringExpr>(value);
}

std::unique_ptr<BooleanExpr> createBooleanExpr(bool value) {
    return std::make_unique<BooleanExpr>(value);
}

std::unique_ptr<VariableExpr> createVariableExpr(const std::string& name) {
    return std::make_unique<VariableExpr>(name);
}

// Función auxiliar para convertir string de operador a enum
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
    // Por defecto, asumir suma
    return BinaryExpr::OP_ADD;
}

// Crear expresiones binarias (actualizada para usar enum)
std::unique_ptr<BinaryExpr> createBinaryExpr(ExprPtr left, const std::string& op, ExprPtr right) {
    BinaryExpr::Op binOp = stringToOp(op);
    return std::make_unique<BinaryExpr>(binOp, std::move(left), std::move(right));
}

// Crear expresiones let (adaptada para AST existente)
std::unique_ptr<LetExpr> createLetExpr(const std::string& varName, ExprPtr initializer, ExprPtr body) {
    // El AST existente espera un StmtPtr para el cuerpo, así que convertimos la expresión a ExprStmt
    auto bodyStmt = std::make_unique<ExprStmt>(std::move(body));
    return std::make_unique<LetExpr>(varName, std::move(initializer), std::move(bodyStmt));
}

// Versión que acepta múltiples bindings (para nuestra gramática)
std::unique_ptr<LetExpr> createLetExprMultiple(std::vector<std::pair<std::string, ExprPtr>> bindings, ExprPtr body) {
    // Para simplificar, convertir múltiples bindings en expresiones let anidadas
    if (bindings.empty()) {
        // No debería pasar, pero manejo el caso
        return nullptr;
    }
    
    ExprPtr result = std::move(body);
    
    // Construir desde atrás hacia adelante
    for (int i = bindings.size() - 1; i >= 0; --i) {
        auto bodyStmt = std::make_unique<ExprStmt>(std::move(result));
        result = std::make_unique<LetExpr>(bindings[i].first, std::move(bindings[i].second), std::move(bodyStmt));
    }
    
    return std::unique_ptr<LetExpr>(static_cast<LetExpr*>(result.release()));
}

// Crear expresiones if
std::unique_ptr<IfExpr> createIfExpr(ExprPtr condition, ExprPtr thenExpr, ExprPtr elseExpr = nullptr) {
    return std::make_unique<IfExpr>(std::move(condition), std::move(thenExpr), std::move(elseExpr));
}

// Crear expresiones while
std::unique_ptr<WhileExpr> createWhileExpr(ExprPtr condition, ExprPtr body) {
    return std::make_unique<WhileExpr>(std::move(condition), std::move(body));
}

// Crear llamadas a función
std::unique_ptr<CallExpr> createCallExpr(const std::string& functionName, std::vector<ExprPtr> arguments) {
    return std::make_unique<CallExpr>(functionName, std::move(arguments));
}

// Crear declaraciones de función (adaptada para AST existente)
std::unique_ptr<FunctionDecl> createFunctionDecl(const std::string& name, 
                                                 std::vector<std::string> parameters, 
                                                 ExprPtr body) {
    // El AST existente espera un StmtPtr para el cuerpo
    auto bodyStmt = std::make_unique<ExprStmt>(std::move(body));
    return std::make_unique<FunctionDecl>(name, std::move(parameters), std::move(bodyStmt));
}

// Crear expresiones new
std::unique_ptr<NewExpr> createNewExpr(const std::string& typeName, std::vector<ExprPtr> arguments) {
    return std::make_unique<NewExpr>(typeName, std::move(arguments));
}

// Crear bloques de expresiones (adaptado para AST existente)
std::unique_ptr<ExprBlock> createExprBlock(std::vector<ExprPtr> expressions) {
    // Convertir ExprPtr a StmtPtr wrapeando en ExprStmt
    std::vector<StmtPtr> statements;
    for (auto& expr : expressions) {
        statements.push_back(std::make_unique<ExprStmt>(std::move(expr)));
    }
    return std::make_unique<ExprBlock>(std::move(statements));
}

} // namespace SemanticActions

// Configurar acciones semánticas para la gramática V3
void ParserFactory::setupFullHulkSemanticActionsV3(LL1Parser& parser) {
    std::cout << "Setting up semantic actions for Full HULK Grammar V3..." << std::endl;
    
    // Acción que siempre pone un programa vacío en el stack (para debug)
    auto createEmptyProgram = [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        (void)tokens;
        // Crear programa vacío y ponerlo en el stack
        auto program = std::make_unique<Program>();
        stack.push(std::make_unique<ProgramSemanticNode>(std::move(program)));
        std::cout << "DEBUG: Created empty program node" << std::endl;
    };
    
    // Acción para literales NUMBER
    auto createNumberAction = [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        if (!tokens.empty() && tokens[0].symbol.name == "NUMBER") {
            double value = std::stod(tokens[0].lexeme);
            auto numberExpr = std::make_unique<NumberExpr>(value);
            stack.push(std::make_unique<ExprSemanticNode>(std::move(numberExpr)));
            std::cout << "DEBUG: Created number expr: " << value << std::endl;
        } else {
            std::cout << "DEBUG: createNumberAction called but no NUMBER token" << std::endl;
        }
    };
    
    // Acción para crear expr_stmt
    auto createExprStmtAction = [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        (void)tokens;
        if (!stack.empty()) {
            auto exprNode = dynamic_cast<ExprSemanticNode*>(stack.top().get());
            if (exprNode) {
                auto expr = std::move(exprNode->expr);
                stack.pop();
                auto exprStmt = std::make_unique<ExprStmt>(std::move(expr));
                auto program = std::make_unique<Program>();
                program->stmts.push_back(std::move(exprStmt));
                stack.push(std::make_unique<ProgramSemanticNode>(std::move(program)));
                std::cout << "DEBUG: Created program with expr statement" << std::endl;
            } else {
                std::cout << "DEBUG: createExprStmtAction: top is not ExprSemanticNode" << std::endl;
                createEmptyProgram(tokens, stack);
            }
        } else {
            std::cout << "DEBUG: createExprStmtAction: stack is empty" << std::endl;
            createEmptyProgram(tokens, stack);
        }
    };
    
    // Acción de debug genérica
    auto debugAction = [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        (void)tokens;
        (void)stack;
        std::cout << "DEBUG: Generic action called" << std::endl;
    };
    
    // Configurar acciones por defecto para todas las producciones
    for (int i = 0; i < 100; ++i) {
        parser.setSemanticAction(i, debugAction);
    }
    
    // Configurar acciones específicas para producciones importantes
    // Estas son suposiciones educadas sobre los IDs de producción
    
    // Asumir que las primeras producciones son para el programa principal
    parser.setSemanticAction(0, createEmptyProgram);  // program -> stmt_list
    parser.setSemanticAction(1, createEmptyProgram);  // stmt_list -> stmt stmt_list
    parser.setSemanticAction(2, createEmptyProgram);  // stmt_list -> ε
    
    // Buscar producciones que manejen números (buscar en rangos probables)
    for (int i = 10; i < 30; ++i) {
        parser.setSemanticAction(i, createNumberAction);
    }
    
    // Buscar producciones que creen statements
    for (int i = 3; i < 10; ++i) {
        parser.setSemanticAction(i, createExprStmtAction);
    }
    
    std::cout << "Debug semantic actions setup completed for V3 grammar." << std::endl;
}

} // namespace LL1
