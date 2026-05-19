#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include<queue>
using namespace std;

// for different types of alarms, we can create a alarmBehaviour class and use strategy pattern
class Alarm{
    public:
    int alarmId;
    int time;
    string label;
    vector<int>repeatDays;
    bool enabled; 
    int snoozeDur;

    Alarm(int alarmId, int time, string label, vector<int> repeatDays, bool enabled, int snoozeDur){
        this->alarmId = alarmId;
        this->time = time;
        this->label = label;
        this->repeatDays = repeatDays;
        this->enabled = enabled;
        this->snoozeDur = snoozeDur;
    }
};

class comparator{
    public:
    bool operator()(Alarm* a, Alarm* b){
        return a->time > b->time; // Min-heap based on alarm time
    }
};

class AlarmManager{
    unordered_map<int, Alarm*> alarms;
    priority_queue<Alarm*, vector<Alarm*>, comparator> alarmQueue; // Min-heap based on alarm time
    public:
    void addAlarm(Alarm* alarm){
        alarms[alarm->alarmId] = alarm;
        alarmQueue.push(alarm);
    }

    void enableAlarm(int alarmId){
        auto it = alarms.find(alarmId);
        if(it != alarms.end()){
            it->second->enabled = true;
        }
    }

    void disableAlarm(int alarmId){
        auto it = alarms.find(alarmId);
        if(it != alarms.end()){
            it->second->enabled = false;
        }
    }

    void changeLabel(int alarmId, string newLabel){
        auto it = alarms.find(alarmId);
        if(it != alarms.end()){
            it->second->label = newLabel;
        }
    }

    void changeSnoozeDuration(int alarmId, int newSnoozeDur){
        auto it = alarms.find(alarmId);
        if(it != alarms.end()){
            it->second->snoozeDur = newSnoozeDur;
        }
    }

    void snoozeAlarm(int alarmId){
        auto it = alarms.find(alarmId);
        if(it != alarms.end() && it->second->enabled){
            it->second->time += it->second->snoozeDur;
            cout << "Snoozing alarm " << alarmId << " for " << it->second->snoozeDur << " minutes." << endl;
            // Reinsert the alarm into the priority queue with the new time
            alarmQueue.push(it->second);
        }
    }

    void deleteAlarm(int alarmId){
        alarms.erase(alarmId);
    }

    void runAlarm(){
        while(true){
            int currentTime = time(nullptr);
            if(!alarmQueue.empty() && alarmQueue.top()->time == currentTime){
                Alarm* alarm = alarmQueue.top();
                alarmQueue.pop();
                if(alarm->enabled){
                    cout << "Alarm " << alarm->alarmId << " is ringing! Label: " << alarm->label << endl;
                    // If alarm is set to repeat, reschedule it for the next day
                    if(!alarm->repeatDays.empty()){
                        alarm->time += 24 * 60; // Reschedule for the next day
                        alarmQueue.push(alarm);
                    }
                }
            }
        }
    }
};