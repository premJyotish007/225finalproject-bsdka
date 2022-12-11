# 225 Final Project Team-bsdka

## File structure
```
📦225finalproject-bsdka
 ┣ 📂.vscode
 ┃ ┗ 📜settings.json
 ┣ 📂data (data files to populate data structures)
 ┃ ┣ 📜Gowalla_edges.txt
 ┃ ┗ 📜Gowalla_totalCheckins.txt
 ┣ 📂includes (header files)
 ┃ ┣ 📂json_parser (json utility header file to read, update, and write to json file)
 ┃ ┃ ┗ 📜json.hpp
 ┃ ┗ 📜network.h (network class headerfile)
 ┣ 📂lib (imported pixel and png class to read, render, and write png's to and from files)
 ┃ ┣ 📂cs225
 ┃ ┃ ┣ 📜HSLAPixel.cpp
 ┃ ┃ ┣ 📜HSLAPixel.hpp
 ┃ ┃ ┣ 📜PNG.cpp
 ┃ ┃ ┣ 📜PNG.hpp
 ┃ ┃ ┗ 📜RGB_HSL.hpp
 ┃ ┗ 📂lodepng
 ┃ ┃ ┣ 📜lodepng.cpp
 ┃ ┃ ┗ 📜lodepng.hpp
 ┣ 📂src (source code for network class and main function for testing locally)
 ┃ ┣ 📜main.cpp
 ┃ ┗ 📜network.cpp
 ┣ 📂test_images (image output generated from visualization local testing)
 ┃ ┣ 📜test.png
 ┃ ┣ 📜test2.png
 ┃ ┗ 📜test_actual.png
 ┣ 📂tests (test cases and catch2 framework)
 ┃ ┣ 📜catch.hpp
 ┃ ┣ 📜test_checkins.txt
 ┃ ┣ 📜test_edges.txt
 ┃ ┗ 📜tests.cc
 ┣ 📜.gitattributes
 ┣ 📜Makefile
 ┣ 📜README.md
 ┣ 📜a.out
 ┣ 📜project_proposal.md
 ┗ 📜team_contract.md
 ```
 
 ## Instructions to run code
 ### Targets:
 
 ``` bash
 make main
 make tests
 ```
 ### Executables/Output directory
 ```
 ./a.out
 ```
 The executable would contain the results of the latest make executed.

Final Presentation Link:
https://drive.google.com/file/d/1xPIDqESz-aQSAYdh7JTcuRHjGS0F07kc/view?usp=sharing
