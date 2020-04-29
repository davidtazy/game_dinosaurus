#pragma once

#include <functional>
#include <tuple>
#include <variant>

template <typename... States>
class StateMachine {
 public:
  StateMachine() = default;

  StateMachine(States... states) : states(std::move(states)...) {}

  template <typename State>
  State& transitionTo() {
    State& state = std::get<State>(states);

    int id_prev_state = currentState.index();

    currentState = &state;
    int id_state = currentState.index();
    if (onSwitch && id_prev_state != id_state) {
      onSwitch();
    }
    return state;
  }

  template <typename Event>
  void handle(const Event& event) {
    handleBy(event, *this);
  }

  template <typename Event, typename Machine>
  void handleBy(const Event& event, Machine& machine) {
    auto passEventToState = [&machine, &event](auto statePtr) {
      auto action = statePtr->handle(event);
      action.execute(machine, *statePtr, event);
    };
    std::visit(passEventToState, currentState);
  }

  template <typename State>
  bool isInState() const {
    return std::holds_alternative<State*>(currentState);
  }

  void on_switch_state(std::function<void()> callback) { onSwitch = callback; }

 private:
  std::function<void()> onSwitch{};
  std::tuple<States...> states;
  std::variant<States*...> currentState{&std::get<0>(states)};
};
