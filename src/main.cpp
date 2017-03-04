
#include "AnvilChainsGetter.hpp"
#include "MainWindow.hpp"

/*
#include <FL/Fl.h>
#include <FL/Fl_PNG_Image.h>
#include <FL/Fl_Multi_Label.h>
#include <FL/Fl_Window.h>
#include <FL/Fl_Choice.h>
#include <FL/Fl_Box.h>

*/

int main(int argc, char **argv) {
    //AnvilChainsGetter chains_getter;
    //print_chain(chains_getter.get({2, 2, 2}, 149));

    /*
    Fl_PNG_Image img1("img1.png");

    Fl_Multi_Label label;
    label.labela = reinterpret_cast<const char*>(&img1);
    label.typea = _FL_IMAGE_LABEL;

    label.labelb = "punch";
    label.typeb = FL_NORMAL_LABEL;

    Fl_Menu_Item menu_items[] = {{0}, {0}};
    menu_items[0].image(&img1);
    label.label(&menu_items[0]);

    Fl_Window *window = new Fl_Window(340,180);
    window->label("Anvil calcualtor");

    Fl_Choice *choice = new Fl_Choice(0, 0, 120, 32);
    choice->copy(menu_items);

    //Fl_Box *box = new Fl_Box(20,40,20,20);
    //box->image(&img1);

    window->show(argc, argv);
    */

    MainWindow window;
    return window.run(argc, argv);
}

/*
int main(int argc, char **argv) {
  Fl_Window *window = new Fl_Window(340,180);

  Fl_Box *box = new Fl_Box(20,40,300,100,u8"кошка 日本国");
  window->add_resizable(*box);
  box->box(FL_UP_BOX);
  box->labelfont(FL_BOLD+FL_ITALIC);
  box->labelsize(36);
  box->labeltype(FL_SHADOW_LABEL);
  Fl::set_font(0, "Source Han Sans CN");
  box->labelfont(0);

  window->end();
  window->show(argc, argv);

  return Fl::run();
}
*/
