#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

SOCKET serverSocket = INVALID_SOCKET;
SOCKET acceptSocket = INVALID_SOCKET;

SOCKET player1Socket;
SOCKET player2Socket;

int player1Answer;
int player2Answer;

std::string player1Name;
std::string player2Name;

void receiveData(SOCKET client_socket, char* buffer, size_t bufferSize) {
    int valread = recv(client_socket, buffer, bufferSize, 0);
    if (valread == SOCKET_ERROR) {
        std::cerr << "Recv error: " << WSAGetLastError() << std::endl;
    } else {
        buffer[valread] = '\0';
        cout << "Received data are = " << buffer << std::endl;
    }
}

void sendData(SOCKET client_socket, const char* message) {
    if (send(client_socket, message, strlen(message), 0) == SOCKET_ERROR) {
        std::cerr << "Send error: " << WSAGetLastError() << std::endl;
    } else {
        cout << "Data sent to the client " << endl;
    }
}

void HandleClient(SOCKET client_socket, int& playerAnswer){
    char buffer[200];
    const char *request = "";
    request = "Masukkan Jawaban yang anda inginkan \n 1 Untuk Gunting \n 2 untuk kertas \n 3 untuk batu \n Jawab :";
    sendData(client_socket, request);
    receiveData(client_socket,buffer,200);
    playerAnswer = atoi(buffer);
}

SOCKET AcceptConnection(SOCKET socket){
    SOCKET newSocket = accept(socket, NULL, NULL);
        if (newSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }
    return newSocket;
}

std::string DetermineWinner(int answer1, int answer2){
    if(answer1 == answer2){
            return "GAME IS DRAW \n";
    }
    if ((answer1 == 1 && answer2 == 2) || // Scissors cut paper
        (answer1 == 2 && answer2 == 3) || // Paper covers rock
        (answer1 == 3 && answer2 == 1)) { // Rock crushes scissors
        return "Player 1 wins! \n";
    } else {
        return "Player 2 wins! \n";
    }
}


bool ServerStart(const char* ipAddress, int port)
{
    WSAData wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    // Check for initialization success
    if (wsaerr != 0)
    {
        std::cout << "The Winsock dll not found!" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "The Winsock dll found" << std::endl;
        std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
    }
//    return 0;
    // Create a socket
//    SOCKET serverSocket;
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for socket creation success
    if (serverSocket == INVALID_SOCKET)
    {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }
    else
    {
        std::cout << "Socket is OK!" << std::endl;
    }
    // Bind the socket to an IP address and port number
    sockaddr_in service;
    service.sin_family = AF_INET;
//    service.sin_addr.s_addr = inet_addr("127.0.0.1");  // Replace with your desired IP address
    service.sin_addr.s_addr = inet_addr(ipAddress);
    service.sin_port = htons(port);  // Choose a port number

    // Use the bind function
    if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR)
    {
        std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }
    else
    {
        std::cout << "bind() is OK!" << std::endl;
    }
    // Listen for incoming connections
    if (listen(serverSocket, 1) == SOCKET_ERROR)
    {
        std::cout << "listen(): Error listening on socket: " << WSAGetLastError() << std::endl;
//        closesocket(serverSocket);
//        WSACleanup();
//        return false;
    }
    else
    {
        std::cout << "listen() is OK! I'm waiting for new connections..." << std::endl;
    }
//    SOCKET acceptSocket;
//    acceptSocket = accept(serverSocket, NULL, NULL);
//    if(acceptSocket == INVALID_SOCKET)
//    {
//        cout << "Accept Failed : " << WSAGetLastError() << endl;
//        WSACleanup();
//        return false;
//    }
    player1Socket = AcceptConnection(serverSocket);
    cout << "First Player Connected , Waiting for Second player ..." << endl;
    sendData(player1Socket, "Anda Terdaftar Sebagai Player 1");

    player2Socket = AcceptConnection(serverSocket);
    cout << "Second Player Connected, Game Starting..." << endl;
    sendData(player2Socket, "Anda Terdaftar Sebagai Player 2");

    std::thread t1(HandleClient, player1Socket, std::ref(player1Answer));
    std::thread t2(HandleClient, player2Socket, std::ref(player2Answer));

    t1.join();
    t2.join();
    cout << "Player 1 answer are == " << player1Answer << endl;
    cout << "Player 2 answer are == " << player2Answer << endl;

    std::string result;
    result = DetermineWinner(player1Answer, player2Answer);
    sendData(player1Socket, result.c_str());
    sendData(player2Socket, result.c_str());
//    return true;
}


void ServerStop()
{
    if(acceptSocket != INVALID_SOCKET)
    {
        shutdown(acceptSocket, SD_BOTH);
        closesocket(acceptSocket);
    }
    if(serverSocket != INVALID_SOCKET)
    {
        closesocket(serverSocket);
    }
    WSACleanup();
    cout << "Server Stopped and cleaned up." << endl;
}

bool sendMessage(SOCKET clientSocket, const char* message)
{
    int sbyteCount = send(clientSocket, message, strlen(message), 0);
    if (sbyteCount == SOCKET_ERROR)
    {
        std::cout << "Server send error: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

int main()
{
    const char* ipAddress = "127.0.0.1";
    int port = 55555;

    while(true)
    {
        cout << "Selamat datang di Indomaret, selamat belanja" << endl;
        cout << "Berikut adalah perintah yang bisa anda lakukan ! " << endl;
        cout << "1. Mulai Server" << endl;
        cout << "2. Stop Server"<< endl;
        cout << "3. Keluar"<< endl;
        cout << "Silahkan masukkan perintah yang ingin anda lakukan : ";
        int serverConsoleCommand;
        cin >> serverConsoleCommand;

        switch (serverConsoleCommand)
        {
        case 1:
            cout << "Server dimulai dengan IP 127.0.0.1 dan port 55555" << endl;
            ServerStart(ipAddress, port);
            break;
        case 2:
            cout << "Anda memilih Perintah nomor 2" << endl;
            break;
        case 3:
            cout << "Anda memilih perintah nomor 3" << endl;
            cout << "Terima kasih sudah berbelanja di indomaret" << endl;
            return 0;
            break;
        default:
            cout << "anda memasukkan input yang tidak ada di dalam list!" << endl;
        }

    }

//// Receive data from the client
//    char receiveBuffer[200];
//    int rbyteCount = recv(acceptSocket, receiveBuffer, 200, 0);
//    if (rbyteCount < 0)
//    {
//        std::cout << "Server recv error: " << WSAGetLastError() << std::endl;
//        return;
//    }
//    else
//    {
//        std::cout << "Received data: " << receiveBuffer << std::endl;
//    }
//
//
//// Send a response to the client
//    char buffer[200];
//    std::cout << "Enter the message: ";
//    std::cin.getline(buffer, 200);
//    int sbyteCount = send(acceptSocket, buffer, 200, 0);
//    if (sbyteCount == SOCKET_ERROR)
//    {
//        std::cout << "Server send error: " << WSAGetLastError() << std::endl;
//        return;
//    }
//    else
//    {
//        std::cout << "Server: Sent " << sbyteCount << " bytes" << std::endl;
//    }
}
