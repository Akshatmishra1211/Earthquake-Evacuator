#include <bits/stdc++.h>
#include <chrono>
using namespace std;

// Struct and global arrays (kept from original)
struct process
{
    int at, bt, pr, pno, zone;
};

// Global storage (original)
process proc[50];
bool evacuated[50] = {false};

// Global helper: current number of people (so wrappers can re-sort safely)
int current_n = 0;

// ----------------------------------------------------------------------------
// Utility / Logging helpers (added: do not alter program state)
// ----------------------------------------------------------------------------

static void noop_line_break()
{
    return;
}

static void verbose_print_header(const string &title)
{
    cout << "----------------------------------------" << endl;
    cout << "  " << title << endl;
    cout << "----------------------------------------" << endl;
}

static inline void debug_log(const string &msg)
{
    cout << "[DEBUG] " << msg << endl;
}

int safe_input_int()
{
    int x;
    cin >> x;
    return x;
}

// -----------------------------------------------------------------------------
// Comparator function (zone-aware: arrival, zone desc, priority asc, pno asc)
// -----------------------------------------------------------------------------
bool comp(process a, process b)
{
    if (a.at != b.at)
    {
        return a.at < b.at;
    }
    if (a.zone != b.zone)
    {
        return a.zone > b.zone; // higher zone => earlier
    }
    if (a.pr != b.pr)
    {
        return a.pr < b.pr;     // lower numeric pr => earlier
    }
    return a.pno < b.pno;
}

// -----------------------------------------------------------------------------
// get_wt_time / wrappers (unchanged semantics except debug logging)
// -----------------------------------------------------------------------------
void get_wt_time(int wt[], int totalprocess)
{
    int service[50];
    service[0] = proc[0].at;
    wt[0] = 0;

    for (int i = 1; i < totalprocess; i++)
    {
        service[i] = proc[i - 1].bt + service[i - 1];
        wt[i] = service[i] - proc[i].at;
        if (wt[i] < 0)
        {
            wt[i] = 0;
        }
    }
}

void get_wt_time_wrapper(int wt[], int totalprocess)
{
    debug_log("get_wt_time_wrapper: computing waiting times for " + to_string(totalprocess) + " processes");
    get_wt_time(wt, totalprocess);
    noop_line_break();
}

void get_tat_time(int tat[], int wt[], int totalprocess)
{
    for (int i = 0; i < totalprocess; i++)
    {
        tat[i] = proc[i].bt + wt[i];
    }
}

void get_tat_time_wrapper(int tat[], int wt[], int totalprocess)
{
    debug_log("get_tat_time_wrapper: computing turnaround times");
    get_tat_time(tat, wt, totalprocess);
    noop_line_break();
}

// -----------------------------------------------------------------------------
// staged_evacuation: Stage1 -> pr==1, Stage2 -> pr==2 (do not auto-rescue pr>=3)
// -----------------------------------------------------------------------------
void staged_evacuation(int totalprocess)
{
    cout << "Stage 1 Evacuation: Priority 1 only" << endl;
    for (int i = 0; i < totalprocess; i++)
    {
        if (proc[i].pr == 1 && !evacuated[i])
        {
            cout << "Evacuating priority-1 person " << proc[i].pno << endl;
            evacuated[i] = true;
        }
    }

    cout << "Stage 2 Evacuation: Priority 2 only" << endl;
    for (int i = 0; i < totalprocess; i++)
    {
        if (proc[i].pr == 2 && !evacuated[i])
        {
            cout << "Evacuating priority-2 person " << proc[i].pno << endl;
            evacuated[i] = true;
        }
    }

    cout << "Stage 3: Lower priority (3+) NOT evacuated automatically in this staged operation." << endl;
}

void staged_evacuation_wrapper(int totalprocess)
{
    debug_log("staged_evacuation_wrapper: starting staged evacuation");
    staged_evacuation(totalprocess);
    debug_log("staged_evacuation_wrapper: staged evacuation finished");
    noop_line_break();
}

// -----------------------------------------------------------------------------
// findgc (scheduling & stats) - unchanged semantics (uses proc[] order)
// -----------------------------------------------------------------------------
void findgc(int totalprocess)
{
    int wt[50], tat[50];
    double wavg = 0, tavg = 0;

    get_wt_time_wrapper(wt, totalprocess);
    get_tat_time_wrapper(tat, wt, totalprocess);

    int stime[50], ctime[50];

    stime[0] = proc[0].at;
    ctime[0] = stime[0] + tat[0];
    for (int i = 1; i < totalprocess; i++)
    {
        stime[i] = ctime[i - 1];
        ctime[i] = stime[i] + tat[i] - wt[i];
    }

    cout << "Process No.\tStart time\tComplete time\tTurn Around Time\tWaiting Time" << endl;
    for (int i = 0; i < totalprocess; i++)
    {
        wavg += wt[i];
        tavg += tat[i];
        cout << proc[i].pno << "\t\t" << stime[i] << "\t\t" << ctime[i] << "\t\t"
             << tat[i] << "\t\t" << wt[i] << endl;
    }

    cout << fixed << setprecision(2);
    double wavg1 = wavg / ((double)totalprocess);
    double tavg1 = tavg / ((double)totalprocess);
    cout << "\nAverage wait time for evacuation: ";
    if (wavg1 < 1) cout << "Less than a minute" << endl;
    else cout << wavg1 << " minutes" << endl;

    cout << "Average time to evacuate all: ";
    if (tavg1 < 1) cout << "Less than a minute" << endl;
    else cout << tavg1 << " minutes" << endl;
}

void findgc_wrapper(int totalprocess)
{
    verbose_print_header("Running Priority Scheduling Simulation (findgc)");
    findgc(totalprocess);
    noop_line_break();
}

// -----------------------------------------------------------------------------
// update_priority: find by pno, update, re-sort
// -----------------------------------------------------------------------------
void update_priority(int person, int new_priority)
{
    bool found = false;
    for (int i = 0; i < current_n; ++i)
    {
        if (proc[i].pno == person)
        {
            found = true;
            proc[i].pr = new_priority;
            cout << "Updated priority of person " << person << " to " << new_priority << endl;
            break;
        }
    }
    if (!found)
    {
        cout << "Person " << person << " not found. No update performed." << endl;
    }
    else
    {
        sort(proc, proc + current_n, comp);
        cout << "Process list re-sorted to apply new priorities." << endl;
    }
}

void update_priority_wrapper()
{
    cout << "Enter person number and new priority: ";
    int person = safe_input_int();
    int new_priority = safe_input_int();
    update_priority(person, new_priority);
    noop_line_break();
}

// -----------------------------------------------------------------------------
// allocate_resources (UPDATED: batch / repeated evacuation rounds)
// -----------------------------------------------------------------------------
void allocate_resources(int totalprocess, int available_resources)
{
    if (available_resources <= 0)
    {
        cout << "No resources available to evacuate anyone." << endl;
        return;
    }

    int round = 1;
    while (true)
    {
        cout << "\n--- Evacuation Round " << round << " (max " << available_resources << " people) ---" << endl;
        int evacuated_this_round = 0;

        // Walk the process list in order and evacuate up to available_resources
        for (int i = 0; i < totalprocess && evacuated_this_round < available_resources; ++i)
        {
            if (!evacuated[i])
            {
                cout << "Evacuating person " << proc[i].pno << " (round " << round << ")" << endl;
                evacuated[i] = true;
                evacuated_this_round++;
            }
        }

        if (evacuated_this_round == 0)
        {
            cout << "No additional people could be evacuated in round " << round << "." << endl;
            break;
        }

        int evacuated_count = 0;
        for (int i = 0; i < totalprocess; ++i) if (evacuated[i]) ++evacuated_count;
        int remaining = totalprocess - evacuated_count;

        cout << "Round " << round << " complete. Evacuated this round: " << evacuated_this_round
             << ". Total evacuated: " << evacuated_count << "/" << totalprocess << "." << endl;

        if (remaining == 0)
        {
            cout << "All people evacuated." << endl;
            break;
        }

        cout << "People remaining: " << remaining << ". Continue to next round? (1 = yes, 0 = no): ";
        int choice = safe_input_int();
        if (choice != 1)
        {
            cout << "Stopping further evacuation rounds. Remaining people will stay waiting." << endl;
            break;
        }

        cout << "Preparing next round..." << endl;
        ++round;
    }
}

void allocate_resources_wrapper(int totalprocess, int available_resources)
{
    debug_log("allocate_resources_wrapper: available_resources = " + to_string(available_resources));
    allocate_resources(totalprocess, available_resources);
    debug_log("allocate_resources_wrapper: allocation complete");
    noop_line_break();
}

// -----------------------------------------------------------------------------
// set_zones: records zone only; sorting done in wrapper to reflect zone effect
// -----------------------------------------------------------------------------
void set_zones(int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << "Enter zone severity for person " << proc[i].pno << " (1-3): ";
        int z;
        cin >> z;
        if (z < 1) z = 1;
        if (z > 3) z = 3;
        proc[i].zone = z;
    }
}

// -----------------------------------------------------------------------------
// REPLACED wrapper: prints before/after and re-sorts so zone changes take effect
// -----------------------------------------------------------------------------
void set_zones_wrapper(int n)
{
    verbose_print_header("Applying Zone Severity to People (set_zones)");
    cout << "Before zone update (index -> pno, pr, zone):" << endl;
    for (int i = 0; i < n; ++i)
    {
        cout << i << " -> Person " << proc[i].pno << ", pr=" << proc[i].pr << ", zone=" << proc[i].zone << endl;
    }

    // Collect zone values
    set_zones(n);

    cout << "Zone severity applied. (Re-sorting processes so priorities take effect.)" << endl;

    // Re-sort using comparator that considers zone, then priority
    sort(proc, proc + n, comp);
    cout << "Process list re-sorted to apply zone-based priority changes." << endl;

    cout << "After zone update: order (index -> pno, pr, zone):" << endl;
    for (int i = 0; i < n; ++i)
    {
        cout << i << " -> Person " << proc[i].pno << ", pr=" << proc[i].pr << ", zone=" << proc[i].zone << endl;
    }

    noop_line_break();
}

// -----------------------------------------------------------------------------
// disaster_type (Earthquake triggers set_zones; Flood prints info only)
// -----------------------------------------------------------------------------
void disaster_type(int type, int n)
{
        cout << "Disaster Type: Earthquake\nPrioritizing evacuation based on age and zone severity." << endl;
        set_zones(n);


}

void disaster_type_wrapper(int n)
{
    cout << "Enter disaster : Earthquake  ";
    int type = safe_input_int();
    disaster_type(type, n);
    noop_line_break();
}

// -----------------------------------------------------------------------------
// simulate_aftershock: 10% chance, adds delay, prints which remaining persons cannot be evacuated now
// -----------------------------------------------------------------------------
void simulate_aftershock(int &waiting_time, int totalprocess)
{
    int random_event = rand() % 10; // 10% chance
    if (random_event == 1)
    {
        cout << "Aftershock! Evacuation temporarily stopped." << endl;
        waiting_time += 10;
        // Inform which remaining persons (non-evacuated) are blocked due to aftershock
        for (int i = 0; i < totalprocess; ++i)
        {
            if (!evacuated[i])
            {
                cout << "Aftershock blocks evacuation of person " << proc[i].pno << " (pr=" << proc[i].pr << ", zone=" << proc[i].zone << ")" << endl;
            }
        }
    }
    else
    {
        cout << "No aftershock. Evacuation continues normally." << endl;
    }
}

void simulate_aftershock_wrapper(int totalprocess)
{
    cout << "Enter current waiting time to simulate aftershock: ";
    int waiting_time;
    cin >> waiting_time;
    simulate_aftershock(waiting_time, totalprocess);
    cout << "New waiting time: " << waiting_time << endl;
    noop_line_break();
}

// -----------------------------------------------------------------------------
// evacuation_report: meaningful count of evacuated[] true values
// -----------------------------------------------------------------------------
void evacuation_report(int totalprocess)
{
    int evacuated_count = 0;
    for (int i = 0; i < totalprocess; i++)
    {
        if (evacuated[i])
        {
            evacuated_count++;
        }
    }
    cout << "Total people evacuated: " << evacuated_count << "/" << totalprocess << endl;

    if (evacuated_count < totalprocess)
    {
        cout << "People remaining (not evacuated):" << endl;
        for (int i = 0; i < totalprocess; ++i)
        {
            if (!evacuated[i])
            {
                cout << "Person " << proc[i].pno << " (pr=" << proc[i].pr << ", zone=" << proc[i].zone << ")" << endl;
            }
        }
    }
}

void evacuation_report_wrapper(int totalprocess)
{
    verbose_print_header("Evacuation Report");
    evacuation_report(totalprocess);
    noop_line_break();
}

// -----------------------------------------------------------------------------
// suggest_best_option / printMenu (unchanged style)
// -----------------------------------------------------------------------------
void suggest_best_option(int n, int resources)
{
    if (n > 50)
    {
        cout << "Large crowd detected. Staged Evacuation is the most suitable option for managing large numbers." << endl;
    }
    else if (resources < n / 2)
    {
        cout << "Limited resources available. Use resource allocation method to manage evacuations effectively." << endl;
    }
    else
    {
        cout << "Regular evacuation with priority scheduling will be effective for this situation." << endl;
    }
}

void printMenu()
{
    cout << "\nChoose an option:\n";
    cout << "1. Simulate Evacuation\n";
    cout << "2. Update Priority of a Person\n";
    cout << "3. Allocate Resources\n";
    cout << "4. Set Disaster Type\n";
    cout << "5. Simulate Aftershock\n";
    cout << "6. Staged Evacuation\n";
    cout << "7. Evacuation Report\n";
    cout << "8. Exit\n";
    cout << endl;
    cout << "Note: The program uses arrival times as initial zeros and uses ages as burst times." << endl;
    cout << endl;
}

// -----------------------------------------------------------------------------
// main (wire-up wrappers and new signatures)
// -----------------------------------------------------------------------------
int main()
{
    cout << "\n\t\tDISASTER MANAGEMENT SYSTEM\n";
    noop_line_break();

    srand((unsigned)time(NULL));

    cout << "Enter the total number of people to be evacuated: ";
    int n;
    cin >> n;

    if (n <= 0)
    {
        cout << "Invalid number of people. Exiting." << endl;
        return 0;
    }
    if (n > 50)
    {
        cout << "Maximum supported people is 50 in this build. Please re-run with n <= 50." << endl;
        return 0;
    }

    current_n = n;

    cout << endl;
    debug_log("Number of people read: " + to_string(n));

    vector<int> arrivaltime(n);
    vector<int> bursttime(n);
    vector<int> priority(n);

    for (int i = 0; i < n; i++)
    {
        arrivaltime[i] = 0;
        bursttime[i] = 0;
        priority[i] = 0;
    }

    for (int i = 0; i < n; i++)
    {
        cout << "Enter the age of person " << i + 1 << ": ";
        int age;
        cin >> age;
        if (age < 0) age = 0;
        bursttime[i] = age;
        if (age < 18)
        {
            priority[i] = 1;
        }
        else if (age >= 18 && age < 60)
        {
            priority[i] = 3;
        }
        else
        {
            priority[i] = 2;
        }
    }

    for (int i = 0; i < n; i++)
    {
        proc[i].at = arrivaltime[i];
        proc[i].bt = bursttime[i];
        proc[i].pr = priority[i];
        proc[i].pno = i + 1;
        proc[i].zone = 0;
        evacuated[i] = false;
    }

    // Initial sort by arrival, zone (all 0), then priority
    sort(proc, proc + n, comp);

    int available_resources;
    cout << "Enter available rescue resources: ";
    cin >> available_resources;

    suggest_best_option(n, available_resources);

    while (true)
    {
        printMenu();

        int choice;
        cin >> choice;

        if (choice == 1)
        {
            findgc_wrapper(n);
        }
        else if (choice == 2)
        {
            update_priority_wrapper();
        }
        else if (choice == 3)
        {
            allocate_resources_wrapper(n, available_resources);
        }
        else if (choice == 4)
        {
            set_zones_wrapper(n);
        }
        else if (choice == 5)
        {
            simulate_aftershock_wrapper(n);
        }
        else if (choice == 6)
        {
            staged_evacuation_wrapper(n);
        }
        else if (choice == 7)
        {
            evacuation_report_wrapper(n);
        }
        else if (choice == 8)
        {
            cout << "\nwith the Priority Scheduling Algorithm, we devised a method to effectively evacuate people during a natural calamity.\n";
            break;
        }
        else
        {
            cout << "Invalid choice! Please select again." << endl;
        }

        cout << endl;
        noop_line_break();
    }

    noop_line_break();
    return 0;
}