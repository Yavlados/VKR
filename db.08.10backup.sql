--
-- PostgreSQL database dump
--

-- Dumped from database version 9.5.16
-- Dumped by pg_dump version 9.5.16

-- Started on 2019-10-08 16:20:44

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
-- TOC entry 211 (class 1259 OID 16710)
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
-- TOC entry 214 (class 1259 OID 16735)
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
-- TOC entry 2272 (class 0 OID 0)
-- Dependencies: 214
-- Name: contacts_contact_list_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.contacts_contact_list_id_seq OWNED BY public.contacts.contact_list_id;


--
-- TOC entry 209 (class 1259 OID 16704)
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
    dop_info character varying(50),
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
-- TOC entry 205 (class 1259 OID 16516)
-- Name: official_tel; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.official_tel (
    tel_num character varying(20),
    service_name character varying(25),
    of_t_id integer NOT NULL
);


ALTER TABLE public.official_tel OWNER TO postgres;

--
-- TOC entry 206 (class 1259 OID 16519)
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
-- TOC entry 2273 (class 0 OID 0)
-- Dependencies: 206
-- Name: official_tel_of_t_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.official_tel_of_t_id_seq OWNED BY public.official_tel.of_t_id;


--
-- TOC entry 210 (class 1259 OID 16707)
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
-- TOC entry 213 (class 1259 OID 16722)
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
-- TOC entry 2274 (class 0 OID 0)
-- Dependencies: 213
-- Name: owners_tel_telephone_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.owners_tel_telephone_id_seq OWNED BY public.owners_tel.telephone_id;


--
-- TOC entry 218 (class 1259 OID 41091)
-- Name: zk_links; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.zk_links (
    row_id1 uuid,
    row_id2 uuid
);


ALTER TABLE public.zk_links OWNER TO postgres;

--
-- TOC entry 212 (class 1259 OID 16713)
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
-- TOC entry 2275 (class 0 OID 0)
-- Dependencies: 212
-- Name: zk_zk_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.zk_zk_id_seq OWNED BY public.zk.zk_id;


--
-- TOC entry 2126 (class 2604 OID 16737)
-- Name: contact_list_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contacts ALTER COLUMN contact_list_id SET DEFAULT nextval('public.contacts_contact_list_id_seq'::regclass);


--
-- TOC entry 2120 (class 2604 OID 16534)
-- Name: of_t_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.official_tel ALTER COLUMN of_t_id SET DEFAULT nextval('public.official_tel_of_t_id_seq'::regclass);


--
-- TOC entry 2123 (class 2604 OID 16724)
-- Name: telephone_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.owners_tel ALTER COLUMN telephone_id SET DEFAULT nextval('public.owners_tel_telephone_id_seq'::regclass);


--
-- TOC entry 2121 (class 2604 OID 16715)
-- Name: zk_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zk ALTER COLUMN zk_id SET DEFAULT nextval('public.zk_zk_id_seq'::regclass);


--
-- TOC entry 2262 (class 0 OID 16710)
-- Dependencies: 211
-- Data for Name: contacts; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('89225475587', 'МАМА', 147, 50, false, true);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('71244166122', 'фвйкфф', 147, 51, false, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('89877022574', 'Контакт1', 147, 52, false, true);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('89023568974', 'Контакт3', 147, 53, true, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('77987897727', '134412', 148, 54, true, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('77895555555', 'Петр', 149, 55, true, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('79995555555', 'qweas', 150, 56, true, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('71212555555', NULL, 36, 16, false, true);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('77895555555', 'Евгений', 21, 19, false, true);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('89225475587', 'МАМА', 129, 20, false, true);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('71244166122', 'фвйкфф', 129, 21, false, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('89877022574', 'Контакт1', 129, 22, false, true);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('89023568974', 'Контакт3', 129, 23, true, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('77987897727', '134412', 130, 24, true, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('77895555555', 'Петр', 131, 25, true, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('79995555555', 'qweas', 132, 26, true, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('89258558882', 'Владик', 133, 27, false, false);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id, oldnum, internum) VALUES ('88888888', 'Контакт для теста', 133, 28, true, false);


--
-- TOC entry 2276 (class 0 OID 0)
-- Dependencies: 214
-- Name: contacts_contact_list_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.contacts_contact_list_id_seq', 108, true);


--
-- TOC entry 2258 (class 0 OID 16516)
-- Dependencies: 205
-- Data for Name: official_tel; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.official_tel (tel_num, service_name, of_t_id) VALUES ('8(499)-720-56-22', 'Главное отделение ФСБ', 1);
INSERT INTO public.official_tel (tel_num, service_name, of_t_id) VALUES ('892885522', 'йцуйк', 3);
INSERT INTO public.official_tel (tel_num, service_name, of_t_id) VALUES ('988882888', 'Й!2133', 5);


--
-- TOC entry 2277 (class 0 OID 0)
-- Dependencies: 206
-- Name: official_tel_of_t_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.official_tel_of_t_id_seq', 5, true);


--
-- TOC entry 2261 (class 0 OID 16707)
-- Dependencies: 210
-- Data for Name: owners_tel; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (63, 128, '75255551555', false, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (64, 129, '71212555555', false, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (64, 130, '79148855887', false, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (64, 131, '76178888888', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (64, 132, '71255123333', true, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (65, 133, '89145203678', true, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (76, 147, '71212555555', false, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (76, 148, '79148855887', false, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (76, 149, '76178888888', false, true);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (76, 150, '71255123333', true, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (9, 36, '79885547777', true, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (56, 107, '74354211322', false, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (56, 108, '79567532412', false, NULL);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (7, 21, '79888888188', true, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (20, 31, '79872255588', false, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (20, 32, '71414225555', false, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (20, 33, '71245551555', false, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (20, 34, '71257777777', false, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (26, 47, '72222222222', false, NULL);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (26, 48, '73333333333', false, NULL);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (26, 50, '79999999999', false, NULL);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (61, 123, '75255551555', false, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (56, 181, '79888888188', false, NULL);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (45, 182, '79888888188', false, NULL);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (109, 183, '79888888188', false, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (110, 184, '74354211322', false, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (110, 185, '79567532412', false, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (111, 186, '72222222222', false, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (111, 187, '73333333333', false, false);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_id, telephone_num, internum, oldnum) VALUES (111, 188, '79999999999', false, false);


--
-- TOC entry 2278 (class 0 OID 0)
-- Dependencies: 213
-- Name: owners_tel_telephone_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.owners_tel_telephone_id_seq', 188, true);


--
-- TOC entry 2260 (class 0 OID 16704)
-- Dependencies: 209
-- Data for Name: zk; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Житенев', 'Владислав', 'Германович', '1997-10-23', 'Москва', 'улица', 'дом', '1', '200', 'Тверь', 'Горького', '1', 'б', '202', 'ASD', 'ФСБ', 76, NULL, '2015-10-15', '12:55:14', NULL, '3e3a53d2-d636-11e9-9de3-5b0c0b15301c');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Кузьмин', 'Леонтий', 'Игнатович', NULL, '', '', '', '', '', '', '', '', '', '', '', '', 8, '2019-10-07 20:50:43', NULL, NULL, NULL, 'e865532a-d717-11e9-9de9-e790330d43c1');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Короткин', 'Геннадий', 'Сергеевич', NULL, '', '', '', '', '', '', '', '', '', '', '', '', 9, '2019-10-07 20:50:51', NULL, NULL, ',5', 'eac6796e-d717-11e9-9dea-bbfd600d0034');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('1341', '2313', 'qweaaa', NULL, '', '', '', '', '', '', '', '', '', '', '', '', 61, '2019-09-17 19:28:22', '2019-09-16', '23:35:43', NULL, '8e0231e0-d8c1-11e9-a9fe-efa59c5495b1');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('4ssx', '', '', NULL, '', '', '', '', '', '', '', '', '', '', '', '', 56, '2019-10-07 20:51:19', '2019-09-16', '22:58:31', NULL, '5be68012-d8bc-11e9-8434-c394e4a67b17');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Житенев', 'Владислав', 'Германович', '1997-10-23', '', '', '', '', '', '', '', '', '', '', '', '', 45, '2019-10-07 20:52:16', NULL, NULL, ',30', '0cb1e770-d718-11e9-9df5-43e111a9c351');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Житенев', 'Владислав', 'Германович', '1997-10-23', '', '', '', '', '', '', '', '', '', '', '', '', 109, NULL, NULL, NULL, NULL, 'b31f0b54-e92b-11e9-bcb9-eb3fae326e10');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('4ssx', '', '', NULL, '', '', '', '', '', '', '', '', '', '', '', '', 110, NULL, '2019-09-16', '22:58:31', NULL, 'b3201a4e-e92b-11e9-bcba-83295835c311');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('ADQEQESD', 'adasda', '', NULL, '', '', '', '', '', '', '', '', '', '', '', '', 63, NULL, '2019-09-17', '19:28:51', NULL, '3c10561c-d968-11e9-b1fe-c37a8af4f6f2');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Житенев', 'Владислав', 'Германович', '1997-10-23', '', '', '', '', '', '', '', '', '', '', '', '', 111, NULL, '2019-08-27', '18:47:08', NULL, 'b320b68e-e92b-11e9-bcbb-5f0ed5254ecb');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Семибратченко', 'Всеволод', 'Максимович', NULL, '', '', '', '', '', '', '', '', '', '', '', '', 10, NULL, NULL, NULL, NULL, 'ed5ec604-d717-11e9-9deb-9bb3d31e1f21');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Румянцев', 'Евгений', 'Петрович', NULL, '', '', '', '', '', '', '', '', '', '', '', '', 11, NULL, NULL, NULL, NULL, 'f01ce452-d717-11e9-9dec-978f682c2e27');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Васнецов', 'Олег', 'Иванович', '1990-01-13', '12', '', '', '', '', '', '', '', '', '', '', 'ФСКН', 20, '2019-09-11 18:25:38', NULL, NULL, ',22', 'f62806f6-d717-11e9-9dee-d798d9df460b');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Житенев', 'Владислав', 'Германович', '1997-10-23', 'Москва', 'улица', 'дом', '1', '200', 'Тверь', 'Горького', '1', 'б', '202', 'ASD', 'ФСБ', 64, '2019-10-04 23:35:03', '2015-10-15', '12:55:14', NULL, '5897804a-e6e6-11e9-bf86-ff9df17305f6');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Александрова', 'Ирина', 'Владимировна', NULL, '', '', '', '', '', '', '', '', '', '', '', '', 7, '2019-10-04 23:35:25', NULL, NULL, NULL, 'e5dfab8c-d717-11e9-9de8-0fa3c57c9326');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Житенев', 'Владислав', 'Германович', '1997-10-23', '', '', '', '', '', '', '', '', '', '', '', '', 26, '2019-10-05 00:25:37', '2019-08-27', '18:47:08', NULL, 'fd93dfbe-d717-11e9-9df1-1f9a79a5c2ac');
INSERT INTO public.zk (lastname, name, mid_name, birth_date, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, dop_info, check_for, zk_id, date_upd, date_add, time_add, linked_id, row_id) VALUES ('Савина', 'Елизавета', 'Ивановна', '2000-12-02', 'Москва', 'Лесная', '', '', '', 'Москва', 'Лесная', '', '', '', '', 'МВД', 65, '2019-10-06 22:38:08', NULL, NULL, NULL, '58afa5f8-e6e6-11e9-bf87-4bf04f5d9660');


--
-- TOC entry 2266 (class 0 OID 41091)
-- Dependencies: 218
-- Data for Name: zk_links; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('5be68012-d8bc-11e9-8434-c394e4a67b17', 'e5dfab8c-d717-11e9-9de8-0fa3c57c9326');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('0cb1e770-d718-11e9-9df5-43e111a9c351', '5897804a-e6e6-11e9-bf86-ff9df17305f6');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('0cb1e770-d718-11e9-9df5-43e111a9c351', 'fd93dfbe-d717-11e9-9df1-1f9a79a5c2ac');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('0cb1e770-d718-11e9-9df5-43e111a9c351', 'e5dfab8c-d717-11e9-9de8-0fa3c57c9326');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('0cb1e770-d718-11e9-9df5-43e111a9c351', '5be68012-d8bc-11e9-8434-c394e4a67b17');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('0cb1e770-d718-11e9-9df5-43e111a9c351', '3e3a53d2-d636-11e9-9de3-5b0c0b15301c');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('b31f0b54-e92b-11e9-bcb9-eb3fae326e10', 'fd93dfbe-d717-11e9-9df1-1f9a79a5c2ac');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('b3201a4e-e92b-11e9-bcba-83295835c311', '5be68012-d8bc-11e9-8434-c394e4a67b17');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('b320b68e-e92b-11e9-bcbb-5f0ed5254ecb', 'fd93dfbe-d717-11e9-9df1-1f9a79a5c2ac');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('b31f0b54-e92b-11e9-bcb9-eb3fae326e10', 'b3201a4e-e92b-11e9-bcba-83295835c311');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('b31f0b54-e92b-11e9-bcb9-eb3fae326e10', 'b320b68e-e92b-11e9-bcbb-5f0ed5254ecb');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('3c10561c-d968-11e9-b1fe-c37a8af4f6f2', '8e0231e0-d8c1-11e9-a9fe-efa59c5495b1');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('5897804a-e6e6-11e9-bf86-ff9df17305f6', '0cb1e770-d718-11e9-9df5-43e111a9c351');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('0cb1e770-d718-11e9-9df5-43e111a9c351', '5897804a-e6e6-11e9-bf86-ff9df17305f6');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('fd93dfbe-d717-11e9-9df1-1f9a79a5c2ac', '0cb1e770-d718-11e9-9df5-43e111a9c351');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('fd93dfbe-d717-11e9-9df1-1f9a79a5c2ac', '5897804a-e6e6-11e9-bf86-ff9df17305f6');
INSERT INTO public.zk_links (row_id1, row_id2) VALUES ('3e3a53d2-d636-11e9-9de3-5b0c0b15301c', '5897804a-e6e6-11e9-bf86-ff9df17305f6');


--
-- TOC entry 2279 (class 0 OID 0)
-- Dependencies: 212
-- Name: zk_zk_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.zk_zk_id_seq', 111, true);


--
-- TOC entry 2138 (class 2606 OID 16742)
-- Name: PK_contact_list_id; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contacts
    ADD CONSTRAINT "PK_contact_list_id" PRIMARY KEY (contact_list_id);


--
-- TOC entry 2130 (class 2606 OID 16568)
-- Name: PK_of_t_id; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.official_tel
    ADD CONSTRAINT "PK_of_t_id" PRIMARY KEY (of_t_id);


--
-- TOC entry 2136 (class 2606 OID 16729)
-- Name: PK_telephone_id; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.owners_tel
    ADD CONSTRAINT "PK_telephone_id" PRIMARY KEY (telephone_id);


--
-- TOC entry 2132 (class 2606 OID 16721)
-- Name: PK_zk_id; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zk
    ADD CONSTRAINT "PK_zk_id" PRIMARY KEY (zk_id);


--
-- TOC entry 2134 (class 2606 OID 41095)
-- Name: zk_row_id_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zk
    ADD CONSTRAINT zk_row_id_key UNIQUE (row_id);


--
-- TOC entry 2139 (class 2606 OID 41117)
-- Name: FK; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.owners_tel
    ADD CONSTRAINT "FK" FOREIGN KEY (fk_telephone_zk) REFERENCES public.zk(zk_id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2140 (class 2606 OID 24584)
-- Name: fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contacts
    ADD CONSTRAINT fk FOREIGN KEY (fk_cl_telephone) REFERENCES public.owners_tel(telephone_id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2142 (class 2606 OID 41102)
-- Name: row_id_1_zk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zk_links
    ADD CONSTRAINT row_id_1_zk FOREIGN KEY (row_id1) REFERENCES public.zk(row_id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2141 (class 2606 OID 41097)
-- Name: row_id_2_zk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zk_links
    ADD CONSTRAINT row_id_2_zk FOREIGN KEY (row_id2) REFERENCES public.zk(row_id) ON UPDATE CASCADE ON DELETE CASCADE;


-- Completed on 2019-10-08 16:20:45

--
-- PostgreSQL database dump complete
--

