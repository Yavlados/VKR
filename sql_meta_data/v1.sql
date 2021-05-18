SELECT DISTINCT 	f_t.from_event_id as from_event_id,
					f_t.from_event_category as from_event_category,
					f_t.from_event_detention_date as from_event_detention_date,
					f_t.from_event_detention_time as from_event_detention_time,
					f_t.from_event_detention_reason as from_event_detention_reason,
					f_t.from_event_detention_by as from_event_detention_by,
					f_t.from_event_keeping_place as from_event_keeping_place,
					f_t.from_event_keeping_additional as from_event_keeping_additional,
					f_t.from_name as from_name,
					f_t.from_lastname as from_lastname,
					f_t.from_midname as from_midname,
					f_t.from_telephone as from_telephone,
					f_t.from_telephone_oldnum as from_telephone_oldnum,
					f_t.from_telephone_internum as from_telephone_internum,
					contact.number as to_contact,
					contact.alias as to_conntact_alias,
					contact.oldnum as to_conntact_oldnum,
					contact.internum as to_conntact_internum,
					t.number as to_telephone,
					t.oldnum  as to_telephone_oldnum,
					t.internum  as to_telephone_internum,
					p.name  as to_name,
					p.lastname as to_lastname,
					p.midname as to_midname,
					e.category as to_event_category,
					e.detention_date as to_event_detention_date,
					e.detention_time as to_event_detention_time,
					e.detention_reason as to_event_detention_reason,
					e.detention_by as to_event_detention_by,
					e.keeping_place as to_event_keeping_place,
					e.additional as to_event_keeping_additional,
					e.id as to_event_id
FROM notebook2.event as e,
notebook2.event_person as ep,
notebook2.person as p,
notebook2.telephone as t,
notebook2.contact 
INNER JOIN
	(SELECT DISTINCT 
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
					t.id as telephone_id
					FROM notebook2.person
					, notebook2.event_person as ep,
					notebook2.telephone as t,
					notebook2.event as events
					WHERE events.id = 21
					AND ep.event_id = 21
					AND ep.person_id = person.id
					AND t.person_id = person.id) as f_t
								ON f_t.from_telephone = contact.number
								WHERE contact.telephone_id = t.id
								AND t.person_id = p.id
								AND p.id = ep.person_id
								AND ep.event_id = e.id
								AND e.id != 21