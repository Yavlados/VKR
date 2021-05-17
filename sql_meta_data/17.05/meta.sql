--
-- PostgreSQL database dump
--

-- Dumped from database version 9.5.16
-- Dumped by pg_dump version 12.0

-- Started on 2021-05-17 15:14:43

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
-- TOC entry 10 (class 2615 OID 41215)
-- Name: notebook2; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA notebook2;


ALTER SCHEMA notebook2 OWNER TO postgres;

SET default_tablespace = '';

--
-- TOC entry 194 (class 1259 OID 41216)
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
-- TOC entry 195 (class 1259 OID 41224)
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
-- TOC entry 2250 (class 0 OID 0)
-- Dependencies: 195
-- Name: contact_id_seq; Type: SEQUENCE OWNED BY; Schema: notebook2; Owner: postgres
--

ALTER SEQUENCE notebook2.contact_id_seq OWNED BY notebook2.contact.id;


--
-- TOC entry 196 (class 1259 OID 41226)
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
-- TOC entry 197 (class 1259 OID 41232)
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
-- TOC entry 2251 (class 0 OID 0)
-- Dependencies: 197
-- Name: event_id_seq; Type: SEQUENCE OWNED BY; Schema: notebook2; Owner: postgres
--

ALTER SEQUENCE notebook2.event_id_seq OWNED BY notebook2.event.id;


--
-- TOC entry 198 (class 1259 OID 41234)
-- Name: event_person; Type: TABLE; Schema: notebook2; Owner: postgres
--

CREATE TABLE notebook2.event_person (
    event_id integer NOT NULL,
    person_id integer NOT NULL
);


ALTER TABLE notebook2.event_person OWNER TO postgres;

--
-- TOC entry 204 (class 1259 OID 41343)
-- Name: official_telephones; Type: TABLE; Schema: notebook2; Owner: postgres
--

CREATE TABLE notebook2.official_telephones (
    id integer NOT NULL,
    number character varying,
    name character varying
);


ALTER TABLE notebook2.official_telephones OWNER TO postgres;

--
-- TOC entry 203 (class 1259 OID 41341)
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
-- TOC entry 2252 (class 0 OID 0)
-- Dependencies: 203
-- Name: official_telephones_id_seq; Type: SEQUENCE OWNED BY; Schema: notebook2; Owner: postgres
--

ALTER SEQUENCE notebook2.official_telephones_id_seq OWNED BY notebook2.official_telephones.id;


--
-- TOC entry 199 (class 1259 OID 41237)
-- Name: person; Type: TABLE; Schema: notebook2; Owner: postgres
--

CREATE TABLE notebook2.person (
    lastname character varying(50),
    name character varying(50),
    midname character varying(50),
    alias character varying(50),
    id integer NOT NULL
);


ALTER TABLE notebook2.person OWNER TO postgres;

--
-- TOC entry 200 (class 1259 OID 41240)
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
-- TOC entry 2253 (class 0 OID 0)
-- Dependencies: 200
-- Name: person_id_seq; Type: SEQUENCE OWNED BY; Schema: notebook2; Owner: postgres
--

ALTER SEQUENCE notebook2.person_id_seq OWNED BY notebook2.person.id;


--
-- TOC entry 201 (class 1259 OID 41242)
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
-- TOC entry 202 (class 1259 OID 41250)
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
-- TOC entry 2254 (class 0 OID 0)
-- Dependencies: 202
-- Name: telephone_id_seq; Type: SEQUENCE OWNED BY; Schema: notebook2; Owner: postgres
--

ALTER SEQUENCE notebook2.telephone_id_seq OWNED BY notebook2.telephone.id;


--
-- TOC entry 2094 (class 2604 OID 41252)
-- Name: contact id; Type: DEFAULT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.contact ALTER COLUMN id SET DEFAULT nextval('notebook2.contact_id_seq'::regclass);


--
-- TOC entry 2095 (class 2604 OID 41253)
-- Name: event id; Type: DEFAULT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.event ALTER COLUMN id SET DEFAULT nextval('notebook2.event_id_seq'::regclass);


--
-- TOC entry 2100 (class 2604 OID 41346)
-- Name: official_telephones id; Type: DEFAULT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.official_telephones ALTER COLUMN id SET DEFAULT nextval('notebook2.official_telephones_id_seq'::regclass);


--
-- TOC entry 2096 (class 2604 OID 41254)
-- Name: person id; Type: DEFAULT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.person ALTER COLUMN id SET DEFAULT nextval('notebook2.person_id_seq'::regclass);


--
-- TOC entry 2099 (class 2604 OID 41255)
-- Name: telephone id; Type: DEFAULT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.telephone ALTER COLUMN id SET DEFAULT nextval('notebook2.telephone_id_seq'::regclass);


--
-- TOC entry 2102 (class 2606 OID 41257)
-- Name: contact contact_pkey; Type: CONSTRAINT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.contact
    ADD CONSTRAINT contact_pkey PRIMARY KEY (id);


--
-- TOC entry 2106 (class 2606 OID 41259)
-- Name: event_person event_person_pkey; Type: CONSTRAINT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.event_person
    ADD CONSTRAINT event_person_pkey PRIMARY KEY (person_id, event_id);


--
-- TOC entry 2104 (class 2606 OID 41261)
-- Name: event event_pkey; Type: CONSTRAINT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.event
    ADD CONSTRAINT event_pkey PRIMARY KEY (id);


--
-- TOC entry 2114 (class 2606 OID 41351)
-- Name: official_telephones official_telephones_pkey; Type: CONSTRAINT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.official_telephones
    ADD CONSTRAINT official_telephones_pkey PRIMARY KEY (id);


--
-- TOC entry 2108 (class 2606 OID 41263)
-- Name: person person_pkey; Type: CONSTRAINT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.person
    ADD CONSTRAINT person_pkey PRIMARY KEY (id);


--
-- TOC entry 2110 (class 2606 OID 49534)
-- Name: person pk_unique; Type: CONSTRAINT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.person
    ADD CONSTRAINT pk_unique UNIQUE (id);


--
-- TOC entry 2112 (class 2606 OID 41265)
-- Name: telephone telephone_pkey; Type: CONSTRAINT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.telephone
    ADD CONSTRAINT telephone_pkey PRIMARY KEY (id);


--
-- TOC entry 2116 (class 2606 OID 41266)
-- Name: event_person event_fk; Type: FK CONSTRAINT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.event_person
    ADD CONSTRAINT event_fk FOREIGN KEY (event_id) REFERENCES notebook2.event(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2117 (class 2606 OID 41271)
-- Name: event_person person_fk; Type: FK CONSTRAINT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.event_person
    ADD CONSTRAINT person_fk FOREIGN KEY (person_id) REFERENCES notebook2.person(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2118 (class 2606 OID 41276)
-- Name: telephone person_fk; Type: FK CONSTRAINT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.telephone
    ADD CONSTRAINT person_fk FOREIGN KEY (person_id) REFERENCES notebook2.person(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2115 (class 2606 OID 41281)
-- Name: contact telephone_fk; Type: FK CONSTRAINT; Schema: notebook2; Owner: postgres
--

ALTER TABLE ONLY notebook2.contact
    ADD CONSTRAINT telephone_fk FOREIGN KEY (telephone_id) REFERENCES notebook2.telephone(id) ON UPDATE CASCADE ON DELETE CASCADE;


-- Completed on 2021-05-17 15:14:43

--
-- PostgreSQL database dump complete
--

