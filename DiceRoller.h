#ifndef DM_HANDY_DICEROLLER_H
#define DM_HANDY_DICEROLLER_H

#include <vector>
#include <string>
#include <random>
#include <numeric>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

class DiceRoller {
private:
    std::vector<int> m_history;
    std::string m_lastRollMsg = "Roll a die!";
    std::mt19937 m_gen;

public:
    DiceRoller() : m_gen(std::random_device{}()) {}

    void roll(int sides) {
        std::uniform_int_distribution<> dis(1, sides);
        int result = dis(m_gen);
        m_history.push_back(result);
        if (m_history.size() > 10) m_history.erase(m_history.begin());
        m_lastRollMsg = "Rolled d" + std::to_string(sides) + ": " + std::to_string(result);
    }

    ftxui::Component GetComponent() {
        using namespace ftxui;
        auto container = Container::Vertical({
            Container::Horizontal({
                Button(" d4 ",  [this]{ roll(4); }),
                Button(" d6 ",  [this]{ roll(6); }),
                Button(" d8 ",  [this]{ roll(8); }),
                Button(" d10 ", [this]{ roll(10); }),
                Button(" d12 ", [this]{ roll(12); }),
                Button(" d20 ", [this]{ roll(20); }),
                Button(" d100 ",[this]{ roll(100); }),
            }),
            Button(" Clear History ", [this]{ m_history.clear(); m_lastRollMsg = "History cleared."; })
        });

        return Renderer(container, [this, container] {
            int total = std::accumulate(m_history.begin(), m_history.end(), 0);
            ftxui::Elements history_view;
            for (int r : m_history) history_view.push_back(text(std::to_string(r)) | border | color(Color::Yellow));

            return vbox({
                text(" DICE STATION ") | bold | hcenter,
                container->Render() | hcenter,
                separator(),
                text(m_lastRollMsg) | center | color(Color::Cyan) | size(HEIGHT, EQUAL, 3),
                separator(),
                hbox({ text(" History: "), hbox(history_view) }) | hcenter,
                text(" TOTAL SUM: " + std::to_string(total)) | bold | hcenter | color(Color::Green)
            });
        });
    }
};

#endif