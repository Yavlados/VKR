--
-- PostgreSQL database dump
--

-- Dumped from database version 9.5.16
-- Dumped by pg_dump version 12.0

-- Started on 2021-05-17 15:13:12

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 2234 (class 0 OID 41216)
-- Dependencies: 194
-- Data for Name: contact; Type: TABLE DATA; Schema: notebook2; Owner: postgres
--

INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('891235466244', 'ДрУг2', false, false, 2, 1);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('891234567788', 'ASd', false, false, 3, 2);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('12412414', '11111', false, false, 9, 9);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('2342525', 'CCCCC', false, false, 11, 10);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('124124124124', '11111', false, false, 8, 1);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('11111', 'ssdsa', true, true, 1, 1);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('2342525', 'CCCCC', false, false, 12, 10);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('123123', 'ASD', false, false, 30, 9);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('89121234422', 'Виталя', false, false, 4, 2);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('111', 'AAAA', false, false, 10, 10);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('111', 'AAA', false, false, 41, 34);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('111', 'AAA', false, false, 42, 35);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('333', '', true, false, 308, 224);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('666', '', true, false, 309, 226);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('89555555', 'Friend', true, false, 312, 228);


--
-- TOC entry 2236 (class 0 OID 41226)
-- Dependencies: 196
-- Data for Name: event; Type: TABLE DATA; Schema: notebook2; Owner: postgres
--

INSERT INTO notebook2.event (category, detention_time, detention_reason, detention_by, keeping_place, additional, id, detention_date) VALUES ('Category1', '10:50', 'Reason1', 'MVD1', 'MVD1', '', 1, '2010-10-10');
INSERT INTO notebook2.event (category, detention_time, detention_reason, detention_by, keeping_place, additional, id, detention_date) VALUES ('qwe', '10:10', 'qwe', 'qwe', 'qwe', '', 74, '2002-01-10');
INSERT INTO notebook2.event (category, detention_time, detention_reason, detention_by, keeping_place, additional, id, detention_date) VALUES ('1112', '', '111', '2', '2', '111', 17, '2014-10-10');


--
-- TOC entry 2238 (class 0 OID 41234)
-- Dependencies: 198
-- Data for Name: event_person; Type: TABLE DATA; Schema: notebook2; Owner: postgres
--

INSERT INTO notebook2.event_person (event_id, person_id) VALUES (1, 1);
INSERT INTO notebook2.event_person (event_id, person_id) VALUES (1, 3);
INSERT INTO notebook2.event_person (event_id, person_id) VALUES (17, 25);
INSERT INTO notebook2.event_person (event_id, person_id) VALUES (17, 120);
INSERT INTO notebook2.event_person (event_id, person_id) VALUES (17, 121);
INSERT INTO notebook2.event_person (event_id, person_id) VALUES (1, 123);
INSERT INTO notebook2.event_person (event_id, person_id) VALUES (74, 126);


--
-- TOC entry 2244 (class 0 OID 41343)
-- Dependencies: 204
-- Data for Name: official_telephones; Type: TABLE DATA; Schema: notebook2; Owner: postgres
--



--
-- TOC entry 2239 (class 0 OID 41237)
-- Dependencies: 199
-- Data for Name: person; Type: TABLE DATA; Schema: notebook2; Owner: postgres
--

INSERT INTO notebook2.person (lastname, name, midname, alias, id) VALUES ('Иванов', 'Иван', 'Иванович', 'Ваня', 1);
INSERT INTO notebook2.person (lastname, name, midname, alias, id) VALUES ('Дмитриев', 'Дмитрий', 'Дмитриевич', 'Димка', 3);
INSERT INTO notebook2.person (lastname, name, midname, alias, id) VALUES ('222', '222', '222', '222', 120);
INSERT INTO notebook2.person (lastname, name, midname, alias, id) VALUES ('333', '333', '333', '333', 121);
INSERT INTO notebook2.person (lastname, name, midname, alias, id) VALUES ('asd', 'asd', 'asd', 'asd', 123);
INSERT INTO notebook2.person (lastname, name, midname, alias, id) VALUES ('ASD', 'ASD', 'ASD', 'ASD', 25);
INSERT INTO notebook2.person (lastname, name, midname, alias, id) VALUES ('Иванов', 'Иван', 'Иванович', 'Ванька', 126);


--
-- TOC entry 2241 (class 0 OID 41242)
-- Dependencies: 201
-- Data for Name: telephone; Type: TABLE DATA; Schema: notebook2; Owner: postgres
--

INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('1234124', 10, 1, false, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('422322', 1, 1, true, true);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('89142345689', 3, 1, false, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('8914124124', 9, 1, false, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('8915289022', 2, 1, false, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('12414124', 34, 3, false, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('111', 35, 25, true, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('222', 224, 25, true, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('444', 225, 121, true, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('555', 226, 121, true, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('89155206399', 228, 126, false, false);


--
-- TOC entry 2255 (class 0 OID 0)
-- Dependencies: 195
-- Name: contact_id_seq; Type: SEQUENCE SET; Schema: notebook2; Owner: postgres
--

SELECT pg_catalog.setval('notebook2.contact_id_seq', 312, true);


--
-- TOC entry 2256 (class 0 OID 0)
-- Dependencies: 197
-- Name: event_id_seq; Type: SEQUENCE SET; Schema: notebook2; Owner: postgres
--

SELECT pg_catalog.setval('notebook2.event_id_seq', 74, true);


--
-- TOC entry 2257 (class 0 OID 0)
-- Dependencies: 203
-- Name: official_telephones_id_seq; Type: SEQUENCE SET; Schema: notebook2; Owner: postgres
--

SELECT pg_catalog.setval('notebook2.official_telephones_id_seq', 1, false);


--
-- TOC entry 2258 (class 0 OID 0)
-- Dependencies: 200
-- Name: person_id_seq; Type: SEQUENCE SET; Schema: notebook2; Owner: postgres
--

SELECT pg_catalog.setval('notebook2.person_id_seq', 126, true);


--
-- TOC entry 2259 (class 0 OID 0)
-- Dependencies: 202
-- Name: telephone_id_seq; Type: SEQUENCE SET; Schema: notebook2; Owner: postgres
--

SELECT pg_catalog.setval('notebook2.telephone_id_seq', 228, true);


-- Completed on 2021-05-17 15:13:12

--
-- PostgreSQL database dump complete
--

