# CSE20212_Project (Robot-Bunny Apolcalypse)

##Dependencies
This project was developed for and works ideally on <b>Linux. </b>

To download the source code using git, you can simply type:
```
git clone http://github.com/BuchananBourdon/CSE20212_Project
```

SDL1.2 is used for graphics rendering so you'll want to ensure you have the necessary dev/runtime libraries installed.
```
apt-get install libsdl1.2-dev
apt-get install libsdl-net1.2 libsdl-net1.2-dev
apt-get install libsdl-image1.2 libsdl-image1.2-dev
apt-get install libsdl-gfx1.2 libsdl-gfx1.2-dev
```

##Compilation/Setup
To compile the game, simply type: ``` make ```

Currently, there is a bug affecting the connection between two players on different systems (e.g. separate personal computers). For development and testing, it's easiest to simply play the game on the same system (i.e. two terminal windows).

First the host game will need to be setup, so in the first terminal window enter: 
```
./main host localhost:55555
```
The prototype for this call is ```./main host/join address:port``` so localhost specifies that the game will be run locally and the port number can realistically be any unused port. It's best to ensure that you don't attempt to use a reserved port. 

Once the host game has been instantiated, it will wait for a join request. In the second terminal window enter:
```
./main join localhost:55555
```

##Gameplay
Players control robots or bunnies who must collect resourses (coal and candy, respectively). Resource count is shown on a progress bar at the bottom of the action bar, which contains buttons that, when selected, determine mouse actions. Each user can place spawn structures at valid locations of their choosing and can spawn their minions by selecting (click-dragging a selection box over the unit) the spawn structure and then left-clicking on it. Creating spawn structures and spawning minions both cost resources. Users can select multiple minions at once with the selection box and order them to move by left-clicking on a location on the map. Additionally, minions can be ordered to attack by selecting them and then left-clicking on an enemy unit. The game ends when all of a player's units have been destroyed.


![](Gameplay.png?raw=true "Gameplay")
