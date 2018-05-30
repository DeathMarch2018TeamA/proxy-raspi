#include <iostream>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include "proxy.hpp"

const int BUFFER_SIZE = 256;
const int CLIENT_PORT = 11050;

int server_sock;
int client_sock;

struct sockaddr_in server_addr;
struct sockaddr_in client_addr;

/* クライアントとのコネクション */
bool connect_client()
{
  int addr_size;

  // ソケットの作成
  client_sock = socket(AF_INET, SOCK_STREAM, 0);
  if(client_sock < 0){
    std::cout << "ERROR:opening client socket" << std::endl;
    return false;
  }

    // クライアントsockaddr_in構造体の設定
  memset((char *)&client_addr, 0, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_port = htons(CLIENT_PORT);
  client_addr.sin_addr.s_addr = INADDR_ANY; // どれでも要求を受け付ける
  bind(client_sock, (struct sockaddr*)&client_addr, sizeof(client_addr));

  // クライアントからの接続要求を待つ
  listen(client_sock, 1);
  // クライアントからの要求を受付
  addr_size = sizeof(client_addr);
  client_sock = accept(client_sock, (struct sockaddr*)&client_addr, (socklen_t*)&addr_size);
  if(client_sock < 0){
    std::cout << "Connection to client failed." << std::endl;
    return false;
  }else{
    std::cout << "Connected client." << std::endl;
    return true;
  }
}

/* サーバとのコネクション */
bool connect_server(const char* server_dst, int server_port)
{
  struct hostent *hp;

    // ソケットの作成
  server_sock = socket(AF_INET, SOCK_STREAM, 0);

  if(server_sock < 0){
    std::cerr << "ERROR opening socket" << std::endl;
    return false;
  }

  // サーバsockaddr_in構造体の設定
  memset((char *)&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(server_port);

  hp = gethostbyname(server_dst);
  bcopy(hp->h_addr, &server_addr.sin_addr, hp->h_length);

  // サーバとの接続
  if(connect(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
    std::cout << server_dst << " could not be connected" << std::endl;
    return false;
  }else{
    std::cout << "connected server" << std::endl;
    return true;
  }
}

/* socket通信の初期化 */
bool initialize()
{
  char buf[BUFFER_SIZE];
  int read_size;
  char server_dst[32];
  int server_port;
  int addr_size;

  // クライアントとの接続
  if(!connect_client()){
    std::cout << "ERROR:connecting client failed" << std::endl;
    return false;
  }

  // サーバのIPアドレス、ポート番号を入力
  std::cout << "server_ip:" << std::flush;
  std::cin >> server_dst;
  std::cout << "server_port:" << std::flush;
  std::cin >> server_port;

  // サーバとの接続
  if(!connect_server(server_dst, server_port)){
    std::cout << "ERROR: connecting server failed" << std::endl;
    return false;
  }

  // サーバからの"connection success"メッセージをクライアントへ送る
  read_size = read(server_sock, buf, sizeof(buf));
  sendto(client_sock, buf, sizeof(buf), 0,
	 (struct sockaddr*)&client_addr, sizeof(client_addr));

  // クライアントからのチーム名をサーバへ送る
  read_size = read(client_sock, buf, sizeof(buf));
  sendto(server_sock, buf, sizeof(buf), 0,
	 (struct sockaddr*)&client_addr, sizeof(client_addr));

  // サーバからの0の個数をクライアントへ送る
  read_size = read(server_sock, buf, sizeof(buf));
  sendto(client_sock, buf, sizeof(buf), 0,
	 (struct sockaddr*)&client_addr, sizeof(client_addr));
  return true;
}

/* nonce値およびblockを中継 */
void proxy_core()
{
  int i;
  int read_size;
  char buf[BUFFER_SIZE];

  while(true){
    // サーバからのブロックをクライアントへ送信
    read_size = read(server_sock, buf, sizeof(buf));
    sendto(client_sock, buf, sizeof(buf),0,
	   (struct sockaddr*)&client_addr, sizeof(client_addr));

    // サーバから"FINISH"を受信したら終了
    if(strcmp(buf, "FINISH") == 0)
      break;

    // クライアントからのnonce値をサーバへ送信
    read_size = read(client_sock, buf, sizeof(buf));
    sendto(server_sock, buf, sizeof(buf), 0,
	   (struct sockaddr*)&server_addr, sizeof(server_addr));
  }
  //finalize();
}

// サーバとのソケットを終了する
bool finalize()
{
  if(close(server_sock) == 0)
    return true;
  else
    return false;
}


