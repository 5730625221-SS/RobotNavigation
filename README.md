# Robot Navigation
This project is about making the robot to clean the floor tiles. 

## Description
1. The floor tile has dust quantity 0 (no dust) to 5 (max of dust). 
2. If dust quantity of tile is 0, robot can't move over. 
3. Robot can clean dusts by its brush which has value 0 to 5 and the floor tile will cleaned by absolute of brush value minus dust value. 
4. Robot can't know the quantity of the dust but it can know whether the floor tile has dust or not when camera is opened.
5. Power mode of robot has 3 states : off, stand by, on.
6. Robot can clean the dust when in state on.
6. Robot can move 8 directions around itself and stay still.
7. Every time the robot has to change direction, it need to be in stay still state.

## Goal
1. Sum of dust quantity when all area in the floor grid is visited.
2. Number of movement used for completely clean the floor.

## Q&A

**1 state representation**

a) state stucture 
```vim
xx x xxx xxxxxxxx

BIT 0-1	  Power (00 = Off // 01 = Standby // 10 = On)

BIT 2     Camera (0 = Off // 1 = On)

BIT 3-5	  Brush (0->5 = 000 001 010 100 101 111)

BIT 6-13  Direction ( 0 = up -> 10000000 // 1 = right up -> 01000000 // 2 = right -> 00100000 // 
          3 = right down -> 00010000 // ... // 7 = left up -> 00000001 // 8 = STOP -> 00000000)
```

b) list your first state
```vim
00 0 000 00000000 ( power off / camera off / brush off / stop )
01 0 000 00000000 ( Standby / camera off / brush off / stop )
10 0 000 00000000 ( power on / camera off / brush off / stop )
10 1 000 00000000 ( power on / camera on / brush off / stop )
```

**2 data structure storing unvisited states**
```vim
Queue
```

** 3 how to handle the data structure (get/put)**
```vim
push_back  ( PUT TO QUEUE = enqueue)
pop_front  ( GET OUT QUEUE = dequeue)
```

**4 how to check if the new state is**

a) valid and consistent 
```vim
เช็คว่า ไปได้ไหม โดยดูว่า เคยผ่านไปหรือยัง(จะไม่เดินซ้ำทางเดิม) หรือ ช่องที่จะไปคือ 0 ที่โจทย์ตั้งต้นมาแต่แรกไหม ถ้าใช่ก็ไม่ไปทางนั้น
```
b) end state or goal
```vim
เช็คว่า ทำความสะอาดจนฝุ่นหมดยัง โดยจะมี function ที่ใช้ในการนับ dust 
```

c) if not a goal, how the current state be changed to new state(s)
```vim
ก็ทำการเช็คต่อไป แต่ถ้า มีการเปลี่ยนมากกว่า 1 บิตของ state จะมีการเปลี่ยนแปลงดังนี้ คือ 1. ถ้ามีการเปลี่ยนทิศหุ่น หุ่นจะ resetตัวเอง
ให้หยุดนิ่งก่อน 1 state แล้วจัดไปยังทิศนั้น 2. ถ้ามีการเปลี่ยน brust หุ่นจะทำการ reset brush เป็น 0 ก่อนจึงจะเปลี่ยนเป้น brush นั้น 
(นอกซะจากจะเปลี่ยนเป็น0หรือbrushนั้นเป้น0มาก่อนหน้าแล้วจะไปต้องมีstate reset)
```
**5 programming results**

a) number of states used for completely clean the floor
```vim
2045 States 
```
b) number of dust units after 2000 states
```vim
7 dust units
```
c) number of states and dust units when all area in the floor grid is visited.
```vim
613 states / 434 dust units
```

## Presentation
https://www.youtube.com/watch?v=Wo3wE7ursAw&feature=youtu.be

## Developed by
**Mr.Sitthichai Saejia 5730625221**

## About
Side Project of "ALGORITHM DESIGN" 2016/1.

Computer Engineering CP41.

Faculty of Engineering, Chulalongkorn University.

