#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

/*
# market                get the current market status
# info                  get your partners' info
# buy <amount> <price>  place raw buy request
# sell <amount> <price> place production sell request
# prod <amount>         set production plan for this month
# ?                     see your requests
# build                 build a new plant
# abuild                build a new automatic plant
# upgrade               upgrade a plant to automatic
# turn                  end turn
# help                  for help
# quit                  to resign and quit the game
# chat on|off           switch global chat on or off
# say <whattosay>       say something to your partners
#                      (for global chat, use '.say')

MARKET:

# ------        Raw  MinPrice      Prod  MaxPrice
& MARKET         4       500         4      5500
# ------
*/

class commands
{
	public:
	//char *market = "market\n", *info = "info\n";
	void sendmarket(int sock);
	void sendinfo(int sock);
	void sendbuy(int sock, char *amount, char *price);
	void sendsell(int sock, char *amount, char *price);
	void sendprod(int sock, char *amount);
	void sendturn(int sock);
};

void commands::sendmarket(int sock)
{
	send(sock, "market\n", 7, 0);
}

void commands::sendinfo(int sock)
{
        send(sock, "info\n", 5, 0);
}

void commands::sendbuy(int sock, char *amount, char *price)
{
	send(sock, "buy " , 4 , 0);
        send(sock, amount, strlen(amount), 0);
	send(sock, " ", 1, 0);
	send(sock, price, strlen(price), 0);
	send(sock, "\n", 1, 0);
}

void commands::sendsell(int sock, char *amount, char *price)
{
        send(sock, "sell ", 5 , 0);
        send(sock, amount, strlen(amount), 0);
	send(sock, " ", 1, 0);
        send(sock, price, strlen(price), 0);
        send(sock, "\n", 1, 0);
}

void commands::sendprod(int sock, char *amount)
{
        send(sock, "prod " , 5 , 0);
        send(sock, amount, strlen(amount), 0);
        send(sock, "\n", 1, 0);
}

void commands::sendturn(int sock)
{
        send(sock, "turn\n", 5, 0);
}

class cut
{
	public:
	char *marketmaxprice(char *buffer);
	char *marketminprice(char *buffer);
	char *marketraw(char *buffer);
	char *marketprod(char *buffer);

	int winlose(char *buffer);

	char *inforaw(char *name, char *buffer);
	char *infoprod(char *name, char *buffer);
	char *infomoney(char *name, char *buffer);
	char *infoplants(char *name, char *buffer);
	char *infoautoplants(char *name, char *buffer);
};
// info: Raw Prod    Money Plants AutoPlant
/*
MARKET:

# ------        Raw  MinPrice      Prod  MaxPrice
& MARKET         4       500         4      5500
# ------
*/


char *cut::marketminprice(char *buffer)
{
	char *digit;
	int i=0, j=0, status=0, len=0, beg=0;
	while(status!=1){
		if ((buffer[i]<58)&&(buffer[i]>47)){
			status++;
			while (buffer[i]!=' '){
				i++;
			}
			while (buffer[i]==' '){
                                i++;
                        }
			i--;
		}
		i++;
	}
	beg=i;
	while(buffer[i]!=' '){
		len++;
		i++;
	}
	//cout << "len: " << len << "\n";
	digit=(char*)malloc(len*sizeof(char*));

	while(buffer[beg]!=' '){
                digit[j]=buffer[beg];
                beg++;
                j++;
        }
	//cout << "j: " << j << "\n";
	digit[j]='\0';
	cout << "min: " << digit << "\n";
	return digit;
}

char *cut::marketprod(char *buffer)
{
        char *digit;
        int i=0, j=0, status=0, beg=0, len=0;
        while(status!=2){
                if ((buffer[i]<58)&&(buffer[i]>47)){
                        status++;
                        while (buffer[i]!=' '){
                                i++;
                        }
			while (buffer[i]==' '){
                                i++;
                        }
                        i--;
                }
                i++;
        }
	beg=i;
        while(buffer[i]!=' '){
                len++;
                i++;
        }
        //cout << "len: " << len << "\n";
        digit=(char*)malloc(len*sizeof(char*));

        while(buffer[beg]!=' '){
                digit[j]=buffer[beg];
                beg++;
                j++;
        }

        digit[j]='\0';
        cout << "prod: " << digit << "\n";
        return digit;
}

char *cut::marketmaxprice(char *buffer)
{
	char *digit;
        int i=0, j=0, status=0, beg=0, len=0;
        while(status!=3){
                if ((buffer[i]<58)&&(buffer[i]>47)){
                        status++;
                        while (buffer[i]!=' '){
                                i++;
                        }
                        while (buffer[i]==' '){
                                i++;
                        }
                        i--;
                }
                i++;
        }
        beg=i;
        while(buffer[i]!='\n'){
                len++;
                i++;
        }
        //cout << "len: " << len << "\n";
        digit=(char*)malloc(len*sizeof(char*));

        while(buffer[beg]!='\n'){
                digit[j]=buffer[beg];
                beg++;
                j++;
        }

        digit[j]='\0';
        cout << "max: " << digit << "\n";
        return digit;






        /*char digit[10]={' '};
        int i=0, j=0, status=0;
        while(status!=3){
                if ((buffer[i]<58)&&(buffer[i]>47)){
                        status++;
                        while (buffer[i]!=' '){
                                i++;
                        }
                        while (buffer[i]==' '){
                                i++;
                        }
                        i--;
                }
                i++;
        }
        while(buffer[i]!='\n'){
                digit[j]=buffer[i];
                i++;
                j++;
        }
        //digit[j+1]='\n';
        cout << "max: " << digit << "\n";
        return digit;*/
}

char *cut::marketraw(char *buffer)
{
        char *digit;
        int i=0, j=0, beg=0, len=0;
        while((buffer[i]<48)||(buffer[i]>57)){
                i++;
        }
	//cout << "i: " << i << "\n";
	beg=i;
	while (buffer[i]!=' '){
		len++;
		i++;
	}
	digit=(char*)malloc(len*sizeof(char*));
        while(buffer[beg]!=' '){
                digit[j]=buffer[beg];
                beg++;
                j++;
        }
	//cout << "j: " << j << "\n";
	//cout << "len: " << len << "\n";
        digit[j]='\0';
	cout << "raw: " << digit << "\n";
	return digit;
}

/*
info
# -----             Name  Raw Prod    Money Plants AutoPlants
& INFO              egor    2    2    10000    2    0
& INFO              lena    2    2    10000    2    0
# -----
& PLAYERS           2         WATCHERS    0
# -----
*/

char *cut::inforaw(char *name, char *buffer)
{
        char *digit;
        int i=0, j=0, status=0, namestatus=0, len=0, beg=0;

        while (namestatus!=1){
                while (buffer[i]!=name[0]){
                        i++;
                }
                while (buffer[i]==name[j]){
                        i++;
                        j++;
                        if ((name[j]=='\0')&&(buffer[i]==' ')){
                                namestatus=1;
                        }
                }
		j = 0;
        }

        i=0;
        j=0;

	while((buffer[i]<48)||(buffer[i]>57)){
                i++;
        }
	beg=i;
	while(buffer[i]!=' '){
                i++;
                len++;
        }
	digit=(char*)malloc(len*sizeof(char*));
        while(buffer[beg]!=' '){
                digit[j]=buffer[beg];
                beg++;
                j++;
        }
        digit[j]='\0';
        cout << "inforaw: " << digit << "\n";
        return digit;

}


char *cut::infoprod(char *name, char *buffer)
{
	char *digit;
        int i=0, j=0, status=0, namestatus=0, len=0, beg=0;

	while (namestatus!=1){
		while (buffer[i]!=name[0]){
			i++;
		}
		while (buffer[i]==name[j]){
			i++;
			j++;
			if ((name[j]=='\0')&&(buffer[i]==' ')){
				namestatus=1;
			}
		}
		j = 0;
	}
	i=0;
	j=0;
        while(status!=1){
                if ((buffer[i]<58)&&(buffer[i]>47)){
                        status++;
                        while (buffer[i]!=' '){
                                i++;
                        }
                        while (buffer[i]==' '){
                                i++;
                        }
                        i--;
                }
                i++;
        }
	beg=i;
	while(buffer[i]!=' '){
                i++;
                len++;
        }
	digit=(char*)malloc(len*sizeof(char*));
        while(buffer[beg]!=' '){
                digit[j]=buffer[beg];
                beg++;
                j++;
        }
        digit[j]='\0';
        cout << "infoprod: " << digit << "\n";
        return digit;
}

char *cut::infomoney(char *name, char *buffer)
{
	char *digit;
        int i=0, j=0, status=0, namestatus=0, len=0, beg=0;

        while (namestatus!=1){
                while (buffer[i]!=name[0]){
                        i++;
                }
                while (buffer[i]==name[j]){
                        i++;
                        j++;
                        if ((name[j]=='\0')&&(buffer[i]==' ')){
                                namestatus=1;
                        }
                }
                j = 0;
        }
        i=0;
        j=0;
        while(status!=2){
                if ((buffer[i]<58)&&(buffer[i]>47)){
                        status++;
                        while (buffer[i]!=' '){
                                i++;
                        }
                        while (buffer[i]==' '){
                                i++;
                        }
                        i--;
                }
                i++;
        }
        beg=i;
        while(buffer[i]!=' '){
                i++;
                len++;
        }
	digit=(char*)malloc(len*sizeof(char*));
        while(buffer[beg]!=' '){
                digit[j]=buffer[beg];
                beg++;
                j++;
        }
        digit[j]='\0';
        cout << "infomoney: " << digit << "\n";
        return digit;

	/*
        char digit[10]={' '};
        int i=0, j=0, status=0, namestatus=0;

        while (namestatus!=1){
                while (buffer[i]!=name[0]){
                        i++;
                }
                while (buffer[i]==name[j]){
                        i++;
                        j++;
                        if ((name[j]=='\0')&&(buffer[i]==' ')){
                                namestatus=1;
                        }
                }
		j = 0;
        }

        i=0;
        j=0;
        while(status!=2){
                if ((buffer[i]<58)&&(buffer[i]>47)){
                        status++;
                        while (buffer[i]!=' '){
                                i++;
                        }
                        while (buffer[i]==' '){
                                i++;
                        }
                        i--;
                }
                i++;
        }
        while(buffer[i]!=' '){
                digit[j]=buffer[i];
                i++;
                j++;
        }
        digit[j+1]='\n';
        cout << "infomoney: " << digit << "\n";
        return digit;
	*/
}


char *cut::infoplants(char *name, char *buffer)
{
	char *digit;
        int i=0, j=0, status=0, namestatus=0, len=0, beg=0;

        while (namestatus!=1){
                while (buffer[i]!=name[0]){
                        i++;
                }
                while (buffer[i]==name[j]){
                        i++;
                        j++;
                        if ((name[j]=='\0')&&(buffer[i]==' ')){
                                namestatus=1;
                        }
                }
                j = 0;
        }
        i=0;
        j=0;
        while(status!=3){
                if ((buffer[i]<58)&&(buffer[i]>47)){
                        status++;
                        while (buffer[i]!=' '){
                                i++;
                        }
                        while (buffer[i]==' '){
                                i++;
                        }
                        i--;
                }
                i++;
        }
        beg=i;
        while(buffer[i]!=' '){
                i++;
                len++;
        }
	digit=(char*)malloc(len*sizeof(char*));
        while(buffer[beg]!=' '){
                digit[j]=buffer[beg];
                beg++;
                j++;
        }
        digit[j]='\0';
        cout << "plants: " << digit << "\n";
        return digit;


        /*char digit[10]={' '};
        int i=0, j=0, status=0, namestatus=0;

        while (namestatus!=1){
                while (buffer[i]!=name[0]){
                        i++;
                }
                while (buffer[i]==name[j]){
                        i++;
                        j++;
                        if ((name[j]=='\0')&&(buffer[i]==' ')){
                                namestatus=1;
                        }
                }
		j = 0;
        }

        i=0;
        j=0;
        while(status!=3){
                if ((buffer[i]<58)&&(buffer[i]>47)){
                        status++;
                        while (buffer[i]!=' '){
                                i++;
                        }
                        while (buffer[i]==' '){
                                i++;
                        }
                        i--;
                }
                i++;
        }
        while(buffer[i]!=' '){
                digit[j]=buffer[i];
                i++;
                j++;
        }
        digit[j+1]='\n';
        cout << "infoplants: " << digit << "\n";
        return digit;*/
}

char *cut::infoautoplants(char *name, char *buffer)
{
	char *digit;
        int i=0, j=0, status=0, namestatus=0, len=0, beg=0;

        while (namestatus!=1){
                while (buffer[i]!=name[0]){
                        i++;
                }
                while (buffer[i]==name[j]){
                        i++;
                        j++;
                        if ((name[j]=='\0')&&(buffer[i]==' ')){
                                namestatus=1;
                        }
                }
                j = 0;
        }
        i=0;
        j=0;
        while(status!=4){
                if ((buffer[i]<58)&&(buffer[i]>47)){
                        status++;
                        while (buffer[i]!=' '){
                                i++;
                        }
                        while (buffer[i]==' '){
                                i++;
                        }
                        i--;
                }
                i++;
        }
        beg=i;
        while(buffer[i]!='\n'){
                i++;
                len++;
        }
	digit=(char*)malloc(len*sizeof(char*));
        while(buffer[beg]!='\n'){
                digit[j]=buffer[beg];
                beg++;
                j++;
        }
        digit[j]='\0';
        cout << "infoautoplants: " << digit << "\n";
        return digit;

        /*char digit[10]={' '};
        int i=0, j=0, status=0, namestatus=0;

        while (namestatus!=1){
                while (buffer[i]!=name[0]){
                        i++;
                }
                while (buffer[i]==name[j]){
                        i++;
                        j++;
                        if ((name[j]=='\0')&&(buffer[i]==' ')){
                                namestatus=1;
                        }
                }
		j = 0;
        }

        i=0;
        j=0;

        while(status!=4){
                if ((buffer[i]<58)&&(buffer[i]>47)){
                        status++;
                        while (buffer[i]!=' '){
                                i++;
                        }
                        while (buffer[i]==' '){
                                i++;
                        }
                        i--;
                }
                i++;
        }
        while(buffer[i]!='\n'){
                digit[j]=buffer[i];
                i++;
                j++;
        }
        digit[j+1]='\n';
        cout << "infoplants: " << digit << "\n";
        return digit;*/
}

int cut::winlose(char *buffer)
{
	char *win="YOU_WIN\0";
	char *lose="You are a bankrupt\0";
	int status=-1, i = 0, j = 0;

	while (i != 1024) {
                while ((buffer[i]!=lose[0])&&(i!=1024)){
                        i++;
                }
		//cout << "ok1\n";
                while (buffer[i]==lose[j]){
			//cout << "j: " << j << "\n";
			//cout << "i: " << i << "\n";
			//cout << "buffer[i]: " << buffer[i] << "\n";
			//cout << "lose[j]:   " << lose[j] << "\n";
			i++;
			j++;
                        if ((lose[j]=='\0')&&(buffer[i]==',')){
                                status=0;
				return status;
                        }
                }
		j = 0;
		//cout << "ok2\n";
        }
	i = 0;
	j = 0;
	while (i != 1024) {
                while ((buffer[i]!=win[0])&&(i!=1024)){
                        i++;
                }
		//cout << "ok3\n";
                while (buffer[i] == win[j]){
                        i++;
                        j++;
                        if ((win[j] == '\0')&&(buffer[i] == '\n')){
                                status = 1;
                                return status;
                        }
                }
		j = 0;
		//cout << "ok4\n";
        }
	return status;
}



int game(int sock, char *name)
{
	int result = -1, valread, status = 0, i;
	char *markraw, *markminprice, *markmaxprice, *markprod, *infraw;
	char *infprod, *infmoney, *infplants, *infautoplants;
	char buffer[1024] = {0};
	cut mycut;
	commands mycomm;
	while (result == -1){
		mycomm.sendmarket(sock);
		sleep(3);
		valread = read(sock, buffer, 1024);
		cout << "Server said market: \n" << buffer << "\n\n";
		markraw = mycut.marketraw(buffer);
		markminprice = mycut.marketminprice(buffer);
		markmaxprice = mycut.marketmaxprice(buffer);
		markprod = mycut.marketprod(buffer);
		for (i=0; i!=1024; i++){
                                buffer[i] = 0;
		}
		mycomm.sendinfo(sock);
		sleep(3);
                valread = read(sock, buffer, 1024);
                cout << "Server said info: \n" << buffer << "\n\n";
                infraw = mycut.inforaw(name, buffer);
                infmoney = mycut.infomoney(name, buffer);
                infprod = mycut.infoprod(name, buffer);
                infplants = mycut.infoplants(name, buffer);
                infautoplants = mycut.infoautoplants(name, buffer);
		for (i=0; i!=1024; i++){
                                buffer[i] = 0;
                }
		mycomm.sendbuy(sock, "2", markminprice);
		sleep(2);
		valread = read(sock, buffer, 1024);
                cout << "Server said acception buy: \n" << buffer << "\n\n";

		for (i=0; i!=1024; i++){
			buffer[i] = 0;
                }
		mycomm.sendsell(sock, infprod, markmaxprice);
		sleep(2);
		valread = read(sock, buffer, 1024);
                cout << "Server said acception sell: \n" << buffer << "\n\n";

		for (i=0; i!=1024; i++){
                	buffer[i] = 0;
                }
		mycomm.sendprod(sock, infprod);
		sleep(2);
		valread = read(sock, buffer, 1024);
                cout << "Server said acception prod: \n" << buffer << "\n\n";
		for (i=0; i!=1024; i++){
                        buffer[i] = 0;
                }
		mycomm.sendturn(sock);
		sleep(2);
                valread = read(sock, buffer, 1024);
                cout << "Server said turn: \n" << buffer << "\n\n";
		i=0;
		while (status != 1) {
			while (i!=1022){
				if ((buffer[i]=='#')&&(buffer[i+2]=='T')){
					status=1;
					break;
				}
				//cout << "i: " << i << "\n";
				i++;
			}
			cout << "status: " << status << "\n";
			if (status!=1){
				for (i=0; i!=1024; i++){
                        		buffer[i] = 0;
				}
				valread = read(sock, buffer, 1024);
 		                cout << "Server said thinking: \n" << buffer << "\n\n";
                	}
			i=0;
		}
		//cout << "Server said results: \n" << buffer << "\n\n";
		cout << "hello\n";
		result=mycut.winlose(buffer);
		for (i=0; i!=1024; i++){
                	buffer[i] = 0;
		}
		i=0;
		status=0;
	}
	return result;

}

int main(int argc, char *argv[])
{
	int resultgame = 0;
        int i = 0, j = 0, sock = 0, valread = 0, status=0, /*Raw, MinPrice, Prod, MaxPrice,
        gamesplayed, */  port, playersamount, currentamount = 1;
        struct sockaddr_in serv_addr, addr;
	char * serv_ip = argv[1], *pport = argv[2], *creatorjoin = argv[4], *roomorplayers = argv[5];
	char *name = argv[3]; //market = "market\n", *who = "who\n";
        char buffer[1024] = {0};
	char *start="& START\0";
	commands mycom;
	cut mycuts;
	port = atoi(pport);
	cout << "port: " << port << "\n";
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) < 0)
        {
                cout << "\n Socket creation error \n";
                return -1;
        }
	cout << "OK2\n";

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if(!inet_aton(serv_ip, &(addr.sin_addr)))
	{
		cout << "\nInvalid address/ Address not supported \n";
		return -1;
	}
	cout << "OK3\n";
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
                cout << "\nConnection Failed \n";
                return -1;
        }
	cout << "OK4\n";

	valread = read(sock, buffer, 1024);
	cout << "Server said1: " << buffer << "\n";
	for (i=0; i!=1024; i++){
		buffer[i] = 0;
	}

	send(sock, name, strlen(name), 0);
	send(sock, "\n", 1, 0);
	cout << " OK5\n";
	sleep(3);

	valread = read(sock, buffer, 1024);
	cout << "Server said2: " << buffer << "\n";
	for (i=0; i!=1024; i++){
        	buffer[i] = 0;
        }

	if (!strcmp(creatorjoin, "create")) {
		playersamount = atoi(roomorplayers);
		cout << "OK6\n";
		send(sock, ".create\n", 8, 0);
		sleep(3);
		valread = read(sock, buffer, 1024);
		cout << "Server said3: " << buffer << "\n";
		while (currentamount != playersamount) {
			for (i=0; i!=1024; i++){
				buffer[i] = 0;
			}
			valread = read(sock, buffer, 1024);
			cout << "Server said4: " << buffer << "\n";
			i = 0;
			while (buffer[i] != '\0') {
				if ((buffer[i] == '@') && (buffer[i+1] == '+')) {
					currentamount += 1;
				}
				if ((buffer[i] == '@') && (buffer[i+1] == '-')) {
                       			currentamount -= 1;
				}

				i++;
			}
			for (i=0; i!=1024; i++){
				buffer[i] = 0;
			}
			i = 0;
			cout << "Currentamount = " << currentamount << "\n";
		}
		send(sock, "start", 5, 0);
		send(sock, "\n", 1, 0);
		sleep(3);
		valread = read(sock, buffer, 1024);
		cout << "Server said5: " << buffer << "\n";
		for (i=0; i!=1024; i++){
			buffer[i] = 0;
		}
		resultgame = game(sock, name);
                if (resultgame == 1) {
                        cout << "U win\n";
                } else {
                        cout << "U lose\n";
                }
	}
	if (!strcmp(creatorjoin, "join")){
		cout << "roomorplayers " << roomorplayers << "\n";
                send(sock, ".join ", 6, 0);
		send(sock, roomorplayers, strlen(roomorplayers), 0);
                send(sock, "\n", 1, 0);
                sleep(3);

                valread = read(sock, buffer, 1024);
                cout << "Server said3: " << buffer << "\n";
		/*for (i=0; i!=1024; i++){
			buffer[i] = 0;
		}*/

		i=0;
		//char *start="& START\0";

		while (status!=1){
                	while ((buffer[i]!=start[0])&&(i!=1024)){
                        	i++;
                	}
                	//cout << "ok3\n";
                	while (buffer[i] == start[j]){
                        	i++;
                        	j++;
                        	if ((start[j] == '\0')&&((buffer[i] == '\n')||(buffer[i]=='\0'))){
                                	status = 1;
                        	}
                	}
                	j = 0;
			for (i=0; i!=1024; i++){
                                buffer[i] = 0;
                        }
             		if (status!=1){
				valread = read(sock, buffer, 1024); //"start"
	                        cout << "Server said4: " << buffer << "\n";
			}
			i=0;
        	}


		/*while ((buffer[0]!='&')||(buffer[2]!='S')){
			for (i=0; i!=1024; i++){
        	                buffer[i] = 0;
	                }
			valread = read(sock, buffer, 1024); //"start"
			cout << "Server said4: " << buffer << "\n";
			if ((buffer[0]=='&')&&(buffer[2]=='S')){
				break;
			}
                	for (i=0; i!=1024; i++){
                        	buffer[i] = 0;
			}
                }*/

		resultgame = game(sock, name);
		if (resultgame == 1) {
			cout << "U win\n";
		} else {
			cout << "U lose\n";
		}

		/*mycom.sendinfo(sock);
		sleep(3);
		valread = read(sock, buffer, 1024);
                cout << "Server said5: " << buffer << "\n";
		mycuts.infomoney(name,buffer);
                for (i=0; i!=1024; i++){
                        buffer[i] = 0;
                }*/

		/*mycom.sendinfo(sock);
                sleep(3);
                valread = read(sock, buffer, 1024);
                cout << "Server said6: " << buffer << "\n";
                for (i=0; i!=1024; i++){
                        buffer[i] = 0;
                }

		mycom.sendbuy(sock, "1", "700    ");
                sleep(3);
                valread = read(sock, buffer, 1024);
                cout << "Server said7: " << buffer << "\n";
                for (i=0; i!=1024; i++){
                        buffer[i] = 0;
                }
		/*
		mycom.sendsell(sock, "1", "800");
                sleep(3);
                valread = read(sock, buffer, 1024);
                cout << "Server said8: " << buffer << "\n";
                for (i=0; i!=1024; i++){
                        buffer[i] = 0;
                }

		mycom.sendturn(sock);
                sleep(3);
                valread = read(sock, buffer, 1024);
                cout << "Server said9: " << buffer << "\n";
                for (i=0; i!=1024; i++){
                        buffer[i] = 0;
                }


		sleep(15);


		mycom.sendinfo(sock);
                sleep(3);
                valread = read(sock, buffer, 1024);
                cout << "Server said6: " << buffer << "\n";
                for (i=0; i!=1024; i++){
                        buffer[i] = 0;
                }*/

	}

	return 0;
}
