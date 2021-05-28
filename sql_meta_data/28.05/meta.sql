--
-- PostgreSQL database dump
--

-- Dumped from database version 9.5.16
-- Dumped by pg_dump version 13.2

-- Started on 2021-05-28 14:10:38

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
-- TOC entry 10 (class 2615 OID 16384)
-- Name: notebook2; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA notebook2;


ALTER SCHEMA notebook2 OWNER TO postgres;

SET default_tablespace = '';

--
-- TOC entry 184 (class 1259 OID 16385)
-- Name: contact; Type: TABLE; Schema: notebook2; Owner: postgres
--

CREATE TABLE notebook2.contact (
    number character varying,
    alias character varying(50),
    oldnum boolean DEFAULT false,
    internum boolean DEFAULT false,
    id integer NOT NULL,
    telephone_id integer NOT NULL
);


ALTER TABLE notebook2.contact OWNER TO postgres;

--
-- TOC entry 185 (class 1259 OID 16393)
-- Name: contact_id_seq; Type: SEQUENCE; Schema: notebook2; Owner: postgres
--

CREATE SEQUENCE notebook2.contact_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE notebook2.contact_id_seq OWNER TO postgres;

--
-- TOC entry 2232 (class 0 OID 0)
-- Dependencies: 185
-- Name: contact_id_seq; Type: SEQUENCE OWNED BY; Schema: notebook2; Owner: postgres
--

ALTER SEQUENCE notebook2.contact_id_seq OWNED BY notebook2.contact.id;


--
-- TOC entry 186 (class 1259 OID 16395)
-- Name: event; Type: TABLE; Schema: notebook2; Owner: postgres
--

CREATE TABLE notebook2.event (
    category character varying(100),
    detention_time character varying(20),
    detention_reason character varying(200),
    detention_by character varying,
    keeping_place character varying,
    additional character varying,
    id integer NOT NULL,
    detention_date date
);


ALTER TABLE notebook2.event OWNER TO postgres;

--
-- TOC entry 187 (class 1259 OID 16401)
-- Name: event_id_seq; Type: SEQUENCE; Schema: notebook2; Owner: postgres
--

CREATE SEQUENCE notebook2.event_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE notebook2.event_id_seq OWNER TO postgres;

--
-- TOC entry 2233 (class 0 OID 0)
-- Dependencies: 187
-- Name: event_id_seq; Type: SEQUENCE OWNED BY; Schema: notebook2; Owner: postgres
--

ALTER SEQUENCE notebook2.event_id_seq OWNED BY notebook2.event.id;


--
-- TOC entry 188 (class 1259 OID 16403)
-- Name: event_person; Type: TABLE; Schema: notebook2; Owner: postgres
--

CREATE TABLE notebook2.event_person (
    event_id integer NOT NULL,
    person_id integer NOT NULL
);


ALTER TABLE notebook2.event_person OWNER TO postgres;

--
-- TOC entry 189 (class 1259 OID 16406)
-- Name: official_telephones; Type: TABLE; Schema: notebook2; Owner: postgres
--

CREATE TABLE notebook2.official_telephones (
    id integer NOT NULL,
    number character varying,
    name character varying
);


ALTER TABLE notebook2.official_telephones OWNER TO postgres;

--
-- TOC entry 190 (class 1259 OID 16412)
-- Name: official_telephones_id_seq; Type: SEQUENCE; Schema: notebook2; Owner: postgres
--

CREATE SEQUENCE notebook2.official_telephones_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE notebook2.official_telephones_id_seq OWNER TO postgres;

--
-- TOC entry 2234 (class 0 OID 0)
-- Dependencies: 190
-- Name: official_telephones_id_seq; Type: SEQUENCE OWNED BY; Schema: notebook2; Owner: postgres
--

ALTER SEQUENCE notebook2.official_telephones_id_seq OWNED BY notebook2.official_telephones.id;


--
-- TOC entry 191 (class 1259 OID 16414)
-- Name: person; Type: TABLE; Schema: notebook2; Owner: postgres
--

CREATE TABLE notebook2.person (
    lastname character varying(50),
    name character varying(50),
    midname character varying(50),
    alias character varying(50),
    id integer NOT NULL,
    country public.adress,
    city public.adress,
    street public.adress,
    house public.adress,
    corp public.adress,
    flat public.adress,
    hash uuid
);


ALTER TABLE notebook2.person OWNER TO postgres;

--
-- TOC entry 192 (class 1259 OID 16417)
-- Name: person_id_seq; Type: SEQUENCE; Schema: notebook2; Owner: postgres
--

CREATE SEQUENCE notebook2.person_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE notebook2.person_id_seq OWNER TO postgres;

--
-- TOC entry 2235 (class 0 OID 0)
-- Dependencies: 192
-- Name: person_id_seq; Type: SEQUENCE OWNED BY; Schema: notebook2; Owner: postgres
--

ALTER SEQUENCE notebook2.person_id_seq OWNED BY notebook2.person.id;


--
-- TOC entry 196 (class 1259 OID 16523)
-- Name: person_links; Type: TABLE; Schema: notebook2; Owner: postgres
--

CREATE TABLE notebook2.person_links (
    hash uuid,
    links uuid[]
);


ALTER TABLE notebook2.person_links OWNER TO postgres;

--
-- TOC entry 193 (class 1259 OID 16419)
-- Name: telephone; Type: TABLE; Schema: notebook2; Owner: postgres
--

CREATE TABLE notebook2.telephone (
    number character varying,
    id integer NOT NULL,
    person_id integer NOT NULL,
    oldnum boolean DEFAULT false,
    internum boolean DEFAULT false
);


ALTER TABLE notebook2.telephone OWNER TO postgres;

--
-- TOC entry 194 (class 1259 OID 16427)
-- Name: telephone_id_seq; Type: SEQUENCE; Schema: notebook2; Owner: postgres
--

CREATE SEQUENCE notebook2.telephone_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE notebook2.telephone_id_seq OWNER TO postgres;

--
-- TOC entry 2236 (class 0 OID 0)
-- Dependencies: 194
-- Name: telephone_id_seq; Type: SEQUENCE OWNED BY; Schema: notebook2; Owner: postgres
--

ALTER SEQUENCE notebook2.telephone_id_seq OWNED BY notebook2.telephone.id;


--
-- TOC entry 2073 (class 2604 OID 16429)
-- Name: contact id; Type: DEFAULT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.contact ALTER COLUMN id SET DEFAULT nextval('notebook2.contact_id_seq'::regclass);


--
-- TOC entry 2074 (class 2604 OID 16430)
-- Name: event id; Type: DEFAULT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.event ALTER COLUMN id SET DEFAULT nextval('notebook2.event_id_seq'::regclass);


--
-- TOC entry 2075 (class 2604 OID 16431)
-- Name: official_telephones id; Type: DEFAULT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.official_telephones ALTER COLUMN id SET DEFAULT nextval('notebook2.official_telephones_id_seq'::regclass);


--
-- TOC entry 2076 (class 2604 OID 16432)
-- Name: person id; Type: DEFAULT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.person ALTER COLUMN id SET DEFAULT nextval('notebook2.person_id_seq'::regclass);


--
-- TOC entry 2079 (class 2604 OID 16433)
-- Name: telephone id; Type: DEFAULT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.telephone ALTER COLUMN id SET DEFAULT nextval('notebook2.telephone_id_seq'::regclass);


--
-- TOC entry 2215 (class 0 OID 16385)
-- Dependencies: 184
-- Data for Name: contact; Type: TABLE DATA; Schema: notebook2; Owner: postgres
--

INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('2342525', 'CCCCC', false, false, 421, 315);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('2342525', 'CCCCC', false, false, 422, 315);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('111', 'AAAA', false, false, 423, 315);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('891235466244', 'ДрУг2', false, false, 424, 316);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('124124124124', '11111', false, false, 425, 316);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('11111', 'ssdsa', true, true, 426, 316);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('12412414', '11111', false, false, 427, 318);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('123123', 'ASD', false, false, 428, 318);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('891234567788', 'ASd', false, false, 429, 319);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('89121234422', 'Виталя', false, false, 430, 319);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('900', 'sda', true, false, 431, 319);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('111', 'AAA', false, false, 432, 320);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('666', '', true, false, 434, 323);
INSERT INTO notebook2.contact (number, alias, oldnum, internum, id, telephone_id) VALUES ('89555555', 'Friend', true, false, 438, 328);


--
-- TOC entry 2217 (class 0 OID 16395)
-- Dependencies: 186
-- Data for Name: event; Type: TABLE DATA; Schema: notebook2; Owner: postgres
--

INSERT INTO notebook2.event (category, detention_time, detention_reason, detention_by, keeping_place, additional, id, detention_date) VALUES ('Category1', '10:50', 'Reason1', 'MVD1', 'MVD1', '', 93, '2010-10-10');
INSERT INTO notebook2.event (category, detention_time, detention_reason, detention_by, keeping_place, additional, id, detention_date) VALUES ('1112', '', '111', '2', '2', '111', 94, '2014-10-10');
INSERT INTO notebook2.event (category, detention_time, detention_reason, detention_by, keeping_place, additional, id, detention_date) VALUES ('qwe', '10:10', 'qwe', 'qwe', 'qwe', '', 95, '2002-01-10');


--
-- TOC entry 2219 (class 0 OID 16403)
-- Dependencies: 188
-- Data for Name: event_person; Type: TABLE DATA; Schema: notebook2; Owner: postgres
--

INSERT INTO notebook2.event_person (event_id, person_id) VALUES (93, 182);
INSERT INTO notebook2.event_person (event_id, person_id) VALUES (93, 183);
INSERT INTO notebook2.event_person (event_id, person_id) VALUES (94, 188);
INSERT INTO notebook2.event_person (event_id, person_id) VALUES (95, 190);
INSERT INTO notebook2.event_person (event_id, person_id) VALUES (93, 186);
INSERT INTO notebook2.event_person (event_id, person_id) VALUES (95, 186);
INSERT INTO notebook2.event_person (event_id, person_id) VALUES (94, 185);
INSERT INTO notebook2.event_person (event_id, person_id) VALUES (95, 185);
INSERT INTO notebook2.event_person (event_id, person_id) VALUES (93, 185);


--
-- TOC entry 2220 (class 0 OID 16406)
-- Dependencies: 189
-- Data for Name: official_telephones; Type: TABLE DATA; Schema: notebook2; Owner: postgres
--

INSERT INTO notebook2.official_telephones (id, number, name) VALUES (1, '900', 'adasda');
INSERT INTO notebook2.official_telephones (id, number, name) VALUES (2, '800', 'eewfwfw');
INSERT INTO notebook2.official_telephones (id, number, name) VALUES (5, '895788445588', 'adasda');
INSERT INTO notebook2.official_telephones (id, number, name) VALUES (6, '8922342525', 'eewfwfw');


--
-- TOC entry 2222 (class 0 OID 16414)
-- Dependencies: 191
-- Data for Name: person; Type: TABLE DATA; Schema: notebook2; Owner: postgres
--

INSERT INTO notebook2.person (lastname, name, midname, alias, id, country, city, street, house, corp, flat, hash) VALUES ('333', '333', '333', '333', 186, '("","")', '(Москва,Москва)', '(Тверская,Тверская)', '(1,1)', '(2,2)', '(200,200)', 'ea19001a-ba2b-11eb-baea-175cf2bf5e33');
INSERT INTO notebook2.person (lastname, name, midname, alias, id, country, city, street, house, corp, flat, hash) VALUES ('333', '333', '333', '222', 188, '("","")', '(Москва,Москва)', '(Тверская,Тверская)', '(1,1)', '(2,2)', '(200,200)', 'ea1b2318-ba2b-11eb-baec-4b3555a95cf3');
INSERT INTO notebook2.person (lastname, name, midname, alias, id, country, city, street, house, corp, flat, hash) VALUES ('ASD', 'ASD', 'ASD', 'ASD', 185, '("","")', '(Москва,Москва)', '(Тверская,Тверская)', '(1,1)', '(2,2)', '(200,200)', 'ea1815d8-ba2b-11eb-bae9-770605b3b59b');
INSERT INTO notebook2.person (lastname, name, midname, alias, id, country, city, street, house, corp, flat, hash) VALUES ('Дмитриев', 'Дмитрий', 'Дмитриевич', 'Димка', 183, '("Россия","")', '(Москва,Москва)', '(Тверская,Тверская)', '(1,1)', '(2,2)', '(200,200)', 'ea168f74-ba2b-11eb-bae7-cbae05370a46');
INSERT INTO notebook2.person (lastname, name, midname, alias, id, country, city, street, house, corp, flat, hash) VALUES ('Иванов', 'Иван', 'Иванович', 'Ваня', 182, '("","")', '(Москва,Москва)', '(Тверская,Тверская)', '(1,1)', '(2,2)', '(200,200)', 'ea1075da-ba2b-11eb-bae6-3f24f374e4ce');
INSERT INTO notebook2.person (lastname, name, midname, alias, id, country, city, street, house, corp, flat, hash) VALUES ('Иванов', 'Иван', 'Иванович', 'Ванька', 190, '("","")', '(Москва,Москва)', '(Тверская,Тверская)', '(1,1)', '(2,2)', '(200,200)', 'ea1cf79c-ba2b-11eb-baee-cb5a4c02639a');


--
-- TOC entry 2226 (class 0 OID 16523)
-- Dependencies: 196
-- Data for Name: person_links; Type: TABLE DATA; Schema: notebook2; Owner: postgres
--

INSERT INTO notebook2.person_links (hash, links) VALUES ('ea168f74-ba2b-11eb-bae7-cbae05370a46', '{}');
INSERT INTO notebook2.person_links (hash, links) VALUES ('ea1cf79c-ba2b-11eb-baee-cb5a4c02639a', '{ea1075da-ba2b-11eb-bae6-3f24f374e4ce}');
INSERT INTO notebook2.person_links (hash, links) VALUES ('ea1075da-ba2b-11eb-bae6-3f24f374e4ce', '{ea1cf79c-ba2b-11eb-baee-cb5a4c02639a}');
INSERT INTO notebook2.person_links (hash, links) VALUES ('ea1b2318-ba2b-11eb-baec-4b3555a95cf3', '{ea19001a-ba2b-11eb-baea-175cf2bf5e33}');
INSERT INTO notebook2.person_links (hash, links) VALUES ('ea19001a-ba2b-11eb-baea-175cf2bf5e33', '{ea1b2318-ba2b-11eb-baec-4b3555a95cf3}');
INSERT INTO notebook2.person_links (hash, links) VALUES ('ea1815d8-ba2b-11eb-bae9-770605b3b59b', '{ea1b2318-ba2b-11eb-baec-4b3555a95cf3}');


--
-- TOC entry 2224 (class 0 OID 16419)
-- Dependencies: 193
-- Data for Name: telephone; Type: TABLE DATA; Schema: notebook2; Owner: postgres
--

INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('1234124', 315, 182, false, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('422322', 316, 182, true, true);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('89142345689', 317, 182, false, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('8914124124', 318, 182, false, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('8915289022', 319, 182, false, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('12414124', 320, 183, false, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('89155206399', 328, 190, false, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('500', 329, 188, true, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('333', 322, 186, true, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('666', 323, 186, true, false);
INSERT INTO notebook2.telephone (number, id, person_id, oldnum, internum) VALUES ('123', 349, 185, true, false);


--
-- TOC entry 2237 (class 0 OID 0)
-- Dependencies: 185
-- Name: contact_id_seq; Type: SEQUENCE SET; Schema: notebook2; Owner: postgres
--

SELECT pg_catalog.setval('notebook2.contact_id_seq', 439, true);


--
-- TOC entry 2238 (class 0 OID 0)
-- Dependencies: 187
-- Name: event_id_seq; Type: SEQUENCE SET; Schema: notebook2; Owner: postgres
--

SELECT pg_catalog.setval('notebook2.event_id_seq', 95, true);


--
-- TOC entry 2239 (class 0 OID 0)
-- Dependencies: 190
-- Name: official_telephones_id_seq; Type: SEQUENCE SET; Schema: notebook2; Owner: postgres
--

SELECT pg_catalog.setval('notebook2.official_telephones_id_seq', 6, true);


--
-- TOC entry 2240 (class 0 OID 0)
-- Dependencies: 192
-- Name: person_id_seq; Type: SEQUENCE SET; Schema: notebook2; Owner: postgres
--

SELECT pg_catalog.setval('notebook2.person_id_seq', 197, true);


--
-- TOC entry 2241 (class 0 OID 0)
-- Dependencies: 194
-- Name: telephone_id_seq; Type: SEQUENCE SET; Schema: notebook2; Owner: postgres
--

SELECT pg_catalog.setval('notebook2.telephone_id_seq', 349, true);


-- Completed on 2021-05-28 14:10:38

--
-- PostgreSQL database dump complete
--

