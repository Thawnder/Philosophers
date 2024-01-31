# Philosophers (125/100)

**Philosophers** is a project at 42 School that involves solving the dining philosophers problem using multithreading and synchronization techniques. The goal is to implement a solution that allows philosophers to eat without experiencing starvation or deadlock.

## Features
- Custom implementation of the dining philosophers problem solution.
- Uses multithreading and synchronization primitives (mutexes for the mandatory part, semaphores for the bonus part) for concurrency control.
- Supports configurable parameters such as the number of philosophers, simulation time, and time to eat/think/sleep.

## Usage
1. Clone the repository:
   <pre>
   git clone https://github.com/Thawnder/Philosophers.git
   </pre>
2. Compile the philosophers program:
   <pre>
   make
   </pre>
   If you want to compile the bonus version, open the philo_bonus directory beforehand. <br><br>
3. Run the philosophers program with appropriate arguments:
   <pre>
   ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
   </pre>
   or <br><br>
   <pre>
   ./philo_bonus number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
   </pre> <br>
   Replace the placeholder values with appropriate parameters for your simulation.

## Example
<pre>
$ ./philo 5 800 200 200
</pre>
