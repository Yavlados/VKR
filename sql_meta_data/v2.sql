            SELECT                                                                                                 
                 res.from_event_id ,                                                                               
                 res.from_event_category,                                                                          
                 res.from_event_detention_date,                                                                    
                 res.from_event_detention_time ,                                                                   
                 res.from_event_detention_reason ,                                                                 
                 res.from_event_detention_by,                                                                      
                 res.from_event_keeping_place,                                                                     
                 res.from_event_keeping_additional ,                                                               
                 res.from_name ,                                                                                   
                 res.from_lastname ,                                                                               
                 res.from_midname,                                                                                 
                 res.from_telephone,                                                                               
                 res.from_telephone_oldnum ,                                                                       
                 res.from_telephone_internum,                                                                      
                 res.from_contact,                                                                                 
                 res.from_contact_alias,                                                                           
                 res.from_contact_oldnum,                                                                          
                 res.from_contact_internum,                                                                        
                 res.to_telephone,                                                                                 
                 res.to_telephone_oldnum,                                                                          
                 res.to_telephone_internum,		                                                                
                 res.to_name ,                                                                                     
                 res.to_lastname,                                                                                  
                 res.to_midname,                                                                                   
                 res.to_event_category,                                                                            
                 res.to_event_detention_date,                                                                      
                 res.to_event_detention_time ,                                                                     
                 res.to_event_detention_reason,                                                                    
                 res.to_event_detention_by,                                                                        
                 res.to_event_keeping_place,                                                                       
                 res.to_event_keeping_additional,                                                                  
                 res.to_event_id                                                                                   
            FROM notebook2.official_telephones as ot                                                               
            FULL OUTER JOIN                                                                                        
            (           SELECT                                                                                     
                                        f_c.from_event_id as from_event_id,                                        
                                        f_c.from_event_category as from_event_category,                            
                                        f_c.from_event_detention_date as from_event_detention_date,                
                                        f_c.from_event_detention_time as from_event_detention_time,                
                                        f_c.from_event_detention_reason as from_event_detention_reason,            
                                        f_c.from_event_detention_by as from_event_detention_by,                    
                                        f_c.from_event_keeping_place as from_event_keeping_place,                  
                                        f_c.from_event_keeping_additional as from_event_keeping_additional,        
                                        f_c.from_name as from_name,                                                
                                        f_c.from_lastname as from_lastname,                                        
                                        f_c.from_midname as from_midname,                                          
                                        f_c.from_telephone as from_telephone,                                      
                                        f_c.from_telephone_oldnum as from_telephone_oldnum,                        
                                        f_c.from_telephone_internum as from_telephone_internum,                    
                                        f_c.from_contact as from_contact,                                          
                                        f_c.from_contact_alias as from_contact_alias,                              
                                        f_c.from_contact_oldnum as from_contact_oldnum,                            
                                        f_c.from_contact_internum as from_contact_internum,                        
                                        telephone.number as to_telephone,                                          
                                        telephone.oldnum as to_telephone_oldnum,                                   
                                        telephone.internum as to_telephone_internum,				                
                                        person.name as to_name,                                                    
                                        person.lastname as to_lastname,                                            
                                        person.midname as to_midname,                                              
                                        event.category as to_event_category,                                       
                                        event.detention_date as to_event_detention_date,                           
                                        event.detention_time as to_event_detention_time,                           
                                        event.detention_reason as to_event_detention_reason,                       
                                        event.detention_by as to_event_detention_by,                               
                                        event.keeping_place as to_event_keeping_place,                             
                                        event.additional as to_event_keeping_additional,                           
                                        event.id as to_event_id                                                    
                        FROM notebook2.telephone                                                                   
                        INNER JOIN                                                                                 
                        (                                                                                          
                            SELECT DISTINCT                                                                        
                                            events.id as from_event_id,                                            
                                            events.category as from_event_category,                                
                                            events.detention_date as from_event_detention_date,                    
                                            events.detention_time as from_event_detention_time,                    
                                            events.detention_reason as from_event_detention_reason,                
                                            events.detention_by as from_event_detention_by,                        
                                            events.keeping_place as from_event_keeping_place,                      
                                            events.additional as from_event_keeping_additional,                    
                                            person.id,                                                             
                                            person.name as from_name,                                              
                                            person.lastname as from_lastname,                                      
                                            person.midname as from_midname,                                        
                                            t.number as from_telephone,                                            
                                            t.oldnum as from_telephone_oldnum,                                     
                                            t.internum as from_telephone_internum,                                 
                                            t.id as telephone_id,                                                  
                                            c.number as from_contact,                                              
                                            c.alias as from_contact_alias,                                         
                                            c.oldnum as from_contact_oldnum,                                       
                                            c.internum as from_contact_internum                                    
                                            FROM notebook2.person                                                  
                                            , notebook2.event_person as ep,                                        
                                            notebook2.telephone as t,                                              
                                            notebook2.event as events,                                             
                                            notebook2.contact as c                                                 
                                            WHERE events.id = 76                                                   
                                            AND ep.event_id = events.id                                            
                                            AND ep.person_id = person.id                                           
                                            AND t.person_id = person.id                                            
                                            AND c.telephone_id = t.id                                              
                        ) as f_c                                                                                   
                                        ON f_c.from_contact = telephone.number,                                    
                        notebook2.person,                                                                          
                        notebook2.event_person,                                                                    
                        notebook2.event                                                                            
                        WHERE person.id = telephone.person_id                                                      
                        AND event_person.person_id = person.id                                                     
                        AND event.id=event_person.event_id                                                         
                        AND event.id != 76 ) as res                                                                
                        ON res.from_contact = ot.number                                                            
                          WHERE (res.from_contact is NULL OR ot.number IS NULL)                                    
                          AND res.from_event_id IS NOT NULL                                                        
                          AND res.to_event_id IS NOT NULL                                                          ;