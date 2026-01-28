#include "CharacterCard.h"

ftxui::Element CharacterCard::getCharCard() {
    using namespace ftxui;
    
    auto stat_line = [&](std::string label, std::string val) {
        return hbox({
            text(label + ": ") | size(WIDTH, EQUAL, 5),
            text(val) | bold | color(Color::Yellow)
        });
    };

    Element stats = vbox({
        text(" Stats ") | hcenter | bold,
        separator(),
        stat_line("STR", m_str), stat_line("DEX", m_dex),
        stat_line("CON", m_con), stat_line("INT", m_int),
        stat_line("WIS", m_wis), stat_line("CHA", m_cha)
    }) | border | size(WIDTH, EQUAL, 15);

    Element info_header = vbox({
        hbox({ text("Name: ") | bold, text(m_name) | color(Color::Green) }),
        separator(),
        hbox({
            text("Race: " + m_race) | flex,
            separator(),
            text("Class: " + m_class) | flex,
            separator(),
            text("Lvl: " + m_lvl)
        })
    }) | border;

    return vbox({
        info_header,
        hbox({
            stats,
            vbox({
                hbox({ text(" Armor ") | border | flex, text(" Max HP ") | border | flex }),
                hbox({ text(" Saves ") | border | flex, text(" Curr HP ") | border | flex })
            }) | flex
        })
    });
}