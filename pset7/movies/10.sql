SELECT DISTINCT people.name FROM directors
INNER JOIN movies ON  directors.movie_id = movies.id
INNER JOIN people ON directors.person_id = people.id
INNER JOIN ratings ON movies.id = ratings.movie_id
WHERE ratings.rating > 8.9;