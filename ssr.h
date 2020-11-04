#ifndef SSR_H
#define SSR_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <vector>

using namespace boost::asio::ip;
constexpr uint32_t RETRY_CONNECT = 1000;

enum e_role : unsigned char { SERVER, CLIENT, ALL };
namespace ssr {

template <typename T> class Communicator {
public:
  Communicator(std::string const &host, const uint16_t port)
      : m_socket(m_context) {
    boost::asio::io_context::work worker(m_context);
  }
  ~Communicator() { close(); }
  void close() {
    m_context.stop();
    if (m_thread.joinable())
      m_thread.join();
    m_socket.close();
  }
  bool is_connected() const { return m_socket.is_open(); }
  virtual void send(const T *const input, const int size) {}
  virtual void receive(T *const buffer, const int size) {}

protected:
  boost::asio::io_context m_context;
  tcp::socket m_socket;
  std::thread m_thread;
};

template <typename T> class Sender : public Communicator<T> {
public:
  Sender(std::string const &addr, const uint16_t port)
      : Communicator<T>(addr, port) {
    boost::system::error_code ec;
    tcp::resolver resolver(this->m_context);
    tcp::resolver::iterator endpoint =
        resolver.resolve(tcp::resolver::query(addr, std::to_string(port)));
    for (int i = 0; i < RETRY_CONNECT; i++) {
      boost::asio::connect(this->m_socket, endpoint, ec);
      if (!ec)
        break;
      boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
    }
    if (ec) {
      std::cerr << "[Sender] Connect failed due to timeout!\n";
    }
    this->m_thread = std::thread([this]() { this->m_context.run(); });
  }
  template <typename V, typename Allocator>
  void send(std::vector<V, Allocator> const &message) {
    boost::system::error_code ec;
    this->m_socket.wait(tcp::socket::wait_write, ec);
    boost::asio::write(this->m_socket, boost::asio::buffer(message), ec);
    if (ec) {
      std::cerr << "[Sender] Send Error: " << ec.message() << "\n";
    }
  }
  void send(const T *const input, const int size) override {
    std::vector<T> v(input, input + size);
    send(v);
  }
};

template <typename T> class Receiver : public Communicator<T> {
public:
  Receiver(std::string const &addr, const uint16_t port)
      : Communicator<T>(addr, port) {
    try {
      tcp::acceptor acceptor(this->m_context, tcp::endpoint(tcp::v4(), port));
      acceptor.accept(this->m_socket);
      this->m_thread = std::thread([this]() { this->m_context.run(); });
    } catch (std::exception &e) {
      std::cerr << "[Receiver] Exception: " << e.what() << "\n";
    }
  }
  void receive(T *const buffer, const int size) override {
    try {
      boost::asio::streambuf sb;
      boost::system::error_code ec;
      this->m_socket.wait(tcp::socket::wait_read, ec);
      std::vector<T> target(size);
      boost::asio::read(this->m_socket, sb.prepare(size * sizeof(T)), ec);
      // Ref: https://stackoverflow.com/a/28931673
      sb.commit(size * sizeof(T));
      buffer_copy(boost::asio::buffer(target), sb.data());
      if (ec) {
        std::cerr << "[Receiver] Receive Error: " << ec.message() << "\n";
      }
      for (int i = 0; i < size; i++) {
        buffer[i] = target[i];
      }
    } catch (std::exception &e) {
      std::cerr << "[Receiver] Receive Exception: " << e.what() << "\n";
      exit(1);
    }
  }
};
template <typename T>
Communicator<T> *init(e_role role, const std::string &address,
                      const uint16_t port) {
  if (role == SERVER) {
    return (Communicator<T> *)new Sender<T>(address, port);
  } else {
    return (Communicator<T> *)new Receiver<T>(address, port);
  }
}
} // namespace ssr
#endif /* SSR_H */
