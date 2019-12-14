--
-- PostgreSQL database dump
--

-- Dumped from database version 9.5.16
-- Dumped by pg_dump version 9.5.16

-- Started on 2019-12-06 01:49:12

SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- TOC entry 206 (class 1259 OID 16710)
-- Name: contacts; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.contacts (
    cl_telephone character varying(30),
    cl_info character varying(50),
    fk_cl_telephone integer NOT NULL,
    contact_list_id integer NOT NULL,
    oldnum boolean DEFAULT false,
    internum boolean DEFAULT false
);


ALTER TABLE public.contacts OWNER TO postgres;

--
-- TOC entry 209 (class 1259 OID 16735)
-- Name: contacts_contact_list_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.contacts_contact_list_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.contacts_contact_list_id_seq OWNER TO postgres;

--
-- TOC entry 2264 (class 0 OID 0)
-- Dependencies: 209
-- Name: contacts_contact_list_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.contacts_contact_list_id_seq OWNED BY public.contacts.contact_list_id;


--
-- TOC entry 204 (class 1259 OID 16704)
-- Name: zk; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.zk (
    lastname character varying(20),
    name character varying(10),
    mid_name character varying(20),
    birth_date date,
    reg_city character varying(20),
    reg_street character varying(20),
    reg_home character varying(5),
    reg_corp character varying(5),
    reg_flat character varying(5),
    liv_city character varying(20),
    liv_street character varying(20),
    liv_home character varying(5),
    liv_corp character varying(5),
    liv_flat character varying(5),
    dop_info character varying(500),
    check_for character varying(25),
    zk_id integer NOT NULL,
    date_upd character varying(20),
    date_add date,
    time_add character varying(10),
    linked_id character varying(50),
    row_id uuid DEFAULT public.uuid_generate_v1()
);


ALTER TABLE public.zk OWNER TO postgres;

--
-- TOC entry 210 (class 1259 OID 16748)
-- Name: customer_golutvin; Type: VIEW; Schema: public; Owner: postgres
--

CREATE VIEW public.customer_golutvin AS
 SELECT zk.lastname,
    zk.name,
    zk.mid_name
   FROM public.zk
  WHERE ((zk.reg_city)::text = 'Москва'::text);


ALTER TABLE public.customer_golutvin OWNER TO postgres;

--
-- TOC entry 202 (class 1259 OID 16516)
-- Name: official_tel; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.official_tel (
    tel_num character varying(20),
    service_name character varying(25),
    of_t_id integer NOT NULL
);


ALTER TABLE public.official_tel OWNER TO postgres;

--
-- TOC entry 203 (class 1259 OID 16519)
-- Name: official_tel_of_t_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.official_tel_of_t_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.official_tel_of_t_id_seq OWNER TO postgres;

--
-- TOC entry 2265 (class 0 OID 0)
-- Dependencies: 203
-- Name: official_tel_of_t_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.official_tel_of_t_id_seq OWNED BY public.official_tel.of_t_id;


--
-- TOC entry 205 (class 1259 OID 16707)
-- Name: owners_tel; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.owners_tel (
    fk_telephone_zk integer,
    telephone_id integer NOT NULL,
    telephone_num character varying(30) NOT NULL,
    internum boolean DEFAULT false NOT NULL,
    oldnum boolean DEFAULT false
);


ALTER TABLE public.owners_tel OWNER TO postgres;

--
-- TOC entry 208 (class 1259 OID 16722)
-- Name: owners_tel_telephone_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.owners_tel_telephone_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.owners_tel_telephone_id_seq OWNER TO postgres;

--
-- TOC entry 2266 (class 0 OID 0)
-- Dependencies: 208
-- Name: owners_tel_telephone_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.owners_tel_telephone_id_seq OWNED BY public.owners_tel.telephone_id;


--
-- TOC entry 211 (class 1259 OID 41091)
-- Name: zk_links; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.zk_links (
    row_id1 uuid,
    row_id2 uuid
);


ALTER TABLE public.zk_links OWNER TO postgres;

--
-- TOC entry 207 (class 1259 OID 16713)
-- Name: zk_zk_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.zk_zk_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.zk_zk_id_seq OWNER TO postgres;

--
-- TOC entry 2267 (class 0 OID 0)
-- Dependencies: 207
-- Name: zk_zk_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.zk_zk_id_seq OWNED BY public.zk.zk_id;


--
-- TOC entry 2116 (class 2604 OID 16737)
-- Name: contact_list_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contacts ALTER COLUMN contact_list_id SET DEFAULT nextval('public.contacts_contact_list_id_seq'::regclass);


--
-- TOC entry 2110 (class 2604 OID 16534)
-- Name: of_t_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.official_tel ALTER COLUMN of_t_id SET DEFAULT nextval('public.official_tel_of_t_id_seq'::regclass);


--
-- TOC entry 2113 (class 2604 OID 16724)
-- Name: telephone_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.owners_tel ALTER COLUMN telephone_id SET DEFAULT nextval('public.owners_tel_telephone_id_seq'::regclass);


--
-- TOC entry 2111 (class 2604 OID 16715)
-- Name: zk_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zk ALTER COLUMN zk_id SET DEFAULT nextval('public.zk_zk_id_seq'::regclass);


--
-- TOC entry 2252 (class 0 OID 16710)
-- Dependencies: 206
-- Data for Name: contacts; Type: TABLE DATA; Schema: public; Owner: postgres
--



--
-- TOC entry 2268 (class 0 OID 0)
-- Dependencies: 209
-- Name: contacts_contact_list_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.contacts_contact_list_id_seq', 429, true);


--
-- TOC entry 2248 (class 0 OID 16516)
-- Dependencies: 202
-- Data for Name: official_tel; Type: TABLE DATA; Schema: public; Owner: postgres
--



--
-- TOC entry 2269 (class 0 OID 0)
-- Dependencies: 203
-- Name: official_tel_of_t_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.official_tel_of_t_id_seq', 38, true);


--
-- TOC entry 2251 (class 0 OID 16707)
-- Dependencies: 205
-- Data for Name: owners_tel; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (426, 805, '4769078', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (426, 806, '4944545', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (426, 807, '4940001', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (426, 808, '4940102', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (426, 809, '4940103', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (427, 810, '100', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (427, 811, '200', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (427, 812, '500', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (428, 813, '1001111', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (428, 814, '1001212', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (428, 815, '1001515', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (429, 816, '4944893', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (429, 817, '4944893', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (430, 818, '100', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (430, 819, '101', false, true);


--
-- TOC entry 2270 (class 0 OID 0)
-- Dependencies: 208
-- Name: owners_tel_telephone_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.owners_tel_telephone_id_seq', 819, true);


--
-- TOC entry 2250 (class 0 OID 16704)
-- Dependencies: 204
-- Data for Name: zk; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Ivanov', 'Иван', 'Иванович', NULL, 'Москва', 'Улица улица', '1', '2', '3', NULL, NULL, NULL, NULL, NULL, 'OLD доп инфо
доп инфо 2; Кат.:Категория; Дат: 1.2.2017; Мест:Место хранения; Кем: Кем изъята; Прич:Причина;, Кличка:Кличка', NULL, 426, NULL, NULL, NULL, NULL, 'aed35eb6-17b0-11ea-a94e-2b5cbcbd5747');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Друг 1', 'Имя 1', 'О1', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Кличка: Вован', NULL, 427, NULL, NULL, NULL, NULL, 'aed470c6-17b0-11ea-a94f-6b29ddd30143');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Друг два', 'Имя2', 'О2', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Кличка: ', NULL, 428, NULL, NULL, NULL, NULL, 'aed497d6-17b0-11ea-a950-53f1766356a9');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Ivanov', 'Ivan', 'Ivanovich', NULL, 'Mos', 'vlatisa', '11', '4', '239', NULL, NULL, NULL, NULL, NULL, 'OLD dop info
line 2; Кат.:kategory; Дат: 1.2.2003; Мест:mesto; Кем: kem; Прич:povod;, Кличка:koko', NULL, 429, NULL, NULL, NULL, NULL, 'fb9f2e8c-17b0-11ea-a955-d7e885da268a');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('abonent1', 'ima 1', 'o1', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Кличка: klich-1', NULL, 430, NULL, NULL, NULL, NULL, 'fba018f6-17b0-11ea-a956-6fa2b0a350d9');


--
-- TOC entry 2256 (class 0 OID 41091)
-- Dependencies: 211
-- Data for Name: zk_links; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('aed35eb6-17b0-11ea-a94e-2b5cbcbd5747', 'aed470c6-17b0-11ea-a94f-6b29ddd30143');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('aed35eb6-17b0-11ea-a94e-2b5cbcbd5747', 'aed497d6-17b0-11ea-a950-53f1766356a9');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('fb9f2e8c-17b0-11ea-a955-d7e885da268a', 'fba018f6-17b0-11ea-a956-6fa2b0a350d9');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('fba018f6-17b0-11ea-a956-6fa2b0a350d9', 'aed470c6-17b0-11ea-a94f-6b29ddd30143');


--
-- TOC entry 2271 (class 0 OID 0)
-- Dependencies: 207
-- Name: zk_zk_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.zk_zk_id_seq', 430, true);


--
-- TOC entry 2263 (class 0 OID 0)
-- Dependencies: 11
-- Name: SCHEMA public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


-- Completed on 2019-12-06 01:49:13

--
-- PostgreSQL database dump complete
--

