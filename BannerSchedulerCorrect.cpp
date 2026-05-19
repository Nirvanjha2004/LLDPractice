#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;


// =====================================================
// ENUM
// =====================================================

enum class BannerStatus {
    ACTIVE,
    PAUSED,
    DELETED
};


// =====================================================
// BANNER
// =====================================================

class Banner {
private:

    int bannerId;

    string title;

    string imageUrl;

    int startTime;

    int endTime;

    int priority;

    BannerStatus status;


public:

    Banner() {
        status =
            BannerStatus::ACTIVE;
    }


    Banner(

        int id,

        string title,

        string imageUrl,

        int start,

        int end,

        int priority

    ) {

        this->bannerId =
            id;

        this->title =
            title;

        this->imageUrl =
            imageUrl;

        this->startTime =
            start;

        this->endTime =
            end;

        this->priority =
            priority;

        this->status =
            BannerStatus::ACTIVE;
    }



    bool isActive(
        int currentTime
    ) {

        return

            status ==
            BannerStatus::ACTIVE

            &&

            currentTime >=
            startTime

            &&

            currentTime <=
            endTime;
    }



    int getId() {

        return bannerId;
    }


    string getTitle() {

        return title;
    }


    int getPriority() {

        return priority;
    }


    void setStatus(
        BannerStatus st
    ) {

        status = st;
    }
};



// =====================================================
// BUILDER PATTERN
// =====================================================

class BannerBuilder {

private:

    int id;

    string title;

    string image;

    int start;

    int end;

    int priority;


public:

    BannerBuilder&
    setId(
        int val
    ) {

        id = val;

        return *this;
    }



    BannerBuilder&
    setTitle(
        string val
    ) {

        title = val;

        return *this;
    }



    BannerBuilder&
    setImage(
        string val
    ) {

        image = val;

        return *this;
    }



    BannerBuilder&
    setStart(
        int val
    ) {

        start = val;

        return *this;
    }



    BannerBuilder&
    setEnd(
        int val
    ) {

        end = val;

        return *this;
    }



    BannerBuilder&
    setPriority(
        int val
    ) {

        priority = val;

        return *this;
    }



    Banner* build() {

        return

            new Banner(

                id,

                title,

                image,

                start,

                end,

                priority
            );
    }
};



// =====================================================
// STRATEGY PATTERN
// =====================================================

class BannerSchedulingStrategy {

public:

    virtual Banner* pickBanner(

        vector<Banner*>& banners,

        int currentTime

    ) = 0;


    virtual ~BannerSchedulingStrategy()
    = default;
};



// =====================================================
// PRIORITY STRATEGY
// =====================================================

class PriorityStrategy
    : public BannerSchedulingStrategy {

public:

    Banner* pickBanner(

        vector<Banner*>& banners,

        int currentTime

    ) override {

        Banner* best = nullptr;


        for (

            auto banner :
            banners

        ) {

            if (

                !banner
                ->isActive(
                    currentTime
                )

            ) {

                continue;
            }


            if (

                best == nullptr

                ||

                banner
                ->getPriority()

                >

                best
                ->getPriority()

            ) {

                best = banner;
            }
        }


        return best;
    }
};



// =====================================================
// ROUND ROBIN STRATEGY
// =====================================================

class RoundRobinStrategy
    : public BannerSchedulingStrategy {

private:

    int index = 0;


public:

    Banner* pickBanner(

        vector<Banner*>& banners,

        int currentTime

    ) override {

        vector<Banner*> active;


        for (

            auto banner :
            banners

        ) {

            if (

                banner
                ->isActive(
                    currentTime
                )

            ) {

                active
                .push_back(
                    banner
                );
            }
        }


        if (

            active.empty()

        ) {

            return nullptr;
        }


        Banner* result =

            active[

                index %

                active.size()
            ];


        index++;


        return result;
    }
};



// =====================================================
// BANNER MANAGER
// =====================================================

class BannerManager {

private:

    unordered_map<
        int,
        Banner*
    > bannerMap;


    vector<Banner*>
    banners;


    BannerSchedulingStrategy*
    strategy;


public:

    BannerManager(

        BannerSchedulingStrategy*
        strategy

    ) {

        this->strategy =
            strategy;
    }



    void createBanner(

        int id,

        string title,

        string image,

        int start,

        int end,

        int priority

    ) {

        BannerBuilder builder;


        Banner* banner =

            builder

            .setId(id)

            .setTitle(title)

            .setImage(image)

            .setStart(start)

            .setEnd(end)

            .setPriority(priority)

            .build();


        banners
        .push_back(
            banner
        );


        bannerMap[id] =
            banner;
    }



    void pauseBanner(
        int id
    ) {

        if (

            bannerMap.count(id)

        ) {

            bannerMap[id]
            ->setStatus(

                BannerStatus::PAUSED
            );
        }
    }



    void resumeBanner(
        int id
    ) {

        if (

            bannerMap.count(id)

        ) {

            bannerMap[id]
            ->setStatus(

                BannerStatus::ACTIVE
            );
        }
    }



    void deleteBanner(
        int id
    ) {

        if (

            bannerMap.count(id)

        ) {

            bannerMap[id]
            ->setStatus(

                BannerStatus::DELETED
            );
        }
    }



    Banner* getCurrentBanner(
        int currentTime
    ) {

        return

            strategy
            ->pickBanner(

                banners,

                currentTime
            );
    }



    ~BannerManager() {

        for (

            auto banner :
            banners

        ) {

            delete banner;
        }


        delete strategy;
    }
};



// =====================================================
// DRIVER
// =====================================================

int main() {


    // Strategy can change dynamically


    BannerManager manager(

        new PriorityStrategy()

        // Try:
        // new RoundRobinStrategy()

    );



    manager.createBanner(

        1,

        "Summer Sale",

        "img1",

        100,

        500,

        5
    );



    manager.createBanner(

        2,

        "Flash Sale",

        "img2",

        200,

        400,

        10
    );



    manager.createBanner(

        3,

        "Weekend Offer",

        "img3",

        100,

        600,

        7
    );



    int currentTime = 300;



    Banner* current =

        manager
        .getCurrentBanner(

            currentTime
        );



    if (

        current != nullptr

    ) {

        cout

            << "Banner shown: "

            << current
            ->getTitle()

            << endl;
    }

    else {

        cout

            << "No banner available\n";
    }


    return 0;
}