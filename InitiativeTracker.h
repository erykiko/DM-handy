#ifndef DM_HANDY_INITIATIVETRACKER_H
#define DM_HANDY_INITIATIVETRACKER_H

#include <vector>
#include <string>
#include <algorithm>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

struct Combatant {
    std::string name;
    std::string roll;
    int hp = 0;
};

class InitiativeTracker {
private:
    std::vector<Combatant> m_combatants;
    std::string m_newName = "";
    std::string m_newRoll = "";
    std::string m_newHp = "";
    int m_active_turn_index = 0;

public:
    ftxui::Component GetComponent() {
        using namespace ftxui;

        // --- CUSTOM BUTTON STYLE (No background change) ---
        auto custom_btn_opt = ButtonOption::Simple();
        custom_btn_opt.transform = [](const EntryState& s) {
            auto element = text(s.label);
            if (s.focused) {
                element |= bold | color(Color::White);
            }
            return element;
        };

        auto input_name = Input(&m_newName, "Name...");
        auto input_roll = Input(&m_newRoll, "Init");
        auto input_hp   = Input(&m_newHp, "HP");

        auto btn_add = Button(" ADD ", [this] {
            if (!m_newName.empty()) {
                try {
                    int start_hp = m_newHp.empty() ? 0 : std::stoi(m_newHp);
                    m_combatants.push_back({m_newName, m_newRoll, start_hp});
                } catch (...) {
                    m_combatants.push_back({m_newName, m_newRoll, 0});
                }
                m_newName = ""; m_newRoll = ""; m_newHp = "";
            }
        }, custom_btn_opt);

        auto btn_sort = Button(" SORT ", [this] {
            std::sort(m_combatants.begin(), m_combatants.end(), [](const Combatant& a, const Combatant& b) {
                try {
                    return std::stoi(a.roll) > std::stoi(b.roll);
                } catch (...) { return false; }
            });
            m_active_turn_index = 0;
        }, custom_btn_opt);

        auto btn_next = Button(" NEXT TURN ", [this] {
            if (!m_combatants.empty()) {
                m_active_turn_index = (m_active_turn_index + 1) % m_combatants.size();
            }
        }, custom_btn_opt);

        auto btn_clear = Button(" CLEAR ALL ", [this] {
            m_combatants.clear();
            m_active_turn_index = 0;
        }, custom_btn_opt);

        auto list_container = Container::Vertical({});

        auto main_container = Container::Vertical({
            Container::Horizontal({ input_name, input_roll, input_hp, btn_add }),
            Container::Horizontal({ btn_sort, btn_next, btn_clear }),
            list_container
        });

        return Renderer(main_container, [this, input_name, input_roll, input_hp, btn_add, btn_sort, btn_next, btn_clear, list_container, custom_btn_opt] {
            list_container->DetachAllChildren();

            for (size_t i = 0; i < m_combatants.size(); ++i) {
                auto btn_minus = Button("-", [this, i] { m_combatants[i].hp--; }, custom_btn_opt);
                auto btn_plus  = Button("+", [this, i] { m_combatants[i].hp++; }, custom_btn_opt);
                auto btn_del   = Button(" X ", [this, i] {
                    m_combatants.erase(m_combatants.begin() + i);
                    if (m_active_turn_index >= (int)m_combatants.size() && m_active_turn_index > 0) m_active_turn_index--;
                }, custom_btn_opt);

                auto row_comp = Container::Horizontal({ btn_minus, btn_plus, btn_del });
                bool is_active = (i == (size_t)m_active_turn_index);

                list_container->Add(Renderer(row_comp, [this, i, btn_minus, btn_plus, btn_del, is_active] {
                    return hbox({
                        // Indicator logic: Only show Cyan bar and arrow for the active player
                        is_active ? (text("┃") | color(Color::Cyan) | bold) : text(" "),
                        is_active ? (text(" > ") | color(Color::Cyan)) : text("   "),

                        text(m_combatants[i].name) | flex,
                        separator(),
                        text(" Init: " + m_combatants[i].roll) | dim,
                        separator(),
                        text(" HP: "),
                        btn_minus->Render() | color(Color::Red),
                        text(" " + std::to_string(m_combatants[i].hp) + " ") | size(WIDTH, EQUAL, 4) | hcenter,
                        btn_plus->Render() | color(Color::Green),
                        separator(),
                        btn_del->Render() | color(Color::Red)
                    }) | border; // Border color remains default for everyone
                }));
            }

            return vbox({
                text(" INITIATIVE & HP TRACKER ") | bold | hcenter,
                hbox({
                    text(" Name: "), input_name->Render() | border | flex,
                    text(" Init: "), input_roll->Render() | border | size(WIDTH, EQUAL, 6),
                    text(" HP: "),   input_hp->Render() | border | size(WIDTH, EQUAL, 6),
                    btn_add->Render() | border | color(Color::Green)
                }),
                hbox({
                    btn_sort->Render() | border | flex,
                    btn_next->Render() | border | flex | color(Color::Cyan),
                    btn_clear->Render() | border | flex
                }),
                separator(),
                (m_combatants.empty() ? text("No active combat...") | center | dim
                                     : list_container->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 15)) | flex
            });
        });
    }
};

#endif