#include <iostream>     // input output operations
#include <sys/socket.h> // socket handling
#include <netinet/in.h> // internet address handling
#include <arpa/inet.h>  // internet operations -> e.g. inet_ntoa()
#include <cstring>      // string operations -> e.g. memset()
#include <unistd.h>     // unix standard -> e.g. close()

static const unsigned int SERVER_PORT = 8080U;
static const unsigned int MAX_CONNECTIONS = 2U;
static const int SOCKET_ERROR_CODE = -1;
static const unsigned int MESSAGE_BUFFER_SIZE = 256U;

int main(const int argc, const char *argv[])
{
    // Create socket
    std::cout << "SERVER: Creating socket..." << std::endl;
    const int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == SOCKET_ERROR_CODE)
    {
        std::cerr << "Failed to create socket" << std::endl;
        return EXIT_FAILURE;
    }

    // Bind socket to an address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(SERVER_PORT);

    std::cout << "SERVER: Binding socket to address..." << std::endl;
    const int bindResult = bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (bindResult == SOCKET_ERROR_CODE)
    {
        std::cerr << "Failed to bind socket to address" << std::endl;
        return EXIT_FAILURE;
    }

    // Listen for incoming connections
    std::cout << "SERVER: Listening for incoming connections on port " << SERVER_PORT << "..." << std::endl;
    const int listenResult = listen(serverSocket, MAX_CONNECTIONS);
    if (listenResult == SOCKET_ERROR_CODE)
    {
        std::cerr << "Failed to listen for incoming connections" << std::endl;
        return EXIT_FAILURE;
    }

    // Accept incoming connections from a client
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    std::cout << "SERVER: Accepting incoming connections..." << std::endl;
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
    if (clientSocket == SOCKET_ERROR_CODE)
    {
        std::cerr << "Failed to accept incoming connection" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "SERVER: Accepted incoming connection from" << inet_ntoa(clientAddress.sin_addr) << std::endl;

    // Chatting with the client
    char messageBuffer[MESSAGE_BUFFER_SIZE];
    while (true)
    {
        // clear buffer
        memset(messageBuffer, 0, sizeof(messageBuffer));

        // get message from client
        const int bytesRead = recv(clientSocket, messageBuffer, sizeof(messageBuffer), 0);
        if (bytesRead == SOCKET_ERROR_CODE)
        {
            std::cerr << "Failed to read from client" << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "SERVER: Received message from client: " << messageBuffer << std::endl;

        // get input from server
        std::cout << "SERVER: ";
        std::cin.getline(messageBuffer, sizeof(messageBuffer));

        // send message to client
        const int bytesSent = send(clientSocket, messageBuffer, sizeof(messageBuffer), 0);
        if (bytesSent == SOCKET_ERROR_CODE)
        {
            std::cerr << "Failed to send message to client" << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "SERVER: Sent message to client: " << messageBuffer << std::endl;
    }

    // Close the sockets
    std::cout << "SERVER: Closing sockets..." << std::endl;

    std::cout << "SERVER: Closing client socket..." << std::endl;
    close(clientSocket);

    std::cout << "SERVER: Closing server socket..." << std::endl;
    close(serverSocket);

    return EXIT_SUCCESS;
}