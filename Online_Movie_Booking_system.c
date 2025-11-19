

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MOVIES 3
#define SHOWS 3
#define ROWS 3
#define COLS 5

typedef struct {
    char title[50];
    char showtime[SHOWS][20];
    char seats[SHOWS][ROWS][COLS];
    float base_price;
} Movie;

typedef struct {
    char name[50];
    char phone[20];
    char movie[50];
    char show_time[20];
    char seat[10];
    float amount;
    char payment_mode[20];
} Booking;

void init_movies(Movie m[]) {
    strcpy(m[0].title, "The Silent Dawn");
    strcpy(m[1].title, "Galaxy Raiders");
    strcpy(m[2].title, "Romance in Rain");

    char times[SHOWS][20] = {"10:00 AM", "1:00 PM", "4:00 PM"};

    for (int i = 0; i < MOVIES; i++) {
        for (int t = 0; t < SHOWS; t++)
            strcpy(m[i].showtime[t], times[t]);

        m[i].base_price = (i == 0 ? 120 : (i == 1 ? 150 : 100));

        for (int t = 0; t < SHOWS; t++)
            for (int r = 0; r < ROWS; r++)
                for (int c = 0; c < COLS; c++)
                    m[i].seats[t][r][c] = 'O';
    }
}

void show_movies(Movie m[]) {
    printf("\n------ MOVIES ------\n");
    for (int i = 0; i < MOVIES; i++)
        printf("%d. %s\n", i + 1, m[i].title);
}

void show_timings(Movie *mv) {
    printf("\nAvailable Show Timings:\n");
    for (int i = 0; i < SHOWS; i++)
        printf("%d. %s\n", i + 1, mv->showtime[i]);
}

void show_seats(Movie *mv, int showIndex) {
    printf("\nSeat Layout (%s)\n", mv->showtime[showIndex]);
    printf("    1  2  3  4  5\n");

    for (int r = 0; r < ROWS; r++) {
        printf(" %c ", 'A' + r);
        for (int c = 0; c < COLS; c++)
            printf("[%c]", mv->seats[showIndex][r][c]);
        printf("\n");
    }
    printf("[O] Available   [X] Booked\n");
}

int book_seat(Movie *mv, int showIndex, char *pos) {
    char rowChar = toupper(pos[0]);
    int row = rowChar - 'A';
    int col = atoi(pos + 1) - 1;

    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return -1;

    if (mv->seats[showIndex][row][col] == 'X')
        return 1;

    mv->seats[showIndex][row][col] = 'X';
    return 0;
}

float seat_price(Movie *mv, char *pos) {
    int row = toupper(pos[0]) - 'A';

    if (row == 0) return mv->base_price * 1.3;
    if (row == 1) return mv->base_price;
    return mv->base_price * 0.9;
}

void save_booking(Booking *b) {
    FILE *fp = fopen("bookings.txt", "a");
    if (!fp) return;

    fprintf(fp,
        "Name: %s | Phone: %s | Movie: %s | Show: %s | Seat: %s | Amount: %.2f | Mode: %s\n",
        b->name, b->phone, b->movie, b->show_time, b->seat, b->amount, b->payment_mode);
    fclose(fp);
}

void print_bill(Booking *b) {
    printf("\n=========== BILL ===========\n");
    printf("Name   : %s\n", b->name);
    printf("Phone  : %s\n", b->phone);
    printf("Movie  : %s\n", b->movie);
    printf("Show   : %s\n", b->show_time);
    printf("Seat   : %s\n", b->seat);
    printf("Amount : ₹%.2f\n", b->amount);
    printf("Payment: %s\n", b->payment_mode);
    printf("============================\n");
}

void booking_process(Movie movies[]) {
    Booking b;
    int movie_choice, show_choice;
    char seat[10];
    int seat_status;

    
    show_movies(movies);
    printf("Select movie: ");
    scanf("%d", &movie_choice);

    if (movie_choice < 1 || movie_choice > MOVIES) {
        printf("Invalid movie!\n");
        return;
    }

    Movie *mv = &movies[movie_choice - 1];


    show_timings(mv);
    printf("Select show time: ");
    scanf("%d", &show_choice);

    if (show_choice < 1 || show_choice > SHOWS) {
        printf("Invalid timing!\n");
        return;
    }
    int showIndex = show_choice - 1;

    
    show_seats(mv, showIndex);
    getchar();

    printf("Enter your name: ");
    fgets(b.name, 50, stdin);
    b.name[strcspn(b.name, "\n")] = 0;

    printf("Enter phone: ");
    fgets(b.phone, 20, stdin);
    b.phone[strcspn(b.phone, "\n")] = 0;


    do {
        printf("Choose seat (A1 - C5): ");
        scanf("%s", seat);

        seat_status = book_seat(mv, showIndex, seat);

        if (seat_status == -1)
            printf("Invalid seat!\n");
        else if (seat_status == 1)
            printf("Already booked!\n");
        else
            break;

    } while (1);

    
    strcpy(b.movie, mv->title);
    strcpy(b.show_time, mv->showtime[showIndex]);
    strcpy(b.seat, seat);
    b.amount = seat_price(mv, seat);

    
    int pay;
    printf("\nPayment Mode: 1.Card  2.UPI  3.Cash\nChoose: ");
    scanf("%d", &pay);

    if (pay == 1) strcpy(b.payment_mode, "Card");
    else if (pay == 2) strcpy(b.payment_mode, "UPI");
    else strcpy(b.payment_mode, "Cash");

    printf("Payment Successful!\n");

    save_booking(&b);
    print_bill(&b);
}

int main() {
    Movie movies[MOVIES];
    init_movies(movies);

    int choice;

    do {
        printf("\n=========== MAIN MENU ===========\n");
        printf("1. View Movies\n");
        printf("2. Book Ticket\n");
        printf("3. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                show_movies(movies);
                break;

            case 2:
                booking_process(movies);
                break;

            case 3:
                printf("Thank you! Come again.\n");
                break;

            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 3);

    return 0;
}
