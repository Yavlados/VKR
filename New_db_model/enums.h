#ifndef ENUMS_H
#define ENUMS_H
enum DbState { IsReaded = 1,        ///< считан из БД
               IsNewing = 0,        ///< новый, еще не записан в БД
               IsRemoved = -1 ,     ///< удален в ПО, еще не удален в БД
               IsChanged = 2};       ///< считан из БД и изменен в ПО
enum ExportType {
    events = 0,
    official,
    eventsPassword,
    officialPassword
};
enum CheckStateEvent {  Checked_ =1,
                        Unchecked_ = 0};

enum importType {
    events_,
    eventsPassword_,
    official_,
    officialPassword_,
    oldMultiple_,
    oldFolder_
};

#endif // ENUMS_H
