SELECT movies.title FROM stars
INNER JOIN movies ON  stars.movie_id = movies.id
INNER JOIN people ON stars.person_id = people.id
INNER JOIN ratings ON movies.id = ratings.movie_id
WHERE people.name = 'Chadwick Boseman'
ORDER BY ratings.rating DESC
LIMIT 5;