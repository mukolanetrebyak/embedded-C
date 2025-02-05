#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <streambuf>
#include <sstream>
#include <string>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Header: /opt/vc/include/bcm_host.h
// Library: /opt/vc/lib/libbcm_host.so
#include <bcm_host.h>

// ----------------------------------------------------------------------------------------------------

std::string ReadCPUInfo()
{
    std::ifstream fileStream("/proc/cpuinfo");

    if(!fileStream)
    {
        throw std::runtime_error("Could not open: `/proc/cpuinfo`.");
    }

    std::string cpuinfo((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
                 
    fileStream.close();

    return cpuinfo;
}

// ----------------------------------------------------------------------------------------------------

std::string ReadSysBoardModel()
{
    std::ifstream fileStream("/sys/firmware/devicetree/base/model");

    if(!fileStream)
    {
        throw std::runtime_error("Could not open: `/sys/firmware/devicetree/base/model`.");
    }

    std::string model((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
                 
    fileStream.close();

    return model;
}

// ----------------------------------------------------------------------------------------------------

std::string ReadSysBoardSerial()
{
    std::ifstream fileStream("/sys/firmware/devicetree/base/serial-number");

    if(!fileStream)
    {
        throw std::runtime_error("Could not open: `/sys/firmware/devicetree/base/serial-number`.");
    }

    std::string serial((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
                 
    fileStream.close();

    return serial;
}

// ----------------------------------------------------------------------------------------------------

std::string ReadSysMACAddress()
{
    std::ifstream fileStream("/sys/class/net/eth0/address");

    if(!fileStream)
    {
        throw std::runtime_error("Could not open: `/sys/class/net/eth0/address`.");
    }

    std::string address((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
                 
    fileStream.close();

    return address;
}

// ----------------------------------------------------------------------------------------------------

std::string ReadMailboxBoardModel()
{
    int fd = open("/dev/vcio", 0);
    
    if (fd == -1)
    {
        throw std::runtime_error("Could not open: `/dev/vcio`.");
    }

    uint32_t property[32] =
    {
        0x0000001C, // Buffer size in bytes (including the header values, the end tag and padding): 7 * 4 = 28 = 0x1C.
        0x00000000, // Request code: 0x00000000 = process request.
        0x00010001, // Tag identifier: Get board model.
        0x00000004, // Value buffer size in bytes: 1 * 4 = 4 = 0x4.
        0x00000000, // Request code: b31 clear: request, b30-b0: reserved.
        0x00000000, // Value buffer.
        0x00000000  // End tag: 0x0
    };

    const unsigned long MAJOR_NUM = 100;
    if (ioctl(fd, _IOWR(MAJOR_NUM, 0, char*), property) < 0)
    {
        close(fd);
        throw std::runtime_error("`ioctl` failed.");
    }

    if (property[1] != 0x80000000)
    {
        close(fd);
        throw std::runtime_error("Request failed.");
    }

    close(fd);

    std::stringstream stream;
    stream << std::hex << property[5];
    std::string result = stream.str();
    
    return result;
}

// ----------------------------------------------------------------------------------------------------

std::string ReadMailboxBoardRevision()
{
    int fd = open("/dev/vcio", 0);
    
    if (fd == -1)
    {
        throw std::runtime_error("Could not open: `/dev/vcio`.");
    }

    uint32_t property[32] =
    {
        0x0000001C, // Buffer size in bytes (including the header values, the end tag and padding): 7 * 4 = 28 = 0x1C.
        0x00000000, // Request code: 0x00000000 = process request.
        0x00010002, // Tag identifier: Get board revision.
        0x00000004, // Value buffer size in bytes: 1 * 4 = 4 = 0x4.
        0x00000000, // Request code: b31 clear: request, b30-b0: reserved.
        0x00000000, // Value buffer.
        0x00000000  // End tag: 0x0
    };

    const unsigned long MAJOR_NUM = 100;
    if (ioctl(fd, _IOWR(MAJOR_NUM, 0, char*), property) < 0)
    {
        close(fd);
        throw std::runtime_error("`ioctl` failed.");
    }

    if (property[1] != 0x80000000)
    {
        close(fd);
        throw std::runtime_error("Request failed.");
    }

    close(fd);

    std::stringstream stream;
    stream << std::hex << property[5];
    std::string result = stream.str();
    
    return result;
}

// ----------------------------------------------------------------------------------------------------

std::string ReadMailboxBoardSerial()
{
    int fd = open("/dev/vcio", 0);
    
    if (fd == -1)
    {
        throw std::runtime_error("Could not open: `/dev/vcio`.");
    }

    uint32_t property[32] =
    {
        0x00000020, // Buffer size in bytes (including the header values, the end tag and padding): 8 * 4 = 32 = 0x20.
        0x00000000, // Request code: 0x00000000 = process request.
        0x00010004, // Tag identifier: Get board serial.
        0x00000008, // Value buffer size in bytes: 2 * 4 = 8 = 0x8.
        0x00000000, // Request code: b31 clear: request, b30-b0: reserved.
        0x00000000, // Value buffer.
        0x00000000, // Value buffer.
        0x00000000  // End tag: 0x0
    };

    const unsigned long MAJOR_NUM = 100;
    if (ioctl(fd, _IOWR(MAJOR_NUM, 0, char*), property) < 0)
    {
        close(fd);
        throw std::runtime_error("`ioctl` failed.");
    }

    if (property[1] != 0x80000000)
    {
        close(fd);
        throw std::runtime_error("Request failed.");
    }

    close(fd);

    std::stringstream stream;
    stream << std::hex << property[5]; // property[6] ignored.
    std::string result = stream.str();
    
    return result;
}

// ----------------------------------------------------------------------------------------------------

std::string ReadMailboxMACAddress()
{
    int fd = open("/dev/vcio", 0);
    
    if (fd == -1)
    {
        throw std::runtime_error("Could not open: `/dev/vcio`.");
    }

    uint32_t property[32] =
    {
        0x00000020, // Buffer size in bytes (including the header values, the end tag and padding): 8 * 4 = 32 = 0x20.
        0x00000000, // Request code: 0x00000000 = process request.
        0x00010003, // Tag identifier: Get board MAC address.
        0x00000008, // Value buffer size in bytes: 2 * 4 = 8 = 0x8.
        0x00000000, // Request code: b31 clear: request, b30-b0: reserved.
        0x00000000, // Value buffer.
        0x00000000, // Value buffer.
        0x00000000  // End tag: 0x0
    };

    const unsigned long MAJOR_NUM = 100;
    if (ioctl(fd, _IOWR(MAJOR_NUM, 0, char*), property) < 0)
    {
        close(fd);
        throw std::runtime_error("`ioctl` failed.");
    }

    if (property[1] != 0x80000000)
    {
        close(fd);
        throw std::runtime_error("Request failed.");
    }

    close(fd);
    
    unsigned char* ptr = reinterpret_cast<unsigned char*>(&property[5]);

    std::stringstream stream;
    stream
        << std::hex
        << static_cast<int>(ptr[0]) << ":"
        << static_cast<int>(ptr[1]) << ":"
        << static_cast<int>(ptr[2]) << ":"
        << static_cast<int>(ptr[3]) << ":"
        << static_cast<int>(ptr[4]) << ":"
        << static_cast<int>(ptr[5]);
    std::string result = stream.str();
    
    return result;
}

// ----------------------------------------------------------------------------------------------------

std::string ReadOTPDump()
{
    bcm_host_init();
    
    char buffer[1024] = { 0 };

    if (vc_gencmd(buffer, sizeof(buffer), "otp_dump") != 0)
    {
        bcm_host_deinit();
        throw std::runtime_error("Could not execute `otp_dump` command.");
    }
    
    bcm_host_deinit();
    
    std::string otpDump = buffer;

    return otpDump;
}

// ----------------------------------------------------------------------------------------------------

int main()
{
    std::cout << "-------------------- CPU Info: $ cat /proc/cpuinfo --------------------" << std::endl;
    std::cout << ReadCPUInfo() << std::endl;
    
    std::cout << "-------------------- Board Model: $ cat /sys/firmware/devicetree/base/model --------------------" << std::endl;
    std::cout << ReadSysBoardModel() << std::endl;
    
    std::cout << "-------------------- Board Serial: $ cat /sys/firmware/devicetree/base/serial-number --------------------" << std::endl;
    std::cout << ReadSysBoardSerial() << std::endl;
    
    std::cout << "-------------------- MAC Address: $ cat /sys/class/net/eth0/address --------------------" << std::endl;
    std::cout << ReadSysMACAddress() << std::endl;
    
    std::cout << "-------------------- Board Model: $ /opt/vc/bin/vcmailbox 0x10001 0x4 0x0 0x0 --------------------" << std::endl;
    std::cout << ReadMailboxBoardModel() << std::endl;
    
    std::cout << "-------------------- Board Revision: $ /opt/vc/bin/vcmailbox 0x10002 0x4 0x0 0x0 --------------------" << std::endl;
    std::cout << ReadMailboxBoardRevision() << std::endl;
    
    std::cout << "-------------------- Board Serial: $ /opt/vc/bin/vcmailbox 0x10004 0x8 0x0 0x0 0x0 --------------------" << std::endl;
    std::cout << ReadMailboxBoardSerial() << std::endl;
    
    std::cout << "-------------------- MAC Address: $ /opt/vc/bin/vcmailbox 0x10003 0x8 0x0 0x0 0x0 --------------------" << std::endl;
    std::cout << ReadMailboxMACAddress() << std::endl;
        
    std::cout << "-------------------- OTP: vcgencmd otp_dump --------------------" << std::endl;
    std::cout << ReadOTPDump() << std::endl;

    return 0;
}