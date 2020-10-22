# Homework 2

Transform.cpp, readfile.cpp, display.cpp, shaders/light.frag.glsl were all edited by me. There is a small tweak in main.cpp to adjust the window screen correctly for screenshot grading (something needed to be done on MacOS that I also did for the previous hw)

* Run `make` to compile
* Run `./transforms [textfile]` to execute the application where textfile is hw1.txt or demo.txt, or a custom one if you wish
* Run `./transforms demo.txt input.txt` and press 'i' to run the grader's script to take required screenshots

This assignment was tedious and challenging, having me hunched over the computer at 3am in the morning for at least a week. The hints provided by other students were very helpful in leading me to the right direction, but some technical issues--like my binary files not updating correctly--really threw me off, because I had all the logic right (for the most part). I missed passing one variable, numusedcol, and that just threw a few hours of my life away as I combed through the code line by line for any possible error when parsing the light data. After that simple pass of the variable to the shader, all my lights turned on beautifully! How irksome to have typed up so much code, and have it been correct this whole time, thinking I was a total failure even after following all the hints very closely. Gah, I guess that's (coding) life. 

Finished Wednesday October 21st, 2020.

All done on my MacBookPro 2016 macOS Catalina
