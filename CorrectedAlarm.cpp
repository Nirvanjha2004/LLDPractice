#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <thread>
#include <chrono>
#include <ctime>

using namespace std;


// =====================================================
// ALARM
// =====================================================

class Alarm {

public:

    int alarmId;

    time_t triggerTime;

    string label;

    bool enabled;

    int snoozeDuration; // seconds

    bool repeatDaily;

    int version;


    Alarm(

        int id,

        time_t triggerTime,

        string label,

        int snoozeDuration,

        bool repeatDaily = false

    ) {

        this->alarmId = id;

        this->triggerTime = triggerTime;

        this->label = label;

        this->enabled = true;

        this->snoozeDuration =
            snoozeDuration;

        this->repeatDaily =
            repeatDaily;

        this->version = 1;
    }
};


// =====================================================
// HEAP NODE
// =====================================================

class HeapNode {

public:

    Alarm* alarm;

    time_t triggerTime;

    int version;


    HeapNode(
        Alarm* alarm
    ) {

        this->alarm = alarm;

        this->triggerTime =
            alarm->triggerTime;

        this->version =
            alarm->version;
    }
};


// =====================================================
// COMPARATOR
// =====================================================

class Compare {

public:

    bool operator()(

        HeapNode a,

        HeapNode b

    ) {

        return

            a.triggerTime >

            b.triggerTime;
    }
};


// =====================================================
// ALARM MANAGER
// =====================================================

class AlarmManager {

private:

    unordered_map<
        int,
        Alarm*
    > alarms;


    priority_queue<

        HeapNode,

        vector<HeapNode>,

        Compare

    > pq;


public:

    // =========================
    // ADD ALARM
    // =========================

    void addAlarm(

        Alarm* alarm

    ) {

        alarms[
            alarm->alarmId
        ] = alarm;


        pq.push(
            HeapNode(alarm)
        );
    }



    // =========================
    // ENABLE
    // =========================

    void enableAlarm(
        int id
    ) {

        if(
            alarms.count(id)
        ) {

            alarms[id]
            ->enabled = true;
        }
    }



    // =========================
    // DISABLE
    // =========================

    void disableAlarm(
        int id
    ) {

        if(
            alarms.count(id)
        ) {

            alarms[id]
            ->enabled = false;
        }
    }



    // =========================
    // DELETE
    // =========================

    void deleteAlarm(
        int id
    ) {

        if(
            alarms.count(id)
        ) {

            alarms[id]
            ->enabled = false;

            alarms.erase(id);
        }
    }



    // =========================
    // SNOOZE
    // =========================

    void snoozeAlarm(
        int id
    ) {

        if(
            !alarms.count(id)
        ) {
            return;
        }


        Alarm* alarm =
            alarms[id];


        if(
            !alarm->enabled
        ) {
            return;
        }


        // invalidate old heap entry
        alarm->version++;


        alarm->triggerTime +=
            alarm->snoozeDuration;


        pq.push(
            HeapNode(alarm)
        );


        cout

            << "Alarm snoozed: "

            << alarm->label

            << endl;
    }



    // =========================
    // RUN SCHEDULER
    // =========================

    void runScheduler() {

        while(true) {

            time_t currentTime =
                time(nullptr);


            while(

                !pq.empty()

                &&

                pq.top()
                .triggerTime

                <= currentTime

            ) {

                HeapNode node =
                    pq.top();

                pq.pop();


                Alarm* alarm =
                    node.alarm;


                // =====================
                // LAZY INVALIDATION
                // =====================

                if(

                    node.version

                    !=

                    alarm->version

                ) {

                    continue;
                }


                // deleted / disabled
                if(
                    !alarm->enabled
                ) {

                    continue;
                }


                // =====================
                // RING
                // =====================

                cout

                    << "RINGING: "

                    << alarm->label

                    << endl;


                // =====================
                // REPEAT
                // =====================

                if(
                    alarm->repeatDaily
                ) {

                    alarm->version++;

                    alarm->triggerTime +=

                        24 * 60 * 60;


                    pq.push(
                        HeapNode(alarm)
                    );
                }
            }


            // =====================
            // IMPORTANT
            // avoid busy waiting
            // =====================

            this_thread
            ::sleep_for(

                chrono::seconds(1)
            );
        }
    }
};



// =====================================================
// DRIVER
// =====================================================

int main() {

    AlarmManager manager;


    time_t now =
        time(nullptr);


    Alarm* a1 = new Alarm(

        1,

        now + 5,

        "Wake Up",

        10,

        true
    );


    Alarm* a2 = new Alarm(

        2,

        now + 8,

        "Meeting",

        5
    );


    manager.addAlarm(a1);

    manager.addAlarm(a2);


    thread scheduler(

        &AlarmManager::runScheduler,

        &manager
    );


    this_thread
    ::sleep_for(
        chrono::seconds(6)
    );


    manager.snoozeAlarm(1);


    scheduler.join();


    return 0;
}