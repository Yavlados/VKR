--
-- PostgreSQL database dump
--

-- Dumped from database version 9.5.16
-- Dumped by pg_dump version 9.5.16

-- Started on 2020-01-28 23:22:10

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
-- TOC entry 207 (class 1259 OID 16710)
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
-- TOC entry 210 (class 1259 OID 16735)
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
-- TOC entry 2265 (class 0 OID 0)
-- Dependencies: 210
-- Name: contacts_contact_list_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.contacts_contact_list_id_seq OWNED BY public.contacts.contact_list_id;


--
-- TOC entry 205 (class 1259 OID 16704)
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
    row_id uuid DEFAULT public.uuid_generate_v1(),
    nickname character varying
);


ALTER TABLE public.zk OWNER TO postgres;

--
-- TOC entry 211 (class 1259 OID 16748)
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
-- TOC entry 203 (class 1259 OID 16516)
-- Name: official_tel; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.official_tel (
    tel_num character varying(20),
    service_name character varying(25),
    of_t_id integer NOT NULL
);


ALTER TABLE public.official_tel OWNER TO postgres;

--
-- TOC entry 204 (class 1259 OID 16519)
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
-- TOC entry 2266 (class 0 OID 0)
-- Dependencies: 204
-- Name: official_tel_of_t_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.official_tel_of_t_id_seq OWNED BY public.official_tel.of_t_id;


--
-- TOC entry 206 (class 1259 OID 16707)
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
-- TOC entry 209 (class 1259 OID 16722)
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
-- TOC entry 2267 (class 0 OID 0)
-- Dependencies: 209
-- Name: owners_tel_telephone_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.owners_tel_telephone_id_seq OWNED BY public.owners_tel.telephone_id;


--
-- TOC entry 212 (class 1259 OID 41091)
-- Name: zk_links; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.zk_links (
    row_id1 uuid,
    row_id2 uuid
);


ALTER TABLE public.zk_links OWNER TO postgres;

--
-- TOC entry 208 (class 1259 OID 16713)
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
-- TOC entry 2268 (class 0 OID 0)
-- Dependencies: 208
-- Name: zk_zk_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.zk_zk_id_seq OWNED BY public.zk.zk_id;


--
-- TOC entry 2117 (class 2604 OID 16737)
-- Name: contact_list_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contacts ALTER COLUMN contact_list_id SET DEFAULT nextval('public.contacts_contact_list_id_seq'::regclass);


--
-- TOC entry 2111 (class 2604 OID 16534)
-- Name: of_t_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.official_tel ALTER COLUMN of_t_id SET DEFAULT nextval('public.official_tel_of_t_id_seq'::regclass);


--
-- TOC entry 2114 (class 2604 OID 16724)
-- Name: telephone_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.owners_tel ALTER COLUMN telephone_id SET DEFAULT nextval('public.owners_tel_telephone_id_seq'::regclass);


--
-- TOC entry 2112 (class 2604 OID 16715)
-- Name: zk_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zk ALTER COLUMN zk_id SET DEFAULT nextval('public.zk_zk_id_seq'::regclass);


--
-- TOC entry 2253 (class 0 OID 16710)
-- Dependencies: 207
-- Data for Name: contacts; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('4956751', 'Белоусова Валентина Ивановна Кличка: Мама', 1515, 710, true, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('1074420', 'Белоусова Валентина Ивановна Кличка: Мама', 1515, 711, true, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('2636020', 'Папшев Игорь Степанович Кличка: Коллега', 1515, 712, true, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('2675434', 'Папшев Игорь Степанович Кличка: Коллега', 1515, 713, true, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('3795637', 'Кузнецова Нина Марковна Кличка: Подруга', 1515, 714, true, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('3794545', 'Кузнецова Нина Марковна Кличка: Подруга', 1515, 715, true, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('100', 'abonent1 ima 1 o1 Кличка: klich-1', 1524, 716, false, true);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('101', 'abonent1 ima 1 o1 Кличка: klich-1', 1524, 717, false, true);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('100', 'abonent1 ima 1 o1 Кличка: klich-1', 1540, 724, false, true);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('101', 'abonent1 ima 1 o1 Кличка: klich-1', 1540, 725, false, true);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('100', 'Друг 1 Имя 1 О1 ', 1542, 726, false, true);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('200', 'Друг 1 Имя 1 О1 ', 1542, 727, false, true);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('500', 'Друг 1 Имя 1 О1 ', 1542, 728, false, true);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('1001111', 'Друг два Имя2 О2 ', 1542, 729, true, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('1001212', 'Друг два Имя2 О2 ', 1542, 730, true, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('1001515', 'Друг два Имя2 О2 ', 1542, 731, true, false);


--
-- TOC entry 2269 (class 0 OID 0)
-- Dependencies: 210
-- Name: contacts_contact_list_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.contacts_contact_list_id_seq', 731, true);


--
-- TOC entry 2249 (class 0 OID 16516)
-- Dependencies: 203
-- Data for Name: official_tel; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.official_tel (tel_num, service_name, of_t_id) VALUES ('4995203699', 'Главное отделение ВТБ', 42);
INSERT INTO public.official_tel (tel_num, service_name, of_t_id) VALUES ('4997971658', 'фывфыв', 43);
INSERT INTO public.official_tel (tel_num, service_name, of_t_id) VALUES ('4956602399', 'Служба такси', 45);


--
-- TOC entry 2270 (class 0 OID 0)
-- Dependencies: 204
-- Name: official_tel_of_t_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.official_tel_of_t_id_seq', 45, true);


--
-- TOC entry 2252 (class 0 OID 16707)
-- Dependencies: 206
-- Data for Name: owners_tel; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (747, 1515, '9032888673', true, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (747, 1516, '9031074420', true, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (747, 1517, '4944893', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (748, 1518, '4956751', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (748, 1519, '1074420', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (749, 1520, '2636020', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (749, 1521, '2675434', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (750, 1522, '3795637', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (750, 1523, '3794545', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (751, 1524, '4944893', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (751, 1525, '4944893', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (752, 1526, '100', true, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (752, 1527, '101', true, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (753, 1528, '100', true, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (753, 1529, '200', true, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (753, 1530, '500', true, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (757, 1539, '1231212', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (758, 1540, '4944893', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (758, 1541, '4944893', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (760, 1542, '4769078', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (760, 1543, '4944545', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (760, 1544, '4940001', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (760, 1545, '4940102', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (760, 1546, '4940103', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (761, 1547, '100', true, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (761, 1548, '200', true, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (761, 1549, '500', true, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (762, 1550, '1001111', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (762, 1551, '1001212', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (762, 1552, '1001515', false, true);


--
-- TOC entry 2271 (class 0 OID 0)
-- Dependencies: 209
-- Name: owners_tel_telephone_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.owners_tel_telephone_id_seq', 1552, true);


--
-- TOC entry 2251 (class 0 OID 16704)
-- Dependencies: 205
-- Data for Name: zk; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id, nickname) VALUES ('Виноградова', 'Мария', 'Валерьевна', NULL, 'Г. Москва', 'Вилиса Лациса', '11', '4', '239', NULL, NULL, NULL, NULL, NULL, 'OLD Пример номер 1.
Строка 2.
Строка 3.     Пробелы.; Кат.:Преподаватель; Дат: 1.01.2019; Мест:Архив ГУВД; Кем: РОВД Северное Тушино; Прич:Тестирование системы;, Кличка:Машук', NULL, 747, NULL, NULL, NULL, NULL, '8a5659c4-2757-11ea-912f-f3f04d4c6e81', 'Ы');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id, nickname) VALUES ('Белоусова', 'Валентина', 'Ивановна', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Кличка: Мама', NULL, 748, NULL, NULL, NULL, NULL, '8a5743fc-2757-11ea-9130-2782643cad21', 'в');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id, nickname) VALUES ('Папшев', 'Игорь', 'Степанович', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Кличка: Коллега', NULL, 749, NULL, NULL, NULL, NULL, '8a576b16-2757-11ea-9131-c7773929e35d', 'п');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id, nickname) VALUES ('Кузнецова', 'Нина', 'Марковна', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Кличка: Подруга', NULL, 750, NULL, NULL, NULL, NULL, '8a579226-2757-11ea-9132-4f7d8fab64b2', 'я');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id, nickname) VALUES ('Ivanov', 'Ivan', 'Ivanovich', NULL, 'Mos', 'vlatisa', '11', '4', '239', NULL, NULL, NULL, NULL, NULL, 'OLD dop info
line 2; Кат.:kategory; Дат: 1.2.2003; Мест:mesto; Кем: kem; Прич:povod;, Кличка:koko', NULL, 751, NULL, NULL, NULL, NULL, '8bac6e8a-2757-11ea-9133-5f8b94650d5e', 'ы');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id, nickname) VALUES ('abonent1', 'ima 1', 'o1', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Кличка: klich-1', NULL, 752, NULL, NULL, NULL, NULL, '8bacbd22-2757-11ea-9134-9f4f7f37905c', 'м');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id, nickname) VALUES ('Друг 1', 'Имя 1', 'О1', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Кличка: Вован', NULL, 753, NULL, NULL, NULL, NULL, '64b6d36a-25c7-11ea-beae-1b201684aeba', 'и');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id, nickname) VALUES ('123qwe', 'qwe', 'asd', '1995-12-11', '', '', '', '', '', '', '', '', '', '', '', '', 756, NULL, '2019-12-28', '21:41:50', NULL, 'b5f41318-29a1-11ea-8ec4-ff6ba6104f87', 'и');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id, nickname) VALUES ('тестов', '', '', NULL, '', '', '', '', '', '', '', '', '', '', '', '', 757, NULL, '2020-01-22', '20:09:11', NULL, 'fb3ac3a4-3f5e-11ea-8084-3306386c7be4', 'н');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id, nickname) VALUES ('Ivanov', 'Ivan', 'Ivanovich', '1997-11-23', 'Mos', 'vlatisa', '11', '4', '239', '', '', '', '', '', 'OLD dop info
line 2; Кат.:kategory; Дат: 1.2.2003; Мест:mesto; Кем: kem; Прич:povod;, Кличка:koko', '', 758, NULL, NULL, NULL, NULL, '7d86b496-2757-11ea-912c-277a54a48667', 'БУГОР');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id, nickname) VALUES ('asdas', 'dasdasd', 'we', NULL, '', '', '', '', '', '', '', '', '', '', '', '', 759, NULL, '2020-01-28', '22:52:58', NULL, 'c8b59188-4207-11ea-840d-5f13c08209f5', 'qwe');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id, nickname) VALUES ('Ivanov', 'Иван', 'Иванович', NULL, 'Москва', 'Улица улица', '1', '2', '3', NULL, NULL, NULL, NULL, NULL, 'OLD доп инфо
доп инфо 2; Кат.:Категория; Дат: 1.2.2017; Мест:Место хранения; Кем: Кем изъята; Прич:Причина;', NULL, 760, NULL, '2020-01-28', NULL, NULL, '089f2b8a-420b-11ea-bba5-4bcbe7360e1a', 'Кличка');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id, nickname) VALUES ('Друг 1', 'Имя 1', 'О1', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 761, NULL, '2020-01-28', NULL, NULL, '089fe836-420b-11ea-bba6-036f352ca50f', 'Вован');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id, nickname) VALUES ('Друг два', 'Имя2', 'О2', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 762, NULL, '2020-01-28', NULL, NULL, '08a036f6-420b-11ea-bba7-2fa50847e1b1', NULL);


--
-- TOC entry 2257 (class 0 OID 41091)
-- Dependencies: 212
-- Data for Name: zk_links; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('8a5659c4-2757-11ea-912f-f3f04d4c6e81', '8a5743fc-2757-11ea-9130-2782643cad21');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('8a5659c4-2757-11ea-912f-f3f04d4c6e81', '8a576b16-2757-11ea-9131-c7773929e35d');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('8a5659c4-2757-11ea-912f-f3f04d4c6e81', '8a579226-2757-11ea-9132-4f7d8fab64b2');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('8bac6e8a-2757-11ea-9133-5f8b94650d5e', '8bacbd22-2757-11ea-9134-9f4f7f37905c');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('7d86b496-2757-11ea-912c-277a54a48667', '8bac6e8a-2757-11ea-9133-5f8b94650d5e');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('089f2b8a-420b-11ea-bba5-4bcbe7360e1a', '089fe836-420b-11ea-bba6-036f352ca50f');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('089f2b8a-420b-11ea-bba5-4bcbe7360e1a', '08a036f6-420b-11ea-bba7-2fa50847e1b1');


--
-- TOC entry 2272 (class 0 OID 0)
-- Dependencies: 208
-- Name: zk_zk_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.zk_zk_id_seq', 762, true);


--
-- TOC entry 2129 (class 2606 OID 16742)
-- Name: PK_contact_list_id; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contacts
    ADD CONSTRAINT "PK_contact_list_id" PRIMARY KEY (contact_list_id);


--
-- TOC entry 2121 (class 2606 OID 16568)
-- Name: PK_of_t_id; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.official_tel
    ADD CONSTRAINT "PK_of_t_id" PRIMARY KEY (of_t_id);


--
-- TOC entry 2127 (class 2606 OID 16729)
-- Name: PK_telephone_id; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.owners_tel
    ADD CONSTRAINT "PK_telephone_id" PRIMARY KEY (telephone_id);


--
-- TOC entry 2123 (class 2606 OID 16721)
-- Name: PK_zk_id; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zk
    ADD CONSTRAINT "PK_zk_id" PRIMARY KEY (zk_id);


--
-- TOC entry 2125 (class 2606 OID 41095)
-- Name: zk_row_id_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zk
    ADD CONSTRAINT zk_row_id_key UNIQUE (row_id);


--
-- TOC entry 2130 (class 2606 OID 41117)
-- Name: FK; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.owners_tel
    ADD CONSTRAINT "FK" FOREIGN KEY (fk_telephone_zk) REFERENCES public.zk(zk_id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2131 (class 2606 OID 24584)
-- Name: fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contacts
    ADD CONSTRAINT fk FOREIGN KEY (fk_cl_telephone) REFERENCES public.owners_tel(telephone_id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2133 (class 2606 OID 41102)
-- Name: row_id_1_zk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zk_links
    ADD CONSTRAINT row_id_1_zk FOREIGN KEY (row_id1) REFERENCES public.zk(row_id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2132 (class 2606 OID 41097)
-- Name: row_id_2_zk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zk_links
    ADD CONSTRAINT row_id_2_zk FOREIGN KEY (row_id2) REFERENCES public.zk(row_id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2264 (class 0 OID 0)
-- Dependencies: 11
-- Name: SCHEMA public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


-- Completed on 2020-01-28 23:22:11

--
-- PostgreSQL database dump complete
--

