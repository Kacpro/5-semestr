CREATE TABLE OSOBY
(
 ID_OSOBY INT GENERATED ALWAYS AS IDENTITY NOT NULL
, IMIE VARCHAR2(50)
, NAZWISKO VARCHAR2(50)
, PESEL VARCHAR2(11)
, KONTAKT VARCHAR2(100)
, CONSTRAINT OSOBY_PK PRIMARY KEY
 (
 ID_OSOBY
 )
 ENABLE
);

CREATE TABLE WYCIECZKI
(
 ID_WYCIECZKI INT GENERATED ALWAYS AS IDENTITY NOT NULL
, NAZWA VARCHAR2(100)
, KRAJ VARCHAR2(50)
, DATA DATE
, OPIS VARCHAR2(200)
, LICZBA_MIEJSC INT
, CONSTRAINT WYCIECZKI_PK PRIMARY KEY
 (
 ID_WYCIECZKI
 )
 ENABLE
);

CREATE TABLE REZERWACJE
(
 NR_REZERWACJI INT GENERATED ALWAYS AS IDENTITY NOT NULL
, ID_WYCIECZKI INT
, ID_OSOBY INT
, STATUS CHAR(1)
, CONSTRAINT REZERWACJE_PK PRIMARY KEY
 (
 NR_REZERWACJI
 )
 ENABLE
);

ALTER TABLE REZERWACJE
ADD CONSTRAINT REZERWACJE_FK1 FOREIGN KEY
(
 ID_OSOBY
)
REFERENCES OSOBY
(
 ID_OSOBY
)
ENABLE;

ALTER TABLE REZERWACJE
ADD CONSTRAINT REZERWACJE_FK2 FOREIGN KEY
(
 ID_WYCIECZKI
)
REFERENCES WYCIECZKI
(
 ID_WYCIECZKI
)
ENABLE;

ALTER TABLE REZERWACJE
ADD CONSTRAINT REZERWACJE_CHK1 CHECK
(status IN ('N','P','Z','A'))
ENABLE;






INSERT INTO osoby (imie, nazwisko, pesel, kontakt)
VALUES('Adam', 'Kowalski', '87665524321', 'tel: 6343');
INSERT INTO osoby (imie, nazwisko, pesel, kontakt)
VALUES('Adam', 'Mickiewicz', '87625542321', 'tel: 6623');
INSERT INTO osoby (imie, nazwisko, pesel, kontakt)
VALUES('Juliusz', 'Słowacki', '87653423521', 'tel: 6133');
INSERT INTO osoby (imie, nazwisko, pesel, kontakt)
VALUES('Jan', 'Nowak', '87654234321', 'tel: 2123');
INSERT INTO osoby (imie, nazwisko, pesel, kontakt)
VALUES('Anna', 'Wesołowska', '87615423251', 'tel: 6523');
INSERT INTO osoby (imie, nazwisko, pesel, kontakt)
VALUES('Beata', 'Kowalska', '87625425321', 'tel: 6621');
INSERT INTO osoby (imie, nazwisko, pesel, kontakt)
VALUES('Konstanty', 'Piotrowicz', '87565477321', 'tel: 1223');
INSERT INTO osoby (imie, nazwisko, pesel, kontakt)
VALUES('Krystyna', 'Janda', '87652433721', 'tel: 6654');
INSERT INTO osoby (imie, nazwisko, pesel, kontakt)
VALUES('Wojciech', 'Cejrowski', '87625447321', 'tel: 6613');
INSERT INTO osoby (imie, nazwisko, pesel, kontakt)
VALUES('Jurek', 'Killer', '87635664321', 'tel: 6625');




INSERT INTO wycieczki (nazwa, kraj, data, opis, liczba_miejsc)
VALUES ('Wycieczka do Paryza','Francja',TO_DATE('2019-01-01','YYYY-MM-DD'),'Ciekawa wycieczka ...',3);
INSERT INTO wycieczki (nazwa, kraj, data, opis, liczba_miejsc)
VALUES ('Piękny Kraków','Polska',TO_DATE('2017-02-03','YYYY-MM-DD'),'Najciekawa wycieczka ...',2);
INSERT INTO wycieczki (nazwa, kraj, data, opis, liczba_miejsc)
VALUES ('Wieliczka','Polska',TO_DATE('2019-03-03','YYYY-MM-DD'),'Zadziwiająca kopalnia ...',2);
INSERT INTO wycieczki (nazwa, kraj, data, opis, liczba_miejsc)
VALUES ('Sosnowiec','Polska',TO_DATE('2018-12-03','YYYY-MM-DD'), 'Niesamowite wrażenia ...',2);
INSERT INTO wycieczki (nazwa, kraj, data, opis, liczba_miejsc)
VALUES ('Praga','Czechy',TO_DATE('2018-10-14','YYYY-MM-DD'), 'Warto ...',3);




INSERT INTO rezerwacje(id_wycieczki, id_osoby, status)
VALUES (3,22,'P');
INSERT INTO rezerwacje(id_wycieczki, id_osoby, status)
VALUES (3,25,'Z');
INSERT INTO rezerwacje(id_wycieczki, id_osoby, status)
VALUES (5,28,'P');
INSERT INTO rezerwacje(id_wycieczki, id_osoby, status)
VALUES (5,22,'A');
INSERT INTO rezerwacje(id_wycieczki, id_osoby, status)
VALUES (5,21,'Z');
INSERT INTO rezerwacje(id_wycieczki, id_osoby, status)
VALUES (2,27,'P');
INSERT INTO rezerwacje(id_wycieczki, id_osoby, status)
VALUES (2,28,'N');
INSERT INTO rezerwacje(id_wycieczki, id_osoby, status)
VALUES (1,25,'A');
INSERT INTO rezerwacje(id_wycieczki, id_osoby, status)
VALUES (1,21,'N');
INSERT INTO rezerwacje(id_wycieczki, id_osoby, status)
VALUES (4,29,'P');
INSERT INTO rezerwacje(id_wycieczki, id_osoby, status)
VALUES (5,29,'N');


CREATE VIEW wycieczki_osoby
AS
SELECT
w.ID_WYCIECZKI,
o.ID_OSOBY,
w.KRAJ,
w.DATA,
w.NAZWA,
o.IMIE,
o.NAZWISKO,
r.STATUS
FROM WYCIECZKI w
JOIN REZERWACJE r ON w.ID_WYCIECZKI = r.ID_WYCIECZKI
JOIN OSOBY o ON r.ID_OSOBY = o.ID_OSOBY;



CREATE VIEW wycieczki_osoby_potwierdzone
AS
SELECT
w.KRAJ,
w.DATA,
w.NAZWA,
o.IMIE,
o.NAZWISKO,
r.STATUS
FROM WYCIECZKI w
JOIN REZERWACJE r ON w.ID_WYCIECZKI = r.ID_WYCIECZKI
JOIN OSOBY o ON r.ID_OSOBY = o.ID_OSOBY
WHERE r.STATUS = 'P' or r.STATUS = 'Z';



CREATE VIEW wycieczki_przyszle
AS
SELECT
w.KRAJ,
w.DATA,
w.NAZWA,
o.ID_OSOBY,
o.IMIE,
o.NAZWISKO,
r.STATUS
FROM WYCIECZKI w
JOIN REZERWACJE r ON w.ID_WYCIECZKI = r.ID_WYCIECZKI
JOIN OSOBY o ON r.ID_OSOBY = o.ID_OSOBY
WHERE w.DATA > CURRENT_DATE;



CREATE VIEW wycieczki_miejsca
AS
SELECT
w.ID_WYCIECZKI,
w.KRAJ,
w.DATA,
w.NAZWA,
w.LICZBA_MIEJSC,
w.LICZBA_MIEJSC - (select count(*) from REZERWACJE r where r.ID_WYCIECZKI = w.ID_WYCIECZKI and (r.STATUS = 'Z' or r.STATUS = 'P')) as WOLNE_MIEJSCA
FROM WYCIECZKI w;




CREATE VIEW wycieczki_dostepne
AS
SELECT
wm.KRAJ,
wm.DATA,
wm.NAZWA,
wm.LICZBA_MIEJSC,
wm.WOLNE_MIEJSCA
FROM wycieczki_miejsca wm
WHERE wm.WOLNE_MIEJSCA > 0 and wm.DATA > CURRENT_DATE;



CREATE VIEW rezerwacje_do_anulowania
AS
SELECT
r.NR_REZERWACJI,
w.NAZWA,
o.IMIE,
o.NAZWISKO,
r.STATUS
FROM rezerwacje r
JOIN wycieczki w on w.ID_WYCIECZKI = r.ID_WYCIECZKI
JOIN osoby o on o.ID_OSOBY = r.ID_OSOBY
WHERE w.DATA - CURRENT_DATE < 7 and r.STATUS = 'N' and w.DATA > CURRENT_DATE;


CREATE OR REPLACE TYPE uczestnicy_wycieczki_type
AS OBJECT
(
  KRAJ VARCHAR(30),
  DATA DATE,
  NAZWA VARCHAR(30),
  IMIE VARCHAR(30),
  NAZWISKO VARCHAR(30),
  STATUS VARCHAR(1)
);

CREATE OR REPLACE TYPE uczestnicy_wycieczki_table_type
AS TABLE OF uczestnicy_wycieczki_type;

CREATE OR REPLACE FUNCTION uczestnicy_wycieczki(wycieczka_id in NUMBER)
RETURN uczestnicy_wycieczki_table_type
PIPELINED
AS
BEGIN
  declare
    counter number;
  begin
    select count(*) into counter from wycieczki w where w.ID_WYCIECZKI = wycieczka_id;
    if counter = 0 then
      raise NO_DATA_FOUND;
    else
      FOR x in (select * from wycieczki_osoby wo where wo.ID_WYCIECZKI = wycieczka_id) LOOP
        PIPE ROW (uczestnicy_wycieczki_type(x.KRAJ, x.DATA, x.NAZWA, x.IMIE, x.NAZWISKO, x.STATUS));
      END LOOP;
    end if;
    end;
END;


select * from table(uczestnicy_wycieczki(2));
select * from wycieczki;


CREATE OR REPLACE TYPE rezerwacje_osoby_type
AS OBJECT
(
  KRAJ VARCHAR(30),
  DATA DATE,
  NAZWA VARCHAR(30),
  IMIE VARCHAR(30),
  NAZWISKO VARCHAR(30),
  STATUS VARCHAR(1)
);

CREATE OR REPLACE TYPE rezerwacje_osoby_table_type
AS TABLE OF rezerwacje_osoby_type;

CREATE OR REPLACE FUNCTION rezerwacje_osoby(osoba_id in NUMBER)
RETURN rezerwacje_osoby_table_type
PIPELINED
AS
BEGIN
  declare
    counter number;
    begin
      select count(*) into counter from osoby o where o.ID_OSOBY = osoba_id;
      if (counter = 0) then
        raise NO_DATA_FOUND;
      else
        FOR x in (select * from wycieczki_osoby wo where wo.ID_OSOBY = osoba_id) LOOP
          PIPE ROW (rezerwacje_osoby_type(x.KRAJ, x.DATA, x.NAZWA, x.IMIE, x.NAZWISKO, x.STATUS));
        END LOOP;
      end if;
    end;
END;



CREATE OR REPLACE TYPE przyszle_rezerwacje_osoby_type
AS OBJECT
(
  KRAJ VARCHAR(30),
  DATA DATE,
  NAZWA VARCHAR(30),
  IMIE VARCHAR(30),
  NAZWISKO VARCHAR(30),
  STATUS VARCHAR(1)
);

CREATE OR REPLACE TYPE przyszle_rezerwacje_osoby_table_type
AS TABLE OF przyszle_rezerwacje_osoby_type;

CREATE OR REPLACE FUNCTION przyszle_rezerwacje_osoby(osoba_id in NUMBER)
RETURN przyszle_rezerwacje_osoby_table_type
PIPELINED
AS
BEGIN
  declare
  counter number;
    begin
      select count(*) into counter from OSOBY o where o.ID_OSOBY = osoba_id;
      if (counter = 0) then
        raise NO_DATA_FOUND;
      else
        FOR x in (select * from wycieczki_przyszle wp where wp.ID_OSOBY = osoba_id) LOOP
          PIPE ROW (przyszle_rezerwacje_osoby_type(x.KRAJ, x.DATA, x.NAZWA, x.IMIE, x.NAZWISKO, x.STATUS));
        END LOOP;
      end if;
    end;
END;




CREATE OR REPLACE TYPE dostepne_wycieczki_type
AS OBJECT
(
  NAZWA VARCHAR(30),
  KRAJ VARCHAR(30),
  DATA DATE,
  OPIS VARCHAR(50),
  LICZBA_MIEJSC NUMBER
);

CREATE OR REPLACE TYPE dostepne_wycieczki_table_type
AS TABLE OF dostepne_wycieczki_type;

CREATE OR REPLACE FUNCTION dostepne_wycieczki(kraj in VARCHAR, data_od in DATE, data_do in DATE)
RETURN dostepne_wycieczki_table_type
PIPELINED
AS
BEGIN
  FOR x in (select * from wycieczki w where w.KRAJ = kraj and w.DATA >= data_od and w.DATA <= data_do) LOOP
    PIPE ROW (dostepne_wycieczki_type(x.NAZWA, x.KRAJ, x.DATA, x.OPIS, x.LICZBA_MIEJSC));
  END LOOP;
END;




create or replace procedure dodaj_rezerwacje(wycieczka_id NUMBER, osoba_id NUMBER)
is
begin
  declare
    counter1 number;
    counter2 number;
  begin
    select count(*) into counter1 from wycieczki w where w.ID_WYCIECZKI = wycieczka_id and w.DATA > CURRENT_DATE;
    select count(*) into counter2 from wycieczki_miejsca wm where wm.ID_WYCIECZKI = wycieczka_id and wm.WOLNE_MIEJSCA > 0;
    if (counter1 = 0 or  counter2 = 0) then
      raise NO_DATA_FOUND ;
    else
      insert into REZERWACJE(ID_WYCIECZKI, ID_OSOBY, STATUS)
             values(wycieczka_id, osoba_id, 'N');

      insert into REZERWACJE_LOG(ID_REZERWACJI, DATA, STATUS)
            values((select r.NR_REZERWACJI from rezerwacje r where r.ID_WYCIECZKI = wycieczka_id and r.ID_OSOBY = osoba_id and rownum = 1) , CURRENT_DATE, 'N');
      commit;
    end if;
  end;
end;

select * from rezerwacje;
select * from wycieczki;
select * from wycieczki_miejsca;
begin
  dodaj_rezerwacje(1, 22);
end;


create or replace procedure zmien_status_rezerwacji(rezerwacja_id NUMBER, nowy_status VARCHAR)
is
begin
  declare
    counter number;
  begin
    select count(*) into counter from rezerwacje r where r.NR_REZERWACJI = rezerwacja_id;
    if (counter = 0)  then
      raise NO_DATA_FOUND ;
    else
      declare
      obecny_status VARCHAR(1);
      begin
        select r.STATUS into obecny_status from REZERWACJE r where r.NR_REZERWACJI = rezerwacja_id;
        if obecny_status = 'A' then
          raise INVALID_NUMBER;
        else
          update REZERWACJE set STATUS = nowy_status where NR_REZERWACJI = rezerwacja_id;

          insert into REZERWACJE_LOG(ID_REZERWACJI, DATA, STATUS)
            values(rezerwacja_id, CURRENT_DATE, nowy_status);
          commit;
        end if;
      end;
    end if;
  end;
end;



create or replace procedure zmien_liczbe_miejsc(wycieczka_id NUMBER, nowa_liczba NUMBER)
is
begin
  declare
    counter1 number;
    counter2 number;
  begin
    select count(*) into counter1 from wycieczki w where w.ID_WYCIECZKI = wycieczka_id;
    if (counter1 = 0)  then
      raise NO_DATA_FOUND ;
    else
      select count(*) into counter2 from wycieczki_miejsca wm where wm.ID_WYCIECZKI = wycieczka_id and wm.WOLNE_MIEJSCA >= wm.LICZBA_MIEJSC - nowa_liczba;
      if (counter2 = 0) then
        raise INVALID_NUMBER;
      else
        update WYCIECZKI set LICZBA_MIEJSC = nowa_liczba where ID_WYCIECZKI = wycieczka_id;
        commit;
      end if;
    end if;
  end;
end;



CREATE TABLE REZERWACJE_LOG
(
 ID INT GENERATED ALWAYS AS IDENTITY NOT NULL
,ID_REZERWACJI INT
,DATA DATE
,STATUS VARCHAR(1)
,CONSTRAINT REZERWACJE_LOG_PK PRIMARY KEY(ID)
 ENABLE
);

ALTER TABLE REZERWACJE_LOG
ADD CONSTRAINT REZERWACJE_LOG_FK1 FOREIGN KEY(ID_REZERWACJI)
REFERENCES REZERWACJE(NR_REZERWACJI)
ENABLE;



alter table WYCIECZKI
add LICZBA_WOLNYCH_MIEJSC NUMBER;


CREATE VIEW wycieczki_miejsca2
AS
SELECT
w.ID_WYCIECZKI,
w.KRAJ,
w.DATA,
w.NAZWA,
w.LICZBA_MIEJSC,
w.LICZBA_WOLNYCH_MIEJSC
FROM WYCIECZKI w;


CREATE VIEW wycieczki_dostepne2
AS
SELECT
w.KRAJ,
w.DATA,
w.NAZWA,
w.LICZBA_MIEJSC,
w.LICZBA_WOLNYCH_MIEJSC
FROM wycieczki w
WHERE w.LICZBA_WOLNYCH_MIEJSC > 0 and w.DATA > CURRENT_DATE;



create or replace procedure przelicz
is
begin
  update WYCIECZKI w
  set w.LICZBA_WOLNYCH_MIEJSC = (select wm.WOLNE_MIEJSCA from wycieczki_miejsca wm where wm.ID_WYCIECZKI = w.ID_WYCIECZKI)
  where w.LICZBA_WOLNYCH_MIEJSC is null;
  commit;
end;

  select * from wycieczki_miejsca;

  select * from wycieczki;
select * from wycieczki_miejsca;



  begin
    przelicz();
  end;


create or replace procedure dodaj_rezerwacje2(wycieczka_id NUMBER, osoba_id NUMBER)
is
begin
  declare
    counter number;
  begin
    select count(*) into counter from wycieczki w where w.ID_WYCIECZKI = wycieczka_id and w.DATA > CURRENT_DATE and w.LICZBA_WOLNYCH_MIEJSC > 0;
    if (counter = 0) then
      raise NO_DATA_FOUND ;
    else
      insert into REZERWACJE(ID_WYCIECZKI, ID_OSOBY, STATUS)
             values(wycieczka_id, osoba_id, 'N');

      insert into REZERWACJE_LOG(ID_REZERWACJI, DATA, STATUS)
            values((select r.NR_REZERWACJI from rezerwacje r where r.ID_WYCIECZKI = wycieczka_id and r.ID_OSOBY = osoba_id and rownum = 1) , CURRENT_DATE, 'N');
      commit;
    end if;
  end;
end;


create or replace procedure zmien_liczbe_miejsc2(wycieczka_id NUMBER, nowa_liczba NUMBER)
is
begin
  declare
    counter number;
  begin
    select count(*) into counter from wycieczki w where w.ID_WYCIECZKI = wycieczka_id and w.LICZBA_WOLNYCH_MIEJSC >= w.LICZBA_MIEJSC - nowa_liczba;
    if (counter = 0)  then
      raise NO_DATA_FOUND ;
    else
      update WYCIECZKI w
      set w.LICZBA_WOLNYCH_MIEJSC = nowa_liczba - (w.LICZBA_MIEJSC - w.LICZBA_WOLNYCH_MIEJSC),
          w.LICZBA_MIEJSC = nowa_liczba
      where w.ID_WYCIECZKI = wycieczka_id;
      commit;

    end if;
  end;
end;


select * from wycieczki;

begin
  zmien_liczbe_miejsc2(5, 7);
end;



CREATE OR REPLACE TRIGGER dodaj_log
AFTER INSERT OR UPDATE ON REZERWACJE
FOR EACH ROW
BEGIN
   insert into REZERWACJE_LOG(ID_REZERWACJI, DATA, STATUS)
   values(:new.NR_REZERWACJI, CURRENT_DATE, :new.STATUS);
END;


CREATE OR REPLACE TRIGGER blokuj_usuwanie
BEFORE DELETE ON REZERWACJE
BEGIN
  raise PROGRAM_ERROR;
END;


create or replace procedure dodaj_rezerwacje3(wycieczka_id NUMBER, osoba_id NUMBER)
is
begin
  declare
    counter1 number;
    counter2 number;
  begin
    select count(*) into counter1 from wycieczki w where w.ID_WYCIECZKI = wycieczka_id and w.DATA > CURRENT_DATE and w.LICZBA_WOLNYCH_MIEJSC > 0;
    select count(*) into counter2 from osoby o where o.ID_OSOBY = osoba_id;
    if (counter1 = 0 or counter2 = 0) then
      raise NO_DATA_FOUND ;
    else
      insert into REZERWACJE(ID_WYCIECZKI, ID_OSOBY, STATUS)
             values(wycieczka_id, osoba_id, 'N');
      commit;
    end if;
  end;
end;



create or replace procedure zmien_status_rezerwacji3(rezerwacja_id NUMBER, nowy_status VARCHAR)
is
begin
  declare
    counter number;
  begin
    select count(*) into counter from rezerwacje r where r.NR_REZERWACJI = rezerwacja_id;
    if (counter = 0)  then
      raise NO_DATA_FOUND ;
    else
      declare
      obecny_status VARCHAR(1);
      begin
        select r.STATUS into obecny_status from REZERWACJE r where r.NR_REZERWACJI = rezerwacja_id;
        if obecny_status = 'A' then
          raise INVALID_NUMBER;
        else
          update REZERWACJE set STATUS = nowy_status where NR_REZERWACJI = rezerwacja_id;
          commit;
        end if;
      end;
    end if;
  end;
end;

select * from rezerwacje;
begin
  zmien_status_rezerwacji(63, 'Z');
end;



create or replace procedure dodaj_rezerwacje3(wycieczka_id NUMBER, osoba_id NUMBER)
is
begin
  declare
    counter1 number;
    counter2 number;
  begin
    select count(*) into counter1 from wycieczki w where w.ID_WYCIECZKI = wycieczka_id and w.DATA > CURRENT_DATE;
    select count(*) into counter2 from wycieczki_miejsca wm where wm.ID_WYCIECZKI = wycieczka_id and wm.WOLNE_MIEJSCA > 0;
    if (counter1 = 0) or  (counter2 = 0) then
      raise NO_DATA_FOUND ;
    else
      insert into REZERWACJE(ID_WYCIECZKI, ID_OSOBY, STATUS)
             values(wycieczka_id, osoba_id, 'N');
      commit;
    end if;
  end;
end;

  select * from rezerwacje;
begin
  dodaj_rezerwacje3(1, 23);
end;



CREATE OR REPLACE TRIGGER dodanie_rezerwacji
AFTER INSERT ON REZERWACJE
FOR EACH ROW
BEGIN
   if (:new.STATUS = 'P' or :new.STATUS = 'Z') then
     update WYCIECZKI w set w.LICZBA_WOLNYCH_MIEJSC = w.LICZBA_WOLNYCH_MIEJSC - 1 where w.ID_WYCIECZKI = :new.ID_WYCIECZKI;
   end if;
END;


CREATE OR REPLACE TRIGGER edycja_rezerwacji
AFTER UPDATE ON REZERWACJE
FOR EACH ROW
BEGIN
   if (:new.STATUS = 'P' or :new.STATUS = 'Z') and (:old.STATUS != 'Z' and :old.STATUS != 'P') then
     update WYCIECZKI w set w.LICZBA_WOLNYCH_MIEJSC = w.LICZBA_WOLNYCH_MIEJSC - 1 where w.ID_WYCIECZKI = :new.ID_WYCIECZKI;
   end if;
END;


CREATE OR REPLACE TRIGGER edycja_wycieczki
BEFORE UPDATE ON WYCIECZKI
FOR EACH ROW
BEGIN
   :new.LICZBA_WOLNYCH_MIEJSC := :new.LICZBA_MIEJSC - (:old.LICZBA_MIEJSC - :old.LICZBA_WOLNYCH_MIEJSC);
END;

create or replace procedure zmien_liczbe_miejsc3(wycieczka_id NUMBER, nowa_liczba NUMBER)
is
begin
  declare
    counter number;
  begin
    select count(*) into counter from wycieczki w where w.ID_WYCIECZKI = wycieczka_id and w.LICZBA_WOLNYCH_MIEJSC >= w.LICZBA_MIEJSC - nowa_liczba;
    if  (counter = 0)  then
      raise NO_DATA_FOUND ;
    else
      update WYCIECZKI w
      set w.LICZBA_MIEJSC = nowa_liczba
      where w.ID_WYCIECZKI = wycieczka_id;
      commit;
    end if;
  end;
end;

select * from wycieczki;

begin
  zmien_liczbe_miejsc3(5, 7);
end;