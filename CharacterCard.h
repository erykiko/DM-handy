#ifndef DM_HANDY_CHARACTERCARD_H
#define DM_HANDY_CHARACTERCARD_H

#include <string>
#include <fstream>
#include <ftxui/dom/elements.hpp>
#include "Enums.h"

class CharacterCard {
public:
    std::string m_str = "10", m_dex = "10", m_con = "10";
    std::string m_int = "10", m_wis = "10", m_cha = "10";
    std::string m_lvl = "1";
    std::string m_name = "New_Hero";
    std::string m_class = "Fighter";
    std::string m_race = "Human";

    ftxui::Element getCharCard();

    void saveToFile() {
        // Save using the character name as the filename
        std::ofstream out(m_name + ".char");
        if (out.is_open()) {
            out << m_name << "\n" << m_race << "\n" << m_class << "\n" << m_lvl << "\n"
                << m_str << "\n" << m_dex << "\n" << m_con << "\n"
                << m_int << "\n" << m_wis << "\n" << m_cha << "\n";
        }
    }

    void loadFromFile(std::string filename) {
        std::ifstream in(filename);
        if (in.is_open()) {
            std::getline(in, m_name); std::getline(in, m_race);
            std::getline(in, m_class); std::getline(in, m_lvl);
            std::getline(in, m_str); std::getline(in, m_dex);
            std::getline(in, m_con); std::getline(in, m_int);
            std::getline(in, m_wis); std::getline(in, m_cha);
        }
    }
};

#endif