#include "text_handler.h"

Text_handler *Text_handler::_instance = 0;
int Text_handler::counter = 1;

Text_handler::Text_handler()
{
    report_text = "";
}

Text_handler *Text_handler::instance()
{
    if( _instance == 0)
        _instance = new Text_handler();

    return _instance;
}

void Text_handler::set_mode(text_mode m)
{
    mode = m;
    if(mode == off_report)
        report_text = "\t\t РЕЗУЛЬТАТЫ ИМПОРТА СЛУЖЕБНЫХ ТЕЛЕФОНОВ \r\n \r\n";
    else if(mode == zk_report)
        report_text = "\t\t РЕЗУЛЬТАТЫ ИМПОРТА ЗАПИСНЫХ КНИГ \r\n \r\n";
}

QString Text_handler::get_text()
{
    return report_text;
}

void Text_handler::clear_text()
{
    report_text.clear();
    counter = 1;
}

//void Text_handler::set_save_import_text(Crud *cr_imp, Crud *cr_pg)
//{
//     report_text += "#"+QString::number(cr_imp->zk_id)+" - "+cr_imp->lastname+" "+cr_imp->name+" "+cr_imp->mid_name+
//             " импортирована, #"+QString::number(cr_pg->zk_id)+" - "+cr_pg->lastname+" "+cr_pg->name+" "+cr_pg->mid_name+
//             " удалена из базы\r\n";
//}

//void Text_handler::set_save_main_text(Crud *cr_imp, Crud *cr_pg)
//{
//    report_text += "#"+QString::number(cr_imp->zk_id)+" - "+cr_imp->lastname+" "+cr_imp->name+" "+cr_imp->mid_name+
//            " отброшена, #"+QString::number(cr_pg->zk_id)+" - "+cr_pg->lastname+" "+cr_pg->name+" "+cr_pg->mid_name+
//            " оставлена в базе\r\n";
//}

//void Text_handler::set_skip_All_text(Crud *cr_imp)
//{
//    report_text += "#"+QString::number(cr_imp->zk_id)+" - "+cr_imp->lastname+" "+cr_imp->name+" "+cr_imp->mid_name+
//            " отброшена \r\n";
//}

//void Text_handler::set_make_link_text(Crud *cr_imp, Crud *cr_pg)
//{
//    report_text += "#"+QString::number(cr_imp->zk_id)+" - "+cr_imp->lastname+" "+cr_imp->name+" "+cr_imp->mid_name+
//            " связана с #"+QString::number(cr_pg->zk_id)+" - "+cr_pg->lastname+" "+cr_pg->name+" "+cr_pg->mid_name+
//            "\r\n";
//}

//void Text_handler::set_successful_add_text(Crud *cr_imp)
//{
//    report_text += "#"+QString::number(cr_imp->zk_id)+" - "+cr_imp->lastname+" "+cr_imp->name+" "+cr_imp->mid_name+
//            " успешно добавлена \r\n";
//}

void Text_handler::set_successful_add_text(Off_tels *off_t)
{
    report_text += "#"+QString::number(off_t->of_t_id)+" - "+off_t->service_name+
            " успешно добавлен \r\n";
}

void Text_handler::set_zk_folder_line()
{
       report_text += "ИМПОРТ ИЗ СТАРОЙ ЗК #"+QString::number(counter)+"\r\n";
       counter++;
}

void Text_handler::set_off_t_del_import(Off_tels *off_t)
{
    report_text += "#"+QString::number(off_t->of_t_id)+" - "+off_t->service_name+
            " удален из базы \r\n";
}

void Text_handler::set_off_t_add_import(Off_tels *off_t)
{
    report_text += "#"+QString::number(off_t->of_t_id)+" - "+off_t->service_name+
            " успешно добавлен \r\n";
}

//void Text_handler::set_merge_text(Crud *cr_imp, Crud *cr_pg)
//{
//    report_text += "СЛИЯНИЕ #"+QString::number(cr_imp->zk_id)+" - "+cr_imp->lastname+" "+cr_imp->name+" "+cr_imp->mid_name+
//            "(основная) с #"+QString::number(cr_pg->zk_id)+" - "+cr_pg->lastname+" "+cr_pg->name+" "+cr_pg->mid_name+
//            "\r\n";
//}

