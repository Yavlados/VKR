#include "for_analysis.h"

For_analysis::For_analysis()
{
    list = new List_master(Analysis);
    this->v1Result = 0;
    this->v2Result = 0;
    this->v3Result = 0;
}

void For_analysis::ClearAll()
{
    temp_str_for_num.clear();

    temp_str_for_cont_num.clear();

    analysis_res.clear();

    match_counter.clear();
}

void For_analysis::getDataFromBaseFace(int eventId, QVector<int> *eventIdList, QMap<QString, QString> *period)
{
    this->analysisResult.clear();
    this->v1Result = this->getV1(eventId, eventIdList, period);
    this->v2Result = this->getV2(eventId, eventIdList, period);
    this->v3Result = this->getV3(eventId, eventIdList, period);

    for(int i=0; i< this->v1Result->size(); i++ ){
        Analysis_V1 *v =  this->v1Result->at(i);
        QString fio = v->from_lastname + " " + v->from_name + " " + v->from_midname;
        QList<Analysis_general> b = this->analysisResult[fio];
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        b.append(*obj);
        this->analysisResult[fio] = b;
    }
    for(int i=0; i< this->v2Result->size(); i++ ){
        Analysis_V2 *v =  this->v2Result->at(i);
        QString fio = v->from_lastname + " " + v->from_name + " " + v->from_midname;
        QList<Analysis_general> b = this->analysisResult[fio];
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        b.append(*obj);
        this->analysisResult[fio] = b;
    }
    for(int i=0; i< this->v3Result->size(); i++ ){
        Analysis_V3 *v =  this->v3Result->at(i);
        QString fio = v->from_lastname + " " + v->from_name + " " + v->from_midname;
        QList<Analysis_general> b = this->analysisResult[fio];
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        b.append(*obj);
        this->analysisResult[fio] = b;
    }
}

void For_analysis::getDataFromBaseTelephone(int eventId, QVector<int> *eventIdList, QMap<QString, QString> *period)
{
    this->analysisResult.clear();
    this->v1Result = this->getV1(eventId, eventIdList, period);
    this->v2Result = this->getV2(eventId, eventIdList, period);
    this->v3Result = this->getV3(eventId, eventIdList, period);

    for(int i=0; i< this->v1Result->size(); i++ ){
        Analysis_V1 *v =  this->v1Result->at(i);
        QString telephone = v->from_telephone + ";"+v->from_lastname + " " + v->from_name + " " + v->from_midname;
        QList<Analysis_general> b = this->analysisResult[telephone];
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        b.append(*obj);
        this->analysisResult[telephone] = b;
    }
    for(int i=0; i< this->v2Result->size(); i++ ){
        Analysis_V2 *v =  this->v2Result->at(i);
        QString telephone = v->from_telephone + ";"+v->from_lastname + " " + v->from_name + " " + v->from_midname;
        QList<Analysis_general> b = this->analysisResult[telephone];
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        b.append(*obj);
        this->analysisResult[telephone] = b;
    }
    for(int i=0; i< this->v3Result->size(); i++ ){
        Analysis_V3 *v =  this->v3Result->at(i);
        QString telephone = v->from_telephone + ";"+v->from_lastname + " " + v->from_name + " " + v->from_midname;
        QList<Analysis_general> b = this->analysisResult[telephone];
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        b.append(*obj);
        this->analysisResult[telephone] = b;
    }
}

void For_analysis::getDataFromBaseFullFace(int eventId, QVector<int> *eventIdList, QMap<QString, QString> *period)
{
    this->analysisResultFull.clear();
    this->v1Result = this->getV1(eventId, eventIdList, period);
    this->v2Result = this->getV2(eventId, eventIdList, period);
    this->v3Result = this->getV3(eventId, eventIdList, period);

    for (int i=0; i< this->v1Result->size(); i++ ) {
        Analysis_V1 *v =  this->v1Result->at(i);
        QString fio = v->from_lastname + " " + v->from_name + " " + v->from_midname;
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        this->analysisResultFull[fio]["var1"].append(*obj);
    }

    for (int i=0; i< this->v2Result->size(); i++ ) {
        Analysis_V2 *v =  this->v2Result->at(i);
        QString fio = v->from_lastname + " " + v->from_name + " " + v->from_midname;
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        this->analysisResultFull[fio]["var2"].append(*obj);
    }

    for (int i=0; i< this->v3Result->size(); i++ ) {
        Analysis_V3 *v =  this->v3Result->at(i);
        QString fio = v->from_lastname + " " + v->from_name + " " + v->from_midname;
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        this->analysisResultFull[fio]["var3"].append(*obj);
    }
}

void For_analysis::getDataFromBaseFullTelephone(int eventId, QVector<int> *eventIdList, QMap<QString, QString> *period)
{
    this->analysisResultFull.clear();
    this->v1Result = this->getV1(eventId, eventIdList, period);
    this->v2Result = this->getV2(eventId, eventIdList, period);
    this->v3Result = this->getV3(eventId, eventIdList, period);

    for (int i=0; i< this->v1Result->size(); i++ ) {
        Analysis_V1 *v =  this->v1Result->at(i);
        QString telephone = v->from_telephone + ";"+v->from_lastname + " " + v->from_name + " " + v->from_midname;
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        this->analysisResultFull[telephone]["var1"].append(*obj);
    }

    for (int i=0; i< this->v2Result->size(); i++ ) {
        Analysis_V2 *v =  this->v2Result->at(i);
        QString telephone = v->from_telephone + ";"+v->from_lastname + " " + v->from_name + " " + v->from_midname;
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        this->analysisResultFull[telephone]["var2"].append(*obj);
    }

    for (int i=0; i< this->v3Result->size(); i++ ) {
        Analysis_V3 *v =  this->v3Result->at(i);
        QString telephone = v->from_telephone + ";"+v->from_lastname + " " + v->from_name + " " + v->from_midname;
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        this->analysisResultFull[telephone]["var3"].append(*obj);
    }
}

QList<Analysis_V1*> *For_analysis::getV1(int eventId, QVector<int> *eventIdList, QMap<QString, QString> *period)
{
    if( !db_connection::instance()->db_connect() )
        return 0;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    if(eventIdList != 0){
        temp.prepare(AnalysisModels::instance()->v1(QString::number(eventId), eventIdList));
    }
    else if(period != 0){
        temp.prepare(AnalysisModels::instance()->v1(QString::number(eventId), period));
    }else {
        temp.prepare(AnalysisModels::instance()->v1(QString::number(eventId)));
    }

    if (!temp.exec())
    {
        db_connection::instance()->lastError = "For_analysis::getV1 " + temp.lastError().text();

        qDebug() << "For_analysis::getV1" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();

        return 0;
    }

    QList<Analysis_V1*> *v1Result = new QList<Analysis_V1*>;

    while(temp.next()){
        Analysis_V1 *v1 = new Analysis_V1();
        v1->from_event_id 					= temp.value(0).toString()  ;
        v1->from_event_category 			= temp.value(1).toString()  ;
        v1->from_event_detention_date 		= temp.value(2).toString()  ;
        v1->from_event_detention_time 		= temp.value(3).toString()  ;
        v1->from_event_detention_reason 	= temp.value(4).toString()  ;
        v1->from_event_detention_by 		= temp.value(5).toString()  ;
        v1->from_event_keeping_place 		= temp.value(6).toString()  ;
        v1->from_event_keeping_additional 	= temp.value(7).toString()  ;
        v1->from_name 						= temp.value(8).toString()  ;
        v1->from_lastname 					= temp.value(9).toString()  ;
        v1->from_midname 					= temp.value(10).toString() ;
        v1->from_telephone 					= temp.value(11).toString() ;
        v1->from_telephone_oldnum 			= temp.value(12).toBool()   ;
        v1->from_telephone_internum 		= temp.value(13).toBool()   ;
        v1->to_contact 						= temp.value(14).toString() ;
        v1->to_contact_alias 				= temp.value(15).toString() ;
        v1->to_contact_oldnum 				= temp.value(16).toBool()   ;
        v1->to_contact_internum 			= temp.value(17).toBool()   ;
        v1->to_telephone 					= temp.value(18).toString() ;
        v1->to_telephone_oldnum 			= temp.value(19).toBool()   ;
        v1->to_telephone_internum 			= temp.value(20).toBool()   ;
        v1->to_name 						= temp.value(21).toString() ;
        v1->to_lastname 					= temp.value(22).toString() ;
        v1->to_midname 						= temp.value(23).toString() ;
        v1->to_event_category 				= temp.value(24).toString() ;
        v1->to_event_detention_date 		= temp.value(25).toString() ;
        v1->to_event_detention_time 		= temp.value(26).toString() ;
        v1->to_event_detention_reason 		= temp.value(27).toString() ;
        v1->to_event_detention_by 			= temp.value(28).toString() ;
        v1->to_event_keeping_place 			= temp.value(29).toString() ;
        v1->to_event_keeping_additional 	= temp.value(30).toString() ;
        v1->to_event_id  					= temp.value(31).toString() ;
        v1Result->append(v1);
    }
    db_connection::instance()->db().database(cname).commit();
    return v1Result;
}

QList<Analysis_V2*> *For_analysis::getV2(int eventId, QVector<int> *eventIdList,QMap<QString, QString> *period)
{
    if( !db_connection::instance()->db_connect() )
        return 0;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    if(eventIdList != 0){
        temp.prepare(AnalysisModels::instance()->v2(QString::number(eventId), eventIdList));
    }
    else if(period != 0){
        temp.prepare(AnalysisModels::instance()->v2(QString::number(eventId), period));
    }else {
        temp.prepare(AnalysisModels::instance()->v2(QString::number(eventId)));
    }

    if (!temp.exec())
    {
        db_connection::instance()->lastError = "For_analysis::getV2 " + temp.lastError().text();

        qDebug() << "For_analysis::getV2" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();

        return 0;
    }

    QList<Analysis_V2*> *v2Result = new QList<Analysis_V2*>;

    while(temp.next()){
        Analysis_V2 *v2 = new Analysis_V2();
        v2->from_event_id                   = temp.value(0).toString()  ;
        v2->from_event_category             = temp.value(1).toString()  ;
        v2->from_event_detention_date       = temp.value(2).toString()  ;
        v2->from_event_detention_time       = temp.value(3).toString()  ;
        v2->from_event_detention_reason     = temp.value(4).toString()  ;
        v2->from_event_detention_by         = temp.value(5).toString()  ;
        v2->from_event_keeping_place        = temp.value(6).toString()  ;
        v2->from_event_keeping_additional   = temp.value(7).toString()  ;
        v2->from_name                       = temp.value(8).toString()  ;
        v2->from_lastname                   = temp.value(9).toString()  ;
        v2->from_midname                    = temp.value(10).toString() ;
        v2->from_telephone                  = temp.value(11).toString() ;
        v2->from_telephone_oldnum           = temp.value(12).toBool()   ;
        v2->from_telephone_internum         = temp.value(13).toBool()   ;
        v2->from_contact                    = temp.value(14).toString() ;
        v2->from_contact_alias              = temp.value(15).toString() ;
        v2->from_contact_oldnum             = temp.value(16).toBool()   ;
        v2->from_contact_internum           = temp.value(17).toBool()   ;
        v2->to_telephone                    = temp.value(18).toString() ;
        v2->to_telephone_oldnum             = temp.value(19).toBool()   ;
        v2->to_telephone_internum           = temp.value(20).toBool()   ;
        v2->to_name                         = temp.value(21).toString() ;
        v2->to_lastname                     = temp.value(22).toString() ;
        v2->to_midname                      = temp.value(23).toString() ;
        v2->to_event_category               = temp.value(24).toString() ;
        v2->to_event_detention_date         = temp.value(25).toString() ;
        v2->to_event_detention_time         = temp.value(26).toString() ;
        v2->to_event_detention_reason       = temp.value(27).toString() ;
        v2->to_event_detention_by           = temp.value(28).toString() ;
        v2->to_event_keeping_place          = temp.value(29).toString() ;
        v2->to_event_keeping_additional     = temp.value(30).toString() ;
        v2->to_event_id                     = temp.value(31).toString() ;
        v2Result->append(v2);
    }
    db_connection::instance()->db().database(cname).commit();

    return v2Result;
}

QList<Analysis_V3 *> *For_analysis::getV3(int eventId, QVector<int> *eventIdList,QMap<QString, QString> *period)
{
    if( !db_connection::instance()->db_connect() )
        return 0;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    if(eventIdList != 0){
        temp.prepare(AnalysisModels::instance()->v3(QString::number(eventId), eventIdList));
    }
    else if(period != 0){
        temp.prepare(AnalysisModels::instance()->v3(QString::number(eventId), period));
    }else {
        temp.prepare(AnalysisModels::instance()->v3(QString::number(eventId)));
    }

    if (!temp.exec())
    {
        db_connection::instance()->lastError = "For_analysis::getV3 " + temp.lastError().text();

        qDebug() << "For_analysis::getV3" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();

        return 0;
    }

    QList<Analysis_V3*> *v3Result = new QList<Analysis_V3*>;

    while(temp.next()){
        Analysis_V3 *v3 = new Analysis_V3();
        v3->from_event_id                   = temp.value(0).toString()  ;
        v3->from_event_category             = temp.value(1).toString()  ;
        v3->from_event_detention_date       = temp.value(2).toString()  ;
        v3->from_event_detention_time       = temp.value(3).toString()  ;
        v3->from_event_detention_reason     = temp.value(4).toString()  ;
        v3->from_event_detention_by         = temp.value(5).toString()  ;
        v3->from_event_keeping_place        = temp.value(6).toString()  ;
        v3->from_event_keeping_additional   = temp.value(7).toString()  ;
        v3->from_name                       = temp.value(8).toString()  ;
        v3->from_lastname                   = temp.value(9).toString()  ;
        v3->from_midname                    = temp.value(10).toString() ;
        v3->from_telephone                  = temp.value(11).toString() ;
        v3->from_telephone_oldnum           = temp.value(12).toBool()   ;
        v3->from_telephone_internum         = temp.value(13).toBool()   ;
        v3->from_contact                    = temp.value(14).toString() ;
        v3->from_contact_alias              = temp.value(15).toString() ;
        v3->from_contact_oldnum             = temp.value(16).toBool()   ;
        v3->from_contact_internum           = temp.value(17).toBool()   ;
        v3->to_contact                      = temp.value(18).toString() ;
        v3->to_contact_alias                = temp.value(19).toString() ;
        v3->to_contact_oldnum               = temp.value(20).toBool()   ;
        v3->to_contact_internum             = temp.value(21).toBool()   ;
        v3->to_telephone                    = temp.value(22).toString() ;
        v3->to_telephone_oldnum             = temp.value(23).toBool() ;
        v3->to_telephone_internum           = temp.value(24).toBool() ;
        v3->to_name                         = temp.value(25).toString() ;
        v3->to_lastname                     = temp.value(26).toString() ;
        v3->to_midname                      = temp.value(27).toString() ;
        v3->to_event_category               = temp.value(28).toString() ;
        v3->to_event_detention_date         = temp.value(29).toString() ;
        v3->to_event_detention_time         = temp.value(30).toString() ;
        v3->to_event_detention_reason       = temp.value(31).toString() ;

        v3->to_event_detention_by           = temp.value(32).toString()  ;
        v3->to_event_keeping_place          = temp.value(33).toString()  ;
        v3->to_event_keeping_additional     = temp.value(34).toString()  ;
        v3->to_event_id                     = temp.value(35).toString()  ;
        v3Result->append(v3);
    }
    db_connection::instance()->db().database(cname).commit();

    return v3Result;
}

void For_analysis::short_face_analysis(int eventId)
{
    this->analysis_res +="\t КРАТКАЯ СПРАВКА ПРИВЯЗКА К ЛИЦАМ СОБЫТИЕ #"+QString::number(eventId)+" \r\n";
    for(int i=0; i<this->analysisResult.keys().size(); i++ ){
        QString fio = this->analysisResult.keys().at(i);
        QList<Analysis_general> list = this->analysisResult[fio];
        this->analysis_res +=  "\r\nФигурант события #"+QString::number(eventId)+" " +
                fio+" возможно знаком";
                if(list.size()>1)
                    this->analysis_res+=" со следующими лицами: \r\n \r\n";
                else
                    this->analysis_res += " со следующим лицом: \r\n \r\n";
        for(int a=0; a<list.size();a++){
            Analysis_general res = list.at(a);
            this->analysis_res += "\t#"+res.to_event_id+" "+res.to_lastname +" "+res.to_name +" "+ res.to_midname + " "+
                        ", окраска ???, номер события: "+res.to_event_id+", дата добавления: "+
                        res.to_event_detention_date+" "+ res.to_event_detention_time+";\r\n";
        }
    }
}

void For_analysis::short_tel_analysis(int eventId)
{
    this->analysis_res +="\t КРАТКАЯ СПРАВКА ПРИВЯЗКА К ТЕЛЕФОНАМ СОБЫТИЕ #"+QString::number(eventId)+" \r\n";
     for(int i=0; i<this->analysisResult.keys().size(); i++ ){
         QString key = this->analysisResult.keys().at(i);
         auto metaData = key.split(';');
         QList<Analysis_general> list = this->analysisResult[key];
         this->analysis_res +=  "\r\nНомер телефона " +metaData.at(0)+", принадлежащий владельцу записной книжки " +
                 metaData.at(1) + "обнаружен в ";
                 if(list.size()>1)
                     this->analysis_res+=" записных книжках: \r\n \r\n";
                 else
                     this->analysis_res += " записной книжке: \r\n \r\n";
          for(int a=0; a<list.size();a++){
              Analysis_general res = list.at(a);
              this->analysis_res += "\t #"+ res.to_event_id + " сформированной " + res.to_event_detention_date + " " + res.to_event_detention_time+
                      " в отношении: "+ res.to_lastname + " " + res.to_name + " " + res.to_midname + ", по окраске: ???, в интересах "+
                      res.to_event_keeping_place;
              if(res.to_event_keeping_additional != "")
                  this->analysis_res +=" с пометкой " + res.to_event_keeping_additional;

              this->analysis_res +="\r\n";
          }
     }
}

void For_analysis::long_face_analysis(int eventId)
{
    this->analysis_res +="\t ПОЛНАЯ СПРАВКА ПРИВЯЗКА К ЛИЦАМ СОБЫТИЕ #"+QString::number(eventId)+" ";
    for(int i =0; i<this->analysisResultFull.keys().size(); i++){
        QString fio = this->analysisResultFull.keys().at(i);
        QMap<QString, QList<Analysis_general> > map = this->analysisResultFull[fio];
        this->analysis_res +=  "\r\n\r\nФигурант события " +fio+" возможно знаком:";

        QList<Analysis_general> var1List = map["var1"];
        if(var1List.size() > 0){
            this->analysis_res += "\r\n 1 ВАРИАНТ: ";
            for(int a=0; a<var1List.size();a++){
                Analysis_general res = var1List.at(a);

                this->analysis_res += " \r\n \t ЗК#"+res.to_event_id +" "  + " Номер телефона"+
                        res.from_telephone + " ";
                if( res.from_telephone_oldnum || res.from_telephone_internum ){
                    this->analysis_res += "(";

                    if( res.from_telephone_oldnum && res.from_telephone_internum )
                        this->analysis_res += "старый международный";

                    else if( res.from_telephone_oldnum )
                        this->analysis_res += "старый";

                    else if( res.from_telephone_internum )
                        this->analysis_res += "международный";

                    this->analysis_res += ")";
                }

                this->analysis_res += ", принадлежащий "+fio +
                        ", был обнаружен в списке телефонных контактов "+ res.to_lastname +" "+
                        res.to_name +" "+ res.to_midname+" с пометкой "+res.to_contact_alias;
            }
        }
        QList<Analysis_general> var2List = map["var2"];
        if(var2List.size() > 0){
            this->analysis_res += "\r\n 2 ВАРИАНТ: ";
            for(int a=0; a<var2List.size();a++){
              Analysis_general res = var2List.at(a);
              this->analysis_res += " \r\n \t ЗК#" + res.to_event_id +" телефонный номер " +
                      "списка контактов "+
                      res.from_contact +", с пометкой " + res.from_contact_alias + " в записной книжке, "+
                      "владельцем которой является " + fio + " обнаружен, как "+
                      "принадлежащий владельцу записной книжки: "+ res.to_lastname +" "+
                      res.to_name +" "+ res.to_midname + "; \r\n";
            }
        }

        QList<Analysis_general> var3List = map["var3"];
        if(var3List.size() > 0){
            this->analysis_res += "\r\n 3 ВАРИАНТ: ";
            for(int a=0; a<var3List.size();a++){
                Analysis_general res = var3List.at(a);
                this->analysis_res += " \r\n \t ЗК#"+res.to_event_id;
                this->analysis_res += " Номер телефона "+ res.to_contact + " с пометкой "
                        + res.from_contact_alias +", обнаруженный в списке контактов "
                        + fio + " был также обнаружен в списке контактов с пометкой "+
                        res.to_contact_alias + ", владельцем которого является "+
                        res.to_lastname +" "+ res.to_name +" "+ res.to_midname;
            }
        }
    }
}

void For_analysis::long_tel_analysis(int eventId)
{
    this->analysis_res +="\t ПОЛНАЯ СПРАВКА ПРИВЯЗКА К НОМЕРАМ СОБЫТИЕ #"+QString::number(eventId)+" ";
     for(int i =0; i<this->analysisResultFull.keys().size(); i++){
         QString key = this->analysisResultFull.keys().at(i);
         auto metaData = key.split(';');
         this->analysis_res += "\r\n\r\nНомер телефона "+metaData.at(0) +", в записной книжке, владельцем которой является  "+
                 metaData.at(1) + ", обнаружен среди записей: ";

         QMap<QString, QList<Analysis_general> > map = this->analysisResultFull[key];
         QList<Analysis_general> var1List = map["var1"];
         if(var1List.size() > 0){
             this->analysis_res += "\r\n 1 ВАРИАНТ: ";
             for(int a=0; a<var1List.size();a++){
                 Analysis_general res = var1List.at(a);

                 this->analysis_res += " \r\n \t ЗК#"+res.to_event_id +" "  + "Номер телефона"+
                         res.from_telephone + " ";
                 if( res.from_telephone_oldnum || res.from_telephone_internum ){
                     this->analysis_res += "(";

                     if( res.from_telephone_oldnum && res.from_telephone_internum )
                         this->analysis_res += "старый международный";

                     else if( res.from_telephone_oldnum )
                         this->analysis_res += "старый";

                     else if( res.from_telephone_internum )
                         this->analysis_res += "международный";

                     this->analysis_res += ")";
                 }

                 this->analysis_res += ", принадлежащий "+metaData.at(1) +
                         ", был обнаружен в списке телефонных контактов "+ res.to_lastname +" "+
                         res.to_name +" "+ res.to_midname+" с пометкой "+res.to_contact_alias;
             }
         }
         QList<Analysis_general> var2List = map["var2"];
         if(var2List.size() > 0){
             this->analysis_res += "\r\n 2 ВАРИАНТ: ";
             for(int a=0; a<var2List.size();a++){
               Analysis_general res = var2List.at(a);
               this->analysis_res += " \r\n \t ЗК#" + res.to_event_id +" телефонный номер " +
                       "списка контактов "+
                       res.from_contact +", с пометкой " + res.from_contact_alias + " в записной книжке, "+
                       "владельцем которой является " + metaData.at(1) + " обнаружен, как "+
                       "принадлежащий владельцу записной книжки: "+ res.to_lastname +" "+
                       res.to_name +" "+ res.to_midname + "; \r\n";
             }
         }

         QList<Analysis_general> var3List = map["var3"];
         if(var3List.size() > 0){
             this->analysis_res += "\r\n 3 ВАРИАНТ: ";
             for(int a=0; a<var3List.size();a++){
                 Analysis_general res = var3List.at(a);
                 this->analysis_res += " \r\n \t ЗК#"+res.to_event_id;
                 this->analysis_res += " Номер телефона "+ res.to_contact + " с пометкой "
                         + res.from_contact_alias +", обнаруженный в списке контактов "
                         + metaData.at(1) + " был также обнаружен в списке контактов с пометкой "+
                         res.to_contact_alias + ", владельцем которого является "+
                         res.to_lastname +" "+ res.to_name +" "+ res.to_midname;
             }
         }
     }

}

