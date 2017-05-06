
#include "MainWindow.hpp"

#include "AnvilChainsGetter.hpp"
#include "Config.hpp"
#include "Plans.hpp"
#include "Rule.hpp"
#include "ScanImage.hpp"
#include "get_last_file.hpp"
#include "png.hpp"
#include "utils.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Window.H>

#include <array>
#include <vector>

#undef Status // чёртов Xlib содержит макрос Status

struct MainWindow::Private {
    std::array<std::unique_ptr<Fl_PNG_Image>, static_cast<std::size_t>(RuleType::last)> m_rule_imgs;
    std::unique_ptr<Fl_Window> m_window;
    std::vector<Fl_Text_Display::Style_Table_Entry> m_styles;
    std::unique_ptr<Fl_Hold_Browser> m_plans_browser;
    std::unique_ptr<Fl_Box> m_rule1;
    std::unique_ptr<Fl_Box> m_rule2;
    std::unique_ptr<Fl_Box> m_rule3;
    Fl_Text_Buffer* m_chain_text;
    Fl_Text_Buffer* m_chain_style;

    Config m_config;
    Plans m_plans;

    static void windowCallback(Fl_Widget*, void*);
    static void planCallback(Fl_Widget*, void*);
    static void searchCallback(Fl_Widget*, void*);

    void changeRulesImagesAndTooltips(std::uint8_t rule1_index, std::uint8_t rule2_index, std::uint8_t rule3_index);
    void clearChain();

    Private();
};

MainWindow::Private::Private() {
    m_rule_imgs[to_under(RuleType::any)].reset(new Fl_PNG_Image("any.png", g_any_png.data(), g_any_png.size()));
    m_rule_imgs[to_under(RuleType::hit)].reset(new Fl_PNG_Image("hit.png", g_hit_png.data(), g_hit_png.size()));
    m_rule_imgs[to_under(RuleType::draw)].reset(new Fl_PNG_Image("draw.png", g_draw_png.data(), g_draw_png.size()));
    m_rule_imgs[to_under(RuleType::punch)].reset(new Fl_PNG_Image("punch.png", g_punch_png.data(), g_punch_png.size()));
    m_rule_imgs[to_under(RuleType::shrink)].reset(new Fl_PNG_Image("shrink.png", g_shrink_png.data(), g_shrink_png.size()));
    m_rule_imgs[to_under(RuleType::upset)].reset(new Fl_PNG_Image("upset.png", g_upset_png.data(), g_upset_png.size()));
    m_rule_imgs[to_under(RuleType::bend)].reset(new Fl_PNG_Image("bend.png", g_bend_png.data(), g_bend_png.size()));

#if defined(_WIN32) || defined(_WIN64)
    Fl::set_font(FL_SCREEN, "Courier New");
#endif

    m_styles = {
         // FONT COLOR      FONT FACE   FONT SIZE
         // --------------- ----------- --------------
         {  FL_RED,         FL_SCREEN, 18, 0 }, // A - Red
         {  FL_DARK_GREEN,  FL_SCREEN, 18, 0 }, // B - Green
     };

    m_window.reset(new Fl_Window(348, 108));
    m_window->label("Anvil calculator");
    m_window->icon(m_rule_imgs[to_under(RuleType::hit)].get());
    m_window->callback(&windowCallback);

    m_plans_browser.reset(new Fl_Hold_Browser(4, 4, 220, 68));
    m_plans_browser->callback(&planCallback, this);
    for(std::size_t i = 0; i < m_plans.getNumPlans(); ++i) {
        const Plan& plan = m_plans.getPlan(i);
        m_plans_browser->add(plan.m_item.c_str(), const_cast<Plan*>(&plan));
    }

    m_rule1.reset(new Fl_Box(239, 8, 26, 26));
    m_rule1->box(Fl_Boxtype::FL_THIN_DOWN_BOX);
    m_rule2.reset(new Fl_Box(276, 8, 26, 26));
    m_rule2->box(Fl_Boxtype::FL_THIN_DOWN_BOX);
    m_rule3.reset(new Fl_Box(313, 8, 26, 26));
    m_rule3->box(Fl_Boxtype::FL_THIN_DOWN_BOX);
    changeRulesImagesAndTooltips(g_rule_any, g_rule_any, g_rule_any);

    Fl_Return_Button* search = new Fl_Return_Button(234, 40, 110, 30, "Search");
    search->callback(&searchCallback, this);

    m_chain_text = new Fl_Text_Buffer();
    m_chain_text->canUndo(0);
    m_chain_style = new Fl_Text_Buffer();
    m_chain_style->canUndo(0);

    Fl_Text_Display* chain = new Fl_Text_Display(4, 74, 340, 30);
    chain->buffer(m_chain_text);
    chain->highlight_data(m_chain_style, m_styles.data(), m_styles.size(), 'A', 0, 0);

    if(m_config.getFail())
        m_chain_text->append("Can't load the config file");

    switch(m_plans.getStatus()) {
    case Plans::Status::cannot_open:
        m_chain_text->append("Can't load the file with plans");
        break;
    case Plans::Status::cannot_parse:
        m_chain_text->append("Can't parse the file with plans");
        break;
    }
}

void MainWindow::Private::changeRulesImagesAndTooltips(std::uint8_t rule1_index, std::uint8_t rule2_index, std::uint8_t rule3_index) {
    const Rule& rule1 = g_rules.at(rule1_index);
    const Rule& rule2 = g_rules.at(rule2_index);
    const Rule& rule3 = g_rules.at(rule3_index);

    m_rule1->image(m_rule_imgs[to_under(rule1.getType())].get());
    m_rule1->redraw();
    m_rule1->tooltip(rule1.getName().c_str());

    m_rule2->image(m_rule_imgs[to_under(rule2.getType())].get());
    m_rule2->redraw();
    m_rule2->tooltip(rule2.getName().c_str());

    m_rule3->image(m_rule_imgs[to_under(rule3.getType())].get());
    m_rule3->redraw();
    m_rule3->tooltip(rule3.getName().c_str());
}

void MainWindow::Private::windowCallback(Fl_Widget*, void*) {
    if(Fl::event() == FL_SHORTCUT && Fl::event_key() == FL_Escape)
        return;

    exit(0);
}

void MainWindow::Private::planCallback(Fl_Widget*, void* private_) {
    auto* p = static_cast<Private*>(private_);
    p->clearChain();

    int value = p->m_plans_browser->value();

    if(value == 0) {
        p->changeRulesImagesAndTooltips(g_rule_any, g_rule_any, g_rule_any);
        return;
    }

    const auto& plan = *reinterpret_cast<const Plan*>(p->m_plans_browser->data(value));
    p->changeRulesImagesAndTooltips(plan.m_rule1_index, plan.m_rule2_index, plan.m_rule3_index);
}

void MainWindow::Private::searchCallback(Fl_Widget*, void* private_) {
    auto* p = static_cast<Private*>(private_);

    p->clearChain();

    std::string screenshot;
    if(!get_last_file(p->m_config.getScreenshotsDir(), screenshot)) {
        p->m_chain_text->append("Can't find screenshot");
        return;
    }

    ScanImage si;
    si.setScale(p->m_config.getScale());
    si.openImage(screenshot);

    if(si.getStatus() != ScanImage::Status::ok) {
        switch(si.getStatus()) {
        case ScanImage::Status::cannot_open:
            p->m_chain_text->append("Can't open screenshot");
            return;
        case ScanImage::Status::is_not_anvil:
            p->m_chain_text->append("Can't parse screenshot");
            return;
        }
        p->m_chain_text->append("Error");
        return;
    }

    int red_score;
    int green_score;
    si.getScores(red_score, green_score);

    int plan_row = p->m_plans_browser->value();
    if (plan_row == 0)
        return;

    const auto& plan = *reinterpret_cast<const Plan*>(p->m_plans_browser->data(plan_row));
    std::uint8_t rule1 = plan.m_rule1_index;
    std::uint8_t rule2 = plan.m_rule2_index;
    std::uint8_t rule3 = plan.m_rule3_index;

    auto chain = AnvilChainsGetter::get({rule1, rule2, rule3}, green_score, red_score);
    if(chain.empty()) {
        p->m_chain_text->append("Nothing");
        return;
    }

    std::uint8_t last_technique = to_under(TechniqueType::last);
    std::size_t last_technique_counter = 0;

    auto flush_last_technique = [p, &last_technique, &last_technique_counter]() {
        if(last_technique_counter == 0)
            return;

        const Technique& technique = g_techniques.at(last_technique);

        std::string delim(p->m_chain_text->length() > 0, ' ');
        std::string text = delim + technique.m_name;
        if(last_technique_counter > 1)
            text += "×" + std::to_string(last_technique_counter);

        std::string style(text.size(), technique.m_red ? 'A' : 'B');

        p->m_chain_text->append(text.c_str());
        p->m_chain_style->append(style.c_str());

        last_technique_counter = 0;
    };

    for(std::uint8_t t : chain) {
        if(last_technique != t)
            flush_last_technique();

        ++last_technique_counter;
        last_technique = t;
    }

    flush_last_technique();
}

void MainWindow::Private::clearChain() {
    m_chain_style->remove(0, m_chain_style->length());
    m_chain_text->remove(0, m_chain_text->length());
}

MainWindow::MainWindow()
    : m_p(new Private()) {}

MainWindow::~MainWindow() = default;

int MainWindow::run(int argc, char **argv) {
    m_p->m_window->show(argc, argv);

    return Fl::run();
}
