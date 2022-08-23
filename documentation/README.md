# Below the Surface Documentation

This is a collaborative effort to create an **exceptional game** to be implemented within the **Arcade Machine**.


## Header Files

In this game there here are the following header files that you should be aware of while developing this game. (To add more detail later)

 - player.h
 - map.h
 - level.h
 - enemy.h
 - behaviour.h
 - block.h
 - camera.h
 - cellsheet.h
 - collision.h
 - get_level.h
 - hud.h
 - screen.h
 - testing.h
 
## Level Text Files
Within folder **levels** there are text files filled with block id. These ids are the what represents the levels that you see within the game. And you can create one by using the level editor. Just make sure to add them in the folder once you've created it. **DO NOT CHANGE THE VALUES INSIDE THE TEXT FILES**. Otherwise, the levels will be modified. If you want to test out the level you have created. Place them outside of the **levels** folder, compile the game, and write the following code.

### Format
```
./test -l {amount of layers} {layer0.txt} {layer1.txt} {name of the level}
```

### Example
```
./test -l 2 file0.txt file1.txt test
```
Keep in mind, to have both the latest versions of the game and the level editor.

## Create files and folders
The file explorer is accessible using the button in left corner of the navigation bar. You can create a new file by clicking the **New file** button in the file explorer. You can also create folders by clicking the **New folder** button.

## Compiling the game and opening the .exe file
To compile the game, write the following prompt onto the terminal. Keep in mind to also have and set the directory of where you have the repo located in your system onto the terminal 
```
skm clang++ -o test program.cpp
```
Once compiled, run the .exe file by writing the following prompt onto the same terminal
```
./test
```


