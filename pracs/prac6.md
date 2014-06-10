Dam Gate Automated Control System Test Plan

Dams and reservoirs play an important role in maintaining a fresh water supply around the world. They also represent a huge risk to all those that rely on them and that live around them. In January 2011, widespread flooding devastated areas of Queensland. In the wake of this flooding, and in search of someone to blame, attention turned to the operation of Wivenhoe Dam during the event. Questions were asked such as how much water was released, how much water actually needed to be released, and were there appropriate systems in place to manage the release of water. While these flooding events do not happen often, when they do you need to release enough water that the dam doesn't overflow, but not so much water that you are flooding areas downstream. This is just one example where control of a dam level requires careful calculation. However, given that controlling the level can be calculated, it is also something that can be automated. A system which monitors the dam level and rain fall levels to determine how much water should be released could be put in place, removing reliance on the operator to make hasty decisions, and providing a testable and provable system which would stand up in court.

The system would consist of the following components
Actuated dam gates
Dam level sensor
Distributed weather stations
Control system

For each components, the functions would be
Actuated dam gates
	Control system can open and close to control the flow of water from the dam
	Current gate position is sent to the control system
Dam level sensor
	Record the current level of the dam and send to control system
Distributed weather stations
	Record the current rainfall level and send to control system
Control system
	Calculate how far open the dam gates need to be based on a formula which uses the gate position, change in dam level and rainfall

Each function can fail in one or more ways
	Control system can open and close to control the flow of water from the dam
		Gates don't open
		Gates open too much or not enough
	Current gate position is sent to the control system
		Gate position is inaccurate
	Record the current level of the dam and send to control system
		Level sensor is inaccurate
		Level sensor is unavailable
	Record the current rainfall level and send to control system
		Level is inaccurate
		Level is unavailable, for one, many or all weather stations		
	Calculate how far open the dam gates need to be based on a formula which uses the gate position, change in dam level and rainfall
		Calculate an incorrect position
		Send an incorrect position


Testing of this system would be fundamental and critical, as the consequences of failure are severe - possible fatalities, law suits, insurance claims, the list goes on. Each component of the system, and even subcomponents of them, would need to be meticulously tested to ensure correct function. On-going maintenance and testing would also be required, so the system should be both built to maintain and built to test.

Each component should be individually tested
Gates
	Test dam gate open and close manually
	View the calculated gate position and validate against actual gate position (ie calibrate gate position)
Dam Level
	View the level sensors dam level and validate against actual dam level
Control System
	Design the control system for testing, allow test inputs to be fed in and outputs viewed. For example, feed in the current dam level, rainfall and view the calculated gate position.

The external interfaces of each component should be tested
Gates
	Test that when a position is set by the control system that the gates move to that position
	Check that the position recorded in the control system is the same as the position of the gates
Level
	Check that the dam level recorded in the control system matches the level recorded at the sensor	
Rain
	Check that the rainfall level recorded in the control system matches the level at the guage
Control System
Finally, the system should be tested as a whole. Given that it may not possible to test in real circumstances, the testing input of the control system should be used to operate the dam gates. Manual input should be supplied and the gate position observed.
Testing the calculation itself - the formula which determines how far to open the gates - would rely on actual data to back up the assumptions made in the formula.
