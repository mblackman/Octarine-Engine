// Behavioral checks for the type-erased EventBus pub/sub. The documented hazard is a dangling
// callback after an owner dies, so UnsubscribeAll is exercised explicitly. gtest-free; exit code =
// failed-check count. Registered with ctest as EventBusTest. Header-only bus; links Logger only.

#include <vector>

#include "EventBus/Event.h"
#include "EventBus/EventBus.h"
#include "TestHarness.h"

using octarine::test::Check;

namespace {
struct PingEvent : Event {
  int value = 0;
  explicit PingEvent(const int v = 0) : value(v) {}
};

struct SilentEvent : Event {
  SilentEvent() = default;
};

struct Listener {
  int count = 0;
  int last = 0;
  void OnPing(PingEvent& e) {
    ++count;
    last = e.value;
  }
};

struct ConstListener {
  int count = 0;
  int last = 0;
  void OnPing(const PingEvent& e) {
    ++count;
    last = e.value;
  }
};

struct OrderRecorder {
  std::vector<int>* log = nullptr;
  int id = 0;
  void OnPing(PingEvent&) { log->push_back(id); }
};
}  // namespace

int main() {
  // Emit invokes a subscribed handler with the forwarded payload.
  {
    EventBus bus;
    Listener listener;
    bus.SubscribeEvent<Listener, PingEvent>(&listener, &Listener::OnPing);
    bus.EmitEvent<PingEvent>(7);
    Check(listener.count == 1, "subscribed handler fired exactly once");
    Check(listener.last == 7, "handler received the forwarded payload");
  }

  // Emitting with no subscribers is a harmless no-op.
  {
    EventBus bus;
    bus.EmitEvent<SilentEvent>();  // must not throw or crash
    Listener listener;
    bus.SubscribeEvent<Listener, PingEvent>(&listener, &Listener::OnPing);
    bus.EmitEvent<SilentEvent>();  // a different event type has no PingEvent subscribers
    Check(listener.count == 0, "emit of an unsubscribed event type invokes nothing");
  }

  // Multiple subscribers all fire, in subscription order.
  {
    EventBus bus;
    std::vector<int> order;
    OrderRecorder first{&order, 1};
    OrderRecorder second{&order, 2};
    bus.SubscribeEvent<OrderRecorder, PingEvent>(&first, &OrderRecorder::OnPing);
    bus.SubscribeEvent<OrderRecorder, PingEvent>(&second, &OrderRecorder::OnPing);
    bus.EmitEvent<PingEvent>(0);
    Check(order.size() == 2, "every subscriber fired");
    Check(order.size() == 2 && order[0] == 1 && order[1] == 2, "subscribers fire in subscription order");
  }

  // UnsubscribeAll drops exactly the named owner's callbacks (the dangling-callback guard).
  {
    EventBus bus;
    Listener stays;
    Listener leaves;
    bus.SubscribeEvent<Listener, PingEvent>(&stays, &Listener::OnPing);
    bus.SubscribeEvent<Listener, PingEvent>(&leaves, &Listener::OnPing);
    bus.UnsubscribeAll(&leaves);
    bus.EmitEvent<PingEvent>(3);
    Check(leaves.count == 0, "unsubscribed owner is not invoked");
    Check(stays.count == 1, "other owners remain subscribed after UnsubscribeAll");
  }

  // The const-ref callback overload dispatches too.
  {
    EventBus bus;
    ConstListener listener;
    bus.SubscribeEvent<ConstListener, PingEvent>(&listener, &ConstListener::OnPing);
    bus.EmitEvent<PingEvent>(5);
    Check(listener.count == 1 && listener.last == 5, "const TEvent& handler overload dispatches");
  }

  return octarine::test::Result();
}
