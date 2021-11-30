select title from (select movies.title, count(movies.title) as count from movies inner join stars on stars.movie_id = movies.id inner join people on stars.person_id = people.id where people.name like "%Johnny Depp%" or people.name like "%Helena Bonham Carter%" group by movies.title) where count = 2;


