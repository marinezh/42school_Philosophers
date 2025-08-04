# 42school_Philosophers

https://medium.com/@denaelgammal/dining-philosophers-problem-42-project-guide-mandatory-part-a20fb8dc530e

pthread_create(thread, attr, routine, arg);

(pthread_create(&data->philos[i].thread, NULL, routine,
				&data->philos[i]) != 0) - m

(pthread_create(&(party->philosophers[i].thread), NULL,
		philosopher_routine, (void *)&(party->philosophers[i])) != 0)

no dies 
./philo 5 600 150 150  // checked 3 times
./philo 5 800 200 200  // checked 3 times
./philo 105 800 200 200 // checked 3 times
./philo 3 190 60 60  // checked 3 times
./philo 101 179 60 60 // smth death, smth deadlock, smth works

no dies 
./philo 4 410 200 200 // checked 3 times
./philo 100 800 200 200 // checked 3 times
./philo 200 800 200 200  // checked 3 times
./philo 4 800 200 200 
./philo 2 170 60 60 
./philo 2 150 60 60 
./philo 4 405 200 200 
./philo 4 130 60 60 
./philo 100 410 200 200 
./philo 100 130 60 60 

no dies 
./philo 4 410 200 200 12 
./philo 5 800 200 200 7 
./philo 4 410 200 200 10 
./philo 4 410 200 200 15 
./philo 2 800 700 90 1

one die 
./philo 5 200 800000 200 


test case from eval sheet
5 800 200 200
5 800 200 200 7
4 410 200 200

4 310 200 100 one should die