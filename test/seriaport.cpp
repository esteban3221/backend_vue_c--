#include <iostream>
#include "serialfunc.c" 
#include <iomanip>
#include <vector>
#include <cstdint>

// Función para encriptar los datos antes de enviarlos al dispositivo
std::vector<uint8_t> encryptData(const std::vector<uint8_t> &data, uint64_t encryptionKey)
{
    std::vector<uint8_t> encryptedData;
    for (uint8_t byte : data)
    {
        encryptedData.push_back(byte ^ (encryptionKey & 0xFF));
    }
    return encryptedData;
}

int main()
{
    const char *port_name = "/dev/ttyUSB0";        // Nombre del puerto serie
    SSP_PORT serial_port = OpenSSPPort(port_name); // Abrir el puerto serie

    if (serial_port == -1)
    {
        std::cerr << "Error al abrir el puerto serie" << std::endl;
        return 1;
    }

    // Comando que deseas enviar al dispositivo (por ejemplo, un comando sin encriptar)
    std::vector<uint8_t> command = {0x01, 0x02, 0x03};

    // Encriptar el comando antes de enviarlo al dispositivo
    uint64_t encryptionKey = 0x0123456701234567L; // Clave de encriptación
    std::vector<uint8_t> encryptedCommand = encryptData(command, encryptionKey);

    // Escribir el comando en el puerto serie
    if (!WriteData(encryptedCommand.data(), encryptedCommand.size(), serial_port))
    {
        std::cerr << "Error al enviar el comando" << std::endl;
        CloseSSPPort(serial_port); // Cerrar el puerto serie
        return 1;
    }

    // Leer la respuesta del dispositivo
    unsigned char response[256]; // Tamaño máximo de respuesta
    int bytes_read = ReadData(serial_port, response, sizeof(response));

    if (bytes_read < 0)
    {
        std::cerr << "Error al leer la respuesta" << std::endl;
    }
    else
    {
        std::cout << "Respuesta recibida: ";
        for (int i = 0; i < bytes_read; ++i)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)response[i] << " ";
        }
        std::cout << std::dec << std::endl;
    }

    CloseSSPPort(serial_port); // Cerrar el puerto serie
    return 0;
}