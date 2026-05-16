#include<string>
#include<unordered_map>
#include<unordered_set>
using namespace std; 
class User{
    int userId;
    string name;
    string email;
    User(int userId, string name, string email){
        this->userId = userId;
        this->name = name; 
        this->email = email; 
    }
};

class Movie{
    int movieId;
    int duration;
    string title; 
    string genre; 
    string language;

    Movie(string title, string genre, string language, int movieId, int duration){
        this->duration = duration; 
        this->movieId = movieId;
        this->title = title;
        this->genre = genre; 
        this->language = language; 
    }
};

class Theatre{
    int theatreId; 
    string location;
    string name;
    vector<Screen*> screens; 

    Theatre(int id, string location, string name){
        this->theatreId = id; 
        this->location = location; 
        this->name = name; 
    }
};

class Screen{
    int scId; 
    vector<Seat*> seats;  
    Screen(int id){
        this->scId = id; 
    }
};
enum class SeatType {
    REGULAR,
    PREMIUM,
    RECLINER
}; 
class Seat{
    public:
    int seatId;
    SeatType seat;
    bool avl; 
    Seat(int id, SeatType st){
        this->seatId = id; 
        this->avl = true; 
        this->seat = st; 
    }

    void Lock(){
        this->avl = false; 
    }

    void release(){
        this->avl = true; 
    }
};

class Show{
    int showId;
    Movie* mv; 
    Screen * sc; 
    int stTime; 
    int edTime; 

    Show(int id, Movie* mr, Screen* sc, int st, int ed){
        this->edTime = ed; 
        this->mv = mr; 
        this->sc = sc; 
        this->showId = id; 
        this->stTime = st; 
    }
};

class Booking{
    int bookId;
    User* user;
    Show* show;
    vector<Seat*> seats;
    int amnt;

    Booking(int id, User* user, Show* show, int amnt){
        this->amnt = amnt;
        this->bookId = id; 
        this->show = show; 
        this->user = user; 
    }
}; 

class SeatLockManager{
    public:

    void lock_seat(Seat* seat){
        seat->Lock();
    }

    void release_seat(Seat* seat){
        seat->release();
    }
};

class BookingSystem{
    unordered_set<Movie*> movies;
    unordered_set<Theatre*> theatres; 
    Show* sh;
    User* ur; 
    vector<Seat*> seats; 
    SeatLockManager* seatmanager;
    void select_show(Show* sh){
        this->sh = sh;
    };

    void select_seats(vector<Seat*>st){
        for(auto it : st){
            seats.push_back(it);
            seatmanager->lock_seat(it);
        }
    }
    
}
