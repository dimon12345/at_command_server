#include <iostream>
#include <system_error>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>


#include "TtyTerminal.h"

TtyTerminal::TtyTerminal() {
    int raw_fd = ::posix_openpt(O_RDWR | O_NOCTTY);
    if (raw_fd == -1) {
        throw std::system_error(errno, std::generic_category(), "posix_openpt failed");
    }
    tty_master_fd_ = UniqueFileDescriptor{raw_fd};

    if (::grantpt(tty_master_fd_.get()) == -1) {
        throw std::system_error(errno, std::generic_category(), "grantpt failed");
    }

    if (::unlockpt(tty_master_fd_.get()) == -1) {
        throw std::system_error(errno, std::generic_category(), "unlockpt failed");
    }

    const char* name_ptr = ::ptsname(tty_master_fd_.get());
    if (!name_ptr) {
        throw std::system_error(errno, std::generic_category(), "ptsname failed");
    }

    std::cout << "TTY device ready: " << name_ptr << std::endl;
    std::cout << "please use 'screen " << name_ptr << "'" << std::endl;

    tty_slave_name_ = name_ptr;
}

size_t TtyTerminal::readData(std::span<char> buffer) const {
    ssize_t bytes_read = ::read(tty_master_fd_.get(), buffer.data(), buffer.size());
    if (bytes_read < 0) {
        throw std::system_error(errno, std::generic_category(), "read failed");
    }
    return static_cast<size_t>(bytes_read);
}

size_t TtyTerminal::writeData(std::string_view data) const {
    ssize_t bytes_written = ::write(tty_master_fd_.get(), data.data(), data.size());
    if (bytes_written < 0) {
        throw std::system_error(errno, std::generic_category(), "write failed");
    }
    return static_cast<size_t>(bytes_written);
}

int TtyTerminal::bytesAvailable() const {
    int bytes = 0;
    if (::ioctl(tty_master_fd_.get(), FIONREAD, &bytes) == -1) {
        throw std::system_error(errno, std::generic_category(), "ioctl FIONREAD failed");
    }
    return bytes;
}
