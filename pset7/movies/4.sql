select count(title) from movies left join ratings on movies.id = ratings.movie_id where ratings.rating = 10;