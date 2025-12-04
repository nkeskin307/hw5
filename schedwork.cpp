#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, vector<size_t>& shiftCount, size_t currDay, size_t currSlot);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    //Preparing the size of our output: schedule for n days
    sched.resize(avail.size());

    //We will be using a vector to keep track of number of shifts per worker
    //This is to not allow any worker to work more than maxShifts over the n day period
    //We initialize it to 0 for each of the k workers
    size_t numWorkers= avail[0].size();
    vector<size_t> shiftCount(numWorkers, 0); 

    //We will backtrack
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftCount, 0, 0);

}


bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched, 
    vector<size_t>& shiftCount, 
    size_t currDay,
    size_t currSlot
)
{

    //BASECASE: If we have reached the number of days, we can return
    if (currDay==avail.size()){
        return true; 
    }

    //SECOND CASE: This is where we move on to the next day if we are done with current
    if (currSlot==dailyNeed){
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftCount, currDay+1, 0);
    }

    //RECURSIVE CASE: This is where we recurse with a for loop 
    //We loop through for each worker
    for (size_t i=0; i<avail[0].size(); i++){
        if (avail[currDay][i]  //Is the worker available? ? If yes keep going
             && shiftCount[i]<maxShifts  //Has the worker exceeded the max number of shifts? If not keep going
             && find(sched[currDay].begin(), sched[currDay].end(), i )== sched[currDay].end()){  //Is the worker already scheduled for the current day?, if not keep going, I used std::find

           //If all constraints satisfied, adding worker to our schedule & Incrementing the number of shifts of the worker
           sched[currDay].push_back(i);
           shiftCount[i]++; 

            if (scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftCount, currDay, currSlot+1)){ //Recursively trying to fill the next slot 
                return true; 
            }
            else{
                //BACKTRACK:
                sched[currDay].pop_back(); 
                shiftCount[i]--;
            }
        }
    }

    return false; 
}