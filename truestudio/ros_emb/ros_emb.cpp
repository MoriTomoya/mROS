#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include <string>
#include "ros_emb.h"

#include "mbed.h"
//#include "DisplayBace.h"
//#include "rtos.h"
//#include "JPEG_Converter.h"
#include "EthernetInterface.h"


#include "syssvc/logtask.h"





/*初期化（最初のタスク：masterに接続？）
 *マスタに対する名前解決とかもかな？ */
/****Network setteing **/
#define USE_DHCP (1)
#if(USE_DHCP == 0)
	#define IP_ADDRESS  	("")	/*IP address */
	#define SUBNET_MASK		("")	/*Subset mask */
	#define DEFAULT_GATEWAY	("")	/*Default gateway */
#endif

EthernetInterface network;


void connect_host(){
	//printf("Network Setting up...\r\n");

/*TCP/IPでマスタのPCと接続*/
#if (USE_DHCP == 1)
	if(network.init() != 0) {
#else
	if(network.init(IP_ADDRESS, SUBNET_MASK, DEFAULT_GATEWAY) != 0) {
#endif
		syslog(LOG_NOTICE, "Network Initialize Error\r\n");
	return;
	}
		syslog(LOG_NOTICE, "Network Initialized successfully");
	while (network.connect(5000) != 0){
		//	syslog(LOG_NOTICE, "LOG_NOTICE: Network Connect Error");
	}
		syslog(LOG_NOTICE,"MAC Address is %s\r\n", network.getMACAddress());
		syslog(LOG_NOTICE,"IP Address is %s\r\n", network.getIPAddress());
		syslog(LOG_NOTICE,"NetMask is %s\r\n", network.getNetworkMask());
		syslog(LOG_NOTICE,"Gateway Address is %s\r\n", network.getGateway());

		//	syslog(LOG_INFO, "LOG_INFO: Network Setup OK");
		//printf("Network Setup OK\r\n");
	    //SnapshotHandler::attach_req(&snapshot_req);
		//HTTPServerAddHandler<FSHandler>("/");
		//HTTPServerAddHandler<RPCHandler>("/rpc");
		//HTTPServerStart(80);
}

//マスタへのXML-RPCクライアント通信OK
TCPSocketConnection mas_sock;

const char *m_ip = "192.168.0.15";	//ros master IP
const int m_port = 11311;	//ros master xmlrpc port
/* masterとの通信タスク*/
/* HTTPクライアントが必要 */
void connect_master(){
	if(mas_sock.connect(m_ip,m_port) == -1){
		//printf("connect_master error");
		exit(1);
	}
}

//
void xml2master(){
	string xml;
	//テスト用にとりあえずサブスクライバの登録だけ
	xml = registerSubscriber("/mros_node","/tpc_name","/tpc_type","http://mros_uri");
	char *buff = xml.c_str();
	int n;
	n = mas_sock.send(buff,strlen(buff));
	syslog(LOG_NOTICE, "LOG_INFO: data send\n%s",buff);
	if(n < 0){
		//printf("fail register\n");
		exit(1);
	}
	char *bufr;
	bufr = (char *)malloc(1024);
	n = mas_sock.receive(bufr,1024);
	if(n < 0){
		//printf("fail receive\n");
		free(bufr);
		exit(1);
	}else{
		syslog(LOG_NOTICE, "LOG_INFO: data receive\n%s",bufr);
		//printf("****receive data****\n");
		//printf("%s",buff);
		free(bufr);
	}

}


void main_task(){
	syslog(LOG_NOTICE, "**********mROS START******************");
	//printf("********mROS START********\n");
	//printf("<test start>\n");

	//printf("...connecting host PC\n");
	syslog(LOG_NOTICE, "LOG_INFO: connecting host...");
	connect_host();
	syslog(LOG_NOTICE, "LOG_INFO: CONNECTED host");
	//printf("success connect host PC\n");
	
	//printf("...connecting ros master\n");
	syslog(LOG_NOTICE, "LOG_INFO: connecting master...");
	connect_master();
	//printf("success connect ros master\n");
	syslog(LOG_NOTICE, "LOG_INFO: CONNECTED master");
	//printf("...registering this node as subscriver\n");
	syslog(LOG_NOTICE, "LOG_INFO: DO RPC CALL");
	xml2master();
	syslog(LOG_NOTICE, "**********mROS FINISH***********");
	//printf("<test end>\n");

}
