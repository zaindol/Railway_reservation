#include "login.h"

#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include <string.h>
#include<strings.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<stdio.h>

#include "signup.h"
#include "login_function.h"

// int admin_op(int sock, int id);
// int user_op(int sock, int id, int type);
// void view_booking(int sock, int id, int type);

int login(int sock){
    int type, acc_no, valid = 1, invalid  = 0, login_success = 0;
    char password[30];

    struct account temp;

    read(sock, &type, sizeof(type));
    read(sock, &acc_no, sizeof(acc_no));
    read(sock, &password, sizeof(password));

    printf("got details\n");
    // const char *path1 = "/home/anon/Documents/SS Lab-Handson 2/Mini_project/file1.txt";
    int fd;
    fd = open(path[type-1], O_RDWR,00400|00200);
    perror("open");

    struct flock lock;

    lock.l_type = F_WRLCK;
    lock.l_start = (acc_no-1) * sizeof(struct account);
    lock.l_len = sizeof(struct account);
    lock.l_whence = SEEK_SET;
    lock.l_pid = getpid();

    //For user
    if(type == 1){
        fcntl(fd, F_SETLK, &lock);
        perror("fcntl");

        printf("Before lseek\n");
        lseek(fd, (acc_no-1)*sizeof(struct account), SEEK_CUR);
        read(fd, &temp, sizeof(struct account));

        
        if(temp.id == acc_no){
            if(!strcmp(temp.pass,password)){
                printf("acc_no = %d\n",temp.id);
                write(sock, &valid, sizeof(valid));
                perror("write");
                // while(getchar()!='\n');
                // getchar();
                while(user_op(sock, temp.id, type) != -1);
                printf("inside if\n");
                login_success = 1;
            }
        }
        lock.l_type = F_UNLCK;
        fcntl(fd,F_SETLK,&lock);
        close(fd);
        if(login_success)
            return 3;
    }

    //For admin
    else if(type == 3){
        lock.l_type = F_WRLCK;
        fcntl(fd, F_SETLK, &lock);
        perror("fcntl");

        lseek(fd, (acc_no-1)*sizeof(struct account), SEEK_CUR);

        read(fd, &temp, sizeof(struct account));

        
        if(temp.id == acc_no){
            if(!strcmp(temp.pass,password)){
                printf("acc_no = %d\n",temp.id);
                write(sock, &valid, sizeof(valid));
                perror("write");
                // while(getchar()!='\n');
                // getchar();
                while(admin_op(sock, temp.id) != -1);
                printf("inside if\n");
                login_success = 1;
            }
        }
        lock.l_type = F_UNLCK;
        fcntl(fd,F_SETLK,&lock);
        close(fd);
        if(login_success)
            return 3;

    }

    write(sock, &invalid, sizeof(invalid));
    return 3;

}


// int admin_op(int sock, int id){
//     int op_choice;
//     read(sock, &op_choice, sizeof(op_choice));
   

//     //Add train
//     if(op_choice == 1){
//         int tid = 0, tno, av_seats;
//         char tname[20];

//         read(sock, &tname, sizeof(tname));
//         read(sock, &tno, sizeof(tno));
//         read(sock, &av_seats, sizeof(av_seats));

//         struct train temp, temp2;

//         temp.tid = tid;
//         temp.train_no = tno;
//         temp.available_seats = av_seats;
//         strcpy(temp.train_name, tname);
//         temp.last_seatno_used = 0;
//         temp.deleted = 0;

//         int fd = open(trainpath, O_CREAT|O_RDWR, 00400|00200);

//         struct flock lock;
//         lock.l_type = F_WRLCK;
//         lock.l_whence = SEEK_SET;
//         lock.l_start = 0;
//         lock.l_len = 0;
//         lock.l_pid= getpid();

//         fcntl(fd, F_SETLKW, &lock);
//         perror("fcntl");

//         int fp = lseek(fd, 0, SEEK_END);
        
//         if(fp == 0){//1st record
//             write(fd, &temp, sizeof(temp));
//             lock.l_type = F_UNLCK;
//             fcntl(fd, F_SETLK, &lock);
//             close(fd);
//             write(sock, &op_choice, sizeof(op_choice));
//         }
//         else{
//             lseek(fd, -1*sizeof(struct train), SEEK_CUR);
//             read(fd, &temp2, sizeof(temp2));
//             temp.tid = temp2.tid + 1;
//             write(fd, &temp, sizeof(temp));
//             write(sock, &op_choice, sizeof(op_choice));
//             lock.l_type = F_UNLCK;
//             fcntl(fd, F_SETLK, &lock);
//             close(fd);
//         }

//         return 1;
//     }

//     //Delete Train
//     if(op_choice == 2){
//         printf("Inside del train\n");
//         int fd = open(trainpath, O_RDWR);
//         perror("open");

//         int response;
//         struct flock lock;
//         lock.l_type = F_WRLCK;
//         lock.l_whence = SEEK_SET;
//         lock.l_start = 0;
//         lock.l_len = 0;
//         lock.l_pid = getpid();

//         fcntl(fd,F_SETLKW, &lock);

//         //set pointer to last record to find no of records
//         int fp = lseek(fd,0,SEEK_END);
//         int no_of_trains = fp/sizeof(struct train);

//         // printf("Number of train records : %d\n",no_of_trains);

//         write(sock, &no_of_trains, sizeof(no_of_trains));

//         //set pointer at start
//         lseek(fd, 0, SEEK_SET);

//         struct train temp;

//         //run loop till last record
//         while(lseek(fd, 0, SEEK_CUR) != fp)
//         {
//             read(fd, &temp, sizeof(struct train));
//             write(sock, &temp.tid, sizeof(int));
//             write(sock, &temp.train_name, sizeof(temp.train_name));
//             write(sock, &temp.train_no, sizeof(int));
//             write(sock, &temp.deleted, sizeof(int));
//         }

//         read(sock, &response, sizeof(response));

//         if(response != -2){
//             struct train temp;
//             //move pointer to location of record to be deleted
//             lseek(fd, (response)*sizeof(struct train), SEEK_SET);
//             read(fd, &temp, sizeof(struct train));

//             printf("%s train's record is deleted\n",temp.train_name);
//             //set deleted = 1 to show this is deleted
//             temp.deleted = 1;

//             //You have to overwrite this new value on the current value, so first move
//             //pointer back to the record as it moves forward everytime then overwrite it.

//             lseek(fd, -1*sizeof(struct train), SEEK_CUR);
// 			write(fd, &temp, sizeof(struct train));    
//         }
//         write(sock, &response, sizeof(response));

//         lock.l_type = F_UNLCK;
//         fcntl(fd, F_SETLK, &lock);
        
//         close(fd);
//         return 2;
//     }

//     //Modify Train

//     if(op_choice == 3){
//         printf("Inside Modify function\n");
//         int response;
//         int fd = open(trainpath, O_RDWR);
//         perror("open");

//         struct flock lock;
//         lock.l_type = F_WRLCK;
//         lock.l_start = 0;
//         lock.l_len = 0;
//         lock.l_whence = SEEK_SET;
//         lock.l_pid = getpid();

//         fcntl(fd, F_SETLKW, &lock);

//         int fp = lseek(fd, 0, SEEK_END);

//         int no_of_trains = fp / sizeof(struct train);

//         write(sock, &no_of_trains, sizeof(no_of_trains));

//         //Set pntr at start
//         lseek(fd, 0, SEEK_SET);

//         while(lseek(fd,0, SEEK_CUR) != fp)
//         {
//             struct train temp;
//             read(fd, &temp, sizeof(temp));

//             write(sock, &temp.tid, sizeof(temp.tid));
//             write(sock, &temp.train_no, sizeof(temp.train_no));
//             write(sock, &temp.deleted, sizeof(temp.deleted));
//             write(sock, &temp.available_seats, sizeof(temp.available_seats));
//             write(sock, &temp.train_name, sizeof(temp.train_name));
//         }

//         //Reading train id
//         read(sock, &response, sizeof(response));

//         if(response != -2){
//             struct train temp;
//             //Reach at that particular data
//             // lseek(fd, 0, SEEK_SET);
//             lseek(fd, (response)*sizeof(struct train), SEEK_SET);
//             read(fd, &temp, sizeof(temp));

//             int data_to_modify;
//             read(sock, &data_to_modify, sizeof(data_to_modify));

//             //Modify name
//             if(data_to_modify == 1){
//                 char name[20];
//                 write(sock, &temp.train_name, sizeof(temp.train_name));
//                 read(sock,&name, sizeof(name));
//                 strcpy(temp.train_name, name);
//             }

//             //Modify train no
//             else if(data_to_modify == 2){
//                 write(sock, &temp.train_no, sizeof(temp.train_no));
//                 read(sock, &temp.train_no, sizeof(temp.train_no));
//             }

//             //Modify Available seats
//             else{
//                 write(sock, &temp.available_seats, sizeof(temp.available_seats));
//                 read(sock, &temp.available_seats, sizeof(temp.available_seats));
//             }

//             op_choice = 3;
//             printf("Modified data : \n");
//             printf("Train name: %s\t Train no: %d\t Available seats: %d\n",temp.train_name,temp.train_no,temp.available_seats);

//             lseek(fd, -1*sizeof(struct train), SEEK_CUR);
//             write(fd, &temp, sizeof(temp));
//         }
//         write(sock, &response, sizeof(response));

//         lock.l_type = F_UNLCK;
//         fcntl(fd, F_SETLK, &lock);
//         close(fd);
//         return 3;   
//     }

//     //Delete User

//     if(op_choice == 4){
//         printf("Inside Del user function\n");

//         int type, id;
//         struct account acc;

//         read(sock, &type, sizeof(type));

//         int fd = open(path[type-1], O_RDWR);
//         struct flock lock;
//         lock.l_type = F_WRLCK;
//         lock.l_whence = SEEK_SET;
//         lock.l_start = 0;
//         lock.l_len = 0;
//         lock.l_pid = getpid();

//         fcntl(fd, F_SETLKW, &lock);
//         perror("fcntl");

//         int fp = lseek(fd, 0, SEEK_END);
//         int no_of_users = fp/sizeof(struct account);
//         printf("no of uesers = %d\n",no_of_users);
//         write(sock, &no_of_users, sizeof(no_of_users));

//         lseek(fd, 0, SEEK_SET);

//         while(lseek(fd, 0, SEEK_CUR) != fp)
//         {
//             read(fd, &acc, sizeof(struct account));

//             write(sock, &acc.name, sizeof(acc.name));
//             write(sock, &acc.id, sizeof(acc.id));
//             printf("id = %d\t acc = %s\n",acc.id,acc.name);
//         }
        
//         //reading response
//         read(sock, &id, sizeof(id));

//         if(id != -2){
//             //move to that data point
//             lseek(fd, 0, SEEK_SET);
//             lseek(fd, (id-1)*sizeof(struct account), SEEK_CUR);
//             read(fd, &acc, sizeof(struct account));

//             //To overwrite for deleting the user
//             lseek(fd, -1*sizeof(struct account),SEEK_CUR);
//             strcpy(acc.name, "deleted");
//             strcpy(acc.pass, "");
//             write(fd, &acc, sizeof(struct account));
//         }
//         write(sock, &id, sizeof(id));

//         lock.l_type = F_UNLCK;
//         fcntl(fd, F_SETLK, &lock);
//         close(fd);
//         return 4;

//     }

//     //Search User

//     if(op_choice == 5){
//         printf("Inside search user function\n");

//         int type, id;
//         struct account acc;

//         read(sock, &type, sizeof(type));

//         int fd = open(path[type-1], O_RDONLY);
//         struct flock lock;
//         lock.l_type = F_RDLCK;
//         lock.l_start = 0;
//         lock.l_len = 0;
//         lock.l_whence = SEEK_SET;
//         lock.l_pid = getpid();

//         fcntl(fd, F_SETLKW, &lock);
//         perror("fcntl");

//         read(sock, &id, sizeof(id));

//         if(id != -2){
//             printf("ID = %d\n",id);
//             lseek(fd, (id-1)*sizeof(struct account), SEEK_SET);
//             read(fd, &acc, sizeof(acc));

//             write(sock, &acc.name, sizeof(acc.name));
//             write(sock, &acc.id, sizeof(acc.id));
//         }
//         // write(sock, &id, sizeof(id));

//         lock.l_type = F_UNLCK;
//         fcntl(fd, F_SETLK, &lock);
//         close(fd);
//         return 5;
//     }

//     if(op_choice == 6) {
// 		write(sock,&op_choice, sizeof(op_choice));
// 		return -1;
// 	}


// }

// int user_op(int sock, int id, int type){
//     int op_choice;
//     read(sock, &op_choice, sizeof(op_choice));

//     //Book Ticket
//     if(op_choice == 1){
//         int fd = open(trainpath,O_RDWR);

//         struct flock lock;
//         lock.l_type = F_WRLCK;
//         lock.l_start = 0;
//         lock.l_len = 0;
//         lock.l_whence = SEEK_SET;
//         lock.l_pid = getpid();

//         fcntl(fd, F_SETLKW, &lock);
//         perror("lock");

//         struct train temp, temp2;

//         int fp = lseek(fd, 0, SEEK_END);
//         int no_of_trains = fp/sizeof(struct train);

//         write(sock, &no_of_trains, sizeof(no_of_trains));

//         lseek(fd, 0, SEEK_SET);

//         while(lseek(fd, 0, SEEK_CUR) != fp)
//         {
//             read(fd, &temp, sizeof(struct train));

//             write(sock, &temp.tid, sizeof(temp.tid));
//             write(sock, &temp.available_seats, sizeof(temp.available_seats));
//             write(sock, &temp.train_no, sizeof(temp.train_no));
//             write(sock, &temp.train_name, sizeof(temp.train_name));
//             write(sock, &temp.deleted, sizeof(temp.deleted));

//         }

//         int tid, seat_requirement;

//         read(sock, &tid, sizeof(tid));

//         lseek(fd, (tid)*sizeof(struct train), SEEK_SET);

//         read(fd, &temp2, sizeof(struct train));

//         write(sock, &temp2.available_seats, sizeof(int));
//         read(sock, &seat_requirement, sizeof(int));

//         if(seat_requirement > 0){
//             temp2.available_seats -= seat_requirement;

//             int fd2 = open(bookingpath, O_CREAT|O_RDWR, 00400|00200);

//             fcntl(fd2, F_SETLKW, &lock);
//             perror("fcntl");

//             struct bookings book;

//             int fp2 = lseek(fd2, 0, SEEK_END);

//             if(fp2 > 0){
//                 lseek(fd2, -1 * sizeof(struct bookings), SEEK_CUR);
//                 read(fd2, &book, sizeof(struct bookings));
//                 book.bid++;
//             }
//             //for first record
//             else
//                 book.bid = 0;

//             book.type = type;
//             book.acc_no = id;
//             book.cancelled = 0;
//             strcpy(book.trainname, temp2.train_name);
//             book.tr_id = temp2.tid;
//             book.start_seat = temp2.last_seatno_used + 1;    
//             book.end_seat = temp2.last_seatno_used + seat_requirement;

//             temp2.last_seatno_used = book.end_seat;

//             write(fd2, &book, sizeof(book));

//             lock.l_type = F_UNLCK;
//             fcntl(fd2, F_SETLK, &lock);
//             close(fd2);

//             lseek(fd, -1 * sizeof(struct train), SEEK_CUR);
//             write(fd, &temp2, sizeof(struct train));
//         }

//         fcntl(fd, F_SETLK, &lock);

//         close(fd);

//         if(seat_requirement <= 0)
//             op_choice = -1;

//         write(sock, &op_choice, sizeof(op_choice));
//         return 1;
//     }

//     //View Booking
//     if(op_choice == 2){
//         view_booking(sock, id, type);
// 		write(sock, &op_choice, sizeof(op_choice));
// 		return 2;
//     }

//     //Update Booking
//     if(op_choice == 3){
//         int info;

//         //to view the booking
//         view_booking(sock, id, type);

//         //we need to update available seats accordingly so both files have to open
//         int fd1 = open(trainpath, O_RDWR);
//         int fd2 = open(bookingpath, O_RDWR);

//         struct flock lock;
//         lock.l_type = F_WRLCK;
//         lock.l_whence = SEEK_SET;
//         lock.l_start = 0;
//         lock.l_len = 0;
//         lock.l_pid = getpid();

//         fcntl(fd1, F_SETLKW, &lock);
//         perror("fcntl");
//         fcntl(fd2, F_SETLKW, &lock);
//         perror("fcntl");

//         int id;
//         struct bookings book;
//         struct train temp;
        
//         //reading booking id 
//         read(sock, &id, sizeof(id));

//         //pointing at the start of booking data
//         lseek(fd2, 0, SEEK_SET);

//         //pointing to the entry that we want to update
//         lseek(fd2, id * sizeof(struct bookings), SEEK_CUR);

//         //read the booking entry & pointer reach at the end     
//         read(fd2, &book, sizeof(book));

//         //pointing at the start of entry of id data
//         lseek(fd2, -1*sizeof(struct bookings), SEEK_CUR);

//         printf("Train Id = %d\t Train Name = %s\t Last Seat = %d\n",book.tr_id,book.trainname,book.end_seat);

//         //pointing at the start of train data
//         lseek(fd1, 0, SEEK_SET);

//         //pointing at the train entry of train with tid
//         lseek(fd1, (book.tr_id-1)*sizeof(struct train), SEEK_CUR);

//         read(fd1, &temp, sizeof(struct train));

//         //pointing at the start of entry of tid data
//         lseek(fd1, -1*sizeof(struct train), SEEK_CUR);

//         printf("Train Id = %d\t Train Name = %s\t Available Seats = %d\n",temp.tid,temp.train_name,temp.available_seats);

//         read(sock, &info, sizeof(info));

//         if(info == 1){//Increase seats
//             //reading no of seats to increase
//             read(sock, &info, sizeof(info));

//             if(temp.available_seats >= info){
//                 //approach -> cancel the current ticket and then book the ticket again with more seats
//                 book.cancelled = 1;
//                 temp.available_seats += info;

//                 write(fd2, &book, sizeof(book));

//                 //No of seats I want after increasing the seats
//                 int total_seats = book.end_seat - book.start_seat + 1 + info;
//                 struct bookings bk;
                
//                 //pointed to last entry
//                 int fp1 = lseek(fd2, 0, SEEK_END);
//                 lseek(fd2, -1 * sizeof(struct bookings), SEEK_CUR);
                
//                 read(fd2, &bk, sizeof(struct bookings));

//                 //Adding new values to update the data
//                 bk.bid++;
//                 bk.type = book.type;
//                 bk.acc_no = book.acc_no;
//                 bk.tr_id = book.tr_id;
//                 bk.cancelled = 0;
//                 strcpy(bk.trainname, book.trainname);
//                 bk.start_seat = temp.last_seatno_used + 1;
//                 bk.end_seat = temp.last_seatno_used + total_seats;

//                 temp.available_seats -= total_seats;
//                 temp.last_seatno_used = bk.end_seat;

//                 write(fd2, &bk, sizeof(struct bookings));
//                 write(fd1, &temp, sizeof(struct train));
//             }
//             else{
//                 op_choice = -2;
//                 write(sock, &op_choice, sizeof(op_choice));
//             }

//         }

//         else{//decrease seats
//             //reading no of seats to decrease
//             read(sock, &info, sizeof(info));

//             //If the no of seats I want to decrease is equal to tickets I book
//             //then I have to cancel the booking
//             if(book.end_seat - info < book.start_seat){
//                 book.cancelled = 1;
//                 temp.available_seats += info;
//             }
//             else{
//                 book.end_seat -= info;
//                 temp.available_seats += info;
//             }

//             write(fd2, &book, sizeof(struct bookings));
//             write(fd1, &temp, sizeof(struct train));

//         }

//         lock.l_type = F_UNLCK;
// 		fcntl(fd1, F_SETLK, &lock);
// 		fcntl(fd2, F_SETLK, &lock);
// 		close(fd1);
// 		close(fd2);
// 		if(op_choice>0)
// 		    write(sock, &op_choice, sizeof(op_choice));
// 		return 3;
//     }

//     //Cancel Booking

//     if(op_choice == 4){
//         view_booking(sock, id, type);

//         int fd1 = open(trainpath, O_RDWR);
//         int fd2 = open(bookingpath, O_RDWR);

//         struct flock lock;
//         lock.l_type = F_WRLCK;
//         lock.l_start = 0;
//         lock.l_len = 0;
//         lock.l_whence = SEEK_SET;
//         lock.l_pid = getpid();

//         fcntl(fd1, F_SETLKW, &lock);
//         perror("fcntl");
//         fcntl(fd2, F_SETLKW, &lock);
//         perror("fcntl");

//         int book_id;
//         struct train temp;
//         struct bookings book;
//         //reading bid which you want to cancel
//         read(sock, &book_id,sizeof(int));

//         //pointing at that particular entry
//         lseek(fd2, book_id * sizeof(struct bookings), SEEK_SET);

//         //after reading pointer will reach at the end of that entry
//         read(fd2, &book, sizeof(book));

//         //pointing at start of entry of bid
//         lseek(fd2, -1*sizeof(struct bookings), SEEK_CUR);

//         printf("Train ID = %d\t Train Name = %s\t Last Seat = %d\n",book.tr_id,book.trainname,book.end_seat);

//         //pointing to tid entry of train data
//         lseek(fd1, (book.tr_id)*sizeof(struct train), SEEK_SET);

//         read(fd1, &temp, sizeof(temp));
        
//         //pointing at start of entry of tid
//         lseek(fd1, -1*sizeof(struct train), SEEK_CUR);

//         printf("Train ID = %d\t Train Name = %s\t Available Seats = %d\n",temp.tid,temp.train_name,temp.available_seats);

//         //Add seats that cancelled into the available seats
//         temp.available_seats += book.end_seat - book.start_seat + 1;

//         book.cancelled = 1;

//         write(fd1, &temp, sizeof(temp));
//         write(fd2, &book, sizeof(book));

//         lock.l_type = F_UNLCK;
// 		fcntl(fd1, F_SETLK, &lock);
// 		fcntl(fd2, F_SETLK, &lock);
// 		close(fd1);
// 		close(fd2);
// 		write(sock, &op_choice, sizeof(op_choice));
// 		return 4;
//     }

//     //LogOut
//     if(op_choice == 5) {
// 		write(sock, &op_choice, sizeof(op_choice));
// 		return -1;
// 	}

//     return 0;
// }


// //Create this function outside bcz it will be useful in other operation too.
// void view_booking(int sock, int id, int type){
//     int fd = open(bookingpath, O_RDONLY);

//     struct flock lock;
//     lock.l_type = F_RDLCK;
//     lock.l_start = 0;
//     lock.l_len = 0;
//     lock.l_whence = SEEK_SET;
//     lock.l_pid = getpid();

//     fcntl(fd, F_SETLKW, &lock);
//     perror("fcntl");

//     int fp = lseek(fd, 0, SEEK_END);

//     int no_of_entries = 0;

//     if(fp == 0)
//         write(sock, &no_of_entries, sizeof(no_of_entries));
//     else{
//         struct bookings book[5];

//         while(fp > 0 && no_of_entries < 5)
//         {
//             struct bookings temp;

//             //to reach at first position if fp is at end of entry
//             fp = lseek(fd, -1 * sizeof(struct bookings), SEEK_CUR);
//             read(fd, &temp, sizeof(struct bookings));

//             if((temp.acc_no == id) && (temp.type == type))
//                 book[no_of_entries++] = temp;

//             //to bring back pntr at start and when it went into loop again then by first lseek it will go to prev record
//             fp = lseek(fd, -1 * sizeof(struct bookings), SEEK_CUR);    
//         }

//         write(sock, &no_of_entries, sizeof(no_of_entries));
//         printf("Entry : %d\n",no_of_entries);
//         for(int i = 0; i < no_of_entries; i++)
//         {
//             write(sock, &book[i].bid, sizeof(book[i].bid));
//             write(sock, &book[i].trainname, sizeof(book[i].trainname));
//             write(sock, &book[i].start_seat, sizeof(book[i].start_seat));
//             write(sock, &book[i].end_seat, sizeof(book[i].end_seat));
//             write(sock, &book[i].cancelled, sizeof(book[i].cancelled));
//         }
//     }

//     lock.l_type = F_UNLCK;
//     fcntl(fd, F_SETLK, &lock);
//     close(fd);    
// }