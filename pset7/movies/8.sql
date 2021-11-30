select people.name from stars inner join movies on stars.movie_id = movies.id inner join people on people.id = stars.person_id where movies.title like "%Toy Story%";

