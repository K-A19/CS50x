-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Gets the description of the crime in question
SELECT id, description FROM crime_scene_reports
WHERE day = 28 AND
    month = 7 AND
    year = 2021 AND
    street = 'Humphrey Street';

-- Gets the interview transript of all the witnesses
SELECT name, transcript FROM interviews
WHERE day = 28 AND
    month = 7 AND
    year = 2021 AND
    transcript LIKE '%bakery%';

-- Gets the license plate of all cars that exited within the 10 minute time frame of the theft
SELECT license_plate FROM bakery_security_logs
WHERE minute >= 15 AND
    minute <= 25 AND
    hour = 10 AND
    day = 28 AND
    month = 7 AND
    year = 2021 AND
    activity = 'exit';

-- Gets the ids of all the people who used the Legget Street ATM that day and withdrew money
SELECT bank_accounts.person_id FROM atm_transactions
        INNER JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
        WHERE day = 28 AND
            month = 7 AND
            year = 2021 AND
            atm_location = 'Leggett Street' AND
            transaction_type = 'withdraw'

-- Gets all phone callers that were less than a minute on the day of the theft
SELECT caller FROM phone_calls
WHERE day = 28 AND
    month = 7 AND
    year = 2021 AND
    duration < 60;

-- Gets the id of all the earliest flights from Fiftyvill
SELECT id, minute, hour FROM flights
    WHERE origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville') AND
        day = 29 AND
        month = 7 AND
        year = 2021
    GROUP BY hour, minute
    HAVING hour = MIN(hour) AND minute = MIN(minute)
    LIMIT 1;

-- Gets the suspect's ids depending in if they fiit in with all the theifs descriptions
SELECT DISTINCT people. id FROM people
INNER JOIN bank_accounts ON people.id = bank_accounts.person_id
INNER JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
INNER JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
INNER JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE people.id IN (SELECT bank_accounts.person_id FROM atm_transactions
        INNER JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
        WHERE day = 28 AND
            month = 7 AND
            year = 2021 AND
            atm_location = 'Leggett Street' AND
            transaction_type = 'withdraw') AND
    people.license_plate IN (SELECT license_plate FROM bakery_security_logs
        WHERE minute >= 15 AND
            minute <= 25 AND
            hour = 10 AND
            day = 28 AND
            month = 7 AND
            year = 2021 AND
            activity = 'exit') AND
    people.phone_number IN (SELECT caller FROM phone_calls
        WHERE day = 28 AND
            month = 7 AND
            year = 2021 AND
            duration < 60);

-- Gets the suspects name and the place they escaped to
SELECT people.name, airports.city FROM passengers
INNER JOIN flights ON passengers.flight_id = flights.id
INNER JOIN airports ON flights.destination_airport_id = airports.id
INNER JOIN people ON passengers.passport_number = people.passport_number
WHERE people.id IN (SELECT DISTINCT people. id FROM people
        INNER JOIN bank_accounts ON people.id = bank_accounts.person_id
        INNER JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
        INNER JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
        INNER JOIN phone_calls ON people.phone_number = phone_calls.caller
        WHERE people.id IN (SELECT bank_accounts.person_id FROM atm_transactions
                INNER JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
                WHERE day = 28 AND
                month = 7 AND
                year = 2021 AND
                atm_location = 'Leggett Street' AND
                transaction_type = 'withdraw') AND
            people.license_plate IN (SELECT license_plate FROM bakery_security_logs
                WHERE minute >= 15 AND
                minute <= 25 AND
                hour = 10 AND
                day = 28 AND
                month = 7 AND
                year = 2021 AND
                activity = 'exit') AND
            people.phone_number IN (SELECT caller FROM phone_calls
                WHERE day = 28 AND
                    month = 7 AND
                    year = 2021 AND
                    duration < 60)) AND
    flights.id IN (SELECT id FROM flights
        WHERE origin_airport_id = (SELECT id FROM airports
                WHERE city = 'Fiftyville') AND
            day = 29 AND
            month = 7 AND
            year = 2021
        GROUP BY hour, minute
        HAVING minute = MIN(minute) AND
            hour = MIN(hour)
        LIMIT 1);

-- Gets the theif's accomplice by checking the phonce call receiver
SELECT people.name FROM phone_calls
INNER JOIN people  ON phone_calls.receiver = people.phone_number
WHERE caller = (SELECT phone_number FROM people WHERE name = 'Bruce') AND
    day = 28 AND
    month = 7 AND
    year = 2021 AND
    duration < 60;