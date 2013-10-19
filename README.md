behavior_based_robotics
=======================

Files to program a VEX Robot with a Behavior Based Robotics Architecture

This is my repository for code that I'm developing to implement behavior based robotics on my VEX robot. I compile this code in ROBOTC v3.6 onto a VEX cortex processor. Some of the oddities of the code structure compared to ordinary C are due to ROBOTC's limitations (e.g. only one C file can be compiled because there's no linker so some functions are in headers)

Behavior Based Robotics:
========================
Behavior based robotics seeks to build seemingly intelligent robots based on designing simple behaviors that interact to have complex emergement total behaviors. My code is inspired by the Rodney Brooks paper on the subsumption architecture \[1\] but significantly different because it's written in C vs. his LISP programs.

The different levels of competency come from a Rodney Brooks paper \[1\]:
  0. Level 0: Avoid contact with objects (stationary and moving objects)
  1. Level 1: Wander aimlessly without running into things
  2. Level 2: Explore the world by driving towards places that are seen in the distance as reachable
  3. Level 3: Build a map of the world and plan routes from place to place
  ...

Code Description:
=================
The main file to run all of the behaviors is currently level0Competency.c. This file uses includes to pull in the header files that contain the individual behaviors (e.g. collide.h). Other .c files include their respective header and any necessary supporting headers to test that behavior. The .c files are used as test files in the development of the header files.

Physical Robot Description:
===========================
The robot is a three wheeled robot with two powered wheels (left, and right) and one unpowered wheel with fixed orientation in the back (it drags during turns). The two wheel motors have integrated encoders that enable odometry. The robot detects obstacles thru an ultrasonic range sensor that is mounted on a 'head.' The head is mounted on an axel that is driven by a motor so that the robot can look around. An optical encoder allows the robot to track its head angle and two limit switches mounted on either side of the head prevent it from tangling its wires.

Many of the behaviors the robot utilizes are adaptable to robots of other physical specs.

Sources:
========
This is original code but it is heavily inspired by the following sources:

###Rodney Brooks:

  1. Brooks, R. A. "A Robust Layered Control System for a Mobile Robot", IEEE Journal of Robotics and Automation, Vol. 2, No. 1, March 1986, pp. 14–23; also MIT AI Memo 864, September 1985.
    
  2. Brooks, R. A. and A. M. Flynn "Fast, Cheap and Out of Control: A Robot Invasion of the Solar System", Journal of the British Interplanetary Society, October 1989, pp. 478–485.
  
  And many other papers
  
  http://people.csail.mit.edu/brooks/publications.html


###Jon Connell:

  3. "A Colony Architecture for an Artificial Creature", J. Connell, Ph.D thesis, MIT AI Lab 1989, available as MIT AITR-1151 also in book form as Minimalist Mobile Robotics, Academic Press, 1990.
  
  4. "Cooperative Control of a Semi-Autonomous Mobile Robot", J. Connell and P. Viola, Proc. of the 1990 IEEE Int. Conf. on Robotics and Automation (ICRA-90), pp. 1118-1121, 1990.
  
  5. "Designing Behavior-based Robots", J. Connell, Expanded version of a paper that appeared in the SPIE-91 Conf. on Mobile Robots VI, Vol. 1613, pp. 34-45, 1991.
  
  http://researcher.watson.ibm.com/researcher/view.php?person=us-jconnell

###Ronald C. Arkin:

  6. Behavior-Based Robotics (Intelligent Robotics and Autonomous Agents), R.C. Arkin 1998

###Magnus Egerstedt:

  Coursera's Control of Mobile Robots Class
  http://users.ece.gatech.edu/~magnus/index_publications.html

