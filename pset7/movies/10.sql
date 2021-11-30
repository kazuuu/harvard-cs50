select people.name from movies inner join directors on movies.id = directors.movie_id inner join people on people.id = directors.person_id inner join ratings on ratings.movie_id = movies.id where ratings.rating >= 9;