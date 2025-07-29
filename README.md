# 42school_Philosophers

https://medium.com/@denaelgammal/dining-philosophers-problem-42-project-guide-mandatory-part-a20fb8dc530e

pthread_create(thread, attr, routine, arg);

(pthread_create(&data->philos[i].thread, NULL, routine,
				&data->philos[i]) != 0) - m

(pthread_create(&(party->philosophers[i].thread), NULL,
		philosopher_routine, (void *)&(party->philosophers[i])) != 0)