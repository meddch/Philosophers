# Philosophers
# Philosophers

Philosophers is a project at 42 that explores the classic synchronization problem known as the "Dining Philosophers Problem." The goal of this project is to implement a solution that allows a group of philosophers to dine together, each alternating between thinking and eating, while avoiding deadlock and starvation.

## Project Overview

The Philosophers project involves creating a simulation of the dining philosophers problem, where a certain number of philosophers sit around a table with a shared set of forks. Each philosopher has two forks placed on either side of them, and they must alternate between two states: thinking and eating. To avoid conflicts and ensure fairness, a set of rules must be applied.

The main objectives of the project include:

1. Implementing a solution that avoids deadlock and prevents philosophers from starving.
2. Managing concurrent access to shared resources (forks) using mutex locks.
3. Synchronizing the actions of the philosophers to ensure proper alternation between thinking and eating.
4. Handling scenarios where philosophers reach different states (thinking, eating, sleeping) concurrently.
5. Demonstrating a clear understanding of synchronization mechanisms and thread management.

## How to Run the Philosophers Simulation

To compile the Philosophers program, run the following command:

```
make
```

This will generate an executable named `philo`.

To execute the simulation, use the following syntax:

```
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

- `number_of_philosophers` is the number of philosophers sitting at the table.
- `time_to_die` is the time in milliseconds after which a philosopher will die if they have not eaten.
- `time_to_eat` is the time in milliseconds it takes for a philosopher to eat.
- `time_to_sleep` is the time in milliseconds a philosopher spends sleeping after eating.
- `number_of_times_each_philosopher_must_eat` (optional) is the number of times each philosopher must eat before the simulation ends. If not provided, the simulation will run indefinitely.

Here's an example usage of the Philosophers simulation:

```
./philo 5 800 200 200
```

This will run the simulation with 5 philosophers, where the time to die is set to 800ms, the time to eat is set to 200ms, and the time to sleep is set to 200ms.

## Learning Objectives

The Philosophers project provides an opportunity to learn and practice various concepts, including:

- Understanding and implementing synchronization mechanisms.
- Working with mutex locks to manage access to shared resources.
- Handling concurrent access to shared resources using semaphores.
- Managing the lifecycle of multiple threads.
- Detecting and preventing deadlock and starvation.
- Designing and implementing efficient solutions to synchronization problems.

## Resources

Here are some resources that can be helpful for completing the Philosophers project:

- [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Mutex Locks](https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/)
- [Thread Management in C](https://www.geeksforgeeks.org/thread-functions-in-c-c/)
- [Synchronization Mechanisms](https://www.geeksforgeeks.org/synchronization-in-multithreading/)
- [Deadlock and Starvation](https://www.geeksforgeeks.org/livelock-starvation-and-deadlock-in-operating-systems/)
- [Concurrency Control in C](https://www.geeksforgeeks.org/operating-system-concurrency-control-2/)

Good luck with your Philosophers project! Embrace the challenge and enjoy exploring the fascinating world of synchronization!
