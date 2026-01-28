#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include "CharacterManager.h"
#include "DiceRoller.h"
#include "InitiativeTracker.h"

using namespace ftxui;

int main() {
    auto screen = ScreenInteractive::Fullscreen();

    CharacterManager charManager;
    DiceRoller diceRoller;
    InitiativeTracker initiative; // Our new class

    int selected_tab = 0;
    std::vector<std::string> tab_titles = {"Characters", "Dice", "Initiative"};

    auto char_comp = charManager.GetComponent();
    auto dice_comp = diceRoller.GetComponent();
    auto init_comp = initiative.GetComponent();

    auto main_tabs = Container::Tab({ char_comp, dice_comp, init_comp }, &selected_tab);
    auto nav_menu = Menu(&tab_titles, &selected_tab);

    auto main_container = Container::Vertical({
        nav_menu,
        main_tabs,
        Button(" QUIT ", screen.ExitLoopClosure())
    });

    auto renderer = Renderer(main_container, [&] {
        return vbox({
            text(" DM HANDY TOOL ") | bold | center | border,
            nav_menu->Render() | hcenter,
            separator(),
            main_tabs->Render() | flex
        }) | center | size(WIDTH, LESS_THAN, 110) | border;
    });

    screen.Loop(renderer);
    return 0;
}