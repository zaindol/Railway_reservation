// struct account{
//     int id;
//     char name[10];
//     char pass[30];
// };

struct bookings{
	int bid;
	int type;
	int acc_no;
	int tr_id;
	char trainname[20];
	int start_seat;
	int end_seat;
	int cancelled;
};

// char *path[3] = {"./customer.txt", "./agents.txt", "./admin.txt"};
#define trainpath "./train_details.txt"
#define bookingpath "./book_tickets.txt"

struct train{
    int tid;
    char train_name[20];
    int train_no;
    int available_seats;
    int last_seatno_used;
    int deleted;
};


int login(int sock);