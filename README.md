Physics Simulated Environment AI
========

This application shows how you can use basic AI techniques to "animate" your game. 
In this example, computer can chase/intercept/goto and evade, avoid obstacles, and keep a safe distance at the same time.

Application is written in C/C++ (MSVC 2008) suing Allegro4 for rendering.

Techniques:

 - Potential function based (PFB) movements
 - Fuzzy logic (just a little, for "pure" driving of starship in StarShipAI::MaintainVelocity() method )

There are 2 groups of enemies (left and right), every group have:

 - 2 red ships (these will chase you)
 - 1 green ship (this will try to intercept you)
 - 1 blue ship (this will try to chase/follow the green ships and also will try to avoid player (evade)

The red circle represent a danger area/obstacle, and as you will see, computer will try to avoid it (you can put obstacles as many as you want but it will requier an pathfinding+waypoints system)

You can do many things with these simple methods (flocking, enchanced obstacle avoidance with waypoint system) also it is a good starting point for further scripting system.

Control:

	W - accelerate forward
	A - turn right
	D - turn left
	S - accelerate backward
	Q - pause/continue simulation
	ESC - exit

NOTE: this example doesn't have implemented a fully physics engine with collision and response and all stuff, it have only implemented the kinetics of starship. Anyway, you can see how the AI ships are not colliding most of the times which pretty much confirms that the methods works well.