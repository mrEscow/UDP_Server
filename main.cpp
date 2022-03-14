/*
	Simple udp server
*/

//#include "stdafx.h"


#include<stdio.h> 	 //printf
#include<string.h>	 //memset
#include<stdlib.h>	 //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#include<iostream>

#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data

//void die(char *s)
//{
//	perror(s);
//	exit(1);
//}

void die(std::string s)
{
	std::cout << s << std::endl;
	exit(1);
}

int main(void)
{
	int tik = 0;

	struct sockaddr_in si_me, si_other;
	
	socklen_t slen = sizeof(si_other);
	
	int s, i, recv_len;

	char buf[BUFLEN];
	for(int i = 0; i<BUFLEN;++i)
	{
		buf[i]=' ';
	}
	
	//create a UDP socket
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}
	
	// zero out the structure
	memset((char *) &si_me, 0, sizeof(si_me));
	
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//bind socket to port
	if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
	{
		die("bind");
	}
	
	//keep listening for data
	while(1)
	{
		printf("Waiting for data...");
		fflush(stdout);
		
		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
		{
			die("recvfrom()");
		}

		buf[recv_len]='\0';

		//print details of the client/peer and the data received
		printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		printf("Data: %s\n" , buf);
		
		//std::cout << "sendto1_start"<<std::endl;
		//now reply the client with the same data
		if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
		{
			die("sendto()");
		}
		//std::cout << "tik: " << tik++ <<std::endl;
		
		for(int i = 0; i< BUFLEN;++i){
			buf[i]=' ';
		}
		
	}

	//close(s);
	return 0;
}

