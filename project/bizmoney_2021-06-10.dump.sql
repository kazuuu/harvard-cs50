----
-- phpLiteAdmin database dump (http://www.phpliteadmin.org/)
-- phpLiteAdmin version: 1.9.7.1
-- Exported: 8:25pm on June 10, 2021 (UTC)
-- database file: /home/ubuntu/project/bizmoney.db
----
BEGIN TRANSACTION;

DROP TABLE departments;
DROP TABLE transfers;
DROP TABLE charges;
DROP TABLE transactions;
DROP TABLE users;

----
-- Table structure for departments
----
CREATE TABLE "departments" ('id' integer PRIMARY KEY AUTOINCREMENT NOT NULL, 'name' varchar(25) NOT NULL, 'owner_user_id' integer, 'balance' double precision DEFAULT 0);

----
-- Data dump for departments, a total of 2 rows
----
INSERT INTO "departments" ("id","name","owner_user_id","balance") VALUES ('44','DEVTEAM 1','1','0.0');
INSERT INTO "departments" ("id","name","owner_user_id","balance") VALUES ('45','SALES DEPARTMENT','1','8000.0');

----
-- Table structure for transfers
----
CREATE TABLE 'transfers' ('id' integer PRIMARY KEY AUTOINCREMENT NOT NULL,'date' bigint NOT NULL,'value' double precision,'from_department_id' INTEGER,'to_department_id' INTEGER,'description' text);

----
-- Data dump for transfers, a total of 0 rows
----

----
-- Table structure for charges
----
CREATE TABLE 'charges' ('id' integer PRIMARY KEY AUTOINCREMENT NOT NULL,'from_department_id' integer NOT NULL,'to_department_id' integer NOT NULL, 'value' double precision NOT NULL,'date' bigint NOT NULL,'description' text NOT NULL  ,'status' smallint DEFAULT 0);

----
-- Data dump for charges, a total of 1 rows
----
INSERT INTO "charges" ("id","from_department_id","to_department_id","value","date","description","status") VALUES ('22','44','45','2000.0','1623283200','Website development for Client1','1');

----
-- Table structure for transactions
----
CREATE TABLE 'transactions' ('id' integer PRIMARY KEY AUTOINCREMENT NOT NULL, 'date' bigint NOT NULL, 'department_id' integer NOT NULL, 'value' double precision NOT NULL, 'description' text, 'type' INTEGER,'externalId' INTEGER);

----
-- Data dump for transactions, a total of 4 rows
----
INSERT INTO "transactions" ("id","date","department_id","value","description","type","externalId") VALUES ('88','1622851200','44','-2000.0','Developer 1','2',NULL);
INSERT INTO "transactions" ("id","date","department_id","value","description","type","externalId") VALUES ('89','1623283200','45','-2000.0','Website development for Client1','4','22');
INSERT INTO "transactions" ("id","date","department_id","value","description","type","externalId") VALUES ('90','1623283200','44','2000.0','Website development for Client1','4','22');
INSERT INTO "transactions" ("id","date","department_id","value","description","type","externalId") VALUES ('91','1624147200','45','10000.0','Website for Client 1','1',NULL);

----
-- Table structure for users
----
CREATE TABLE 'users' (id INTEGER, username TEXT NOT NULL, hash TEXT NOT NULL, PRIMARY KEY(id));

----
-- Data dump for users, a total of 1 rows
----
INSERT INTO "users" ("id","username","hash") VALUES ('1','user1','pbkdf2:sha256:150000$40Bz9kow$20024c83fcedab56ce74c08c1ab706c842c4e3f8965dad41dd34d42d60dbf70f');

----
-- structure for index username on table users
----
CREATE UNIQUE INDEX username ON users (username);
COMMIT;
