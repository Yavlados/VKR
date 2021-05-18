SELECT DISTINCT
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
	contact.number as to_contact,
	contact.alias as to_contact_alias,
	contact.oldnum as to_contact_oldnum,
	contact.internum as to_contact_internum,
	t.number as to_telephone,
	t.oldnum as to_telephone_oldnum,
	t.internum as to_telephone_internum,
	p.name as to_name,
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
FROM notebook2.contact
INNER JOIN
	(
		SELECT  DISTINCT
					e.id as from_event_id, 
					e.category as from_event_category, 
					e.detention_date as from_event_detention_date, 
					e.detention_time as from_event_detention_time,
					e.detention_reason as from_event_detention_reason,
					e.detention_by as from_event_detention_by,
					e.keeping_place as from_event_keeping_place,
					e.additional as from_event_keeping_additional,
					p.name as from_name,
					p.lastname as from_lastname,
					p.midname as from_midname,
					t.number as from_telephone,
					t.oldnum as from_telephone_oldnum,
					t.internum as from_telephone_internum,		
					t.id as from_telephone_id,
					c.number as from_contact,
					c.alias as from_contact_alias,
					c.oldnum as from_contact_oldnum,
					c.internum as from_contact_internum		
				FROM
				notebook2.event as e,
				notebook2.contact as c,
				notebook2.telephone as t,
				notebook2.person as p,
				notebook2.event_person as ep
				WHERE e.id = 21
				AND e.id = ep.event_id
				AND ep.person_id = p.id
				AND t.person_id = p.id
				AND c.telephone_id = t.id
				
		) as f_c
					ON f_c.from_contact = contact.number,
					notebook2.telephone as t,
					notebook2.person as p,
					notebook2.event_person as ep,
					notebook2.event as e					
					WHERE contact.telephone_id != f_c.from_telephone_id
					AND contact.telephone_id = t.id
					AND t.person_id = p.id
					AND p.id = ep.person_id
					AND ep.event_id = e.id
					AND e.id != 21