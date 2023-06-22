# 🏁 Wumpus World 🏁
This is a **Wumpus World** in which an agent passes through a maze to avoid obstacles to pick up gold himself.

<br/>

## ⚙ Requirement
For building and running the applicaion you need:
* `Visual Studio 2019`
  * C++

<br/>

## 🎮 기능
* 미로 규격  :  `4 X 4`
	```C++
  basic_string<char, char_traits<char>, allocator<char>> maze2[MAZE_SIZE][MAZE_SIZE] = { //문자열 3차원 배열
      {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"},
      {"[ ]", "U", "U", "U", "U", "[ ]"},
      {"[ ]", "U", "U", "U", "U", "[ ]"},
      {"[ ]", "U", "U", "U", "U", "[ ]"},
      {"[ ]", "U", "U", "U", "U", "[ ]"},
      {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}
  };
  ```
  * `[ ]` : 가장자리 벽
  * `U` : 공개되지 않은 미로
  * `>` : Agent의 현재 위치
  * `-` : Agent가 탐색하고 지나간 자리


## 🎮 Functions
* Music Playback Window
	* Music play button
		* The first click changes the playback bar's time from 0:00 to the total playback time of each music
		* The second click, play music!!
	* Like button
		* Click 'Heart' on the music they like
	* Add button
		* Add/Delete the music by clicking '+'
* Progress bar
	* Progress bar showing when music is played
	* Move to the desired music playback location

<br/>

## 📁 Code Package
* musicPlay 패키지
  > * main.java 
  > * main.css  :  화면 디자인 CSS
	> * musicPlay.fxml  :  음악 재생 창 구현
	> * MusicPlayController.java  :  음악 재생 controller

<br/>

## 🎞 DEMO
![MusicPlayer](https://github.com/Ga-000/MusicPlayer/assets/134590236/9ab2a938-f9d9-4cb9-b175-918370d726c4)
