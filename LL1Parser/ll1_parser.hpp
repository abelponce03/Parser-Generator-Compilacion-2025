#pragma once

#include "ll1_grammar.hpp"
#include "../ast.hpp"
#include <stack>
#include <functional>

namespace LL1 {

// Token para el analizador léxico
struct Token {
    Symbol symbol;
    std::string lexeme;
    int line;
    int column;
    
    // Para valores literales
    union {
        double numberValue;
        bool boolValue;
    };
    std::string stringValue;
    
    Token() : line(0), column(0), numberValue(0.0) {}
    Token(const Symbol& sym, const std::string& lex, int l = 0, int c = 0)
        : symbol(sym), lexeme(lex), line(l), column(c), numberValue(0.0) {}
};

// Analizador léxico simple
class Lexer {
private:
    std::string input;
    size_t position;
    int line;
    int column;
    
public:
    Lexer(const std::string& text) : input(text), position(0), line(1), column(1) {}
    
    Token nextToken();
    Token peekToken();
    bool hasMoreTokens() const { return position < input.size(); }
    
private:
    char peek(int offset = 0) const;
    char advance();
    void skipWhitespace();
    Token makeToken(const Symbol& symbol, const std::string& lexeme = "");
    Token readNumber();
    Token readString();
    Token readIdentifier();
};

// Base class for AST nodes on semantic stack
struct SemanticNode {
    virtual ~SemanticNode() = default;
};

// Acciones semánticas para construir el AST
using SemanticAction = std::function<void(const std::vector<Token>&, std::stack<std::unique_ptr<SemanticNode>>&)>;

// Analizador sintáctico LL(1)
class LL1Parser {
private:
    Grammar grammar;
    std::map<int, SemanticAction> semanticActions;
    std::unique_ptr<Lexer> lexer;
    Token currentToken;
    
    // Pila para construir el AST
    std::stack<std::unique_ptr<SemanticNode>> semanticStack;
    
public:
    LL1Parser(const Grammar& g) : grammar(g) {}
    
    // Configurar acciones semánticas
    void setSemanticAction(int productionId, const SemanticAction& action);
    
    // Obtener acción semántica (para verificar si existe)
    SemanticAction* getSemanticAction(int productionId);
    
    // Analizar entrada
    std::unique_ptr<Program> parse(const std::string& input);
    
    // Obtener el programa AST resultante
    std::unique_ptr<Program> getProgram();
    
private:
    void advance();
    void parseInternal();
    void executeSemanticAction(int productionId, const std::vector<Symbol>& rhs);
    void reportSyntaxError(const std::string& message);
};

// Factory para crear analizadores con gramáticas específicas
class ParserFactory {
public:
    // Crear parser para la gramática de HULK
    static std::unique_ptr<LL1Parser> createHulkParser();
    
    // Crear gramática HULK (público para testing)
    static Grammar createHulkGrammar();
    
    // Crear gramática HULK simplificada para testing
    static Grammar createSimpleHulkGrammar();
    
    // Crear parser simple para testing
    static std::unique_ptr<LL1Parser> createSimpleHulkParser();
    
    // Crear gramática HULK intermedia (expandida gradualmente)
    static Grammar createIntermediateHulkGrammar();
    
    // Crear parser HULK intermedio
    static std::unique_ptr<LL1Parser> createIntermediateHulkParser();
    
    // Crear gramática HULK completa
    static Grammar createFullHulkGrammar();
    
    // Crear parser HULK completo
    static std::unique_ptr<LL1Parser> createFullHulkParser();
    
    // Crear gramática HULK completa V2 (mejorada)
    static Grammar createFullHulkGrammarV2();
    
    // Crear parser HULK completo V2
    static std::unique_ptr<LL1Parser> createFullHulkParserV2();
    static Grammar createFullHulkGrammarV3();
    static std::unique_ptr<LL1Parser> createFullHulkParserV3();
    static std::unique_ptr<LL1Parser> createFullHulkParserV4();

    // Métodos para acciones semánticas (si se usan)
    // static void setupFullHulkSemanticActions(LL1Parser& parser);
    static void setupFullHulkSemanticActionsV3(LL1Parser& parser);
    static void setupCompleteSemanticActionsV4(LL1Parser& parser);
    
private:
    static void setupHulkSemanticActions(LL1Parser& parser);
    static void setupSimpleHulkSemanticActions(LL1Parser& parser);
};

} // namespace LL1
