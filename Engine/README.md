# Simulation Engine 
Overview: The simulation engine's goal is to provide the basic features that we need for simulation.
Basically a global event queue and base classes for adding events to the queue. 
                                                                                                                  

## Global Event queue
The gloabl event queue is a min heap prioritized by simulation cycles.
(We could do ticks/picoseconds, but I don't think that level of granularity is needed)
For tie breaking (if a multiple events are scheduled at the same time) we break ties by looking at the uniq id of a Event
- use getCurrentCycle() to get the current cycle to use for scheduling

## SimObject
A small base class with, 
- init() to connect interfaces and ports
- startup() to schedule it's initial event

and helper API
- pushEvent(delay, function()) to schedule a callback at current cycle + delay
- scheduleEvent(time, function()) to schedule a callback function in the future in 'simulation time'
- debug(message) to print debug logs formatted as Cycle : [SimObjectName] Message

## Events
An event represents a scheduled task in simulation time, events should only be created by simObjects
- scheduled_cycle to time for when event will fire
- seq_id to unique id for breaking ties
- simObjectName to used for debug output