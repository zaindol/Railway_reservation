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

#include "client_function.h"


void cl_view_booking(int sock);

//Admin operation

int admin_operation(int sock, int choice){
    switch(choice){
        //Add train
        case 1:{
            int tno,av_seats;
            char tname[20];
            
            write(sock, &choice, sizeof(choice));

            printf("Enter Train Name: \n");
            scanf("%s",tname);
            printf("Enter Train Number: \n");
            scanf("%d",&tno);
            printf("Enter Available Seats: \n");
            scanf("%d",&av_seats);
            
            write(sock, &tname, sizeof(tname));
            write(sock, &tno, sizeof(tno));
            write(sock, &av_seats, sizeof(av_seats));

            read(sock, &choice, sizeof(choice));

            if(choice == 1)
                printf("Train Added Successfully\n");

            while(getchar()!= '\n');
            getchar();
            return choice;
            break;    
        }

        //Delete train
        case 2:{
            int no_of_trains,response;
            write(sock, &choice, sizeof(choice));
            perror("write");

            read(sock, &no_of_trains, sizeof(no_of_trains));

            while(no_of_trains > 0)
            {
                int tid,tno,del;
                char tname[20];
                read(sock, &tid, sizeof(tid));
                read(sock, &tname, sizeof(tname));
                read(sock, &tno, sizeof(tno));
                read(sock, &del, sizeof(del));

                if(del != 1)
                    printf("tid = %d\t tno = %d\t tname = %s\n",tid,tno,tname);

                no_of_trains--;    
            }

            printf("Enter the Train id to delete or -2 to cancel the operation: ");
            scanf("%d",&response);

            write(sock,&response,sizeof(response));
            read(sock, &choice, sizeof(choice));

            if(choice != -2)
                printf("Train deleted successfully\n");
            else
                printf("Operation cancelled\n");

            while(getchar()!= '\n');
            getchar();
            return choice;
            break;    

        }

        //Modify Train Details
        case 3:{
            int no_of_trains, response;
            write(sock,&choice, sizeof(choice));
            read(sock, &no_of_trains,sizeof(no_of_trains));

            while(no_of_trains > 0)
            {
                int tid, tno, del, av_seats;
                char tname[20];

                read(sock, &tid, sizeof(tid));
                read(sock, &tno, sizeof(tno));
                read(sock, &del, sizeof(del));
                read(sock, &av_seats, sizeof(av_seats));
                read(sock, &tname, sizeof(tname));

                if(del != 1)
                    printf("tid = %d\t tno = %d\t Available seats = %d\t Train Name = %s\n",tid, tno, av_seats, tname);

                no_of_trains--;    
            }

            printf("Enter the Train id to modify or Enter -2 to cancel: ");
            scanf("%d",&response);

            write(sock, &response, sizeof(response));

            if(response != -2){
                int modify;
                printf("What detail you want to modify?\n");
                printf("1. Train Name\n 2. Train no.\n 3. Available Seats\n");
                printf("Your choice : ");
                scanf("%d",&modify);

                write(sock, &modify, sizeof(modify));

                if(modify == 2 || modify == 3){
                    int cur_val,new_val;
                    read(sock, &cur_val, sizeof(cur_val));
                    printf("Current Value: %d\n", cur_val);
                    printf("Enter new value : ");
                    scanf("%d",&new_val);
                    write(sock, &new_val, sizeof(new_val));
                }
                else{
                    char name[20];
                    read(sock, &name, sizeof(name));
                    printf("Current Name : %s\n", name);
                    printf("Enter new name: ");
                    scanf("%s",name);
                    write(sock, &name, sizeof(name));
                }
            }

            read(sock, &choice, sizeof(choice));
            if(choice != -2)
                printf("Train Data modify successfully\n");
            else
                printf("Operation Cancelled\n");
            while(getchar()!= '\n');
            getchar();
            return choice;
            break;        

        }

        //Delete User

        case 4:{
            int acc_type, no_of_users, response;

            write(sock, &choice, sizeof(choice));

            printf("What type of account do you want to delete?\n");
            printf("1. Customer\n 2. Agent\n 3. Admin\n");
            printf("Your choice: ");
            scanf("%d", &acc_type);

            write(sock, &acc_type, sizeof(acc_type));

            read(sock, &no_of_users, sizeof(no_of_users));

            printf("No of users : %d\n",no_of_users);

            while(no_of_users > 0)
            {
                char name[20];
                int id;
                // int del;

                read(sock, &name, sizeof(name));
                read(sock, &id, sizeof(id));
                // read(sock, &del, sizeof(del));
                printf("beforeId = %d\t beforeName = %s\n",id,name);
                if(strcmp(name, "deleted") != 0)
                    printf("Id = %d\t Name = %s\n",id,name);


                no_of_users--;    
            }

            printf("Enter the ID which you want to delete or Enter -2 to cancel the operation: ");
            scanf("%d",&response);

            write(sock, &response, sizeof(response));

            read(sock, &choice, sizeof(choice));

            if(choice != -2)
                printf("User deleted successfully\n");
            else
                printf("Operation Cancelled\n");    

            while(getchar()!= '\n');
            getchar();
            return choice;
            break; 

        }

        //Search User
        case 5: {
            int id, acc_type, response;

            write(sock, &choice, sizeof(choice));

            printf("Which type of user do you want to search?\n");
            printf("1. Customer\n 2. Agent\n 3. Admin\n");
            printf("Your response: ");
            scanf("%d",&acc_type);

            write(sock, &acc_type, sizeof(acc_type));

            printf("Enter account number to search the user or Enter -2 to cancel the operation: ");
            scanf("%d",&response);

            write(sock, &response, sizeof(response));

            // read(sock, &choice, sizeof(choice));

            if(response != -2){
                char name[20];

                read(sock, &name, sizeof(name));
                read(sock, &id, sizeof(id));

                printf("ID = %d\t Name = %s\n",id,name);
            }
            else{
                printf("Operation Cancelled\n");
            }

            while(getchar()!= '\n');
            getchar();
            return choice;
            break;

        }   

        //Log out
        case 6: {
            write(sock, &choice, sizeof(choice));
			read(sock, &choice, sizeof(choice));
			if(choice==6) printf("Logged out successfully.\n");
			while(getchar()!='\n');
			getchar();
			return -1;
			break;
        } 


        default: return -1;
    }
}

//User operation

int user_operation(int sock, int choice){
    switch(choice){
        //Book Tickets
        case 1:{
            int no_of_trains, train_id, train_avseats, train_no, del, req_seats;
            char train_name[20];

            write(sock, &choice, sizeof(choice));

            read(sock, &no_of_trains, sizeof(no_of_trains));

            printf("ID\t T_No.\t AV_Seats\t Train Name\n");
            while(no_of_trains > 0)
            {
                read(sock, &train_id, sizeof(train_id));
                read(sock, &train_avseats, sizeof(train_avseats));
                read(sock, &train_no, sizeof(train_no));
                read(sock, &train_name, sizeof(train_name));
                read(sock, &del, sizeof(del));

                if(del != 1)
                    printf("%d\t %d\t %d\t %s\n",train_id, train_no, train_avseats, train_name);

                no_of_trains--;    
            }

            printf("Enter the train id for which you want to book tickets: ");
            scanf("%d",&train_id);

            write(sock, &train_id, sizeof(train_id));

            //getting available seats of the train for which I want to book tickets
            read(sock, &train_avseats, sizeof(train_avseats));

            printf("Enter the number of seats you want to book: ");
            scanf("%d",&req_seats);

            if(train_avseats >= req_seats && req_seats > 0)
                write(sock, &req_seats, sizeof(req_seats));

            else{
                req_seats = -1;
                write(sock, &req_seats, sizeof(req_seats));
            }    

            read(sock, &choice, sizeof(choice));

            if(choice == 1)
                printf("Tickets booked succesfully\n");
            else    
                printf("Tickets not booked, Please try again\n");

            while(getchar()!= '\n');
            getchar();
            return choice;
            break;        
        }

        //View Bookings
        case 2:{
            write(sock, &choice, sizeof(choice));
            cl_view_booking(sock);
            read(sock, &choice, sizeof(choice));
            return 2;
        }

        //Update Bookings
        case 3:{
            int book_id, info;
            write(sock, &choice, sizeof(choice));

            //to check the booking
            cl_view_booking(sock);

            printf("Enter Booking ID to update: ");
            scanf("%d", &book_id);

            write(sock, &book_id, sizeof(book_id));

            printf("What information do you want to update?\n");
            printf("1. Increase the seats\n 2. Decrease the seats\n");
            scanf("%d",&info);

            write(sock, &info, sizeof(info));

            if(info == 1){//Increase the seats
                printf("How many seats do you want to increase?\n");
                scanf("%d",&info);
                write(sock, &info, sizeof(info));
            }

            else{//Decrease the seats
                printf("How many seats do you want to decrease?\n");
                scanf("%d",&info);
                write(sock, &info, sizeof(info));
            }

            read(sock, &choice, sizeof(choice));

            if(choice == -2)
                printf("Operation Failed. No seats available\n");
            else
                printf("Operation successful\n");

            while(getchar()!='\n');
            getchar();
            return 3;        
        }

        //Cancel Booking

        case 4:{
            write(sock, &choice, sizeof(choice));

            cl_view_booking(sock);

            int id;
            printf("Enter Booking ID which you want to cancel: ");
            scanf("%d",&id);
            write(sock, &id, sizeof(id));

            read(sock, &choice, sizeof(choice));

            if(choice == -2)
                printf("Operation Failed.\n");
            else
                printf("Booking Cancelled Successfully\n");

            while(getchar() != '\n');
            getchar();
            return 4;        
        }

        //Log Out
        case 5: {
			write(sock, &choice, sizeof(choice));
			read(sock, &choice, sizeof(choice));
			if(choice == 5) printf("Logged out successfully.\n");
			while(getchar()!='\n');
			getchar();
			return -1;
			break;
		}
        
		default: return -1;
    }

    
}


void cl_view_booking(int sock){
    int no_of_entries;

    read(sock, &no_of_entries, sizeof(int));
    printf("No of entry : %d\n",no_of_entries);
    if(no_of_entries == 0)
        printf("No Entries Found\n");
    else
        printf("Your recent %d bookings are: \n",no_of_entries);
    
    while(no_of_entries > 0)
    {
        int bid, seat_start, seat_end, cancel;
        char tname[20];

        read(sock, &bid, sizeof(bid));
        read(sock, &tname, sizeof(tname));
        read(sock, &seat_start, sizeof(int));
        read(sock, &seat_end, sizeof(int));
        read(sock, &cancel, sizeof(int));

        if(!cancel)
            printf("Bookingid: %d\t1st Seat: %d\tLast Seat: %d\tTrain Name: %s\n",bid,seat_start,seat_end,tname);

        no_of_entries--;    
    }

    printf("Press any key to continue...\n");
    while(getchar() != '\n');
    getchar();
}