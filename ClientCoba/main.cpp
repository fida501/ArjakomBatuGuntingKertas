#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <limits>
using namespace std;


void receiveData(SOCKET sock, char* buffer, size_t bufferSize) {
    int valread = recv(sock, buffer, bufferSize, 0);
    if (valread == SOCKET_ERROR) {
        std::cerr << "Recv error: " << WSAGetLastError() << std::endl;
    } else {
        buffer[valread] = '\0';
        std::cout << "Received from server: " << buffer << endl;
    }
}

void sendData(SOCKET sock, const char* message) {
    if (send(sock, message, strlen(message), 0) == SOCKET_ERROR) {
        std::cerr << "Send error: " << WSAGetLastError() << std::endl;
    }
}


void CommunicateWithServer(int socket){
    char buffer[200];
    //Menerima Pesan dari server untuk memasukkan nama
    receiveData(socket, buffer, 200);
    receiveData(socket, buffer, 200);
    std::cout << buffer << std::endl;
    int choice;
    std::cin >> choice;
    std::string choiceString = std::to_string(choice);
    sendData(socket, choiceString.c_str());
    receiveData(socket, buffer, 200);

}


void ClientStart(const char* serverIp, int port)
{
//void runClient(const char* serverIp, int port)
    WSAData wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    if (wsaerr != 0)
    {
        std::cout << "The Winsock dll not found!" << std::endl;
        return;
    }
    else
    {
        std::cout << "The Winsock dll found" << std::endl;
        std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }
    else
    {
        std::cout << "Socket is OK!" << std::endl;
    }

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(serverIp);
    clientService.sin_port = htons(port);

    if (connect(clientSocket, reinterpret_cast<SOCKADDR*>(&clientService), sizeof(clientService)) == SOCKET_ERROR)
    {
        std::cout << "Client: connect() - Failed to connect: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return;
    }
    else
    {
        std::cout << "Client: Connect() is OK!" << std::endl;
        std::cout << "Client: Can start sending and receiving data..." << std::endl;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    CommunicateWithServer(clientSocket);
}


int main()
{
    const char* serverIp = "127.0.0.1";
    int port = 55555;
    cout << "Selamat datang di alfamaret, selamat belanja" << endl;
    cout << "Berikut adalah perintah yang bisa anda lakukan ! " << endl;
    cout << "1. Sambung ke Server" << endl;
    cout << "2. Keluar"<< endl;
    cout << "Silahkan masukkan perintah yang ingin anda lakukan : ";
    int clientConsoleCommand;
    cin >> clientConsoleCommand;
    cout << endl;
    switch(clientConsoleCommand)
    {
    case 1:
        cout << "Terasmbung ke server dengan IP 127.0.0.1 dan Port 55555" << endl;
        ClientStart(serverIp, port);
        break;
    case 2:
        cout << "Terima kasih sudah berbelanja di alfamaret" << endl;
        return 0;
        break;
    default:
        cout << "Invalid Input " << endl;
    }
    system("pause");

    return 0;
}
