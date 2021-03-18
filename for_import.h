#ifndef FOR_IMPORT_H
#define FOR_IMPORT_H
#include "simplecrypt.h"
#include "enums.h"

class For_import
{
public:
    For_import();
    void openFile(QString filepath, QString password, bool isFolder);

};

#endif // FOR_IMPORT_H
