
#include "MainWindow.hpp"

#include "AnvilChainsGetter.hpp"
#include "Config.hpp"
#include "Rule.hpp"
#include "ScanImage.hpp"
#include "get_last_file.hpp"
#include "png.hpp"
#include "utils.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Multi_Label.H>
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
    std::vector<Fl_Menu_Item> m_rule_items;
    std::vector<Fl_Multi_Label> m_rule_labels;
    std::vector<Fl_Text_Display::Style_Table_Entry> m_styles;
    std::unique_ptr<Fl_Choice> m_rule1;
    std::unique_ptr<Fl_Choice> m_rule2;
    std::unique_ptr<Fl_Choice> m_rule3;
    Fl_Text_Buffer* m_chain_text;
    Fl_Text_Buffer* m_chain_style;

    Config m_config;

    static void windowCallback(Fl_Widget*, void*);
    static void ruleCallback(Fl_Widget*, void*);
    static void searchCallback(Fl_Widget*, void*);

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

    m_window.reset(new Fl_Window(348,140));
    m_window->label("Anvil calculator");
    m_window->icon(m_rule_imgs[to_under(RuleType::hit)].get());
    m_window->callback(&windowCallback);

    m_rule_items.reserve(g_rules.size());
    m_rule_labels.reserve(g_rules.size());

    for(const auto& rule : g_rules) {
        m_rule_labels.emplace_back();
        m_rule_items.emplace_back();

        Fl_Menu_Item* item = &m_rule_items.back();
        Fl_Multi_Label* label = &m_rule_labels.back();
        Fl_PNG_Image* image = m_rule_imgs[to_under(rule.getType())].get();

        label->labela = reinterpret_cast<const char*>(image);
        label->typea = _FL_IMAGE_LABEL;
        label->labelb = rule.getName().c_str();
        label->typeb = FL_NORMAL_LABEL;

        item->image(image);
        label->label(item);
    }

    // terminator
    m_rule_items.emplace_back();

    m_rule1.reset(new Fl_Choice(4, 4, 220, 30));
    m_rule1->copy(m_rule_items.data());
    m_rule1->callback(&ruleCallback, this);

    m_rule2.reset(new Fl_Choice(4, 38, 220, 30));
    m_rule2->copy(m_rule_items.data());
    m_rule2->callback(&ruleCallback, this);

    m_rule3.reset(new Fl_Choice(4, 72, 220, 30));
    m_rule3->copy(m_rule_items.data());
    m_rule3->callback(&ruleCallback, this);

    Fl_Return_Button* search = new Fl_Return_Button(234, 72, 110, 30, "Search");
    search->callback(&searchCallback, this);

    m_chain_text = new Fl_Text_Buffer();
    m_chain_text->canUndo(0);
    m_chain_style = new Fl_Text_Buffer();
    m_chain_style->canUndo(0);

    Fl_Text_Display* chain = new Fl_Text_Display(4, 106, 340, 30);
    chain->buffer(m_chain_text);
    chain->highlight_data(m_chain_style, m_styles.data(), m_styles.size(), 'A', 0, 0);
}

void MainWindow::Private::windowCallback(Fl_Widget*, void*) {
    if (Fl::event() == FL_SHORTCUT && Fl::event_key() == FL_Escape)
        return;

    exit(0);
}

void MainWindow::Private::ruleCallback(Fl_Widget*, void* private_) {
    auto* p = static_cast<Private*>(private_);
    p->clearChain();
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

    std::uint8_t rule1 = p->m_rule1->value();
    std::uint8_t rule2 = p->m_rule2->value();
    std::uint8_t rule3 = p->m_rule3->value();

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
