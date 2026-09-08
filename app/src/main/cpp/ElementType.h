#pragma once

enum class ElementType {
    ELECTRO,
    ANEMO,
    CRYO,
    PYRO,
    NONE,
    SHADOW,
    COUNT
};

inline const char* GetElementName(ElementType type) {
    switch (type) {
        case ElementType::ELECTRO:
            return "Electro";
        case ElementType::ANEMO:
            return "Anemo";
        case ElementType::CRYO:
            return "Cryo";
        case ElementType::PYRO:
            return "Pyro";
        case ElementType::NONE:
            return "Common";
        case ElementType::SHADOW:
            return "Shadow";
        case ElementType::COUNT:
        default:
            return "InvalidType";
            break;
    }
}
