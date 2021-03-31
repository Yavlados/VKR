#ifndef TEXT_HANDLER_H
#define TEXT_HANDLER_H
#include "_Off_tels.h"
#include <QString>

enum text_mode {
    zk_report,
    off_report
};

class Text_handler
{
public:
     ~Text_handler();

     static Text_handler *instance();

     void set_mode(text_mode m);
      QString get_text();
     void clear_text();

//     void set_save_import_text(Crud *cr_imp, Crud *cr_pg);
//     void set_save_main_text(Crud *cr_imp, Crud *cr_pg);
//     void set_skip_All_text(Crud *cr_imp);
//     void set_make_link_text(Crud *cr_imp, Crud *cr_pg);
//     void set_successful_add_text(Crud *cr_imp);
     void set_successful_add_text(Off_tels *off_t);
     void set_zk_folder_line();
     void set_off_t_del_import(Off_tels *off_t);
     void set_off_t_add_import(Off_tels *off_t);
//     void set_merge_text(Crud *cr_imp, Crud *cr_pg);
private:
    Text_handler();
    static int counter;
    static Text_handler *_instance;
     QString report_text;
    text_mode mode;

};
#endif // TEXT_HANDLER_H
