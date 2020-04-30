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
  struct OnAnimationFinished {};
  struct OnCollision {};

  // states
  struct Run;
  struct Died : public Will<ByDefault<Nothing>> {
    using ByDefault::handle;
  };

  struct Jump : public Will<ByDefault<Nothing>> {
    using ByDefault::handle;
    Maybe<TransitionTo<Run>> handle(const OnAnimationFinished& e) const {
      return TransitionTo<Run>{};
    }
    TransitionTo<Died> handle(const OnCollision& e) const { return TransitionTo<Died>{}; };
  };

  struct Run : public Will<ByDefault<Nothing>,
                           On<OnJump, TransitionTo<Jump>>,
                           On<OnCollision, TransitionTo<Died>>> {};

  struct Walk : public Will<ByDefault<Nothing>, On<OnAnimationFinished, TransitionTo<Run>>> {};
  struct Pause : public Will<ByDefault<Nothing>, On<OnStart, TransitionTo<Walk>>> {};

  using DinoSM = StateMachine<Pause, Walk, Run, Jump, Died>;
  DinoSM state;

 public:
  DinoState() = default;
  void on_play() { state.handle(OnStart{}); }
  void on_pause() { state.handle(OnPause{}); }
  void on_jump() { state.handle(OnJump{}); }
  void on_collision() { state.handle(OnCollision{}); }
  void on_animation_finished() { state.handle(OnAnimationFinished{}); }

  bool is_walking() const { return state.isInState<Walk>(); }
  bool is_running() const { return state.isInState<Run>(); }
  bool is_jumping() const { return state.isInState<Jump>(); }
  bool is_died() const { return state.isInState<Died>(); }
  bool is_pause() const { return state.isInState<Pause>(); }

  void on_new_state(std::function<void()> callback) { state.on_switch_state(callback); }
};
