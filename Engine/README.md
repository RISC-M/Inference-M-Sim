# Simulation Engine 
Overview: The simulation engine's goal is to provide the basic features that we need for simulation.
Basically a global event queue and base classes for adding events to the queue. 

## Simulation Workflow
To model hardware using the engine, follow this 3-step workflow:
1. **Create a `SimObject`**: Define your hardware component by inheriting from `SimObject` (e.g., `class DummyCore : public SimObject`).
2. **Write functions to modify the state of an object**: Add member variables and methods that compute operations and update state over time.
3. **Schedule the functions**: Use `pushEvent(delay, callback)` or `scheduleEvent(target_cycle, callback)` inside `startup()` or within your callback methods to schedule state transitions across simulation time.

## Global Event queue
The global event queue is a min heap prioritized by simulation cycles.
(We could do ticks/picoseconds, but I don't think that level of granularity is needed)
For tie breaking (if multiple events are scheduled at the same time) we break ties by looking at the unique ID (`seq_id`) of an Event.
- use `getCurrentCycle()` to get the current cycle to use for scheduling

## SimObject
A small base class with:
- `init()` to connect interfaces and ports
- `startup()` to schedule its initial event

and helper API:
- `pushEvent(delay, function())` to schedule a callback at `current_cycle + delay`
- `scheduleEvent(time, function())` to schedule a callback function in the future in simulation time
- `simout(message)` to print simulation logs formatted as `[Cycle X] : [SimObjectName] : message`

## Events
An event represents a scheduled task in simulation time. Events should only be created by `SimObjects`.
- `scheduled_cycle`: time for when event will fire
- `seq_id`: unique ID for breaking ties
- `sim_object_name`: used for debug output