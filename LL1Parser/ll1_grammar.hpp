#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <memory>

namespace LL1 {

// Símbolos de la gramática
enum class SymbolType {
    TERMINAL,
    NON_TERMINAL,
    EPSILON,
    END_OF_INPUT
};

struct Symbol {
    SymbolType type;
    std::string name;
    
    Symbol() = default;
    Symbol(SymbolType t, const std::string& n) : type(t), name(n) {}
    
    bool operator==(const Symbol& other) const {
        return type == other.type && name == other.name;
    }
    
    bool operator<(const Symbol& other) const {
        if (type != other.type) return type < other.type;
        return name < other.name;
    }
    
    bool isTerminal() const { return type == SymbolType::TERMINAL; }
    bool isNonTerminal() const { return type == SymbolType::NON_TERMINAL; }
    bool isEpsilon() const { return type == SymbolType::EPSILON; }
    bool isEndOfInput() const { return type == SymbolType::END_OF_INPUT; }
};

// Producción de la gramática: A -> α
struct Production {
    Symbol lhs;                    // lado izquierdo (no terminal)
    std::vector<Symbol> rhs;       // lado derecho (secuencia de símbolos)
    int id;                        // identificador único de la producción
    
    Production(const Symbol& left, const std::vector<Symbol>& right, int production_id = -1)
        : lhs(left), rhs(right), id(production_id) {}
    
    bool isEpsilonProduction() const {
        return rhs.size() == 1 && rhs[0].isEpsilon();
    }
    
    std::string toString() const {
        std::string result = lhs.name + " -> ";
        if (rhs.empty()) {
            result += "ε";
        } else {
            for (size_t i = 0; i < rhs.size(); ++i) {
                if (i > 0) result += " ";
                result += rhs[i].name;
            }
        }
        return result;
    }
};

// Gramática LL(1)
class Grammar {
private:
    std::vector<Production> productions;
    std::set<Symbol> terminals;
    std::set<Symbol> nonTerminals;
    Symbol startSymbol;
    
    // Conjuntos FIRST y FOLLOW para análisis LL(1)
    std::map<Symbol, std::set<Symbol>> firstSets;
    std::map<Symbol, std::set<Symbol>> followSets;
    
    // Tabla de análisis LL(1)
    std::map<std::pair<Symbol, Symbol>, int> parseTable;
    
    bool firstSetsComputed = false;
    bool followSetsComputed = false;
    bool parseTableComputed = false;

public:
    Grammar() = default;
    
    // Agregar producción
    void addProduction(const Symbol& lhs, const std::vector<Symbol>& rhs) {
        productions.emplace_back(lhs, rhs, productions.size());
        nonTerminals.insert(lhs);
        
        // Agregar símbolos del lado derecho
        for (const auto& symbol : rhs) {
            if (symbol.isTerminal()) {
                terminals.insert(symbol);
            } else if (symbol.isNonTerminal()) {
                nonTerminals.insert(symbol);
            }
        }
        
        // Resetear cálculos
        firstSetsComputed = false;
        followSetsComputed = false;
        parseTableComputed = false;
    }
    
    void setStartSymbol(const Symbol& start) {
        startSymbol = start;
        nonTerminals.insert(start);
    }
    
    // Getters
    const std::vector<Production>& getProductions() const { return productions; }
    const std::set<Symbol>& getTerminals() const { return terminals; }
    const std::set<Symbol>& getNonTerminals() const { return nonTerminals; }
    const Symbol& getStartSymbol() const { return startSymbol; }
    
    // Métodos para calcular conjuntos FIRST
    void computeFirstSets();
    std::set<Symbol> first(const std::vector<Symbol>& alpha);
    const std::map<Symbol, std::set<Symbol>>& getFirstSets();
    
    // Métodos para calcular conjuntos FOLLOW
    void computeFollowSets();
    const std::map<Symbol, std::set<Symbol>>& getFollowSets();
    
    // Construcción de tabla de análisis LL(1)
    bool buildParseTable();
    const std::map<std::pair<Symbol, Symbol>, int>& getParseTable();
    
    // Verificar si la gramática es LL(1)
    bool isLL1();
    
    // Utilidades
    void printGrammar() const;
    void printFirstSets();
    void printFollowSets();
    void printParseTable();
    
private:
    void computeFirstSetsInternal();
    void computeFollowSetsInternal();
};

// Símbolos especiales
extern const Symbol EPSILON;
extern const Symbol END_OF_INPUT;

} // namespace LL1
