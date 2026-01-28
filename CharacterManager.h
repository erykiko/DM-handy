#ifndef DM_HANDY_CHARACTERMANAGER_H
#define DM_HANDY_CHARACTERMANAGER_H

#include <filesystem>
#include "CharacterCard.h"
#include <ftxui/component/component.hpp>

namespace fs = std::filesystem;

class CharacterManager {
public:
    CharacterCard activeCard;
    int class_selected = 0;
    int file_selected = 0;
    std::vector<std::string> classes = {"Fighter", "Barbarian", "Wizard", "Sorcerer", "Monk", "Cleric", "Warlock", "Paladin", "Monster"};
    std::vector<std::string> saved_files;

    CharacterManager() { refreshFiles(); }

    void refreshFiles() {
        saved_files.clear();
        for (const auto& entry : fs::directory_iterator(".")) {
            if (entry.path().extension() == ".char") saved_files.push_back(entry.path().filename().string());
        }
        if (saved_files.empty()) saved_files.push_back("No saves found");
    }

    ftxui::Component GetComponent() {
        using namespace ftxui;

        auto input_name = Input(&activeCard.m_name, "Name");
        auto input_race = Input(&activeCard.m_race, "Race");
        auto input_lvl  = Input(&activeCard.m_lvl, "Level");

        auto input_str = Input(&activeCard.m_str, "10");
        auto input_dex = Input(&activeCard.m_dex, "10");
        auto input_con = Input(&activeCard.m_con, "10");
        auto input_int = Input(&activeCard.m_int, "10");
        auto input_wis = Input(&activeCard.m_wis, "10");
        auto input_cha = Input(&activeCard.m_cha, "10");

        auto sel_class = Radiobox(&classes, &class_selected);
        auto file_menu = Menu(&saved_files, &file_selected);

        // --- BUTTONS ---
        auto btn_new = Button(" NEW ", [this] {
            activeCard = CharacterCard(); // Reset to defaults
            class_selected = 0;
        });

        auto btn_save = Button(" SAVE ", [this] {
            activeCard.m_class = classes[class_selected];
            activeCard.saveToFile();
            refreshFiles();
        });

        auto btn_load = Button(" LOAD ", [this] {
            if (saved_files[file_selected] != "No saves found") {
                activeCard.loadFromFile(saved_files[file_selected]);
                for(int i=0; i<classes.size(); ++i) if(classes[i] == activeCard.m_class) class_selected = i;
            }
        });

        auto container = Container::Horizontal({
            Container::Vertical({
                input_name, input_race, input_lvl,
                input_str, input_dex, input_con,
                input_int, input_wis, input_cha,
                sel_class,
                Container::Horizontal({btn_new, btn_save}) // Grouped Buttons
            }),
            Container::Vertical({ file_menu, btn_load })
        });

        return Renderer(container, [=] {
            auto stat_field = [](std::string label, Component c) {
                return hbox({ text(label) | size(WIDTH, EQUAL, 5), c->Render() | border | size(WIDTH, EQUAL, 5) });
            };

            return hbox({
                vbox({
                    text(" EDITOR ") | bold | color(Color::Blue),
                    hbox(text(" Name: "), input_name->Render() | border | flex),
                    hbox(text(" Race: "), input_race->Render() | border | flex),
                    separator(),
                    text(" ATTRIBUTES ") | bold | color(Color::Red),
                    hbox({
                        vbox({ stat_field("STR", input_str), stat_field("DEX", input_dex), stat_field("CON", input_con) }),
                        vbox({ stat_field("INT", input_int), stat_field("WIS", input_wis), stat_field("CHA", input_cha) })
                    }),
                    separator(),
                    text(" CLASS ") | bold | color(Color::Cyan),
                    sel_class->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 5) | border,
                    hbox({ btn_new->Render() | border | color(Color::Yellow), btn_save->Render() | border | color(Color::Green) })
                }) | size(WIDTH, EQUAL, 35),
                separator(),
                vbox({
                    text(" SAVES ") | bold | color(Color::Yellow),
                    file_menu->Render() | vscroll_indicator | frame | flex | border,
                    btn_load->Render() | border,
                }) | size(WIDTH, EQUAL, 25),
                separator(),
                activeCard.getCharCard() | flex
            });
        });
    }
};

#endif