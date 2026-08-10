#!/usr/bin/env python3
import sys
import re
import time
import os

def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')

def draw_mesh(cycle, positions, grid_w=2, grid_h=2):
    clear_screen()
    print(f"=== Simulation Cycle: {cycle} ===")
    print()
    
    # Simple grid drawing (Y goes down visually, or up? Let's assume standard X,Y)
    # Our routers: (0,0) is bottom-left or top-left? Let's make (0,0) top-left for easy printing.
    for y in range(grid_h):
        # Draw routers in this row
        row_str = ""
        for x in range(grid_w):
            # Check if any flit is at (x,y)
            flits_here = [fid for fid, pos in positions.items() if pos == (x,y)]
            if flits_here:
                # Just show the first flit ID if there are multiple
                row_str += f"[ F{flits_here[0]} ]"
            else:
                row_str += "[    ]"
            
            if x < grid_w - 1:
                row_str += " -- "
        print(row_str)
        
        # Draw vertical links
        if y < grid_h - 1:
            link_str = ""
            for x in range(grid_w):
                link_str += "  |     "
            print(link_str)
    
    print("\nPress Ctrl+C to exit.")

def main():
    print("Reading simulation logs from stdin...")
    print("Usage: ./build/sim | python3 Network/tools/visualizer.py")
    
    # Store flit positions: flit_id -> (x, y)
    flit_positions = {}
    current_cycle = -1
    
    # Regex to parse the logs
    # Example: [Cycle 2] : Router_0_0 : Router Router_0_0 received flit 0
    cycle_re = re.compile(r"\[Cycle (\d+)\]")
    receive_re = re.compile(r"Router_(\d+)_(\d+) received flit (\d+)")
    
    events_by_cycle = {}

    # Read all lines from stdin
    for line in sys.stdin:
        line = line.strip()
        cycle_match = cycle_re.search(line)
        if cycle_match:
            cycle = int(cycle_match.group(1))
            if cycle not in events_by_cycle:
                events_by_cycle[cycle] = []
            events_by_cycle[cycle].append(line)
            
    if not events_by_cycle:
        print("No simulation events found in input!")
        return

    # Playback the events cycle by cycle
    max_cycle = max(events_by_cycle.keys())
    
    for cycle in range(max_cycle + 1):
        if cycle in events_by_cycle:
            for event in events_by_cycle[cycle]:
                # Update positions based on 'received flit' logs
                receive_match = receive_re.search(event)
                if receive_match:
                    rx = int(receive_match.group(1))
                    ry = int(receive_match.group(2))
                    fid = int(receive_match.group(3))
                    flit_positions[fid] = (rx, ry)
            
            draw_mesh(cycle, flit_positions)
            # Sleep so the user can see the animation
            time.sleep(0.75)

if __name__ == "__main__":
    main()
