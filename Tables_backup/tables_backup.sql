--
-- PostgreSQL database dump
--

-- Dumped from database version 9.5.16
-- Dumped by pg_dump version 9.5.16

-- Started on 2019-06-28 22:17:17

SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'SQL_ASCII';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 2317 (class 0 OID 0)
-- Dependencies: 2316
-- Name: DATABASE postgres; Type: COMMENT; Schema: -; Owner: postgres
--

COMMENT ON DATABASE postgres IS 'default administrative connection database';


--
-- TOC entry 8 (class 2615 OID 16393)
-- Name: pgagent; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA pgagent;


ALTER SCHEMA pgagent OWNER TO postgres;

--
-- TOC entry 2318 (class 0 OID 0)
-- Dependencies: 8
-- Name: SCHEMA pgagent; Type: COMMENT; Schema: -; Owner: postgres
--

COMMENT ON SCHEMA pgagent IS 'pgAgent system tables';


--
-- TOC entry 2 (class 3079 OID 12355)
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- TOC entry 2321 (class 0 OID 0)
-- Dependencies: 2
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


--
-- TOC entry 1 (class 3079 OID 16384)
-- Name: adminpack; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS adminpack WITH SCHEMA pg_catalog;


--
-- TOC entry 2322 (class 0 OID 0)
-- Dependencies: 1
-- Name: EXTENSION adminpack; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION adminpack IS 'administrative functions for PostgreSQL';


--
-- TOC entry 211 (class 1255 OID 16394)
-- Name: pga_exception_trigger(); Type: FUNCTION; Schema: pgagent; Owner: postgres
--

CREATE FUNCTION pgagent.pga_exception_trigger() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE

    v_jobid int4 := 0;

BEGIN

     IF TG_OP = 'DELETE' THEN

        SELECT INTO v_jobid jscjobid FROM pgagent.pga_schedule WHERE jscid = OLD.jexscid;

        -- update pga_job from remaining schedules
        -- the actual calculation of jobnextrun will be performed in the trigger
        UPDATE pgagent.pga_job
           SET jobnextrun = NULL
         WHERE jobenabled AND jobid = v_jobid;
        RETURN OLD;
    ELSE

        SELECT INTO v_jobid jscjobid FROM pgagent.pga_schedule WHERE jscid = NEW.jexscid;

        UPDATE pgagent.pga_job
           SET jobnextrun = NULL
         WHERE jobenabled AND jobid = v_jobid;
        RETURN NEW;
    END IF;
END;
$$;


ALTER FUNCTION pgagent.pga_exception_trigger() OWNER TO postgres;

--
-- TOC entry 2323 (class 0 OID 0)
-- Dependencies: 211
-- Name: FUNCTION pga_exception_trigger(); Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON FUNCTION pgagent.pga_exception_trigger() IS 'Update the job''s next run time whenever an exception changes';


--
-- TOC entry 212 (class 1255 OID 16395)
-- Name: pga_is_leap_year(smallint); Type: FUNCTION; Schema: pgagent; Owner: postgres
--

CREATE FUNCTION pgagent.pga_is_leap_year(smallint) RETURNS boolean
    LANGUAGE plpgsql IMMUTABLE
    AS $_$
BEGIN
    IF $1 % 4 != 0 THEN
        RETURN FALSE;
    END IF;

    IF $1 % 100 != 0 THEN
        RETURN TRUE;
    END IF;

    RETURN $1 % 400 = 0;
END;
$_$;


ALTER FUNCTION pgagent.pga_is_leap_year(smallint) OWNER TO postgres;

--
-- TOC entry 2324 (class 0 OID 0)
-- Dependencies: 212
-- Name: FUNCTION pga_is_leap_year(smallint); Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON FUNCTION pgagent.pga_is_leap_year(smallint) IS 'Returns TRUE if $1 is a leap year';


--
-- TOC entry 225 (class 1255 OID 16396)
-- Name: pga_job_trigger(); Type: FUNCTION; Schema: pgagent; Owner: postgres
--

CREATE FUNCTION pgagent.pga_job_trigger() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    IF NEW.jobenabled THEN
        IF NEW.jobnextrun IS NULL THEN
             SELECT INTO NEW.jobnextrun
                    MIN(pgagent.pga_next_schedule(jscid, jscstart, jscend, jscminutes, jschours, jscweekdays, jscmonthdays, jscmonths))
               FROM pgagent.pga_schedule
              WHERE jscenabled AND jscjobid=OLD.jobid;
        END IF;
    ELSE
        NEW.jobnextrun := NULL;
    END IF;
    RETURN NEW;
END;
$$;


ALTER FUNCTION pgagent.pga_job_trigger() OWNER TO postgres;

--
-- TOC entry 2325 (class 0 OID 0)
-- Dependencies: 225
-- Name: FUNCTION pga_job_trigger(); Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON FUNCTION pgagent.pga_job_trigger() IS 'Update the job''s next run time.';


--
-- TOC entry 210 (class 1255 OID 16397)
-- Name: pga_next_schedule(integer, timestamp with time zone, timestamp with time zone, boolean[], boolean[], boolean[], boolean[], boolean[]); Type: FUNCTION; Schema: pgagent; Owner: postgres
--

CREATE FUNCTION pgagent.pga_next_schedule(integer, timestamp with time zone, timestamp with time zone, boolean[], boolean[], boolean[], boolean[], boolean[]) RETURNS timestamp with time zone
    LANGUAGE plpgsql
    AS $_$
DECLARE
    jscid           ALIAS FOR $1;
    jscstart        ALIAS FOR $2;
    jscend          ALIAS FOR $3;
    jscminutes      ALIAS FOR $4;
    jschours        ALIAS FOR $5;
    jscweekdays     ALIAS FOR $6;
    jscmonthdays    ALIAS FOR $7;
    jscmonths       ALIAS FOR $8;

    nextrun         timestamp := '1970-01-01 00:00:00-00';
    runafter        timestamp := '1970-01-01 00:00:00-00';

    bingo            bool := FALSE;
    gotit            bool := FALSE;
    foundval        bool := FALSE;
    daytweak        bool := FALSE;
    minutetweak        bool := FALSE;

    i                int2 := 0;
    d                int2 := 0;

    nextminute        int2 := 0;
    nexthour        int2 := 0;
    nextday            int2 := 0;
    nextmonth       int2 := 0;
    nextyear        int2 := 0;


BEGIN
    -- No valid start date has been specified
    IF jscstart IS NULL THEN RETURN NULL; END IF;

    -- The schedule is past its end date
    IF jscend IS NOT NULL AND jscend < now() THEN RETURN NULL; END IF;

    -- Get the time to find the next run after. It will just be the later of
    -- now() + 1m and the start date for the time being, however, we might want to
    -- do more complex things using this value in the future.
    IF date_trunc('MINUTE', jscstart) > date_trunc('MINUTE', (now() + '1 Minute'::interval)) THEN
        runafter := date_trunc('MINUTE', jscstart);
    ELSE
        runafter := date_trunc('MINUTE', (now() + '1 Minute'::interval));
    END IF;

    --
    -- Enter a loop, generating next run timestamps until we find one
    -- that falls on the required weekday, and is not matched by an exception
    --

    WHILE bingo = FALSE LOOP

        --
        -- Get the next run year
        --
        nextyear := date_part('YEAR', runafter);

        --
        -- Get the next run month
        --
        nextmonth := date_part('MONTH', runafter);
        gotit := FALSE;
        FOR i IN (nextmonth) .. 12 LOOP
            IF jscmonths[i] = TRUE THEN
                nextmonth := i;
                gotit := TRUE;
                foundval := TRUE;
                EXIT;
            END IF;
        END LOOP;
        IF gotit = FALSE THEN
            FOR i IN 1 .. (nextmonth - 1) LOOP
                IF jscmonths[i] = TRUE THEN
                    nextmonth := i;

                    -- Wrap into next year
                    nextyear := nextyear + 1;
                    gotit := TRUE;
                    foundval := TRUE;
                    EXIT;
                END IF;
           END LOOP;
        END IF;

        --
        -- Get the next run day
        --
        -- If the year, or month have incremented, get the lowest day,
        -- otherwise look for the next day matching or after today.
        IF (nextyear > date_part('YEAR', runafter) OR nextmonth > date_part('MONTH', runafter)) THEN
            nextday := 1;
            FOR i IN 1 .. 32 LOOP
                IF jscmonthdays[i] = TRUE THEN
                    nextday := i;
                    foundval := TRUE;
                    EXIT;
                END IF;
            END LOOP;
        ELSE
            nextday := date_part('DAY', runafter);
            gotit := FALSE;
            FOR i IN nextday .. 32 LOOP
                IF jscmonthdays[i] = TRUE THEN
                    nextday := i;
                    gotit := TRUE;
                    foundval := TRUE;
                    EXIT;
                END IF;
            END LOOP;
            IF gotit = FALSE THEN
                FOR i IN 1 .. (nextday - 1) LOOP
                    IF jscmonthdays[i] = TRUE THEN
                        nextday := i;

                        -- Wrap into next month
                        IF nextmonth = 12 THEN
                            nextyear := nextyear + 1;
                            nextmonth := 1;
                        ELSE
                            nextmonth := nextmonth + 1;
                        END IF;
                        gotit := TRUE;
                        foundval := TRUE;
                        EXIT;
                    END IF;
                END LOOP;
            END IF;
        END IF;

        -- Was the last day flag selected?
        IF nextday = 32 THEN
            IF nextmonth = 1 THEN
                nextday := 31;
            ELSIF nextmonth = 2 THEN
                IF pgagent.pga_is_leap_year(nextyear) = TRUE THEN
                    nextday := 29;
                ELSE
                    nextday := 28;
                END IF;
            ELSIF nextmonth = 3 THEN
                nextday := 31;
            ELSIF nextmonth = 4 THEN
                nextday := 30;
            ELSIF nextmonth = 5 THEN
                nextday := 31;
            ELSIF nextmonth = 6 THEN
                nextday := 30;
            ELSIF nextmonth = 7 THEN
                nextday := 31;
            ELSIF nextmonth = 8 THEN
                nextday := 31;
            ELSIF nextmonth = 9 THEN
                nextday := 30;
            ELSIF nextmonth = 10 THEN
                nextday := 31;
            ELSIF nextmonth = 11 THEN
                nextday := 30;
            ELSIF nextmonth = 12 THEN
                nextday := 31;
            END IF;
        END IF;

        --
        -- Get the next run hour
        --
        -- If the year, month or day have incremented, get the lowest hour,
        -- otherwise look for the next hour matching or after the current one.
        IF (nextyear > date_part('YEAR', runafter) OR nextmonth > date_part('MONTH', runafter) OR nextday > date_part('DAY', runafter) OR daytweak = TRUE) THEN
            nexthour := 0;
            FOR i IN 1 .. 24 LOOP
                IF jschours[i] = TRUE THEN
                    nexthour := i - 1;
                    foundval := TRUE;
                    EXIT;
                END IF;
            END LOOP;
        ELSE
            nexthour := date_part('HOUR', runafter);
            gotit := FALSE;
            FOR i IN (nexthour + 1) .. 24 LOOP
                IF jschours[i] = TRUE THEN
                    nexthour := i - 1;
                    gotit := TRUE;
                    foundval := TRUE;
                    EXIT;
                END IF;
            END LOOP;
            IF gotit = FALSE THEN
                FOR i IN 1 .. nexthour LOOP
                    IF jschours[i] = TRUE THEN
                        nexthour := i - 1;

                        -- Wrap into next month
                        IF (nextmonth = 1 OR nextmonth = 3 OR nextmonth = 5 OR nextmonth = 7 OR nextmonth = 8 OR nextmonth = 10 OR nextmonth = 12) THEN
                            d = 31;
                        ELSIF (nextmonth = 4 OR nextmonth = 6 OR nextmonth = 9 OR nextmonth = 11) THEN
                            d = 30;
                        ELSE
                            IF pgagent.pga_is_leap_year(nextyear) = TRUE THEN
                                d := 29;
                            ELSE
                                d := 28;
                            END IF;
                        END IF;

                        IF nextday = d THEN
                            nextday := 1;
                            IF nextmonth = 12 THEN
                                nextyear := nextyear + 1;
                                nextmonth := 1;
                            ELSE
                                nextmonth := nextmonth + 1;
                            END IF;
                        ELSE
                            nextday := nextday + 1;
                        END IF;

                        gotit := TRUE;
                        foundval := TRUE;
                        EXIT;
                    END IF;
                END LOOP;
            END IF;
        END IF;

        --
        -- Get the next run minute
        --
        -- If the year, month day or hour have incremented, get the lowest minute,
        -- otherwise look for the next minute matching or after the current one.
        IF (nextyear > date_part('YEAR', runafter) OR nextmonth > date_part('MONTH', runafter) OR nextday > date_part('DAY', runafter) OR nexthour > date_part('HOUR', runafter) OR daytweak = TRUE) THEN
            nextminute := 0;
            IF minutetweak = TRUE THEN
        d := 1;
            ELSE
        d := date_part('YEAR', runafter)::int2;
            END IF;
            FOR i IN d .. 60 LOOP
                IF jscminutes[i] = TRUE THEN
                    nextminute := i - 1;
                    foundval := TRUE;
                    EXIT;
                END IF;
            END LOOP;
        ELSE
            nextminute := date_part('MINUTE', runafter);
            gotit := FALSE;
            FOR i IN (nextminute + 1) .. 60 LOOP
                IF jscminutes[i] = TRUE THEN
                    nextminute := i - 1;
                    gotit := TRUE;
                    foundval := TRUE;
                    EXIT;
                END IF;
            END LOOP;
            IF gotit = FALSE THEN
                FOR i IN 1 .. nextminute LOOP
                    IF jscminutes[i] = TRUE THEN
                        nextminute := i - 1;

                        -- Wrap into next hour
                        IF (nextmonth = 1 OR nextmonth = 3 OR nextmonth = 5 OR nextmonth = 7 OR nextmonth = 8 OR nextmonth = 10 OR nextmonth = 12) THEN
                            d = 31;
                        ELSIF (nextmonth = 4 OR nextmonth = 6 OR nextmonth = 9 OR nextmonth = 11) THEN
                            d = 30;
                        ELSE
                            IF pgagent.pga_is_leap_year(nextyear) = TRUE THEN
                                d := 29;
                            ELSE
                                d := 28;
                            END IF;
                        END IF;

                        IF nexthour = 23 THEN
                            nexthour = 0;
                            IF nextday = d THEN
                                nextday := 1;
                                IF nextmonth = 12 THEN
                                    nextyear := nextyear + 1;
                                    nextmonth := 1;
                                ELSE
                                    nextmonth := nextmonth + 1;
                                END IF;
                            ELSE
                                nextday := nextday + 1;
                            END IF;
                        ELSE
                            nexthour := nexthour + 1;
                        END IF;

                        gotit := TRUE;
                        foundval := TRUE;
                        EXIT;
                    END IF;
                END LOOP;
            END IF;
        END IF;

        -- Build the result, and check it is not the same as runafter - this may
        -- happen if all array entries are set to false. In this case, add a minute.

        nextrun := (nextyear::varchar || '-'::varchar || nextmonth::varchar || '-' || nextday::varchar || ' ' || nexthour::varchar || ':' || nextminute::varchar)::timestamptz;

        IF nextrun = runafter AND foundval = FALSE THEN
                nextrun := nextrun + INTERVAL '1 Minute';
        END IF;

        -- If the result is past the end date, exit.
        IF nextrun > jscend THEN
            RETURN NULL;
        END IF;

        -- Check to ensure that the nextrun time is actually still valid. Its
        -- possible that wrapped values may have carried the nextrun onto an
        -- invalid time or date.
        IF ((jscminutes = '{f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f}' OR jscminutes[date_part('MINUTE', nextrun) + 1] = TRUE) AND
            (jschours = '{f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f}' OR jschours[date_part('HOUR', nextrun) + 1] = TRUE) AND
            (jscmonthdays = '{f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f}' OR jscmonthdays[date_part('DAY', nextrun)] = TRUE OR
            (jscmonthdays = '{f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,t}' AND
             ((date_part('MONTH', nextrun) IN (1,3,5,7,8,10,12) AND date_part('DAY', nextrun) = 31) OR
              (date_part('MONTH', nextrun) IN (4,6,9,11) AND date_part('DAY', nextrun) = 30) OR
              (date_part('MONTH', nextrun) = 2 AND ((pgagent.pga_is_leap_year(date_part('DAY', nextrun)::int2) AND date_part('DAY', nextrun) = 29) OR date_part('DAY', nextrun) = 28))))) AND
            (jscmonths = '{f,f,f,f,f,f,f,f,f,f,f,f}' OR jscmonths[date_part('MONTH', nextrun)] = TRUE)) THEN


            -- Now, check to see if the nextrun time found is a) on an acceptable
            -- weekday, and b) not matched by an exception. If not, set
            -- runafter = nextrun and try again.

            -- Check for a wildcard weekday
            gotit := FALSE;
            FOR i IN 1 .. 7 LOOP
                IF jscweekdays[i] = TRUE THEN
                    gotit := TRUE;
                    EXIT;
                END IF;
            END LOOP;

            -- OK, is the correct weekday selected, or a wildcard?
            IF (jscweekdays[date_part('DOW', nextrun) + 1] = TRUE OR gotit = FALSE) THEN

                -- Check for exceptions
                SELECT INTO d jexid FROM pgagent.pga_exception WHERE jexscid = jscid AND ((jexdate = nextrun::date AND jextime = nextrun::time) OR (jexdate = nextrun::date AND jextime IS NULL) OR (jexdate IS NULL AND jextime = nextrun::time));
                IF FOUND THEN
                    -- Nuts - found an exception. Increment the time and try again
                    runafter := nextrun + INTERVAL '1 Minute';
                    bingo := FALSE;
                    minutetweak := TRUE;
            daytweak := FALSE;
                ELSE
                    bingo := TRUE;
                END IF;
            ELSE
                -- We're on the wrong week day - increment a day and try again.
                runafter := nextrun + INTERVAL '1 Day';
                bingo := FALSE;
                minutetweak := FALSE;
                daytweak := TRUE;
            END IF;

        ELSE
            runafter := nextrun + INTERVAL '1 Minute';
            bingo := FALSE;
            minutetweak := TRUE;
        daytweak := FALSE;
        END IF;

    END LOOP;

    RETURN nextrun;
END;
$_$;


ALTER FUNCTION pgagent.pga_next_schedule(integer, timestamp with time zone, timestamp with time zone, boolean[], boolean[], boolean[], boolean[], boolean[]) OWNER TO postgres;

--
-- TOC entry 2326 (class 0 OID 0)
-- Dependencies: 210
-- Name: FUNCTION pga_next_schedule(integer, timestamp with time zone, timestamp with time zone, boolean[], boolean[], boolean[], boolean[], boolean[]); Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON FUNCTION pgagent.pga_next_schedule(integer, timestamp with time zone, timestamp with time zone, boolean[], boolean[], boolean[], boolean[], boolean[]) IS 'Calculates the next runtime for a given schedule';


--
-- TOC entry 226 (class 1255 OID 16399)
-- Name: pga_schedule_trigger(); Type: FUNCTION; Schema: pgagent; Owner: postgres
--

CREATE FUNCTION pgagent.pga_schedule_trigger() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    IF TG_OP = 'DELETE' THEN
        -- update pga_job from remaining schedules
        -- the actual calculation of jobnextrun will be performed in the trigger
        UPDATE pgagent.pga_job
           SET jobnextrun = NULL
         WHERE jobenabled AND jobid=OLD.jscjobid;
        RETURN OLD;
    ELSE
        UPDATE pgagent.pga_job
           SET jobnextrun = NULL
         WHERE jobenabled AND jobid=NEW.jscjobid;
        RETURN NEW;
    END IF;
END;
$$;


ALTER FUNCTION pgagent.pga_schedule_trigger() OWNER TO postgres;

--
-- TOC entry 2327 (class 0 OID 0)
-- Dependencies: 226
-- Name: FUNCTION pga_schedule_trigger(); Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON FUNCTION pgagent.pga_schedule_trigger() IS 'Update the job''s next run time whenever a schedule changes';


--
-- TOC entry 227 (class 1255 OID 16400)
-- Name: pgagent_schema_version(); Type: FUNCTION; Schema: pgagent; Owner: postgres
--

CREATE FUNCTION pgagent.pgagent_schema_version() RETURNS smallint
    LANGUAGE plpgsql
    AS $$
BEGIN
    -- RETURNS PGAGENT MAJOR VERSION
    -- WE WILL CHANGE THE MAJOR VERSION, ONLY IF THERE IS A SCHEMA CHANGE
    RETURN 3;
END;
$$;


ALTER FUNCTION pgagent.pgagent_schema_version() OWNER TO postgres;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- TOC entry 183 (class 1259 OID 16401)
-- Name: pga_exception; Type: TABLE; Schema: pgagent; Owner: postgres
--

CREATE TABLE pgagent.pga_exception (
    jexid integer NOT NULL,
    jexscid integer NOT NULL,
    jexdate date,
    jextime time without time zone
);


ALTER TABLE pgagent.pga_exception OWNER TO postgres;

--
-- TOC entry 184 (class 1259 OID 16404)
-- Name: pga_exception_jexid_seq; Type: SEQUENCE; Schema: pgagent; Owner: postgres
--

CREATE SEQUENCE pgagent.pga_exception_jexid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE pgagent.pga_exception_jexid_seq OWNER TO postgres;

--
-- TOC entry 2328 (class 0 OID 0)
-- Dependencies: 184
-- Name: pga_exception_jexid_seq; Type: SEQUENCE OWNED BY; Schema: pgagent; Owner: postgres
--

ALTER SEQUENCE pgagent.pga_exception_jexid_seq OWNED BY pgagent.pga_exception.jexid;


--
-- TOC entry 185 (class 1259 OID 16406)
-- Name: pga_job; Type: TABLE; Schema: pgagent; Owner: postgres
--

CREATE TABLE pgagent.pga_job (
    jobid integer NOT NULL,
    jobjclid integer NOT NULL,
    jobname text NOT NULL,
    jobdesc text DEFAULT ''::text NOT NULL,
    jobhostagent text DEFAULT ''::text NOT NULL,
    jobenabled boolean DEFAULT true NOT NULL,
    jobcreated timestamp with time zone DEFAULT now() NOT NULL,
    jobchanged timestamp with time zone DEFAULT now() NOT NULL,
    jobagentid integer,
    jobnextrun timestamp with time zone,
    joblastrun timestamp with time zone
);


ALTER TABLE pgagent.pga_job OWNER TO postgres;

--
-- TOC entry 2329 (class 0 OID 0)
-- Dependencies: 185
-- Name: TABLE pga_job; Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON TABLE pgagent.pga_job IS 'Job main entry';


--
-- TOC entry 2330 (class 0 OID 0)
-- Dependencies: 185
-- Name: COLUMN pga_job.jobagentid; Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON COLUMN pgagent.pga_job.jobagentid IS 'Agent that currently executes this job.';


--
-- TOC entry 186 (class 1259 OID 16417)
-- Name: pga_job_jobid_seq; Type: SEQUENCE; Schema: pgagent; Owner: postgres
--

CREATE SEQUENCE pgagent.pga_job_jobid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE pgagent.pga_job_jobid_seq OWNER TO postgres;

--
-- TOC entry 2331 (class 0 OID 0)
-- Dependencies: 186
-- Name: pga_job_jobid_seq; Type: SEQUENCE OWNED BY; Schema: pgagent; Owner: postgres
--

ALTER SEQUENCE pgagent.pga_job_jobid_seq OWNED BY pgagent.pga_job.jobid;


--
-- TOC entry 187 (class 1259 OID 16419)
-- Name: pga_jobagent; Type: TABLE; Schema: pgagent; Owner: postgres
--

CREATE TABLE pgagent.pga_jobagent (
    jagpid integer NOT NULL,
    jaglogintime timestamp with time zone DEFAULT now() NOT NULL,
    jagstation text NOT NULL
);


ALTER TABLE pgagent.pga_jobagent OWNER TO postgres;

--
-- TOC entry 2332 (class 0 OID 0)
-- Dependencies: 187
-- Name: TABLE pga_jobagent; Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON TABLE pgagent.pga_jobagent IS 'Active job agents';


--
-- TOC entry 188 (class 1259 OID 16426)
-- Name: pga_jobclass; Type: TABLE; Schema: pgagent; Owner: postgres
--

CREATE TABLE pgagent.pga_jobclass (
    jclid integer NOT NULL,
    jclname text NOT NULL
);


ALTER TABLE pgagent.pga_jobclass OWNER TO postgres;

--
-- TOC entry 2333 (class 0 OID 0)
-- Dependencies: 188
-- Name: TABLE pga_jobclass; Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON TABLE pgagent.pga_jobclass IS 'Job classification';


--
-- TOC entry 189 (class 1259 OID 16432)
-- Name: pga_jobclass_jclid_seq; Type: SEQUENCE; Schema: pgagent; Owner: postgres
--

CREATE SEQUENCE pgagent.pga_jobclass_jclid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE pgagent.pga_jobclass_jclid_seq OWNER TO postgres;

--
-- TOC entry 2334 (class 0 OID 0)
-- Dependencies: 189
-- Name: pga_jobclass_jclid_seq; Type: SEQUENCE OWNED BY; Schema: pgagent; Owner: postgres
--

ALTER SEQUENCE pgagent.pga_jobclass_jclid_seq OWNED BY pgagent.pga_jobclass.jclid;


--
-- TOC entry 190 (class 1259 OID 16434)
-- Name: pga_joblog; Type: TABLE; Schema: pgagent; Owner: postgres
--

CREATE TABLE pgagent.pga_joblog (
    jlgid integer NOT NULL,
    jlgjobid integer NOT NULL,
    jlgstatus character(1) DEFAULT 'r'::bpchar NOT NULL,
    jlgstart timestamp with time zone DEFAULT now() NOT NULL,
    jlgduration interval,
    CONSTRAINT pga_joblog_jlgstatus_check CHECK ((jlgstatus = ANY (ARRAY['r'::bpchar, 's'::bpchar, 'f'::bpchar, 'i'::bpchar, 'd'::bpchar])))
);


ALTER TABLE pgagent.pga_joblog OWNER TO postgres;

--
-- TOC entry 2335 (class 0 OID 0)
-- Dependencies: 190
-- Name: TABLE pga_joblog; Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON TABLE pgagent.pga_joblog IS 'Job run logs.';


--
-- TOC entry 2336 (class 0 OID 0)
-- Dependencies: 190
-- Name: COLUMN pga_joblog.jlgstatus; Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON COLUMN pgagent.pga_joblog.jlgstatus IS 'Status of job: r=running, s=successfully finished, f=failed, i=no steps to execute, d=aborted';


--
-- TOC entry 191 (class 1259 OID 16440)
-- Name: pga_joblog_jlgid_seq; Type: SEQUENCE; Schema: pgagent; Owner: postgres
--

CREATE SEQUENCE pgagent.pga_joblog_jlgid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE pgagent.pga_joblog_jlgid_seq OWNER TO postgres;

--
-- TOC entry 2337 (class 0 OID 0)
-- Dependencies: 191
-- Name: pga_joblog_jlgid_seq; Type: SEQUENCE OWNED BY; Schema: pgagent; Owner: postgres
--

ALTER SEQUENCE pgagent.pga_joblog_jlgid_seq OWNED BY pgagent.pga_joblog.jlgid;


--
-- TOC entry 192 (class 1259 OID 16442)
-- Name: pga_jobstep; Type: TABLE; Schema: pgagent; Owner: postgres
--

CREATE TABLE pgagent.pga_jobstep (
    jstid integer NOT NULL,
    jstjobid integer NOT NULL,
    jstname text NOT NULL,
    jstdesc text DEFAULT ''::text NOT NULL,
    jstenabled boolean DEFAULT true NOT NULL,
    jstkind character(1) NOT NULL,
    jstcode text NOT NULL,
    jstconnstr text DEFAULT ''::text NOT NULL,
    jstdbname name DEFAULT ''::name NOT NULL,
    jstonerror character(1) DEFAULT 'f'::bpchar NOT NULL,
    jscnextrun timestamp with time zone,
    CONSTRAINT pga_jobstep_check CHECK ((((jstconnstr <> ''::text) AND (jstkind = 's'::bpchar)) OR ((jstconnstr = ''::text) AND ((jstkind = 'b'::bpchar) OR (jstdbname <> ''::name))))),
    CONSTRAINT pga_jobstep_check1 CHECK ((((jstdbname <> ''::name) AND (jstkind = 's'::bpchar)) OR ((jstdbname = ''::name) AND ((jstkind = 'b'::bpchar) OR (jstconnstr <> ''::text))))),
    CONSTRAINT pga_jobstep_jstkind_check CHECK ((jstkind = ANY (ARRAY['b'::bpchar, 's'::bpchar]))),
    CONSTRAINT pga_jobstep_jstonerror_check CHECK ((jstonerror = ANY (ARRAY['f'::bpchar, 's'::bpchar, 'i'::bpchar])))
);


ALTER TABLE pgagent.pga_jobstep OWNER TO postgres;

--
-- TOC entry 2338 (class 0 OID 0)
-- Dependencies: 192
-- Name: TABLE pga_jobstep; Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON TABLE pgagent.pga_jobstep IS 'Job step to be executed';


--
-- TOC entry 2339 (class 0 OID 0)
-- Dependencies: 192
-- Name: COLUMN pga_jobstep.jstkind; Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON COLUMN pgagent.pga_jobstep.jstkind IS 'Kind of jobstep: s=sql, b=batch';


--
-- TOC entry 2340 (class 0 OID 0)
-- Dependencies: 192
-- Name: COLUMN pga_jobstep.jstonerror; Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON COLUMN pgagent.pga_jobstep.jstonerror IS 'What to do if step returns an error: f=fail the job, s=mark step as succeeded and continue, i=mark as fail but ignore it and proceed';


--
-- TOC entry 193 (class 1259 OID 16457)
-- Name: pga_jobstep_jstid_seq; Type: SEQUENCE; Schema: pgagent; Owner: postgres
--

CREATE SEQUENCE pgagent.pga_jobstep_jstid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE pgagent.pga_jobstep_jstid_seq OWNER TO postgres;

--
-- TOC entry 2341 (class 0 OID 0)
-- Dependencies: 193
-- Name: pga_jobstep_jstid_seq; Type: SEQUENCE OWNED BY; Schema: pgagent; Owner: postgres
--

ALTER SEQUENCE pgagent.pga_jobstep_jstid_seq OWNED BY pgagent.pga_jobstep.jstid;


--
-- TOC entry 194 (class 1259 OID 16459)
-- Name: pga_jobsteplog; Type: TABLE; Schema: pgagent; Owner: postgres
--

CREATE TABLE pgagent.pga_jobsteplog (
    jslid integer NOT NULL,
    jsljlgid integer NOT NULL,
    jsljstid integer NOT NULL,
    jslstatus character(1) DEFAULT 'r'::bpchar NOT NULL,
    jslresult integer,
    jslstart timestamp with time zone DEFAULT now() NOT NULL,
    jslduration interval,
    jsloutput text,
    CONSTRAINT pga_jobsteplog_jslstatus_check CHECK ((jslstatus = ANY (ARRAY['r'::bpchar, 's'::bpchar, 'i'::bpchar, 'f'::bpchar, 'd'::bpchar])))
);


ALTER TABLE pgagent.pga_jobsteplog OWNER TO postgres;

--
-- TOC entry 2342 (class 0 OID 0)
-- Dependencies: 194
-- Name: TABLE pga_jobsteplog; Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON TABLE pgagent.pga_jobsteplog IS 'Job step run logs.';


--
-- TOC entry 2343 (class 0 OID 0)
-- Dependencies: 194
-- Name: COLUMN pga_jobsteplog.jslstatus; Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON COLUMN pgagent.pga_jobsteplog.jslstatus IS 'Status of job step: r=running, s=successfully finished,  f=failed stopping job, i=ignored failure, d=aborted';


--
-- TOC entry 2344 (class 0 OID 0)
-- Dependencies: 194
-- Name: COLUMN pga_jobsteplog.jslresult; Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON COLUMN pgagent.pga_jobsteplog.jslresult IS 'Return code of job step';


--
-- TOC entry 195 (class 1259 OID 16468)
-- Name: pga_jobsteplog_jslid_seq; Type: SEQUENCE; Schema: pgagent; Owner: postgres
--

CREATE SEQUENCE pgagent.pga_jobsteplog_jslid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE pgagent.pga_jobsteplog_jslid_seq OWNER TO postgres;

--
-- TOC entry 2345 (class 0 OID 0)
-- Dependencies: 195
-- Name: pga_jobsteplog_jslid_seq; Type: SEQUENCE OWNED BY; Schema: pgagent; Owner: postgres
--

ALTER SEQUENCE pgagent.pga_jobsteplog_jslid_seq OWNED BY pgagent.pga_jobsteplog.jslid;


--
-- TOC entry 196 (class 1259 OID 16470)
-- Name: pga_schedule; Type: TABLE; Schema: pgagent; Owner: postgres
--

CREATE TABLE pgagent.pga_schedule (
    jscid integer NOT NULL,
    jscjobid integer NOT NULL,
    jscname text NOT NULL,
    jscdesc text DEFAULT ''::text NOT NULL,
    jscenabled boolean DEFAULT true NOT NULL,
    jscstart timestamp with time zone DEFAULT now() NOT NULL,
    jscend timestamp with time zone,
    jscminutes boolean[] DEFAULT '{f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f}'::boolean[] NOT NULL,
    jschours boolean[] DEFAULT '{f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f}'::boolean[] NOT NULL,
    jscweekdays boolean[] DEFAULT '{f,f,f,f,f,f,f}'::boolean[] NOT NULL,
    jscmonthdays boolean[] DEFAULT '{f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f}'::boolean[] NOT NULL,
    jscmonths boolean[] DEFAULT '{f,f,f,f,f,f,f,f,f,f,f,f}'::boolean[] NOT NULL,
    CONSTRAINT pga_schedule_jschours_size CHECK ((array_upper(jschours, 1) = 24)),
    CONSTRAINT pga_schedule_jscminutes_size CHECK ((array_upper(jscminutes, 1) = 60)),
    CONSTRAINT pga_schedule_jscmonthdays_size CHECK ((array_upper(jscmonthdays, 1) = 32)),
    CONSTRAINT pga_schedule_jscmonths_size CHECK ((array_upper(jscmonths, 1) = 12)),
    CONSTRAINT pga_schedule_jscweekdays_size CHECK ((array_upper(jscweekdays, 1) = 7))
);


ALTER TABLE pgagent.pga_schedule OWNER TO postgres;

--
-- TOC entry 2346 (class 0 OID 0)
-- Dependencies: 196
-- Name: TABLE pga_schedule; Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON TABLE pgagent.pga_schedule IS 'Job schedule exceptions';


--
-- TOC entry 197 (class 1259 OID 16489)
-- Name: pga_schedule_jscid_seq; Type: SEQUENCE; Schema: pgagent; Owner: postgres
--

CREATE SEQUENCE pgagent.pga_schedule_jscid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE pgagent.pga_schedule_jscid_seq OWNER TO postgres;

--
-- TOC entry 2347 (class 0 OID 0)
-- Dependencies: 197
-- Name: pga_schedule_jscid_seq; Type: SEQUENCE OWNED BY; Schema: pgagent; Owner: postgres
--

ALTER SEQUENCE pgagent.pga_schedule_jscid_seq OWNED BY pgagent.pga_schedule.jscid;


--
-- TOC entry 198 (class 1259 OID 16506)
-- Name: VKR; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."VKR" (
    names character(10),
    pk_id integer NOT NULL,
    uniq_id smallint,
    asd integer NOT NULL,
    date date
);


ALTER TABLE public."VKR" OWNER TO postgres;

--
-- TOC entry 203 (class 1259 OID 16690)
-- Name: VKR_asd_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public."VKR_asd_seq"
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public."VKR_asd_seq" OWNER TO postgres;

--
-- TOC entry 2348 (class 0 OID 0)
-- Dependencies: 203
-- Name: VKR_asd_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public."VKR_asd_seq" OWNED BY public."VKR".asd;


--
-- TOC entry 199 (class 1259 OID 16509)
-- Name: VKR_pk_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public."VKR_pk_id_seq"
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public."VKR_pk_id_seq" OWNER TO postgres;

--
-- TOC entry 2349 (class 0 OID 0)
-- Dependencies: 199
-- Name: VKR_pk_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public."VKR_pk_id_seq" OWNED BY public."VKR".pk_id;


--
-- TOC entry 206 (class 1259 OID 16710)
-- Name: contacts; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.contacts (
    cl_telephone character varying(11),
    cl_info character varying(50),
    fk_cl_telephone integer,
    contact_list_id integer NOT NULL
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
-- TOC entry 2350 (class 0 OID 0)
-- Dependencies: 209
-- Name: contacts_contact_list_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.contacts_contact_list_id_seq OWNED BY public.contacts.contact_list_id;


--
-- TOC entry 200 (class 1259 OID 16516)
-- Name: official_tel; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.official_tel (
    tel_num character(20),
    service_name character(25),
    of_t_id integer NOT NULL
);


ALTER TABLE public.official_tel OWNER TO postgres;

--
-- TOC entry 201 (class 1259 OID 16519)
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
-- TOC entry 2351 (class 0 OID 0)
-- Dependencies: 201
-- Name: official_tel_of_t_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.official_tel_of_t_id_seq OWNED BY public.official_tel.of_t_id;


--
-- TOC entry 205 (class 1259 OID 16707)
-- Name: owners_tel; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.owners_tel (
    fk_telephone_zk integer,
    telephone_num character varying(11) NOT NULL,
    telephone_id integer NOT NULL
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
-- TOC entry 2352 (class 0 OID 0)
-- Dependencies: 208
-- Name: owners_tel_telephone_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.owners_tel_telephone_id_seq OWNED BY public.owners_tel.telephone_id;


--
-- TOC entry 202 (class 1259 OID 16521)
-- Name: testing; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.testing (
    testing_id integer NOT NULL,
    name character(12),
    lastname character(20),
    "Un_col" integer
);


ALTER TABLE public.testing OWNER TO postgres;

--
-- TOC entry 204 (class 1259 OID 16704)
-- Name: zk; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.zk (
    lastname character varying(20),
    name character varying(10),
    mid_name character varying(20),
    birth_date date,
    date_add date,
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
    time_add character varying(10),
    dop_info character varying(50),
    check_for character varying(25),
    zk_id integer NOT NULL
);


ALTER TABLE public.zk OWNER TO postgres;

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
-- TOC entry 2353 (class 0 OID 0)
-- Dependencies: 207
-- Name: zk_zk_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.zk_zk_id_seq OWNED BY public.zk.zk_id;


--
-- TOC entry 2071 (class 2604 OID 16524)
-- Name: jexid; Type: DEFAULT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_exception ALTER COLUMN jexid SET DEFAULT nextval('pgagent.pga_exception_jexid_seq'::regclass);


--
-- TOC entry 2077 (class 2604 OID 16525)
-- Name: jobid; Type: DEFAULT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_job ALTER COLUMN jobid SET DEFAULT nextval('pgagent.pga_job_jobid_seq'::regclass);


--
-- TOC entry 2079 (class 2604 OID 16526)
-- Name: jclid; Type: DEFAULT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_jobclass ALTER COLUMN jclid SET DEFAULT nextval('pgagent.pga_jobclass_jclid_seq'::regclass);


--
-- TOC entry 2082 (class 2604 OID 16527)
-- Name: jlgid; Type: DEFAULT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_joblog ALTER COLUMN jlgid SET DEFAULT nextval('pgagent.pga_joblog_jlgid_seq'::regclass);


--
-- TOC entry 2089 (class 2604 OID 16528)
-- Name: jstid; Type: DEFAULT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_jobstep ALTER COLUMN jstid SET DEFAULT nextval('pgagent.pga_jobstep_jstid_seq'::regclass);


--
-- TOC entry 2096 (class 2604 OID 16529)
-- Name: jslid; Type: DEFAULT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_jobsteplog ALTER COLUMN jslid SET DEFAULT nextval('pgagent.pga_jobsteplog_jslid_seq'::regclass);


--
-- TOC entry 2106 (class 2604 OID 16530)
-- Name: jscid; Type: DEFAULT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_schedule ALTER COLUMN jscid SET DEFAULT nextval('pgagent.pga_schedule_jscid_seq'::regclass);


--
-- TOC entry 2112 (class 2604 OID 16532)
-- Name: pk_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."VKR" ALTER COLUMN pk_id SET DEFAULT nextval('public."VKR_pk_id_seq"'::regclass);


--
-- TOC entry 2113 (class 2604 OID 16692)
-- Name: asd; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."VKR" ALTER COLUMN asd SET DEFAULT nextval('public."VKR_asd_seq"'::regclass);


--
-- TOC entry 2117 (class 2604 OID 16737)
-- Name: contact_list_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contacts ALTER COLUMN contact_list_id SET DEFAULT nextval('public.contacts_contact_list_id_seq'::regclass);


--
-- TOC entry 2114 (class 2604 OID 16534)
-- Name: of_t_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.official_tel ALTER COLUMN of_t_id SET DEFAULT nextval('public.official_tel_of_t_id_seq'::regclass);


--
-- TOC entry 2116 (class 2604 OID 16724)
-- Name: telephone_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.owners_tel ALTER COLUMN telephone_id SET DEFAULT nextval('public.owners_tel_telephone_id_seq'::regclass);


--
-- TOC entry 2115 (class 2604 OID 16715)
-- Name: zk_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zk ALTER COLUMN zk_id SET DEFAULT nextval('public.zk_zk_id_seq'::regclass);


--
-- TOC entry 2284 (class 0 OID 16401)
-- Dependencies: 183
-- Data for Name: pga_exception; Type: TABLE DATA; Schema: pgagent; Owner: postgres
--



--
-- TOC entry 2354 (class 0 OID 0)
-- Dependencies: 184
-- Name: pga_exception_jexid_seq; Type: SEQUENCE SET; Schema: pgagent; Owner: postgres
--

SELECT pg_catalog.setval('pgagent.pga_exception_jexid_seq', 1, false);


--
-- TOC entry 2286 (class 0 OID 16406)
-- Dependencies: 185
-- Data for Name: pga_job; Type: TABLE DATA; Schema: pgagent; Owner: postgres
--



--
-- TOC entry 2355 (class 0 OID 0)
-- Dependencies: 186
-- Name: pga_job_jobid_seq; Type: SEQUENCE SET; Schema: pgagent; Owner: postgres
--

SELECT pg_catalog.setval('pgagent.pga_job_jobid_seq', 1, false);


--
-- TOC entry 2288 (class 0 OID 16419)
-- Dependencies: 187
-- Data for Name: pga_jobagent; Type: TABLE DATA; Schema: pgagent; Owner: postgres
--

INSERT INTO pgagent.pga_jobagent (jagpid, jaglogintime, jagstation) VALUES (3752, '2019-03-31 17:30:37.879698+03', 'Yavlados.Dlink');


--
-- TOC entry 2289 (class 0 OID 16426)
-- Dependencies: 188
-- Data for Name: pga_jobclass; Type: TABLE DATA; Schema: pgagent; Owner: postgres
--

INSERT INTO pgagent.pga_jobclass (jclid, jclname) VALUES (1, 'Routine Maintenance');
INSERT INTO pgagent.pga_jobclass (jclid, jclname) VALUES (2, 'Data Import');
INSERT INTO pgagent.pga_jobclass (jclid, jclname) VALUES (3, 'Data Export');
INSERT INTO pgagent.pga_jobclass (jclid, jclname) VALUES (4, 'Data Summarisation');
INSERT INTO pgagent.pga_jobclass (jclid, jclname) VALUES (5, 'Miscellaneous');


--
-- TOC entry 2356 (class 0 OID 0)
-- Dependencies: 189
-- Name: pga_jobclass_jclid_seq; Type: SEQUENCE SET; Schema: pgagent; Owner: postgres
--

SELECT pg_catalog.setval('pgagent.pga_jobclass_jclid_seq', 5, true);


--
-- TOC entry 2291 (class 0 OID 16434)
-- Dependencies: 190
-- Data for Name: pga_joblog; Type: TABLE DATA; Schema: pgagent; Owner: postgres
--



--
-- TOC entry 2357 (class 0 OID 0)
-- Dependencies: 191
-- Name: pga_joblog_jlgid_seq; Type: SEQUENCE SET; Schema: pgagent; Owner: postgres
--

SELECT pg_catalog.setval('pgagent.pga_joblog_jlgid_seq', 1, false);


--
-- TOC entry 2293 (class 0 OID 16442)
-- Dependencies: 192
-- Data for Name: pga_jobstep; Type: TABLE DATA; Schema: pgagent; Owner: postgres
--



--
-- TOC entry 2358 (class 0 OID 0)
-- Dependencies: 193
-- Name: pga_jobstep_jstid_seq; Type: SEQUENCE SET; Schema: pgagent; Owner: postgres
--

SELECT pg_catalog.setval('pgagent.pga_jobstep_jstid_seq', 1, false);


--
-- TOC entry 2295 (class 0 OID 16459)
-- Dependencies: 194
-- Data for Name: pga_jobsteplog; Type: TABLE DATA; Schema: pgagent; Owner: postgres
--



--
-- TOC entry 2359 (class 0 OID 0)
-- Dependencies: 195
-- Name: pga_jobsteplog_jslid_seq; Type: SEQUENCE SET; Schema: pgagent; Owner: postgres
--

SELECT pg_catalog.setval('pgagent.pga_jobsteplog_jslid_seq', 1, false);


--
-- TOC entry 2297 (class 0 OID 16470)
-- Dependencies: 196
-- Data for Name: pga_schedule; Type: TABLE DATA; Schema: pgagent; Owner: postgres
--



--
-- TOC entry 2360 (class 0 OID 0)
-- Dependencies: 197
-- Name: pga_schedule_jscid_seq; Type: SEQUENCE SET; Schema: pgagent; Owner: postgres
--

SELECT pg_catalog.setval('pgagent.pga_schedule_jscid_seq', 1, false);


--
-- TOC entry 2299 (class 0 OID 16506)
-- Dependencies: 198
-- Data for Name: VKR; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public."VKR" (names, pk_id, uniq_id, asd, date) VALUES ('123       ', 1, 1, 1, NULL);
INSERT INTO public."VKR" (names, pk_id, uniq_id, asd, date) VALUES ('asd       ', 2, 2, 2, NULL);
INSERT INTO public."VKR" (names, pk_id, uniq_id, asd, date) VALUES ('aqwe      ', 5, 3, 3, NULL);


--
-- TOC entry 2361 (class 0 OID 0)
-- Dependencies: 203
-- Name: VKR_asd_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."VKR_asd_seq"', 3, true);


--
-- TOC entry 2362 (class 0 OID 0)
-- Dependencies: 199
-- Name: VKR_pk_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."VKR_pk_id_seq"', 5, true);


--
-- TOC entry 2307 (class 0 OID 16710)
-- Dependencies: 206
-- Data for Name: contacts; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id) VALUES ('89511445544', 'ТЕСТ', 2, 1);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id) VALUES ('891111155', 'ТЕСТ', 1, 2);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id) VALUES ('89225475587', 'МАМА', 3, 3);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id) VALUES ('8914578888', 'Друг', 2, 4);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id) VALUES ('88888888', 'Контакт1', 3, 5);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id) VALUES ('88888888', 'Контакт2', 2, 6);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id) VALUES ('7777777', 'Контакт3', 3, 7);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id) VALUES ('7777777', 'Контакт4', 2, 8);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id) VALUES ('89258558882', 'Кон3', 2, 9);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id) VALUES ('891111111', '2 ВАРИАНТ', 1, 10);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id) VALUES ('88888888', 'Контакт для теста', 5, 11);
INSERT INTO public.contacts (cl_telephone, cl_info, fk_cl_telephone, contact_list_id) VALUES ('89258558882', 'Владик', 5, 12);


--
-- TOC entry 2363 (class 0 OID 0)
-- Dependencies: 209
-- Name: contacts_contact_list_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.contacts_contact_list_id_seq', 12, true);


--
-- TOC entry 2301 (class 0 OID 16516)
-- Dependencies: 200
-- Data for Name: official_tel; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.official_tel (tel_num, service_name, of_t_id) VALUES ('8(499)-720-56-22    ', 'Главное отделение ФСБ    ', 1);
INSERT INTO public.official_tel (tel_num, service_name, of_t_id) VALUES ('892885522           ', 'йцуйк                    ', 3);


--
-- TOC entry 2364 (class 0 OID 0)
-- Dependencies: 201
-- Name: official_tel_of_t_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.official_tel_of_t_id_seq', 3, true);


--
-- TOC entry 2306 (class 0 OID 16707)
-- Dependencies: 205
-- Data for Name: owners_tel; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.owners_tel (fk_telephone_zk, telephone_num, telephone_id) VALUES (1, '89258558882', 1);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_num, telephone_id) VALUES (2, '895558888', 2);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_num, telephone_id) VALUES (1, '8914578888', 3);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_num, telephone_id) VALUES (2, '891111111', 4);
INSERT INTO public.owners_tel (fk_telephone_zk, telephone_num, telephone_id) VALUES (3, '7777777', 5);


--
-- TOC entry 2365 (class 0 OID 0)
-- Dependencies: 208
-- Name: owners_tel_telephone_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.owners_tel_telephone_id_seq', 5, true);


--
-- TOC entry 2303 (class 0 OID 16521)
-- Dependencies: 202
-- Data for Name: testing; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.testing (testing_id, name, lastname, "Un_col") VALUES (15, 'qwe         ', '124                 ', 23);
INSERT INTO public.testing (testing_id, name, lastname, "Un_col") VALUES (1, 'asd         ', 'adf                 ', 2);
INSERT INTO public.testing (testing_id, name, lastname, "Un_col") VALUES (4, 'fgh         ', 'arifov              ', 5);
INSERT INTO public.testing (testing_id, name, lastname, "Un_col") VALUES (7, '4534        ', '786743              ', 8);
INSERT INTO public.testing (testing_id, name, lastname, "Un_col") VALUES (9, '453         ', 'qwe1313             ', 9);
INSERT INTO public.testing (testing_id, name, lastname, "Un_col") VALUES (10, '123qead     ', 'zc1213              ', 10);
INSERT INTO public.testing (testing_id, name, lastname, "Un_col") VALUES (14, '123dasd     ', 'asdqwe              ', 15);
INSERT INTO public.testing (testing_id, name, lastname, "Un_col") VALUES (3, 'asd         ', 'zvbnmm              ', 4);
INSERT INTO public.testing (testing_id, name, lastname, "Un_col") VALUES (12, 'test_error  ', 'test_error          ', 12);
INSERT INTO public.testing (testing_id, name, lastname, "Un_col") VALUES (5, 'qweacax     ', 'qrqweasd            ', 6);
INSERT INTO public.testing (testing_id, name, lastname, "Un_col") VALUES (13, 'ячс         ', 'фыв                 ', 20);
INSERT INTO public.testing (testing_id, name, lastname, "Un_col") VALUES (6, 'asdqczvz    ', 'gsdadad             ', 7);
INSERT INTO public.testing (testing_id, name, lastname, "Un_col") VALUES (0, '            ', '                    ', 0);


--
-- TOC entry 2305 (class 0 OID 16704)
-- Dependencies: 204
-- Data for Name: zk; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.zk (lastname, name, mid_name, birth_date, date_add, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, time_add, dop_info, check_for, zk_id) VALUES ('Семибратченко', 'Всеволод', 'Максимович', NULL, '2019-06-17', '', '', '', '', '', '', '', '', '', '', '10:14:23', '', '', 10);
INSERT INTO public.zk (lastname, name, mid_name, birth_date, date_add, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, time_add, dop_info, check_for, zk_id) VALUES ('Румянцев', 'Евгений', 'Петрович', NULL, '2019-06-17', '', '', '', '', '', '', '', '', '', '', '10:14:54', '', '', 11);
INSERT INTO public.zk (lastname, name, mid_name, birth_date, date_add, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, time_add, dop_info, check_for, zk_id) VALUES ('Житенев', 'Владислав', 'Германович', '1997-10-23', '2017-05-22', 'Москва', 'улица', 'дом', '1', '200', 'Тверь', 'Горького', '23', 'а', '200', NULL, 'ТЕСТ', 'ФСБ', 1);
INSERT INTO public.zk (lastname, name, mid_name, birth_date, date_add, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, time_add, dop_info, check_for, zk_id) VALUES ('Коротеева', 'Мария', 'Олеговна', NULL, '2018-06-17', '', '', '', '', '', '', '', '', '', '', '10:12:04', '', '', 6);
INSERT INTO public.zk (lastname, name, mid_name, birth_date, date_add, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, time_add, dop_info, check_for, zk_id) VALUES ('Александрова', 'Ирина', 'Владимировна', NULL, '2018-06-23', '', '', '', '', '', '', '', '', '', '', '10:12:37', '', '', 7);
INSERT INTO public.zk (lastname, name, mid_name, birth_date, date_add, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, time_add, dop_info, check_for, zk_id) VALUES ('Кузьмин', 'Леонтий', 'Игнатович', NULL, '2017-05-24', '', '', '', '', '', '', '', '', '', '', '10:13:21', '', '', 8);
INSERT INTO public.zk (lastname, name, mid_name, birth_date, date_add, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, time_add, dop_info, check_for, zk_id) VALUES ('Короткин', 'Геннадий', 'Сергеевич', NULL, '2015-06-17', '', '', '', '', '', '', '', '', '', '', '10:13:53', '', '', 9);
INSERT INTO public.zk (lastname, name, mid_name, birth_date, date_add, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, time_add, dop_info, check_for, zk_id) VALUES ('Сумин', 'Сергей', 'Иванович', '1995-03-22', '2016-07-12', '', '', '', '', '', '', '', '', '', '', '23:31:22', '', 'ФСБ', 2);
INSERT INTO public.zk (lastname, name, mid_name, birth_date, date_add, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, time_add, dop_info, check_for, zk_id) VALUES ('Савина', 'Елизавета', 'Ивановна', '2000-12-02', '2012-05-17', 'Москва', 'Лесная', '', '', '', 'Москва', 'Лесная', '', '', '', '14:00:06', '', 'МВД', 3);
INSERT INTO public.zk (lastname, name, mid_name, birth_date, date_add, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, time_add, dop_info, check_for, zk_id) VALUES ('Жеглов', 'Евгений', 'Петрович', '2012-02-04', '2013-04-02', '', '', '', '', '', '', '', '', '', '', '10:11:26', '', '', 4);
INSERT INTO public.zk (lastname, name, mid_name, birth_date, date_add, reg_city, reg_street, reg_home, reg_corp, reg_flat, liv_city, liv_street, liv_home, liv_corp, liv_flat, time_add, dop_info, check_for, zk_id) VALUES ('Васнецов', 'Олег', 'Иванович', '1990-01-13', '2019-06-17', '', '', '', '', '', '', '', '', '', '', '10:11:45', '', 'ФСКН', 5);


--
-- TOC entry 2366 (class 0 OID 0)
-- Dependencies: 207
-- Name: zk_zk_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.zk_zk_id_seq', 11, true);


--
-- TOC entry 2121 (class 2606 OID 16538)
-- Name: pga_exception_pkey; Type: CONSTRAINT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_exception
    ADD CONSTRAINT pga_exception_pkey PRIMARY KEY (jexid);


--
-- TOC entry 2123 (class 2606 OID 16540)
-- Name: pga_job_pkey; Type: CONSTRAINT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_job
    ADD CONSTRAINT pga_job_pkey PRIMARY KEY (jobid);


--
-- TOC entry 2125 (class 2606 OID 16542)
-- Name: pga_jobagent_pkey; Type: CONSTRAINT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_jobagent
    ADD CONSTRAINT pga_jobagent_pkey PRIMARY KEY (jagpid);


--
-- TOC entry 2128 (class 2606 OID 16544)
-- Name: pga_jobclass_pkey; Type: CONSTRAINT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_jobclass
    ADD CONSTRAINT pga_jobclass_pkey PRIMARY KEY (jclid);


--
-- TOC entry 2131 (class 2606 OID 16546)
-- Name: pga_joblog_pkey; Type: CONSTRAINT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_joblog
    ADD CONSTRAINT pga_joblog_pkey PRIMARY KEY (jlgid);


--
-- TOC entry 2134 (class 2606 OID 16548)
-- Name: pga_jobstep_pkey; Type: CONSTRAINT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_jobstep
    ADD CONSTRAINT pga_jobstep_pkey PRIMARY KEY (jstid);


--
-- TOC entry 2137 (class 2606 OID 16550)
-- Name: pga_jobsteplog_pkey; Type: CONSTRAINT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_jobsteplog
    ADD CONSTRAINT pga_jobsteplog_pkey PRIMARY KEY (jslid);


--
-- TOC entry 2140 (class 2606 OID 16552)
-- Name: pga_schedule_pkey; Type: CONSTRAINT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_schedule
    ADD CONSTRAINT pga_schedule_pkey PRIMARY KEY (jscid);


--
-- TOC entry 2156 (class 2606 OID 16742)
-- Name: PK_contact_list_id; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contacts
    ADD CONSTRAINT "PK_contact_list_id" PRIMARY KEY (contact_list_id);


--
-- TOC entry 2146 (class 2606 OID 16568)
-- Name: PK_of_t_id; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.official_tel
    ADD CONSTRAINT "PK_of_t_id" PRIMARY KEY (of_t_id);


--
-- TOC entry 2154 (class 2606 OID 16729)
-- Name: PK_telephone_id; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.owners_tel
    ADD CONSTRAINT "PK_telephone_id" PRIMARY KEY (telephone_id);


--
-- TOC entry 2152 (class 2606 OID 16721)
-- Name: PK_zk_id; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.zk
    ADD CONSTRAINT "PK_zk_id" PRIMARY KEY (zk_id);


--
-- TOC entry 2142 (class 2606 OID 16572)
-- Name: VKR_uniq_id_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."VKR"
    ADD CONSTRAINT "VKR_uniq_id_key" UNIQUE (uniq_id);


--
-- TOC entry 2144 (class 2606 OID 16576)
-- Name: pk_id_VKR; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."VKR"
    ADD CONSTRAINT "pk_id_VKR" PRIMARY KEY (pk_id);


--
-- TOC entry 2148 (class 2606 OID 16578)
-- Name: testing_Un_col_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.testing
    ADD CONSTRAINT "testing_Un_col_key" UNIQUE ("Un_col");


--
-- TOC entry 2150 (class 2606 OID 16580)
-- Name: testing_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.testing
    ADD CONSTRAINT testing_pkey PRIMARY KEY (testing_id);


--
-- TOC entry 2118 (class 1259 OID 16581)
-- Name: pga_exception_datetime; Type: INDEX; Schema: pgagent; Owner: postgres
--

CREATE UNIQUE INDEX pga_exception_datetime ON pgagent.pga_exception USING btree (jexdate, jextime);


--
-- TOC entry 2119 (class 1259 OID 16582)
-- Name: pga_exception_jexscid; Type: INDEX; Schema: pgagent; Owner: postgres
--

CREATE INDEX pga_exception_jexscid ON pgagent.pga_exception USING btree (jexscid);


--
-- TOC entry 2126 (class 1259 OID 16583)
-- Name: pga_jobclass_name; Type: INDEX; Schema: pgagent; Owner: postgres
--

CREATE UNIQUE INDEX pga_jobclass_name ON pgagent.pga_jobclass USING btree (jclname);


--
-- TOC entry 2129 (class 1259 OID 16584)
-- Name: pga_joblog_jobid; Type: INDEX; Schema: pgagent; Owner: postgres
--

CREATE INDEX pga_joblog_jobid ON pgagent.pga_joblog USING btree (jlgjobid);


--
-- TOC entry 2138 (class 1259 OID 16585)
-- Name: pga_jobschedule_jobid; Type: INDEX; Schema: pgagent; Owner: postgres
--

CREATE INDEX pga_jobschedule_jobid ON pgagent.pga_schedule USING btree (jscjobid);


--
-- TOC entry 2132 (class 1259 OID 16586)
-- Name: pga_jobstep_jobid; Type: INDEX; Schema: pgagent; Owner: postgres
--

CREATE INDEX pga_jobstep_jobid ON pgagent.pga_jobstep USING btree (jstjobid);


--
-- TOC entry 2135 (class 1259 OID 16587)
-- Name: pga_jobsteplog_jslid; Type: INDEX; Schema: pgagent; Owner: postgres
--

CREATE INDEX pga_jobsteplog_jslid ON pgagent.pga_jobsteplog USING btree (jsljlgid);


--
-- TOC entry 2167 (class 2620 OID 16588)
-- Name: pga_exception_trigger; Type: TRIGGER; Schema: pgagent; Owner: postgres
--

CREATE TRIGGER pga_exception_trigger AFTER INSERT OR DELETE OR UPDATE ON pgagent.pga_exception FOR EACH ROW EXECUTE PROCEDURE pgagent.pga_exception_trigger();


--
-- TOC entry 2367 (class 0 OID 0)
-- Dependencies: 2167
-- Name: TRIGGER pga_exception_trigger ON pga_exception; Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON TRIGGER pga_exception_trigger ON pgagent.pga_exception IS 'Update the job''s next run time whenever an exception changes';


--
-- TOC entry 2168 (class 2620 OID 16589)
-- Name: pga_job_trigger; Type: TRIGGER; Schema: pgagent; Owner: postgres
--

CREATE TRIGGER pga_job_trigger BEFORE UPDATE ON pgagent.pga_job FOR EACH ROW EXECUTE PROCEDURE pgagent.pga_job_trigger();


--
-- TOC entry 2368 (class 0 OID 0)
-- Dependencies: 2168
-- Name: TRIGGER pga_job_trigger ON pga_job; Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON TRIGGER pga_job_trigger ON pgagent.pga_job IS 'Update the job''s next run time.';


--
-- TOC entry 2169 (class 2620 OID 16590)
-- Name: pga_schedule_trigger; Type: TRIGGER; Schema: pgagent; Owner: postgres
--

CREATE TRIGGER pga_schedule_trigger AFTER INSERT OR DELETE OR UPDATE ON pgagent.pga_schedule FOR EACH ROW EXECUTE PROCEDURE pgagent.pga_schedule_trigger();


--
-- TOC entry 2369 (class 0 OID 0)
-- Dependencies: 2169
-- Name: TRIGGER pga_schedule_trigger ON pga_schedule; Type: COMMENT; Schema: pgagent; Owner: postgres
--

COMMENT ON TRIGGER pga_schedule_trigger ON pgagent.pga_schedule IS 'Update the job''s next run time whenever a schedule changes';


--
-- TOC entry 2157 (class 2606 OID 16591)
-- Name: pga_exception_jexscid_fkey; Type: FK CONSTRAINT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_exception
    ADD CONSTRAINT pga_exception_jexscid_fkey FOREIGN KEY (jexscid) REFERENCES pgagent.pga_schedule(jscid) ON UPDATE RESTRICT ON DELETE CASCADE;


--
-- TOC entry 2158 (class 2606 OID 16596)
-- Name: pga_job_jobagentid_fkey; Type: FK CONSTRAINT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_job
    ADD CONSTRAINT pga_job_jobagentid_fkey FOREIGN KEY (jobagentid) REFERENCES pgagent.pga_jobagent(jagpid) ON UPDATE RESTRICT ON DELETE SET NULL;


--
-- TOC entry 2159 (class 2606 OID 16601)
-- Name: pga_job_jobjclid_fkey; Type: FK CONSTRAINT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_job
    ADD CONSTRAINT pga_job_jobjclid_fkey FOREIGN KEY (jobjclid) REFERENCES pgagent.pga_jobclass(jclid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- TOC entry 2160 (class 2606 OID 16606)
-- Name: pga_joblog_jlgjobid_fkey; Type: FK CONSTRAINT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_joblog
    ADD CONSTRAINT pga_joblog_jlgjobid_fkey FOREIGN KEY (jlgjobid) REFERENCES pgagent.pga_job(jobid) ON UPDATE RESTRICT ON DELETE CASCADE;


--
-- TOC entry 2161 (class 2606 OID 16611)
-- Name: pga_jobstep_jstjobid_fkey; Type: FK CONSTRAINT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_jobstep
    ADD CONSTRAINT pga_jobstep_jstjobid_fkey FOREIGN KEY (jstjobid) REFERENCES pgagent.pga_job(jobid) ON UPDATE RESTRICT ON DELETE CASCADE;


--
-- TOC entry 2162 (class 2606 OID 16616)
-- Name: pga_jobsteplog_jsljlgid_fkey; Type: FK CONSTRAINT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_jobsteplog
    ADD CONSTRAINT pga_jobsteplog_jsljlgid_fkey FOREIGN KEY (jsljlgid) REFERENCES pgagent.pga_joblog(jlgid) ON UPDATE RESTRICT ON DELETE CASCADE;


--
-- TOC entry 2163 (class 2606 OID 16621)
-- Name: pga_jobsteplog_jsljstid_fkey; Type: FK CONSTRAINT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_jobsteplog
    ADD CONSTRAINT pga_jobsteplog_jsljstid_fkey FOREIGN KEY (jsljstid) REFERENCES pgagent.pga_jobstep(jstid) ON UPDATE RESTRICT ON DELETE CASCADE;


--
-- TOC entry 2164 (class 2606 OID 16626)
-- Name: pga_schedule_jscjobid_fkey; Type: FK CONSTRAINT; Schema: pgagent; Owner: postgres
--

ALTER TABLE ONLY pgagent.pga_schedule
    ADD CONSTRAINT pga_schedule_jscjobid_fkey FOREIGN KEY (jscjobid) REFERENCES pgagent.pga_job(jobid) ON UPDATE RESTRICT ON DELETE CASCADE;


--
-- TOC entry 2165 (class 2606 OID 16730)
-- Name: FK_fk_telephone_zk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.owners_tel
    ADD CONSTRAINT "FK_fk_telephone_zk" FOREIGN KEY (fk_telephone_zk) REFERENCES public.zk(zk_id);


--
-- TOC entry 2166 (class 2606 OID 16743)
-- Name: FK_owners_tel_contacts; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.contacts
    ADD CONSTRAINT "FK_owners_tel_contacts" FOREIGN KEY (fk_cl_telephone) REFERENCES public.owners_tel(telephone_id);


--
-- TOC entry 2320 (class 0 OID 0)
-- Dependencies: 9
-- Name: SCHEMA public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


-- Completed on 2019-06-28 22:17:17

--
-- PostgreSQL database dump complete
--

