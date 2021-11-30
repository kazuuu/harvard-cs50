select people.name from stars inner join (select movies.id as movie_id from stars inner join people on stars.person_id = people.id inner join movies on movies.id = stars.movie_id where people.name like "%Kevin Bacon%" and people.birth = 1958) as aux1 on stars.movie_id = aux1.movie_id inner join people on people.id = stars.person_id where people.name not like "%Kevin Bacon%"