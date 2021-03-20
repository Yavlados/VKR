#include "Util.h"

Util *Util::_instance=0;

Util::~Util()
{

}

Util *Util::instance()
{
    if(_instance==0){
        _instance = new Util();
    }

    return _instance;
}

Util::Util()
{
    this->_editPersonList   = 0;
    this->_editEventList    = 0;
    this->_addEventManager   = 0;
}

void Util::clearLayout(QLayout *layout)
{
    int size = layout->count();
    for(int i=0; i< size; i++){
        QLayoutItem *witem = layout->itemAt(0);
        delete witem->widget();
    }
}

QList<EditPerson *> *Util::editPersonList()
{
    if( this->_editPersonList==0){
        this->_editPersonList = new QList<EditPerson*>;
    }

    return this->_editPersonList;
}

QList<editEvent *> *Util::editEventList()
{
    if( this->_editEventList==0){
        this->_editEventList = new QList<editEvent*>;
    }

    return this->_editEventList;
}

QList<AddEventManager *> *Util::addEventManager()
{
    if( this->_addEventManager==0){
        this->_addEventManager = new QList<AddEventManager *>;
    }

    return this->_addEventManager;
}

void Util::linkAddEventPerson(editEvent *ee, EditPerson *ep)
{
    bool fromList = false;
    AddEventManager *aem = 0;
    for( int a=0; a<this->addEventManager()->size(); a++){
        AddEventManager *local = this->addEventManager()->at(a);
        if(local->parent->localEvent->id == ee->localEvent->id){
            aem = local;
            fromList = true;
            break;
        }
    }

    if(aem==0)
        aem = new AddEventManager();

    aem->parent = ee;
    aem->childs->append(ep);

    if(!fromList) this->addEventManager()->append(aem);
}

void Util::unlinkAddEventPerson(editEvent *ee)
{
    QString id = ee->localEvent->id;
    for(int i=0; i<this->addEventManager()->size(); i++){
        AddEventManager *aem = this->addEventManager()->at(i);
        if(aem->parent->localEvent->id == id){
            this->addEventManager()->removeAt(i);
            return;
        }
    }
}

editEvent *Util::getManagerParent(EditPerson *ep)
{
    int i = this->addEventManager()->size();
    for(int a=0; a<i; a++){
        AddEventManager *local = this->addEventManager()->at(a);
        int b = local->childs->size();

        for( int c=0; c<b; c++){
            EditPerson *child = local->childs->at(c);

            if(child->person->id == ep->person->id){

                local->childs->removeAt(c);
                return local->parent;
            }
        }
    }
}


QString Util::convertKey(QString key)
{
    QString convertedKey = "";
    // text key to num convertation
    for (int a =0; a < key.size(); a++) {
           QChar ch = key[a];
           auto un = ch.unicode();
           convertedKey += QString::number(un);
    }
    return convertedKey;
}


AddEventManager::AddEventManager()
{
    this->childs = new QList<EditPerson*>;
    this->parent = new editEvent();
}
