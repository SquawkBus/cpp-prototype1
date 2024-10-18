#include "interactor.hpp"

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"

namespace squawkbus::topicbus
{
  using squawkbus::io::Poller;
  using squawkbus::serialization::FrameBuffer;
  using squawkbus::topicbus::messages::Message;

  Interactor::Interactor(int fd, Poller& poller, const std::string& host)
    : fd_(fd),
      host_(host),
      poller_(poller)
  {
  }

  void Interactor::receive(std::vector<std::vector<char>>&& bufs)
  {
    for (auto &buf : bufs)
      reader_.write(buf);

    while (reader_.has_frame())
    {
      auto frame = reader_.read();
      auto message = Message::read(frame);
      process_message(message);
    }
  }

  void Interactor::process_message(std::shared_ptr<Message> message)
  {

  }

  void Interactor::send(std::shared_ptr<Message> message)
  {
    FrameBuffer frame;
    message->write(frame);
    auto buf = frame.make_frame();
    poller_.write(fd_, buf);
  }
}