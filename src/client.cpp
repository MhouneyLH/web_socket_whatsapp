#include <iostream>     // input output operations
#include <sys/socket.h> // socket handling
#include <netinet/in.h> // internet address handling
#include <arpa/inet.h>  // internet operations -> e.g. inet_ntoa()
#include <cstring>      // string operations -> e.g. memset()
#include <unistd.h>     // unix standard -> e.g. close()

static const unsigned int SERVER_PORT = 8080U;
static const char *SERVER_ADDRESS = "127.0.0.1";
static const int SOCKET_ERROR_CODE = -1;
static const unsigned int MESSAGE_BUFFER_SIZE = 256U;

int main(const int argc, const char *argv[])
{
    // Create a socket
    std::cout << "CLIENT: Creating socket..." << std::endl;
    const int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == SOCKET_ERROR_CODE)
    {
        std::cerr << "Failed to create socket" << std::endl;
        return EXIT_FAILURE;
    }

    // Connect to the server
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serverAddress.sin_port = htons(SERVER_PORT);

    std::cout << "CLIENT: Connecting to server..." << std::endl;
    const int connectResult = connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (connectResult == SOCKET_ERROR_CODE)
    {
        std::cerr << "Failed to connect to server" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "CLIENT: Connected to server" << std::endl;

    // Chatting with the server
    char messageBuffer[MESSAGE_BUFFER_SIZE];
    while (true)
    {
        // Send a message to the server
        std::cout << "CLIENT: Enter a message to send to the server: ";
        std::cin.getline(messageBuffer, MESSAGE_BUFFER_SIZE);
        const int sendResult = send(clientSocket, messageBuffer, strlen(messageBuffer), 0);
        if (sendResult == SOCKET_ERROR_CODE)
        {
            std::cerr << "Failed to send message to server" << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "CLIENT: Sent message to server" << std::endl;

        // Receive a message from the server
        const int receiveResult = recv(clientSocket, messageBuffer, MESSAGE_BUFFER_SIZE, 0);
        if (receiveResult == SOCKET_ERROR_CODE)
        {
            std::cerr << "Failed to receive message from server" << std::endl;
            return EXIT_FAILURE;
        }
        messageBuffer[receiveResult] = '\0';

        std::cout << "CLIENT: Received message from server: " << messageBuffer << std::endl;
    }

    // Close the socket
    std::cout << "CLIENT: Closing socket..." << std::endl;
    close(clientSocket);

    return EXIT_SUCCESS;
}