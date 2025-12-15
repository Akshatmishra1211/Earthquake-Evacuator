# Earthquake Evacuator — Disaster Management System

A C++–based simulation system that uses **Operating System Priority Scheduling algorithms** to optimize evacuation during an earthquake.  
The system treats each person as a "process" and assigns priorities based on **age**, **zone severity**, and **real-time updates**, simulating realistic rescue operations.

---

## 1. Overview

Earthquake Evacuator is a console-based disaster management simulation tool that models evacuation during natural calamities using **OS Scheduling concepts**.  
The system supports:

- **Priority Scheduling**
- **Zone-based urgency**
- **Aftershock simulation**
- **Resource-based evacuation rounds**
- **Staged evacuation**
- **Dynamic priority updates**
- **Evacuation reporting**

This project shows how algorithms used in operating systems can be applied to real-world rescue planning.

---

## 2. Features

### ✔ Priority-Based Evacuation
- Children → Priority 1  
- Senior Citizens (≥60) → Priority 2  
- Adults (18–59) → Priority 3  
- Auto-sorting using:
  - Arrival time  
  - Zone severity  
  - Priority  
  - Person number  

### ✔ Zone Severity Modeling
- User assigns zones (1–3)  
- Higher zones get higher rescue urgency  
- People re-sorted immediately after zone assignment  

### ✔ Aftershock Simulation
- 10% random chance  
- Adds delays  
- Blocks current non-evacuated individuals  

### ✔ Resource Allocation
- Evacuates people in multiple rounds  
- Reflects real-world rescue vehicle/team constraints  
- User decides whether to continue to next round  

### ✔ Staged Evacuation
- Stage 1: Priority 1  
- Stage 2: Priority 2  
- Stage 3: Priority 3+ not auto-evacuated  

### ✔ Comprehensive Reporting
- Waiting time  
- Turnaround time  
- Start & completion time  
- Total evacuated vs. remaining  

---

## 3. File Structure

```
📁 Earthquake-Evacuator/
│── main_final.cpp          # Complete simulation code
│── README.md         # Project documentation (this file)
```

---

## 4. How to Compile & Run

### Using g++ (Recommended)
```sh
g++ main.cpp -o evacuator
./evacuator
```

### Using Windows CMD
```sh
g++ main.cpp -o evacuator.exe
evacuator.exe
```

### Using VS Code
1. Install the C/C++ extension  
2. Open folder  
3. Run → "Run Without Debugging"

---

## 5. Input Format

### During program start
- Enter number of people  
- Enter age of each person  
- Enter available rescue resources  

### Menu Options
```
1. Simulate Evacuation
2. Update Priority
3. Allocate Resources
4. Set Disaster Type
5. Simulate Aftershock
6. Staged Evacuation
7. Evacuation Report
8. Exit
```

---

## 6. Output Includes

- Sorted priority queue  
- Start time / completion time  
- Average waiting & turnaround time  
- Resource-based evacuation logs  
- Aftershock warnings  
- Lists of non-evacuated people  
- Full evacuation report  

---

## 7. Algorithms & Concepts Used

- OS Priority Scheduling  
- Comparator-based sorting  
- Turnaround time calculation  
- Waiting time calculation  
- Batch processing  
- Random event simulation  
- State tracking using global arrays  

---

## 8. Future Enhancements

- Add GUI using Qt/JavaFX  
- Integrate with real-world GIS map APIs  
- Use machine learning to predict collapse zones  
- Real-time IoT sensor-based zone severity detection  
- Multi-threaded simulation to reflect parallel rescue teams  

---

## 9. Conclusion

The Earthquake Evacuator demonstrates that **Operating System Scheduling Algorithms** can be adapted to real-life disaster response planning.  
It effectively models evacuation under constraints such as limited resources, unpredictable aftershocks, and priority-based rescue decisions.

---
