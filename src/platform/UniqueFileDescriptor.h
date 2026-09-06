#pragma once

#include <unistd.h>
#include <memory>

struct fd_t {
    int fd = -1;

    fd_t() noexcept = default;
    fd_t(int v) noexcept : fd(v) {}
    fd_t(std::nullptr_t) noexcept : fd(-1) {}

    operator int() const noexcept { return fd; }

    bool operator==(std::nullptr_t) const noexcept { return fd == -1; }
    bool operator!=(std::nullptr_t) const noexcept { return fd != -1; }
};

struct FileDescriptorDeleter {
    using pointer = fd_t;

    void operator()(int fd) const noexcept {
        if (fd >= 0) {
            ::close(fd);
        }
    }
};

using UniqueFileDescriptor = std::unique_ptr<fd_t, FileDescriptorDeleter>;
