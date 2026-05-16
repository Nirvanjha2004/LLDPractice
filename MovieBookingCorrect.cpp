#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>

using namespace std;


/* ===================================
   ENUMS
=================================== */

enum class SeatType {
    REGULAR,
    PREMIUM,
    RECLINER
};

enum class BookingStatus {
    CREATED,
    PAYMENT_PENDING,
    CONFIRMED,
    CANCELLED,
    EXPIRED
};



/* ===================================
   USER
=================================== */

class User {
private:
    int id;
    string name;

public:
    User(int id, string name) {
        this->id = id;
        this->name = name;
    }

    string getName() {
        return name;
    }
};



/* ===================================
   MOVIE
=================================== */

class Movie {
private:
    int id;
    string title;

public:
    Movie(int id, string title) {
        this->id = id;
        this->title = title;
    }

    string getTitle() {
        return title;
    }
};



/* ===================================
   SEAT
   Physical seat
=================================== */

class Seat {
private:
    int seatId;
    SeatType type;

public:
    Seat(int seatId, SeatType type) {
        this->seatId = seatId;
        this->type = type;
    }

    int getId() {
        return seatId;
    }

    SeatType getType() {
        return type;
    }
};



/* ===================================
   SCREEN
=================================== */

class Screen {
private:
    int screenId;
    vector<Seat*> seats;

public:
    Screen(int id) {
        this->screenId = id;
    }

    void addSeat(Seat* seat) {
        seats.push_back(seat);
    }

    vector<Seat*>& getSeats() {
        return seats;
    }
};



/* ===================================
   SHOW SEAT
   Dynamic seat status for a show
=================================== */

class ShowSeat {
private:
    Seat* seat;

    bool booked;
    bool locked;

    time_t lockTime;

public:
    ShowSeat(Seat* seat) {
        this->seat = seat;

        booked = false;
        locked = false;
        lockTime = 0;
    }

    bool isBooked() {
        return booked;
    }

    bool isLocked() {
        return locked;
    }

    int getSeatId() {
        return seat->getId();
    }

    bool lockSeat() {

        if (booked || locked) {
            return false;
        }

        locked = true;
        lockTime = time(nullptr);

        return true;
    }

    void releaseSeat() {
        locked = false;
    }

    void confirmSeat() {
        booked = true;
        locked = false;
    }
};



/* ===================================
   SHOW
=================================== */

class Show {
private:
    int showId;

    Movie* movie;
    Screen* screen;

    unordered_map<
        int,
        ShowSeat*
    > showSeats;

public:

    Show(
        int showId,
        Movie* movie,
        Screen* screen
    ) {

        this->showId = showId;

        this->movie = movie;

        this->screen = screen;


        for (auto seat : screen->getSeats()) {

            showSeats[
                seat->getId()
            ] = new ShowSeat(seat);
        }
    }

    ShowSeat* getSeat(
        int seatId
    ) {

        if (
            !showSeats.count(seatId)
        ) {

            return nullptr;
        }

        return showSeats[seatId];
    }

    string getMovieName() {
        return movie->getTitle();
    }
};



/* ===================================
   PAYMENT STRATEGY
=================================== */

class PaymentStrategy {

public:

    virtual bool pay(
        int amount
    ) = 0;

    virtual ~PaymentStrategy() =
        default;
};


class UPI : public PaymentStrategy {

public:

    bool pay(
        int amount
    ) override {

        cout
            << "UPI Payment of ₹"
            << amount
            << " successful\n";

        return true;
    }
};


class CardPayment
    : public PaymentStrategy {

public:

    bool pay(
        int amount
    ) override {

        cout
            << "Card Payment of ₹"
            << amount
            << " successful\n";

        return true;
    }
};



/* ===================================
   BOOKING
=================================== */

class Booking {

private:

    int bookingId;

    User* user;
    Show* show;

    vector<ShowSeat*> seats;

    int amount;

    BookingStatus status;

public:

    Booking(
        int id,
        User* user,
        Show* show,
        vector<ShowSeat*> seats,
        int amount
    ) {

        this->bookingId = id;

        this->user = user;

        this->show = show;

        this->seats = seats;

        this->amount = amount;

        status =
            BookingStatus::
            CREATED;
    }


    void confirm() {

        for (
            auto seat :
            seats
        ) {

            seat->confirmSeat();
        }

        status =
            BookingStatus::
            CONFIRMED;
    }


    void cancel() {

        for (
            auto seat :
            seats
        ) {

            seat->releaseSeat();
        }

        status =
            BookingStatus::
            CANCELLED;
    }


    int getAmount() {

        return amount;
    }


    void printBooking() {

        cout
            << "\nBooking Confirmed\n";

        cout
            << "Movie: "
            << show
            ->getMovieName()
            << endl;

        cout
            << "User: "
            << user
            ->getName()
            << endl;

        cout
            << "Amount: ₹"
            << amount
            << endl;
    }
};



/* ===================================
   SEAT LOCK MANAGER
=================================== */

class SeatLockManager {

public:

    bool lockSeats(
        vector<ShowSeat*>& seats
    ) {

        for (
            auto seat :
            seats
        ) {

            bool success =
                seat
                ->lockSeat();

            if (
                !success
            ) {

                return false;
            }
        }

        return true;
    }


    void releaseSeats(
        vector<ShowSeat*>& seats
    ) {

        for (
            auto seat :
            seats
        ) {

            seat
            ->releaseSeat();
        }
    }
};



/* ===================================
   MOVIE BOOKING SYSTEM
=================================== */

class MovieBookingSystem {

private:

    int nextBookingId;

    SeatLockManager*
        lockManager;

public:

    MovieBookingSystem() {

        nextBookingId = 1;

        lockManager =
            new SeatLockManager();
    }


    Booking* bookTickets(

        User* user,

        Show* show,

        vector<int>
        seatIds,

        PaymentStrategy*
        payment
    ) {

        vector<ShowSeat*>
            selectedSeats;


        for (
            int seatId :
            seatIds
        ) {

            ShowSeat*
                seat =
                show
                ->getSeat(
                    seatId
                );

            if (
                !seat
            ) {

                cout
                    << "Seat doesn't exist\n";

                return nullptr;
            }

            selectedSeats
            .push_back(
                seat
            );
        }


        bool locked =
            lockManager
            ->lockSeats(
                selectedSeats
            );


        if (
            !locked
        ) {

            cout
                << "Seat already booked/locked\n";

            return nullptr;
        }


        int amount =
            seatIds.size()
            * 200;


        Booking*
            booking =
            new Booking(
                nextBookingId++,
                user,
                show,
                selectedSeats,
                amount
            );


        bool paymentSuccess =
            payment
            ->pay(
                amount
            );


        if (
            !paymentSuccess
        ) {

            booking
            ->cancel();

            return nullptr;
        }


        booking
        ->confirm();

        return booking;
    }
};



/* ===================================
   MAIN
=================================== */

int main() {

    User* user =
        new User(
            1,
            "Nirvan"
        );


    Movie* movie =
        new Movie(
            101,
            "Interstellar"
        );


    Screen* screen =
        new Screen(1);


    screen
    ->addSeat(
        new Seat(
            1,
            SeatType::REGULAR
        )
    );

    screen
    ->addSeat(
        new Seat(
            2,
            SeatType::PREMIUM
        )
    );

    screen
    ->addSeat(
        new Seat(
            3,
            SeatType::RECLINER
        )
    );


    Show* show =
        new Show(
            1,
            movie,
            screen
        );


    MovieBookingSystem
        system;


    PaymentStrategy*
        payment =
        new UPI();


    Booking* booking =
        system
        .bookTickets(

            user,

            show,

            {1, 2},

            payment
        );


    if (
        booking
    ) {

        booking
        ->printBooking();
    }


    return 0;
}