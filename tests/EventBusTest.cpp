// Behavioral checks for the type-erased EventBus pub/sub. The documented hazard is a dangling
// callback after an owner dies; the bus now hands back a RAII SubscriptionHandle whose destructor
// drops the callback, so handle lifetime is exercised explicitly. gtest-free; exit code =
// failed-check count. Registered with ctest as EventBusTest. Header-only bus; links Logger only.

#include <memory>
#include <optional>
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
    auto sub = bus.SubscribeEvent<Listener, PingEvent>(&listener, &Listener::OnPing);
    bus.EmitEvent<PingEvent>(7);
    Check(listener.count == 1, "subscribed handler fired exactly once");
    Check(listener.last == 7, "handler received the forwarded payload");
  }

  // Emitting with no subscribers is a harmless no-op.
  {
    EventBus bus;
    bus.EmitEvent<SilentEvent>();  // must not throw or crash
    Listener listener;
    auto sub = bus.SubscribeEvent<Listener, PingEvent>(&listener, &Listener::OnPing);
    bus.EmitEvent<SilentEvent>();  // a different event type has no PingEvent subscribers
    Check(listener.count == 0, "emit of an unsubscribed event type invokes nothing");
  }

  // Multiple subscribers all fire, in subscription order.
  {
    EventBus bus;
    std::vector<int> order;
    OrderRecorder first{&order, 1};
    OrderRecorder second{&order, 2};
    auto sub1 = bus.SubscribeEvent<OrderRecorder, PingEvent>(&first, &OrderRecorder::OnPing);
    auto sub2 = bus.SubscribeEvent<OrderRecorder, PingEvent>(&second, &OrderRecorder::OnPing);
    bus.EmitEvent<PingEvent>(0);
    Check(order.size() == 2, "every subscriber fired");
    Check(order.size() == 2 && order[0] == 1 && order[1] == 2, "subscribers fire in subscription order");
  }

  // Resetting a SubscriptionHandle drops exactly that callback (the dangling-callback guard),
  // leaving sibling subscriptions intact.
  {
    EventBus bus;
    Listener stays;
    Listener leaves;
    auto staysSub = bus.SubscribeEvent<Listener, PingEvent>(&stays, &Listener::OnPing);
    auto leavesSub = bus.SubscribeEvent<Listener, PingEvent>(&leaves, &Listener::OnPing);
    leavesSub.Reset();
    bus.EmitEvent<PingEvent>(3);
    Check(leaves.count == 0, "explicitly reset subscription is not invoked");
    Check(stays.count == 1, "other subscriptions remain after one is reset");
    Check(!leavesSub.Active(), "a reset handle reports inactive");
    Check(staysSub.Active(), "a live handle reports active");
  }

  // A handle going out of scope unsubscribes via its destructor — no manual call needed.
  {
    EventBus bus;
    Listener listener;
    {
      auto sub = bus.SubscribeEvent<Listener, PingEvent>(&listener, &Listener::OnPing);
      bus.EmitEvent<PingEvent>(1);
      Check(listener.count == 1, "handler fires while the handle is alive");
    }
    bus.EmitEvent<PingEvent>(1);
    Check(listener.count == 1, "handler stops firing once its handle is destroyed");
  }

  // Shutdown-order independence: a handle that outlives the bus is a safe no-op. The handle below
  // is destroyed AFTER the bus, the reverse of the old void*-owner teardown ordering.
  {
    Listener listener;
    std::optional<EventBus::SubscriptionHandle> sub;
    {
      EventBus bus;
      sub = bus.SubscribeEvent<Listener, PingEvent>(&listener, &Listener::OnPing);
      Check(sub->Active(), "handle is active while its bus lives");
    }  // bus destroyed here, before the handle
    Check(!sub->Active(), "handle reports inactive once its bus is gone");
    sub.reset();  // must not touch the freed bus
    Check(listener.count == 0, "handle destruction after the bus is a safe no-op");
  }

  // The const-ref callback overload dispatches too.
  {
    EventBus bus;
    ConstListener listener;
    auto sub = bus.SubscribeEvent<ConstListener, PingEvent>(&listener, &ConstListener::OnPing);
    bus.EmitEvent<PingEvent>(5);
    Check(listener.count == 1 && listener.last == 5, "const TEvent& handler overload dispatches");
  }

  return octarine::test::Result();
}
