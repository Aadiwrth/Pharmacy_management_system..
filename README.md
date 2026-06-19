# Current Status of banking Project of our BCA

> Under constructon hehe!!

**We finished creating a prototype for create account and the current validation check are as follow :**
- Check If the user has number infront of their name(it is checked via if statement having int as scanf so creating a temp_name will do our job)
- We have initialize that user can only enter number and not any kind of alphabet(we use the same logic behind this functionality by creating an if statement and checking if usre has input as int or not)

The
```C
    while(getchar()!='\n');
```
code is used to read buffer till \n is empty so it run's till it find \n becuase '\n' == '\n' is 
```c
True
```
and so when getchar is ran and the buffer contain let's say 'a' it will comapare it as 'a' != '\n' which is true so it runs but it stops at \n != \n becuase thats false 

**TODO**
- Make the fucking code work 
- ~~Validate the user can't input higher than 200~~
- Make sure the ID is not redundancy
- Making sure the search ID works
- tmr goal is to implement delete as well asupdate and give a nice little touch on sell functions.
