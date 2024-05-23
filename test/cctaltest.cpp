#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <cstdint>

// Función para abrir el puerto serie
int openSerialPort(const char *portName) {
    int fd = open(portName, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("Error opening serial port");
        return -1;
    }

    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    tcsetattr(fd, TCSANOW, &options);
    return fd;
}

// Función para cerrar el puerto serie
void closeSerialPort(int fd) {
    close(fd);
}

// Función para calcular el checksum de un mensaje ccTalk
uint8_t calculateChecksum(const std::vector<uint8_t> &message) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < message.size(); ++i) {
        checksum += message[i];
    }
    return -checksum;
}

// Función para enviar un mensaje ccTalk
bool sendCCTalkMessage(int fd, uint8_t destination, uint8_t command, const std::vector<uint8_t> &data) {
    std::vector<uint8_t> message;
    message.push_back(destination);
    message.push_back(data.size());
    message.push_back(1); // Suponemos que la dirección de la fuente es 1 (el maestro)
    message.push_back(command);
    message.insert(message.end(), data.begin(), data.end());
    message.push_back(calculateChecksum(message));

    ssize_t written = write(fd, message.data(), message.size());
    return written == static_cast<ssize_t>(message.size());
}

// Función para recibir un mensaje ccTalk
bool receiveCCTalkMessage(int fd, std::vector<uint8_t> &message) {
    uint8_t buffer[256];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead <= 0) {
        return false;
    }

    message.assign(buffer, buffer + bytesRead);
    return true;
}

int main() {
    const char *portName = "/dev/ttyUSB1";
    int fd = openSerialPort(portName);
    if (fd == -1) {
        return 1;
    }

    uint8_t destination = 2; // Dirección del dispositivo esclavo
    uint8_t command = 0; // Comando a enviar
    std::vector<uint8_t> data = {0x01, 0x02, 0x03}; // Datos a enviar

    if (!sendCCTalkMessage(fd, destination, command, data)) {
        std::cerr << "Error sending ccTalk message" << std::endl;
    }

    std::vector<uint8_t> response;
    if (!receiveCCTalkMessage(fd, response)) {
        std::cerr << "Error receiving ccTalk message" << std::endl;
    } else {
        std::cout << "Received ccTalk message: ";
        for (uint8_t byte : response) {
            std::cout << std::hex << static_cast<int>(byte) << " ";
        }
        std::cout << std::dec << std::endl;
    }

    closeSerialPort(fd);
    return 0;
}