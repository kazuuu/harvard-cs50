-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Search for crimes on the local and datetime we know.
select description  from crime_scene_reports
where street = "Chamberlin Street" and month = 7 and day = 28 and year = 2020;
-- Result: Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. Interviews were conducted today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.


-- Check what was told on the interviews filtering by the datetime and the word mentioned "courthouse" by all 3 witnesses.
select name, transcript from interviews
where month = 7 and day = 28 and year = 2020 and transcript like "%courthouse%";
-- Results:
-- name | transcript
-- Ruth | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away. If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Eugene | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
-- Raymond | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.


-- Following what Ruth said: Let´s search courthouse_securiy logs every body / license plate who left within 10 minutes after the crime
select courthouse_security_logs.license_plate, people.name, people.id as person_id, activity from courthouse_security_logs
left join people on courthouse_security_logs.license_plate = people.license_plate
where month = 7 and day = 28 and year = 2020  and hour = 10 and minute >= 15 and minute <= 25;
-- Results: Ok, those are the firsts suspects.
-- license_plate | name | person_id | activity
-- 5P2BI95 | Patrick | 221103 | exit
-- 94KL13X | Ernest | 686048 | exit
-- 6P58WS2 | Amber | 243696 | exit
-- 4328GD8 | Danielle | 467400 | exit
-- G412CB7 | Roger | 398010 | exit
-- L93JTIZ | Elizabeth | 396669 | exit
-- 322W7JE | Russell | 514354 | exit
-- 0NTHK55 | Evelyn | 560886 | exit



-- Following Eugene: Let´s search atm_transactions logs from fife Street on that morning.
select atm_transactions.account_number, name, people.id as person_id from atm_transactions
left join bank_accounts on atm_transactions.account_number = bank_accounts.account_number
left join people on bank_accounts.person_id = people.id
where year= 2020 and month = 7 and day = 28 and atm_location = "Fifer Street" and transaction_type = "withdraw";
-- account_number | name | person_id
-- 28500762 | Danielle | 467400
-- 28296815 | Bobby | 395717
-- 76054385 | Madison | 449774
-- 49610011 | Ernest | 686048
-- 16153065 | Roy | 458378
-- 25506511 | Elizabeth | 396669
-- 81061156 | Victoria | 438727
-- 26013199 | Russell | 514354


-- Conclusion: If we compare who left the courtshouse to people who have withdrawed money on that morning we can filter our suspects:
-- 94KL13X | Ernest | 686048 | exit
-- 4328GD8 | Danielle | 467400 | exit
-- L93JTIZ | Elizabeth | 396669 | exit
-- 322W7JE | Russell | 514354 | exit


-- Following Raumond: the thief called someone when living couthouse.
select people_caller.name as caller, people_receiver.name as receiver from phone_calls
left join people as people_caller on people_caller.phone_number = phone_calls.caller
left join people as people_receiver on people_receiver.phone_number = phone_calls.receiver
where month = 7 and day = 28 and year = 2020 and duration < 60;
-- caller | receiver
-- Roger | Jack
-- Evelyn | Larry
-- Ernest | Berthold
-- Evelyn | Melissa
-- Madison | James
-- Russell | Philip
-- Kimberly | Jacqueline
-- Bobby | Doris
-- Victoria | Anna

-- Conclusion: If our suspects to the phone calls, we can filter our suspects:
-- 94KL13X | Ernest | 686048 | exit
-- 322W7JE | Russell | 514354 | exit

-- Still Following Raymond: They were planning to leave Fiftyville on the first flight int he next day. So lets get the flight and passengers;
-- Get earlier flights to leave Fifyville
select flights.id as flight_id, hour, minute, airports_dest.city as city_destination from flights
inner join airports as airports_origin on flights.origin_airport_id = airports_origin.id
inner join airports as airports_dest on flights.destination_airport_id = airports_dest.id
where month = 7 and day = 29 and year = 2020 and airports_origin.city = 'Fiftyville'
order by hour asc, minute asc;
-- flight_id | hour | minute | city_destination
-- 36 | 8 | 20 | London
-- 43 | 9 | 30 | Chicago
-- 23 | 12 | 15 | San Francisco
-- 53 | 15 | 20 | Tokyo
-- 18 | 16 | 0 | Boston

select people.name from passengers
left join people on passengers.passport_number = people.passport_number
where flight_id =36;
-- name
-- Doris
-- Roger
-- Ernest
-- Edward
-- Evelyn
-- Madison
-- Bobby
-- Danielle


-- Conclusion: Comparing the resulto, we can filter our suspects:
-- 94KL13X | Ernest | 686048 | exit

-- Our thief is Enest.. And he called Berthold to buy airplane tickets.
