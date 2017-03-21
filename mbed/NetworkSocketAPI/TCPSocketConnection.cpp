/* Copyright (C) 2012 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "TCPSocketConnection.h"
#include <cstring>
#include "TCPSocket.h"
#include "debug.h"
#include "WiFi.h"

using std::memset;
using std::memcpy;

TCPSocketConnection::TCPSocketConnection(NetworkStack *iface) :
    _iface(iface), _is_connected(false) {
  if ( !_iface ) _iface = WiFi::get_interface();
}

bool TCPSocketConnection::is_connected(void) {
    return _is_connected;
}

int TCPSocketConnection::connect(const char *host, uint16_t port) {
    if (!is_connected()) this->open(_iface);
    int ret = TCPSocket::connect(host, port);
    if (ret < 0) {
        TCPSocket::close();
        ERR("Could not connect");
    } else
        _is_connected = true;
    return ret;
}

int TCPSocketConnection::connect(const SocketAddress &address) {
    if (!is_connected()) TCPSocket::open(_iface);
    int ret = TCPSocket::connect(address);
    if (ret < 0) {
        TCPSocket::close();
        ERR("Could not connect");
    } else
        _is_connected = true;
    return ret;
}

int TCPSocketConnection::close() {
    _is_connected = false;
    return TCPSocket::close();
}

void TCPSocketConnection::set_blocking(bool blocking, unsigned int timeout) {
    TCPSocket::set_blocking(blocking);
    if (!blocking) this->set_timeout(timeout);
}

// -1 if unsuccessful, else number of bytes written
int TCPSocketConnection::send_all(char* data, int length) {
    return this->send(data, length);
}

int TCPSocketConnection::receive(char* data, int length) {
    return this->recv(data, length);
}

// -1 if unsuccessful, else number of bytes received
int TCPSocketConnection::receive_all(char* data, int length) {
    return this->recv(data, length);
}
