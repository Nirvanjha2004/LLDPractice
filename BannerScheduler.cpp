#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <ctime>

using namespace std;
enum class BannerStatus{
    ACTIVE,
    PAUSED,
    DELETED
}; 
class Banner {
    public:
    int bannerId; 
    string title;
    string img;
    int start;
    int end;
    int priority;
    BannerStatus* bs; 

    public:
    // Banner(int id, string title, string img, int st, int en, int p){
    //     this->end = en;
    //     this->start = st; 
    //     this->bannerId = id; 
    //     this->title = title; 
    //     this->img = img;
    //     this->priority = p;
    // }

    int get_priority(){
        return priority;
    }

    BannerStatus getStatus(){
        return *this->bs; 
    }

    Banner* getCurrent(){
        return this;
    }
};

class BannerBuilder {
    private:
    Banner banner;
    public:

    BannerBuilder& setTitle( string name ){
        banner.title = name; 
        return *this; 
    }

    BannerBuilder& setId( int id ){
        banner.bannerId= id; 
        return *this; 
    }

    BannerBuilder& setimage( string img ){
        banner.img = img; 
        return *this; 
    }

    BannerBuilder& setPriority( int p ){
        banner.priority = p; 
        return *this; 
    }

    Banner build(){
        return banner;
    }

};


class BannerScheduling {

}; 

class Priority : public BannerScheduling{
    public:
    
}; 

class TimeBased : public BannerScheduling{

}; 

class BannerManagement{
    public:
    unordered_map<int, Banner> idBanner;
    Banner bn;
    // Create a banner
    void create_banner( int id, string title, string img, int st, int en, int p){
        BannerBuilder bb;
        bn = bb.setId(id).setTitle(title).setimage(img).setPriority(p).build();
        idBanner[id] = bn;
    }

    void update_banner( int id, string title, string img, int st, int en, int p){
        if( idBanner.find(id) != idBanner.end() ){
            BannerBuilder bb;
            bn = bb.setId(id).setTitle(title).setimage(img).setPriority(p).build();
            idBanner[id] = bn;
        }
    }

    void getCurrentBanner(int id){
        if( idBanner.find(id) != idBanner.end() ){
            Banner b = idBanner[id];
            cout << "Current Banner: " << b.title << endl;
        }
    }
};

