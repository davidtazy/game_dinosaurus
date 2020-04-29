#pragma once
#include "fsm/StateMachine.h"
#include "fsm/actions/ByDefault.h"
#include "fsm/actions/Maybe.h"
#include "fsm/actions/Nothing.h"
#include "fsm/actions/On.h"
#include "fsm/actions/TransitionTo.h"
#include "fsm/actions/Will.h"

#include <chrono>
#include <iostream>

class DinoState {
  // events
  struct OnPause {};
  struct OnJump {};
  struct OnStart {};
  struct OnDied {};
  struct OnTime {
    int percent{};
  };
  struct OnCollision {};

  // states
  struct Run;
  struct Died : public Will<ByDefault<Nothing>> {
    using ByDefault::handle;
  };

  struct Jump : public Will<ByDefault<Nothing>> {
    using ByDefault::handle;
    Maybe<TransitionTo<Run>> handle(const OnTime& e) const {
      if (e.percent == 100) {
        return TransitionTo<Run>{};
      }
      return Nothing{};
    }
    TransitionTo<Died> handle(const OnCollision& e) const { return TransitionTo<Died>{}; };
  };

  struct Run : public Will<ByDefault<Nothing>,
                           On<OnJump, TransitionTo<Jump>>,
                           On<OnCollision, TransitionTo<Died>>> {};

  struct Walk : public Will<ByDefault<Nothing>> {
    using ByDefault::handle;

    Maybe<TransitionTo<Run>> handle(const OnTime& e) const {
      if (e.percent == 100) {
        return TransitionTo<Run>{};
      }
      return Nothing{};
    }
  };
  struct Pause : public Will<ByDefault<Nothing>, On<OnStart, TransitionTo<Walk>>> {};

  using DinoSM = StateMachine<Pause, Walk, Run, Jump, Died>;
  DinoSM state;
  bool reset_timer{true};
  std::chrono::milliseconds elapsed{0};

 public:
  void on_timer(std::chrono::milliseconds elapsed_p) {
    if (reset_timer) {
      elapsed = elapsed_p;
      reset_timer = false;
    }

    OnTime t;
    t.percent = animation_percent(elapsed_p);
    state.handle(t);

    // restart
    if (t.percent == 100) {
      elapsed = elapsed_p;
    }
  }
  void on_play() {
    OnStart s;
    state.handle(s);
  }
  void on_pause() { state.handle(OnPause{}); }
  void on_jump() { state.handle(OnJump{}); }
  void on_collision() { state.handle(OnCollision{}); }

  bool is_walking() const { return state.isInState<Walk>(); }
  bool is_running() const { return state.isInState<Run>(); }
  bool is_jumping() const { return state.isInState<Jump>(); }
  bool is_died() const { return state.isInState<Died>(); }
  bool is_pause() const { return state.isInState<Pause>(); }

  int animation_percent(std::chrono::milliseconds elapsed_p) const {
    auto delta = (elapsed_p - elapsed);
    if (delta > std::chrono::milliseconds{1000}) {
      return 100;
    }

    return (delta / 10).count();
  }

  DinoState() {
    state.on_switch_state([&]() { this->reset_timer = true; });
  }
};
