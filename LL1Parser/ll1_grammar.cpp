#include "ll1_grammar.hpp"
#include <algorithm>
#include <iomanip>

namespace LL1 {

// Símbolos especiales
const Symbol EPSILON(SymbolType::EPSILON, "ε");
const Symbol END_OF_INPUT(SymbolType::END_OF_INPUT, "$");

void Grammar::computeFirstSets() {
    if (firstSetsComputed) return;
    
    firstSets.clear();
    
    // Inicializar conjuntos FIRST para todos los símbolos
    for (const auto& terminal : terminals) {
        firstSets[terminal].insert(terminal);
    }
    
    for (const auto& nonTerminal : nonTerminals) {
        firstSets[nonTerminal] = std::set<Symbol>();
    }
    
    // Algoritmo iterativo para calcular FIRST
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (const auto& production : productions) {
            const Symbol& A = production.lhs;
            const std::vector<Symbol>& alpha = production.rhs;
            
            // Si A -> ε, agregar ε a FIRST(A)
            if (production.isEpsilonProduction()) {
                if (firstSets[A].find(EPSILON) == firstSets[A].end()) {
                    firstSets[A].insert(EPSILON);
                    changed = true;
                }
                continue;
            }
            
            // Para A -> X1 X2 ... Xk
            for (size_t i = 0; i < alpha.size(); ++i) {
                const Symbol& Xi = alpha[i];
                
                // Agregar FIRST(Xi) - {ε} a FIRST(A)
                for (const auto& symbol : firstSets[Xi]) {
                    if (!symbol.isEpsilon() && firstSets[A].find(symbol) == firstSets[A].end()) {
                        firstSets[A].insert(symbol);
                        changed = true;
                    }
                }
                
                // Si ε no está en FIRST(Xi), salir del bucle
                if (firstSets[Xi].find(EPSILON) == firstSets[Xi].end()) {
                    break;
                }
                
                // Si llegamos al final y todos los Xi derivan ε
                if (i == alpha.size() - 1) {
                    if (firstSets[A].find(EPSILON) == firstSets[A].end()) {
                        firstSets[A].insert(EPSILON);
                        changed = true;
                    }
                }
            }
        }
    }
    
    firstSetsComputed = true;
}

std::set<Symbol> Grammar::first(const std::vector<Symbol>& alpha) {
    if (!firstSetsComputed) {
        computeFirstSets();
    }
    
    std::set<Symbol> result;
    
    if (alpha.empty()) {
        result.insert(EPSILON);
        return result;
    }
    
    // Si es una producción épsilon explícita (un solo símbolo épsilon)
    if (alpha.size() == 1 && alpha[0].isEpsilon()) {
        result.insert(EPSILON);
        return result;
    }
    
    for (size_t i = 0; i < alpha.size(); ++i) {
        const Symbol& Xi = alpha[i];
        
        // Agregar FIRST(Xi) - {ε} al resultado
        for (const auto& symbol : firstSets[Xi]) {
            if (!symbol.isEpsilon()) {
                result.insert(symbol);
            }
        }
        
        // Si ε no está en FIRST(Xi), terminar
        if (firstSets[Xi].find(EPSILON) == firstSets[Xi].end()) {
            break;
        }
        
        // Si llegamos al final y todos derivan ε
        if (i == alpha.size() - 1) {
            result.insert(EPSILON);
        }
    }
    
    return result;
}

const std::map<Symbol, std::set<Symbol>>& Grammar::getFirstSets() {
    if (!firstSetsComputed) {
        computeFirstSets();
    }
    return firstSets;
}

void Grammar::computeFollowSets() {
    if (followSetsComputed) return;
    if (!firstSetsComputed) {
        computeFirstSets();
    }
    
    followSets.clear();
    
    // Inicializar conjuntos FOLLOW
    for (const auto& nonTerminal : nonTerminals) {
        followSets[nonTerminal] = std::set<Symbol>();
    }
    
    // FOLLOW(S) contiene $
    followSets[startSymbol].insert(END_OF_INPUT);
    
    // Algoritmo iterativo para calcular FOLLOW
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (const auto& production : productions) {
            const std::vector<Symbol>& beta = production.rhs;
            
            // Para cada símbolo en el lado derecho
            for (size_t i = 0; i < beta.size(); ++i) {
                const Symbol& B = beta[i];
                
                if (!B.isNonTerminal()) continue;
                
                // Obtener β (resto de la producción después de B)
                std::vector<Symbol> restBeta(beta.begin() + i + 1, beta.end());
                
                // Calcular FIRST(β)
                std::set<Symbol> firstBeta = first(restBeta);
                
                // Agregar FIRST(β) - {ε} a FOLLOW(B)
                for (const auto& symbol : firstBeta) {
                    if (!symbol.isEpsilon() && followSets[B].find(symbol) == followSets[B].end()) {
                        followSets[B].insert(symbol);
                        changed = true;
                    }
                }
                
                // Si ε ∈ FIRST(β), agregar FOLLOW(A) a FOLLOW(B)
                if (firstBeta.find(EPSILON) != firstBeta.end()) {
                    for (const auto& symbol : followSets[production.lhs]) {
                        if (followSets[B].find(symbol) == followSets[B].end()) {
                            followSets[B].insert(symbol);
                            changed = true;
                        }
                    }
                }
            }
        }
    }
    
    followSetsComputed = true;
}

const std::map<Symbol, std::set<Symbol>>& Grammar::getFollowSets() {
    if (!followSetsComputed) {
        computeFollowSets();
    }
    return followSets;
}

bool Grammar::buildParseTable() {
    if (parseTableComputed) return true;
    
    if (!firstSetsComputed) computeFirstSets();
    if (!followSetsComputed) computeFollowSets();
    
    parseTable.clear();
    
    // Para cada producción A -> α
    for (const auto& production : productions) {
        const Symbol& A = production.lhs;
        const std::vector<Symbol>& alpha = production.rhs;
        
        // Calcular FIRST(α)
        std::set<Symbol> firstAlpha = first(alpha);
        
        // Para cada terminal a en FIRST(α)
        for (const auto& a : firstAlpha) {
            if (a.isTerminal()) {
                auto key = std::make_pair(A, a);
                if (parseTable.find(key) != parseTable.end()) {
                    // Conflicto: la gramática no es LL(1)
                    std::cerr << "Grammar is not LL(1): conflict at [" << A.name 
                              << ", " << a.name << "]" << std::endl;
                    return false;
                }
                parseTable[key] = production.id;
            }
        }
        
        // Si ε ∈ FIRST(α)
        if (firstAlpha.find(EPSILON) != firstAlpha.end()) {
            // Para cada terminal b en FOLLOW(A)
            for (const auto& b : followSets[A]) {
                if (b.isTerminal() || b.isEndOfInput()) {
                    auto key = std::make_pair(A, b);
                    if (parseTable.find(key) != parseTable.end()) {
                        // Conflicto: la gramática no es LL(1)
                        std::cerr << "Grammar is not LL(1): conflict at [" << A.name 
                                  << ", " << b.name << "] - production " << production.id << std::endl;
                        return false;
                    }
                    parseTable[key] = production.id;
                }
            }
        }
    }
    
    parseTableComputed = true;
    return true;
}

const std::map<std::pair<Symbol, Symbol>, int>& Grammar::getParseTable() {
    if (!parseTableComputed) {
        buildParseTable();
    }
    return parseTable;
}

bool Grammar::isLL1() {
    return buildParseTable();
}

void Grammar::printGrammar() const {
    std::cout << "Grammar Productions:" << std::endl;
    std::cout << "===================" << std::endl;
    for (size_t i = 0; i < productions.size(); ++i) {
        std::cout << std::setw(3) << i << ": " << productions[i].toString() << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << "Start Symbol: " << startSymbol.name << std::endl;
    std::cout << std::endl;
}

void Grammar::printFirstSets() {
    if (!firstSetsComputed) computeFirstSets();
    
    std::cout << "FIRST Sets:" << std::endl;
    std::cout << "===========" << std::endl;
    
    for (const auto& nonTerminal : nonTerminals) {
        std::cout << "FIRST(" << nonTerminal.name << ") = { ";
        bool first = true;
        for (const auto& symbol : firstSets.at(nonTerminal)) {
            if (!first) std::cout << ", ";
            std::cout << symbol.name;
            first = false;
        }
        std::cout << " }" << std::endl;
    }
    std::cout << std::endl;
}

void Grammar::printFollowSets() {
    if (!followSetsComputed) computeFollowSets();
    
    std::cout << "FOLLOW Sets:" << std::endl;
    std::cout << "============" << std::endl;
    
    for (const auto& nonTerminal : nonTerminals) {
        std::cout << "FOLLOW(" << nonTerminal.name << ") = { ";
        bool first = true;
        for (const auto& symbol : followSets.at(nonTerminal)) {
            if (!first) std::cout << ", ";
            std::cout << symbol.name;
            first = false;
        }
        std::cout << " }" << std::endl;
    }
    std::cout << std::endl;
}

void Grammar::printParseTable() {
    if (!parseTableComputed) buildParseTable();
    
    std::cout << "LL(1) Parse Table:" << std::endl;
    std::cout << "==================" << std::endl;
    
    // Crear header con terminales
    std::vector<Symbol> terminalsList(terminals.begin(), terminals.end());
    terminalsList.push_back(END_OF_INPUT);
    
    // Imprimir header
    std::cout << std::setw(12) << " ";
    for (const auto& terminal : terminalsList) {
        std::cout << std::setw(8) << terminal.name;
    }
    std::cout << std::endl;
    
    // Línea separadora
    std::cout << std::setw(12) << " ";
    for (size_t i = 0; i < terminalsList.size(); ++i) {
        std::cout << std::setw(8) << "--------";
    }
    std::cout << std::endl;
    
    // Imprimir filas
    for (const auto& nonTerminal : nonTerminals) {
        std::cout << std::setw(12) << nonTerminal.name;
        for (const auto& terminal : terminalsList) {
            auto key = std::make_pair(nonTerminal, terminal);
            auto it = parseTable.find(key);
            if (it != parseTable.end()) {
                std::cout << std::setw(8) << it->second;
            } else {
                std::cout << std::setw(8) << "-";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    // Debug: mostrar todas las entradas de la tabla
    std::cout << "Debug - All parse table entries:" << std::endl;
    for (const auto& entry : parseTable) {
        std::cout << "[" << entry.first.first.name << ", " << entry.first.second.name << "] -> " << entry.second << std::endl;
    }
}

} // namespace LL1
