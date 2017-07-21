### Input
![input](https://i.gyazo.com/9c15947414a2650523698d4bc336140a.png)
### Ouput
![output](https://i.gyazo.com/0f4c3917356fba97e0fb328d2d72724c.png)

### Configuration
Altering puzzle sizes:  
```
const int WIDTH = 15;  
const int HEIGHT = 15;  
```
Changing appearance & behavior:  
```
#define SHOWSTEPS false
#define NOSCROLL true
#define VALIDATION true
```


### About
Doesn't do any guessing, so will report errors for puzzles which require guessing/have no unique solution  
