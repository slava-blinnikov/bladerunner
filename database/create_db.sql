/*
CREATE DATABASE bladerunner
WITH	OWNER = developer
	ENCODING = 'UTF8'
	TABLESPACE = pg_default
	LC_COLLATE = 'ru_RU.UTF-8'
	LC_CTYPE = 'ru_RU.UTF-8'
	CONNECTION LIMIT = -1;
*/

CREATE SCHEMA meta;
CREATE SCHEMA archive;

CREATE ROLE dispatcher LOGIN UNENCRYPTED PASSWORD 'qwerty' NOINHERIT CREATEROLE VALID UNTIL 'infinity';
GRANT USAGE ON SCHEMA meta TO dispatcher;
GRANT USAGE ON SCHEMA archive TO dispatcher;

CREATE ROLE crews NOSUPERUSER NOINHERIT NOCREATEDB NOCREATEROLE NOREPLICATION;
GRANT USAGE ON SCHEMA meta TO GROUP crews;
GRANT USAGE ON SCHEMA archive TO GROUP crews;

CREATE ROLE members NOSUPERUSER NOINHERIT NOCREATEDB NOCREATEROLE NOREPLICATION;
GRANT USAGE ON SCHEMA meta TO GROUP members;
GRANT USAGE ON SCHEMA archive TO GROUP members;

CREATE TABLE meta.crews
(
	crew serial not null,
	rolname name,
	usrname text not null,
	avatar json,
	last_use timestamp not null default now(),
	CONSTRAINT pk_meta_crews PRIMARY KEY (crew),
	CONSTRAINT uk_meta_crews_rolname UNIQUE (rolname),
	CONSTRAINT uk_meta_crews_usrname UNIQUE (usrname),
	CONSTRAINT ck_meta_crews_usrname_sintax CHECK (length(usrname) > 0)
)
WITH (OIDS=TRUE);
GRANT USAGE ON SEQUENCE meta.crews_crew_seq TO dispatcher;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE meta.crews TO dispatcher;

CREATE TABLE meta.members
(
	crew integer not null,
	member serial not null,
	rolname name,
	usrname text not null,
	note text,
	avatar json,
	tracking_period integer NOT NULL DEFAULT 5,
	perm_tracking boolean not null default true,
	perm_recording boolean not null default false,
	recording boolean not null default false,
	CONSTRAINT pk_meta_members PRIMARY KEY (crew, member),
	CONSTRAINT uk_meta_members_member UNIQUE (member),
	CONSTRAINT uk_meta_members_rolname UNIQUE (rolname),
	CONSTRAINT uk_meta_members_usrname UNIQUE (crew, usrname),
	CONSTRAINT ck_meta_members_name_sintax CHECK (length(usrname) > 0),
	CONSTRAINT fk_meta_members_crew FOREIGN KEY (crew) REFERENCES meta.crews(crew) MATCH SIMPLE ON UPDATE CASCADE ON DELETE CASCADE
)
WITH (OIDS=TRUE);
GRANT USAGE ON SEQUENCE meta.members_member_seq TO dispatcher;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE meta.members TO dispatcher;

CREATE TABLE archive.schedule
(
	member integer not null,
	recording_start timestamp not null,
	recording_finish timestamp not null,
	CONSTRAINT fk_archive_schedule_member FOREIGN KEY (member) REFERENCES meta.members(member) MATCH SIMPLE ON UPDATE CASCADE ON DELETE CASCADE,
	CONSTRAINT ck_archive_schedule_period CHECK (recording_start < recording_finish)
)
WITH (OIDS=FALSE);
GRANT SELECT, INSERT, UPDATE, DELETE ON TABLE archive.schedule TO dispatcher;

CREATE TABLE archive.current_locations
(
	crew integer not null,
	member integer not null,
	"time" timestamp not null,
	latitude real not null,
	longitude real not null,
	speed real not null,
	gps boolean not null default true,
	CONSTRAINT uk_archive_current_locations_member UNIQUE (member),
	CONSTRAINT fk_archive_current_locations_member FOREIGN KEY (member) REFERENCES meta.members(member) MATCH SIMPLE ON UPDATE CASCADE ON DELETE CASCADE
)
WITH (OIDS=TRUE);
GRANT SELECT, INSERT, UPDATE, DELETE ON TABLE archive.current_locations TO dispatcher;

CREATE TABLE archive.locations
(
	crew integer not null,
	member integer not null,
	"time" timestamp not null,
	latitude real not null,
	longitude real not null,
	speed real not null,
	gps boolean not null,
	CONSTRAINT pk_archive_locations PRIMARY KEY (member, "time"),
	CONSTRAINT fk_archive_locations_crew FOREIGN KEY (crew) REFERENCES meta.crews(crew) MATCH SIMPLE ON UPDATE CASCADE ON DELETE CASCADE,
	CONSTRAINT fk_archive_locations_member FOREIGN KEY (member) REFERENCES meta.members(member) MATCH SIMPLE ON UPDATE CASCADE ON DELETE CASCADE
)
WITH (OIDS=FALSE);
GRANT SELECT, INSERT, UPDATE, DELETE ON TABLE archive.locations TO dispatcher;


--###############################################################################
CREATE OR REPLACE FUNCTION meta.v_crews()
  RETURNS SETOF meta.crews AS
$BODY$
	declare	role_name name;
		crew_id integer;
		r meta.crews;
	begin
		role_name := session_user;
		select into crew_id crew from meta.crews where rolname = role_name;
		if crew_id is null then
			select into crew_id crew from meta.members where rolname = role_name;
		end if;
		if crew_id is not null then
			select into r * from meta.crews where crew = crew_id;
			return next r;
		end if;
		return;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
ALTER FUNCTION meta.v_crews() OWNER TO dispatcher;
REVOKE EXECUTE ON FUNCTION meta.v_crews() FROM public;
GRANT EXECUTE ON FUNCTION meta.v_crews() TO GROUP crews;
GRANT EXECUTE ON FUNCTION meta.v_crews() TO GROUP members;

CREATE OR REPLACE FUNCTION meta.v_members()
  RETURNS SETOF meta.members AS
$BODY$
	declare	role_name name;
		crew_id integer;
		r meta.members;
	begin
		role_name := session_user;
		select into crew_id crew from meta.crews where rolname = role_name;
		if crew_id is not null then
			for r in
				select * from meta.members where crew = crew_id
			loop
				return next r;
			end loop;
		else
			for r in
				select * from meta.members where rolname = role_name
			loop
				return next r;
			end loop;
		end if;
		return;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
ALTER FUNCTION meta.v_members() OWNER TO dispatcher;
REVOKE EXECUTE ON FUNCTION meta.v_members() FROM public;
GRANT EXECUTE ON FUNCTION meta.v_members() TO GROUP crews;
GRANT EXECUTE ON FUNCTION meta.v_members() TO GROUP members;


--###############################################################################
CREATE OR REPLACE FUNCTION meta.on_insert_crew()
  RETURNS trigger AS
$BODY$
	begin
		update meta.crews set rolname = 'crew' || NEW.oid where crew = NEW.crew;
		return new;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
GRANT EXECUTE ON FUNCTION meta.on_insert_crew() TO dispatcher;
create trigger tg_insert_crew after insert on meta.crews for each row execute procedure meta.on_insert_crew();

CREATE OR REPLACE FUNCTION meta.on_insert_member()
  RETURNS trigger AS
$BODY$
	begin
		update meta.members set rolname = 'member' || NEW.oid where member = NEW.member;
		return new;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
GRANT EXECUTE ON FUNCTION meta.on_insert_member() TO dispatcher;
create trigger tg_insert_member after insert on meta.members for each row execute procedure meta.on_insert_member();


--##################################  crews edit #############################################
CREATE OR REPLACE FUNCTION meta.add_crew(_name text, _password text)
  RETURNS integer AS
$BODY$
	declare	crew_id integer;
		role_name name;
		create_user_query text;
	begin
		insert into meta.crews(usrname) values (_name) returning crew into crew_id;
		if crew_id is not null then
			select into role_name rolname from meta.crews where crew = crew_id;
			create_user_query := 'CREATE ROLE "' || role_name || '" LOGIN UNENCRYPTED PASSWORD ''' || _password || ''' INHERIT VALID UNTIL ''infinity'';';
			execute create_user_query;
			create_user_query := 'GRANT crews TO "' || role_name || '";';
			execute create_user_query;
		end if;
		return crew_id;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
REVOKE EXECUTE ON FUNCTION meta.add_crew(text, text) FROM public;
GRANT EXECUTE ON FUNCTION meta.add_crew(text, text) TO dispatcher;

CREATE OR REPLACE FUNCTION meta.update_crew(_name text, _avatar json)
  RETURNS VOID AS
$BODY$
	begin
		update meta.crews set usrname = _name, avatar = _avatar, last_use = current_timestamp where rolname = session_user;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
REVOKE EXECUTE ON FUNCTION meta.update_crew(text, json) FROM public;
ALTER FUNCTION meta.update_crew(text, json) OWNER TO dispatcher;
GRANT EXECUTE ON FUNCTION meta.update_crew(text, json) TO GROUP crews;

CREATE OR REPLACE FUNCTION meta.set_crew_password(_password text)
  RETURNS VOID AS
$BODY$
	declare	create_user_query text;
		role_name name;
		crew_id integer;
	begin
		role_name := session_user;
		select into crew_id crew from meta.crews where rolname = role_name;
		if crew_id is not null then
			create_user_query :=  'ALTER ROLE "' || role_name || '" UNENCRYPTED PASSWORD ''' || _password || '''';
			execute create_user_query;
			update meta.crews set last_use = current_timestamp where crew = crew_id;
		end if;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
REVOKE EXECUTE ON FUNCTION meta.set_crew_password(text) FROM public;
ALTER FUNCTION meta.set_crew_password(text) OWNER TO dispatcher;
GRANT EXECUTE ON FUNCTION meta.set_crew_password(text) TO GROUP crews;

CREATE OR REPLACE FUNCTION meta.remove_crew(_crew integer)
  RETURNS VOID AS
$BODY$
	declare	create_user_query text;
		crew_rolname name;
		r record;
	begin
		select into crew_rolname rolname from meta.crews where crew = _crew;
		if crew_rolname is not null then
			for r in
				select rolname from meta.members where crew = _crew
			loop
				create_user_query :=  'DROP ROLE IF EXISTS "' || r.rolname || '";';
				execute create_user_query;
			end loop;
			create_user_query :=  'DROP ROLE IF EXISTS "' || crew_rolname || '";';
			execute create_user_query;
			delete from meta.crews where crew = _crew;
		end if;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
REVOKE EXECUTE ON FUNCTION meta.remove_crew(integer) FROM public;
GRANT EXECUTE ON FUNCTION meta.remove_crew(integer) TO dispatcher;

--##################################  members edit #############################################

CREATE OR REPLACE FUNCTION meta.add_member(_name text, _password text)
  RETURNS integer AS
$BODY$
	declare	crew_id integer;
		member_id integer;
		create_user_query text;
		role_name name;
	begin
		member_id := null;
		select into crew_id crew from meta.crews where rolname = session_user;
		if crew_id is not null then
			insert into meta.members(crew, usrname) values (crew_id, _name) returning member into member_id;
			if member_id is not null then
				insert into archive.current_locations(crew, member, "time", latitude, longitude, speed, gps) values(crew_id, member_id, current_timestamp, 0, 0, 0, false);
				update meta.crews set last_use = current_timestamp where crew = crew_id;
				select into role_name rolname from meta.members where member = member_id;
				create_user_query := 'CREATE ROLE "' || role_name || '" LOGIN UNENCRYPTED PASSWORD ''' || _password || ''' INHERIT VALID UNTIL ''infinity'';';
				execute create_user_query;
				create_user_query := 'GRANT members TO "' || role_name || '";';
				execute create_user_query;
			end if;
		end if;
		return member_id;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
REVOKE EXECUTE ON FUNCTION meta.add_member(text, text) FROM public;
ALTER FUNCTION meta.add_member(text, text) OWNER TO dispatcher;
GRANT EXECUTE ON FUNCTION meta.add_member(text, text) TO GROUP crews;

CREATE OR REPLACE FUNCTION meta.update_member(_member integer, _name text, _avatar json)
  RETURNS VOID AS
$BODY$
	declare	create_user_query text;
		crew_id integer;
		mbr record;
	begin
		select into crew_id crew from meta.crews where rolname = session_user;
		if crew_id is not null then
			select into mbr crew, usrname, rolname from meta.members where member = _member;
			if (mbr.crew is not null) and (mbr.crew = crew_id) then
				update meta.members set usrname = _name, avatar = _avatar where member = _member;
				update meta.crews set last_use = current_timestamp where crew = crew_id;
			end if;
		end if;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
REVOKE EXECUTE ON FUNCTION meta.update_member(integer, text, json) FROM public;
ALTER FUNCTION meta.update_member(integer, text, json) OWNER TO dispatcher;
GRANT EXECUTE ON FUNCTION meta.update_member(integer, text, json) TO GROUP crews;

CREATE OR REPLACE FUNCTION meta.set_member_password(_member integer, _password text)
  RETURNS VOID AS
$BODY$
	declare	create_user_query text;
		mbr record;
		crew_id integer;
	begin
		select into crew_id crew from meta.crews where rolname = session_user;
		if crew_id is not null then
			select into mbr crew, rolname from meta.members where member = _member;
			if (mbr.crew = crew_id) and (mbr.rolname is not null) then
				create_user_query :=  'ALTER ROLE "' || mbr.rolname || '" UNENCRYPTED PASSWORD ''' || _password || '''';
				execute create_user_query;
				update meta.crews set last_use = current_timestamp where crew = crew_id;
			end if;
		end if;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
REVOKE EXECUTE ON FUNCTION meta.set_member_password(integer, text) FROM public;
ALTER FUNCTION meta.set_member_password(integer, text) OWNER TO dispatcher;
GRANT EXECUTE ON FUNCTION meta.set_member_password(integer, text) TO GROUP crews;

CREATE OR REPLACE FUNCTION meta.remove_member(_member integer)
  RETURNS VOID AS
$BODY$
	declare	create_user_query text;
		mbr record;
		crew_id integer;
	begin
		select into crew_id crew from meta.crews where rolname = session_user;
		if crew_id is not null then
			select into mbr crew, rolname from meta.members where member = _member;
			if (mbr.crew is not null) and (mbr.crew = crew_id) then
				delete from meta.members where member = _member;
				create_user_query :=  'DROP ROLE IF EXISTS "' || mbr.rolname || '";';
				execute create_user_query;
				update meta.crews set last_use = current_timestamp where crew = crew_id;
			end if;
		end if;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
REVOKE EXECUTE ON FUNCTION meta.remove_member(integer) FROM public;
ALTER FUNCTION meta.remove_member(integer) OWNER TO dispatcher;
GRANT EXECUTE ON FUNCTION meta.remove_member(integer) TO GROUP crews;


--##################################  locations #############################################

CREATE OR REPLACE FUNCTION meta.set_tracking_permission(_ok boolean)
  RETURNS VOID AS
$BODY$
	declare	member_id integer;
	begin
		select into member_id member from meta.members where rolname = session_user;
		if member_id is not null then
			update meta.members set perm_tracking = _ok where member = member_id;
		end if;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
ALTER FUNCTION meta.set_tracking_permission(boolean) OWNER TO dispatcher;
REVOKE EXECUTE ON FUNCTION meta.set_tracking_permission(boolean) FROM public;
GRANT EXECUTE ON FUNCTION meta.set_tracking_permission(boolean) TO GROUP members;

CREATE OR REPLACE FUNCTION meta.set_recording_permission(_ok boolean)
  RETURNS boolean AS
$BODY$
	declare	member_id integer;
		is_recording boolean;
	begin
		select into member_id member from meta.members where rolname = session_user;
		if member_id is not null then
			if _ok then
				select into is_recording exists(select member from archive.schedule where member = member_id and recording_start >= current_timestamp and current_timestamp <= recording_finish);
			else
				is_recording := false;
			end if;
			update meta.members set
				perm_recording = _ok,
				recording = is_recording
			where member = member_id;
			return is_recording;
		end if;
		return null;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
ALTER FUNCTION meta.set_recording_permission(boolean) OWNER TO dispatcher;
REVOKE EXECUTE ON FUNCTION meta.set_recording_permission(boolean) FROM public;
GRANT EXECUTE ON FUNCTION meta.set_recording_permission(boolean) TO GROUP members;

CREATE OR REPLACE FUNCTION archive.on_edit_current_location()
  RETURNS trigger AS
$BODY$
	declare	is_recording boolean;
	begin
		select into is_recording recording from meta.members where member = new.member;
		if is_recording then
			insert into archive.locations(crew, member, "time", latitude, longitude, speed, gps) values (new.crew, new.member, new.time, new.latitude, new.longitude, new.speed, new.gps);
		end if;
		return new;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
GRANT EXECUTE ON FUNCTION archive.on_edit_current_location() TO dispatcher;
create trigger tg_current_location_edit after update on archive.current_locations for each row execute procedure archive.on_edit_current_location();

CREATE OR REPLACE FUNCTION archive.set_current_location(_time timestamp, _latitude real, _longitude real, _speed real, _gps boolean)
  RETURNS VOID AS
$BODY$
	declare	member_id integer;
	begin
		select into member_id member from meta.members where rolname = session_user;
		if member_id is not null then
			update archive.current_locations set  "time" =_time, latitude = _latitude, longitude = _longitude, speed = _speed, gps = _gps
			where member = member_id;
		end if;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
ALTER FUNCTION archive.set_current_location(timestamp, real, real, real, boolean) OWNER TO dispatcher;
REVOKE EXECUTE ON FUNCTION archive.set_current_location(timestamp, real, real, real, boolean) FROM public;
GRANT EXECUTE ON FUNCTION archive.set_current_location(timestamp, real, real, real, boolean) TO GROUP members;


CREATE OR REPLACE FUNCTION archive.v_locations(_member integer, _time1 timestamp, _time2 timestamp)
  RETURNS SETOF archive.locations AS
$BODY$
	declare	crew_id integer;
		member_id integer;
		r archive.locations;
	begin
		select into crew_id crew from meta.crews where rolname = session_user;
		if crew_id is not null then
			select into member_id member from meta.members where crew = crew_id and member = _member;
			if member_id is not null then
				for r in
					select * from archive.locations where (member = _member) and ("time" between _time1 and _time2)  order by "time"
				loop
					return next r;
				end loop;
			end if;
		end if;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
ALTER FUNCTION archive.v_locations(integer, timestamp, timestamp) OWNER TO dispatcher;
REVOKE EXECUTE ON FUNCTION archive.v_locations(integer, timestamp, timestamp) FROM public;
GRANT EXECUTE ON FUNCTION archive.v_locations(integer, timestamp, timestamp) TO GROUP crews;

CREATE OR REPLACE FUNCTION archive.v_current_location(_member integer)
  RETURNS SETOF archive.current_locations AS
$BODY$
	declare	crew_id integer;
		member_id integer;
		r archive.current_locations;
	begin
		select into crew_id crew from meta.crews where rolname = session_user;
		if crew_id is not null then
			select into member_id member from meta.members where crew = crew_id and member = _member;
			if member_id is not null then
				select into r * from archive.current_locations where member = _member;
				return next r;
			end if;
		end if;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
ALTER FUNCTION archive.v_current_location(integer) OWNER TO dispatcher;
REVOKE EXECUTE ON FUNCTION archive.v_current_location(integer) FROM public;
GRANT EXECUTE ON FUNCTION archive.v_current_location(integer) TO GROUP crews;


--##################################  schedule #############################################
CREATE OR REPLACE FUNCTION archive.v_schedule(_member integer)
  RETURNS SETOF archive.schedule AS
$BODY$
	declare	crew_id integer;
		member_id integer;
		r archive.schedule;
	begin
		select into crew_id crew from meta.crews where rolname = session_user;
		if crew_id is not null then
			select into member_id member from meta.members where crew = crew_id and member = _member;
			if member_id is not null then
				for r in
					select * from archive.schedule where member = _member order by recording_start
				loop
					return next r;
				end loop;
			end if;
		end if;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
ALTER FUNCTION archive.v_schedule(integer) OWNER TO dispatcher;
REVOKE EXECUTE ON FUNCTION archive.v_schedule(integer) FROM public;
GRANT EXECUTE ON FUNCTION archive.v_schedule(integer) TO GROUP crews;

CREATE OR REPLACE FUNCTION archive.schedule_recording()
  RETURNS VOID AS
$BODY$
	begin
		delete from archive.schedule where recording_finish < current_timestamp;
		update meta.members as mbr set recording = perm_recording and
		exists(select member from archive.schedule as sdl where sdl.member = mbr.member and sdl.recording_start <= current_timestamp and current_timestamp <= sdl.recording_finish);
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
REVOKE EXECUTE ON FUNCTION archive.schedule_recording() FROM public;
GRANT EXECUTE ON FUNCTION archive.schedule_recording() TO dispatcher;

/*
set session authorization 'dispatcher';
select meta.add_crew('blinnikov', 'qwerty');
select meta.add_crew('test', 'qwerty');
select remove_crew(1);
select remove_crew(2);
*/
/*
set session authorization 'crew49247';
select * from meta.v_crews();
select meta.add_member('slava', 'qwerty');
select meta.add_member('elvira', 'qwerty');
select * from meta.v_members();
select * from archive.v_locations(1, '2015-03-01'::timestamp, '2015-12-31'::timestamp);
select * from archive.v_current_location(1);
*/
/*
set session authorization 'member49251';
select archive.set_current_location(current_timestamp::timestamp, 56.6::real, 59.4::real, 48.7::real, true);
*/
/*
DROP DATABASE IF EXISTS bladerunner;
DROP ROLE IF EXISTS dispatcher;
DROP ROLE IF EXISTS crews;
DROP ROLE IF EXISTS members;
*/